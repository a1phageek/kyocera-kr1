/*
 * Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
 * Copyright (c) 2002-2004 Atheros Communications, Inc.
 * All rights reserved.
 *
 * $Id: //depot/sw/releases/linuxsrc/src/802_11/madwifi/hal/main/ar5312/ar5312_gpio.c#5 $
 */
#include "opt_ah.h"

#ifdef AH_SUPPORT_AR5312

#include "ah.h"
#include "ah_internal.h"
#include "ah_devid.h"

#include "ar5312/ar5312.h"
#include "ar5312/ar5312reg.h"
#include "ar5312/ar5312phy.h"

#define	AR_NUM_GPIO	7		/* 7 GPIO pins */
#define	AR_GPIOD_MASK	0x0000007F	/* GPIO data reg r/w mask */


/* 
 * XXX Divy. Should we change the file name, 
 * or create a specific file per target ?
 */
 
/* Macros to get platform independent code */
#define IS_FREEDOM(ah) \
	(AH_PRIVATE((ah))->ah_devid == AR5212_AR5312_REV7)
	
#define	IS_COBRA(ah) \
	(AH_PRIVATE((ah))->ah_macVersion == AR_SREV_VERSION_COBRA)

#define GET_GPIO_BASE(ah, base) do {			\
	if (IS_FREEDOM(ah))				\
		(base) = AR_GPIO_BASE;			\
	else if (IS_COBRA(ah)) 				\
		(base) = AR5315_GPIO_BASE;		\
} while (0)

#define GET_GPIO_CR(ah, cr) do {			\
	if (IS_FREEDOM(ah))				\
		(cr) = AR_GPIOCR;			\
	else if (IS_COBRA(ah)) 				\
		(cr) = AR5315_GPIOCR;			\
} while (0)

#define GET_GPIO_DO(ah, _do) do {			\
	if (IS_FREEDOM(ah))				\
		(_do) = AR_GPIODO;			\
	else if (IS_COBRA(ah)) 				\
		(_do) = AR5315_GPIODO;			\
} while (0)

#define GET_GPIO_DI(ah, di) do {			\
	if (IS_FREEDOM(ah))				\
		(di) = AR_GPIODI;			\
	else if (IS_COBRA(ah)) 				\
		(di) = AR5315_GPIODI;			\
} while (0)

#define GET_GPIO_CR_0(ah, pin, out) do {		\
	if (IS_FREEDOM(ah))				\
		(out) = GPIO_CR_O(pin);			\
	else if (IS_COBRA(ah)) 				\
		(out) = GPIO_CR_I(pin);			\
} while (0)

#define GET_GPIO_CR_I(ah, pin, out) do {		\
	if (IS_FREEDOM(ah))				\
		(out) = GPIO_CR_I(pin);			\
	else if (IS_COBRA(ah)) 				\
		(out) = GPIO_CR_O(pin);			\
} while (0)

extern struct ar531x_boarddata *ar5312_boardConfig;

/*
 * Configure GPIO Output lines
 */
HAL_BOOL
ar5312GpioCfgOutput(struct ath_hal *ah, u_int32_t gpio)
{
	u_int32_t gpio_base, gpio_cr, gpio_offset;
	u_int32_t gpio_pin;
	u_int32_t gpio_cr_o;
	u_int32_t reg_val;
	
	HALASSERT(gpio < AR_NUM_GPIO);
		
	GET_GPIO_BASE(ah, gpio_base);
	GET_GPIO_CR(ah, gpio_cr);

	gpio_offset = gpio_base - ((u_int32_t)ah->ah_sh);

	if (ar5312_boardConfig)
		gpio_pin = ar5312_boardConfig->sysLedGpio;
	else
		gpio_pin = gpio;
	
	reg_val = OS_REG_READ(ah, gpio_offset + gpio_cr);
	
	reg_val &= ~(GPIO_CR_M(gpio_pin) | 
		     GPIO_CR_UART(gpio_pin) | 
		     GPIO_CR_INT(gpio_pin));
	
	GET_GPIO_CR_0(ah, gpio_pin, gpio_cr_o);
	reg_val |= gpio_cr_o; 
	
	OS_REG_WRITE(ah, gpio_offset + gpio_cr, reg_val);
	OS_REG_READ(ah, gpio_offset + gpio_cr);

	return AH_TRUE;
}

/*
 * Configure GPIO Input lines
 */
HAL_BOOL
ar5312GpioCfgInput(struct ath_hal *ah, u_int32_t gpio)
{
	u_int32_t gpio_base, gpio_cr, gpio_offset;
	u_int32_t gpio_pin;
	u_int32_t gpio_cr_i;
	u_int32_t reg_val;
		
	HALASSERT(gpio < AR_NUM_GPIO);
	
	GET_GPIO_BASE(ah, gpio_base);
	GET_GPIO_CR(ah, gpio_cr);

	gpio_offset = gpio_base - ((u_int32_t)ah->ah_sh);
		
	if (ar5312_boardConfig)
		gpio_pin = ar5312_boardConfig->sysLedGpio;
	else
		gpio_pin = gpio;

	reg_val = OS_REG_READ(ah, gpio_offset + gpio_cr);
	reg_val &= ~(GPIO_CR_M(gpio_pin) | 
		     GPIO_CR_UART(gpio_pin) | 
		     GPIO_CR_INT(gpio_pin));
	
	GET_GPIO_CR_I(ah, gpio_pin, gpio_cr_i);
	reg_val |= gpio_cr_i; 
	
	OS_REG_WRITE(ah, gpio_offset + gpio_cr, reg_val);
	OS_REG_READ(ah, gpio_offset + gpio_cr);

	
	return AH_TRUE;
}

/*
 * Once configured for I/O - set output lines
 */
HAL_BOOL
ar5312GpioSet(struct ath_hal *ah, u_int32_t gpio, u_int32_t val)
{
	u_int32_t gpio_base, gpio_do, gpio_offset;
	u_int32_t gpio_pin;
	u_int32_t reg_val;

	HALASSERT(gpio < AR_NUM_GPIO);
	
	GET_GPIO_BASE(ah, gpio_base);
	GET_GPIO_DO(ah, gpio_do);
	
	gpio_offset = gpio_base - ((u_int32_t)ah->ah_sh);
		       
	if (ar5312_boardConfig)
		gpio_pin = ar5312_boardConfig->sysLedGpio;
	else
		gpio_pin = gpio;
	

	reg_val =  OS_REG_READ(ah, gpio_offset + gpio_do);
	
	reg_val &= ~(1 << gpio_pin);
	reg_val |= (val&1) << gpio_pin;
	
	OS_REG_WRITE(ah, gpio_offset + gpio_do, reg_val);
	OS_REG_READ(ah, gpio_offset + gpio_do);

	return AH_TRUE;
}

/*
 * Once configured for I/O - get input lines
 */
u_int32_t
ar5312GpioGet(struct ath_hal *ah, u_int32_t gpio)
{
	u_int32_t gpio_base, gpio_di, gpio_offset;
	u_int32_t gpio_pin;
	u_int32_t reg_val;
	
	HALASSERT(gpio < AR_NUM_GPIO);
	
	GET_GPIO_BASE(ah, gpio_base);
	GET_GPIO_DI(ah, gpio_di);
	
	gpio_offset = gpio_base - ((u_int32_t)ah->ah_sh);
	
	if (ar5312_boardConfig)
		gpio_pin = ar5312_boardConfig->sysLedGpio;
	else
		gpio_pin = gpio;
			
	if (gpio_pin < AR_NUM_GPIO) {
		reg_val = OS_REG_READ(ah, gpio_offset + gpio_di);
		reg_val = ((reg_val & AR_GPIOD_MASK) >> gpio_pin) & 0x1;
		return reg_val;
	} else  {
		return 0xffffffff;
	}
}

/*
 * Set the GPIO Interrupt
 */
void
ar5312GpioSetIntr(struct ath_hal *ah, u_int gpio, u_int32_t ilevel)
{
	u_int32_t gpio_base, gpio_cr, gpio_offset;
	u_int32_t gpio_pin;
	u_int32_t gpio_cr_i;
	u_int32_t reg_val;

	HALASSERT(gpio < AR_NUM_GPIO);
		
	GET_GPIO_BASE(ah, gpio_base);
	GET_GPIO_CR(ah, gpio_cr);

	gpio_offset = gpio_base - ((u_int32_t)ah->ah_sh);
		
	/* XXX bounds check gpio */
	if (ar5312_boardConfig)
		gpio_pin = ar5312_boardConfig->sysLedGpio;
	else
		gpio_pin = gpio;
		
	reg_val = OS_REG_READ(ah, gpio_offset+AR_GPIOCR);
	/* XXX Divy. Check platform dependancy for these macros */
	reg_val &= ~(GPIO_CR_M(gpio_pin) | 
		     GPIO_CR_UART(gpio_pin) | 
		     GPIO_CR_INT(gpio_pin));
		     
	GET_GPIO_CR_I(ah, gpio_pin, gpio_cr_i);
	reg_val |= gpio_cr_i; 
	reg_val |= GPIO_CR_INT(gpio_pin);

	/* Don't need to change anything for low level interrupt. */
	OS_REG_WRITE(ah, gpio_offset + gpio_cr, reg_val);
	OS_REG_READ(ah, gpio_offset + gpio_cr);
	
	/* Change the interrupt mask. */
	(void) ar5212SetInterrupts(ah, AH5212(ah)->ah_maskReg | HAL_INT_GPIO);
}

#undef IS_FREEDOM
#undef IS_COBRA
#undef GET_GPIO_BASE
#undef GET_GPIO_CR
#undef GET_GPIO_CR_0
#undef GET_GPIO_CR_I
#undef GET_GPIO_D0
#undef GET_GPIO_DI

#endif /* AH_SUPPORT_AR5312 */
