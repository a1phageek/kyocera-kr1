/*
 *  sata_sis.c - Silicon Integrated Systems SATA
 *
 *  Copyright 2004 Uwe Koziolek
 *
 *  The contents of this file are subject to the Open
 *  Software License version 1.1 that can be found at
 *  http://www.opensource.org/licenses/osl-1.1.txt and is included herein
 *  by reference.
 *
 *  Alternatively, the contents of this file may be used under the terms
 *  of the GNU General Public License version 2 (the "GPL") as distributed
 *  in the kernel source COPYING file, in which case the provisions of
 *  the GPL are applicable instead of the above.  If you wish to allow
 *  the use of your version of this file only under the terms of the
 *  GPL and not to allow others to use your version of this file under
 *  the OSL, indicate your decision by deleting the provisions above and
 *  replace them with the notice and other provisions required by the GPL.
 *  If you do not delete the provisions above, a recipient may use your
 *  version of this file under either the OSL or the GPL.
 *
 */

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include "scsi.h"
#include "hosts.h"
#include <linux/libata.h>

#define DRV_NAME	"sata_sis"
#define DRV_VERSION	"0.04"

enum {
	sis_180			= 0,
};

static int sis_init_one (struct pci_dev *pdev, const struct pci_device_id *ent);
static u32 sis_scr_read (struct ata_port *ap, unsigned int sc_reg);
static void sis_scr_write (struct ata_port *ap, unsigned int sc_reg, u32 val);

static struct pci_device_id sis_pci_tbl[] = {
	{ PCI_VENDOR_ID_SI, PCI_DEVICE_ID_SI_180, PCI_ANY_ID, PCI_ANY_ID, 0, 0, sis_180 },
	{ }	/* terminate list */
};


static struct pci_driver sis_pci_driver = {
	.name			= DRV_NAME,
	.id_table		= sis_pci_tbl,
	.probe			= sis_init_one,
	.remove			= ata_pci_remove_one,
};

static Scsi_Host_Template sis_sht = {
	.module			= THIS_MODULE,
	.name			= DRV_NAME,
	.detect			= ata_scsi_detect,
	.release		= ata_scsi_release,
	.queuecommand		= ata_scsi_queuecmd,
	.eh_strategy_handler	= ata_scsi_error,
	.can_queue		= ATA_DEF_QUEUE,
	.this_id		= ATA_SHT_THIS_ID,
	.sg_tablesize		= ATA_MAX_PRD,
	.max_sectors		= ATA_MAX_SECTORS,
	.cmd_per_lun		= ATA_SHT_CMD_PER_LUN,
	.use_new_eh_code	= ATA_SHT_NEW_EH_CODE,
	.emulated		= ATA_SHT_EMULATED,
	.use_clustering		= ATA_SHT_USE_CLUSTERING,
	.proc_name		= DRV_NAME,
	.bios_param		= ata_std_bios_param,
};

static struct ata_port_operations sis_ops = {
	.port_disable		= ata_port_disable,
	.tf_load		= ata_tf_load_pio,
	.tf_read		= ata_tf_read_pio,
	.check_status		= ata_check_status_pio,
	.exec_command		= ata_exec_command_pio,
	.phy_reset		= sata_phy_reset,
	.bmdma_start            = ata_bmdma_start_pio,
	.fill_sg		= ata_fill_sg,
	.eng_timeout		= ata_eng_timeout,
	.irq_handler		= ata_interrupt,
	.scr_read		= sis_scr_read,
	.scr_write		= sis_scr_write,
	.port_start		= ata_port_start,
	.port_stop		= ata_port_stop,
};


MODULE_AUTHOR("Uwe Koziolek");
MODULE_DESCRIPTION("low-level driver for Silicon Integratad Systems SATA controller");
MODULE_LICENSE("GPL");
MODULE_DEVICE_TABLE(pci, sis_pci_tbl);


static u32 sis_scr_read (struct ata_port *ap, unsigned int sc_reg)
{
	if (sc_reg >= 16)
		return 0xffffffffU;

	return inl(ap->ioaddr.scr_addr + (sc_reg * 4));
}

static void sis_scr_write (struct ata_port *ap, unsigned int sc_reg, u32 val)
{
	if (sc_reg >= 16)
		return;
	outl(val, ap->ioaddr.scr_addr + (sc_reg * 4));
}

/* move to PCI layer, integrate w/ MSI stuff */
static void pci_enable_intx(struct pci_dev *pdev)
{
	u16 pci_command;

	pci_read_config_word(pdev, PCI_COMMAND, &pci_command);
	if (pci_command & PCI_COMMAND_INTX_DISABLE) {
		pci_command &= ~PCI_COMMAND_INTX_DISABLE;
		pci_write_config_word(pdev, PCI_COMMAND, pci_command);
	}
}

static int sis_init_one (struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct ata_probe_ent *probe_ent = NULL;
	int rc;

	rc = pci_enable_device(pdev);
	if (rc)
		return rc;

	rc = pci_request_regions(pdev, DRV_NAME);
	if (rc)
		goto err_out;

	rc = pci_set_dma_mask(pdev, ATA_DMA_MASK);
	if (rc)
		goto err_out_regions;

	probe_ent = kmalloc(sizeof(*probe_ent), GFP_KERNEL);
	if (!probe_ent) {
		rc = -ENOMEM;
		goto err_out_regions;
	}

	memset(probe_ent, 0, sizeof(*probe_ent));
	probe_ent->pdev = pdev;
	INIT_LIST_HEAD(&probe_ent->node);

	probe_ent->sht = &sis_sht;
	probe_ent->host_flags = ATA_FLAG_SATA | ATA_FLAG_SATA_RESET |
				ATA_FLAG_NO_LEGACY;
	probe_ent->pio_mask = 0x03;
	probe_ent->udma_mask = 0x7f;
	probe_ent->port_ops = &sis_ops;

	probe_ent->port[0].cmd_addr = pci_resource_start(pdev, 0);
	ata_std_ports(&probe_ent->port[0]);
	probe_ent->port[0].ctl_addr =
		pci_resource_start(pdev, 1) | ATA_PCI_CTL_OFS;
	probe_ent->port[0].bmdma_addr = pci_resource_start(pdev, 4);
	probe_ent->port[0].scr_addr = pci_resource_start(pdev, 5);

	probe_ent->port[1].cmd_addr = pci_resource_start(pdev, 2);
	ata_std_ports(&probe_ent->port[1]);
	probe_ent->port[1].ctl_addr =
		pci_resource_start(pdev, 3) | ATA_PCI_CTL_OFS;
	probe_ent->port[1].bmdma_addr = pci_resource_start(pdev, 4) + 8;
	probe_ent->port[1].scr_addr = pci_resource_start(pdev, 5) + 64;

	probe_ent->n_ports = 2;
	probe_ent->irq = pdev->irq;
	probe_ent->irq_flags = SA_SHIRQ;

	pci_set_master(pdev);
	pci_enable_intx(pdev);

	ata_add_to_probe_list(probe_ent);

	return 0;

err_out_regions:
	pci_release_regions(pdev);

err_out:
	pci_disable_device(pdev);
	return rc;

}

static int __init sis_init(void)
{
	int rc = pci_module_init(&sis_pci_driver);
	if (rc)
		return rc;

	rc = scsi_register_module(MODULE_SCSI_HA, &sis_sht);
	if (rc) {
		pci_unregister_driver(&sis_pci_driver);
		return -ENODEV;
	}

	return 0;
}

static void __exit sis_exit(void)
{
	scsi_unregister_module(MODULE_SCSI_HA, &sis_sht);
	pci_unregister_driver(&sis_pci_driver);
}

module_init(sis_init);
module_exit(sis_exit);

