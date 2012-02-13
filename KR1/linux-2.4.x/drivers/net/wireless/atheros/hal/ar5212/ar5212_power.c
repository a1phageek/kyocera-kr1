/*
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2004 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/releases/SuperG_Linux_AP/src/802_11/madwifi/hal/ar5212/ar5212_power.c#3 $
 */
#include "opt_ah.h"

#ifdef AH_SUPPORT_AR5212

#include "ah.h"
#include "ah_internal.h"

#include "ar5212/ar5212.h"
#include "ar5212/ar5212reg.h"
#include "ar5212/ar5212desc.h"

/*
 * Notify Power Mgt is enabled in self-generated frames.
 * If requested, force chip awake.
 *
 * Returns A_OK if chip is awake or successfully forced awake.
 *
 * WARNING WARNING WARNING
 * There is a problem with the chip where sometimes it will not wake up.
 */
static HAL_BOOL
ar5212SetPowerModeAwake(struct ath_hal *ah, int setChip)
{
#define	POWER_UP_TIME	2000
	u_int32_t val;
	int i;

	if (setChip) {
		OS_REG_WRITE(ah, AR_SCR, AR_SCR_SLE_WAKE);
		OS_DELAY(10);	/* Give chip the chance to awake */

		for (i = POWER_UP_TIME / 200; i != 0; i--) {
			val = OS_REG_READ(ah, AR_PCICFG);
			if ((val & AR_PCICFG_SPWR_DN) == 0)
				break;
			OS_DELAY(200);
			OS_REG_WRITE(ah, AR_SCR,
				SM(AR_SCR_SLE_WAKE, AR_SCR_SLE));
		}
		if (i == 0) {
#ifdef AH_DEBUG
			ath_hal_printf(ah, "%s: Failed to wakeup in %ums\n",
				__func__, POWER_UP_TIME/20);
#endif
			return AH_FALSE;
		}
	} 

	OS_REG_WRITE(ah, AR_STA_ID1,
		OS_REG_READ(ah, AR_STA_ID1) & ~AR_STA_ID1_PWR_SAV);
	return AH_TRUE;
#undef POWER_UP_TIME
}

/*
 * Notify Power Mgt is disabled in self-generated frames.
 * If requested, force chip to sleep.
 */
static void
ar5212SetPowerModeSleep(struct ath_hal *ah, int setChip)
{
	OS_REG_WRITE(ah, AR_STA_ID1,
		OS_REG_READ(ah, AR_STA_ID1) | AR_STA_ID1_PWR_SAV);
	if (setChip)
		OS_REG_WRITE(ah, AR_SCR, SM(AR_SCR_SLE_SLP, AR_SCR_SLE));
}

/*
 * Notify Power Management is enabled in self-generating
 * fames.  If request, set power mode of chip to
 * auto/normal.  Duration in units of 128us (1/8 TU).
 */
static void
ar5212SetPowerModeNetworkSleep(struct ath_hal *ah, int setChip, u_int16_t sleepDuration)
{
	OS_REG_WRITE(ah, AR_STA_ID1,
		OS_REG_READ(ah, AR_STA_ID1) | AR_STA_ID1_PWR_SAV);
	if (setChip)
		OS_REG_WRITE(ah, AR_SCR,
			SM(AR_SCR_SLE_NORM, AR_SCR_SLE) | sleepDuration);
}

/*
 * Set power mgt to the requested mode, and conditionally set
 * the chip as well
 */
HAL_BOOL
ar5212SetPowerMode(struct ath_hal *ah, HAL_POWER_MODE mode,
	int setChip, u_int16_t sleepDuration)
{
	struct ath_hal_5212 *ahp = AH5212(ah);
#ifdef AH_DEBUG
	static const char* modes[] = {
		"UNDEFINED",
		"AUTO",
		"AWAKE",
		"FULL-SLEEP",
		"NETWORK SLEEP"
	};
#endif
	int status = AH_TRUE;

#ifdef AH_DEBUG
	HALDEBUG(ah, "%s: %s -> %s (%ssleep duration %u)\n", __func__,
		modes[ahp->ah_powerMode], modes[mode],
		setChip ? "set chip " : "", sleepDuration);
#endif
	switch (mode) {
	case HAL_PM_AWAKE:
		status = ar5212SetPowerModeAwake(ah, setChip);
		break;
	case HAL_PM_FULL_SLEEP:
		ar5212SetPowerModeSleep(ah, setChip);
		break;
	case HAL_PM_NETWORK_SLEEP:
		ar5212SetPowerModeNetworkSleep(ah, setChip, sleepDuration);
		break;
	default:
		HALDEBUG(ah, "%s: unknown power mode %u\n", __func__, mode);
		return AH_FALSE;
	}
	ahp->ah_powerMode = mode;
	return status; 
}

/*
 * Return the current sleep mode of the chip
 */
HAL_POWER_MODE
ar5212GetPowerMode(struct ath_hal *ah)
{
	/* Just so happens the h/w maps directly to the abstracted value */
	return MS(OS_REG_READ(ah, AR_SCR), AR_SCR_SLE);
}

/*
 * Return the current sleep state of the chip
 * TRUE = sleeping
 */
HAL_BOOL
ar5212GetPowerStatus(struct ath_hal *ah)
{
	return (OS_REG_READ(ah, AR_PCICFG) & AR_PCICFG_SPWR_DN) != 0;
}

/*
 * Initialize for PS-Polls
 */
HAL_BOOL
ar5212InitPSPoll(struct ath_hal *ah)
{
	/* NB: This function should never be called for ar5212 */
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s called!\n", __func__);
#endif
	return AH_FALSE;
}

/*
 * Enable PS-Polls on the STA
 */
HAL_BOOL
ar5212EnablePSPoll(struct ath_hal *ah, u_int8_t *bssid, u_int16_t assocId)
{
	/* NB: This function should never be called for ar5212 */
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s called!\n", __func__);
#endif
	return AH_FALSE;
}

/*
 * Disable PS-Polls on the STA
 */
HAL_BOOL
ar5212DisablePSPoll(struct ath_hal *ah)
{
	/* NB: This function should never be called for ar5212 */
#ifdef AH_DEBUG
	ath_hal_printf(ah, "%s called!\n", __func__);
#endif
	return AH_FALSE;
}

#if 0
/*
 * Initialize for PS-Polls
 */
void
ar5212SetupPSPollDesc(struct ath_hal *ah, struct ath_desc *ds,
	u_int rate, u_int retries)
{
	struct ath_hal_5212 *ahp = AH5212(ah);
	struct ar5212_txdesc *tx = &AR5212DESC(ds)->u.tx;

	/* Send PS-Polls at 6mbps. */
	/* XXX */
	tx->TXRate0 = rate;
	tx->TXDataTries0 = 1 + retries;
	tx->antModeXmit  = 0;

	/* PS-Poll descriptor points to itself with the VEOL bit set. */
	tx->noAck = 0;
	tx->clearDestMask = 1;
	tx->PktType = HAL_PKT_TYPE_PSPOLL;
	tx->VEOL = 1;
}
#endif
#endif /* AH_SUPPORT_AR5212 */
