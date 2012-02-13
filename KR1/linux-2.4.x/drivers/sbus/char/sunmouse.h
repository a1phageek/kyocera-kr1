/* $Id: sunmouse.h,v 1.2 2004/05/31 02:32:38 rupert Exp $
 * sunmouse.h: Interface to the SUN mouse driver.
 *
 * Copyright (C) 1997  Eddie C. Dost  (ecd@skynet.be)
 */

#ifndef _SPARC_SUNMOUSE_H
#define _SPARC_SUNMOUSE_H 1

extern void sun_mouse_zsinit(void);
extern void sun_mouse_inbyte(unsigned char, int);

#endif /* !(_SPARC_SUNMOUSE_H) */
