#ifndef lowlevel_usb_h__
#define lowlevel_usb_h__

#include <stdint.h>

extern void usb_init();
extern void usb_poll();

extern void usb_set_address(uint8_t addr);
extern void usb_ep0_reset();
extern int usb_ep0_interrupt();

extern void usb_ep_read(void *dest, int ep, int len);
extern void usb_ep_write(const void *src, int ep, int len);

#endif
