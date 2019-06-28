#include "lowlevel/usb.h"
#include "lowlevel/ports.h"
#include "lowlevel/memory.h"
#include "lowlevel/printk.h"
#include "lowlevel/clock.h"


#define ALLOW_HIGH_SPEED (1)

void usb_set_address(uint8_t addr) {
  UsbController->USBCSR0 = 0
      | (uint32_t) addr
      | (ALLOW_HIGH_SPEED<<13)
      | (1<<14) // connect to bus
      | 0;
}

void usb_init() {
  // disable OTG + run as device: force ID=1, ignore phy
  UsbController2->USBCRCON = (1<<9) | (1<<8);
  UsbController->USBFIFOA = (0x100<<16) | 0x000;
  // disconnect from USB
  UsbController->USBCSR0 = 0;
  clock_sleep_usec(20000);

  usb_ep0_reset();
}

void usb_poll() {
  uint32_t usbcsr0 = UsbController->USBCSR0;
  //printk("USBCSR0=%08x\r\n", usbcsr0);

  if (usbcsr0 & (1<<11)) {
    usb_ep0_reset();
  } else if (usbcsr0 & (1<<16)) {
    while(usb_ep0_interrupt()) {}
  }
}
