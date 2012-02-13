/* Can_error
 *
 * can4linux -- LINUX CAN device driver source
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * 
 * Copyright (c) 2001 port GmbH Halle/Saale
 * (c) 2001 Heinz-Jürgen Oertel (oe@port.de)
 *          Claus Schroeter (clausi@chemie.fu-berlin.de)
 *------------------------------------------------------------------
 * $Header: /home/cvsroot/uClinux-dist/linux-2.4.x/drivers/char/can4linux/can_error.c,v 1.1 2004/05/31 02:32:33 rupert Exp $
 *
 *--------------------------------------------------------------------------
 *
 *
 * modification history
 * --------------------
 * $Log: can_error.c,v $
 * Revision 1.1  2004/05/31 02:32:33  rupert
 * Initial 2.4.26 merge
 *
 *
 *
 */
#include "can_defs.h"

int Can_errno = 0;


int Error(int err)
{
    Can_errno = err;
    return 0;
}
