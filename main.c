#include <stdint.h>
#include "lowlevel/printk.h"
#include "lowlevel/ports.h"
#include "lowlevel/clock.h"
#include "lowlevel/usb.h"

#define LED_PINS    ((1UL << 13) | (1UL << 14))

void main() {
  printk("Hello, world\r\n");
  usb_init();

  PortC->TRIS.clr = LED_PINS;
  PortC->ODC.clr = LED_PINS;
  for(;;) {
    for (int i=0; i<10000; i++) {
      clock_sleep_usec(100);
      usb_poll();
    }
    PortC->LAT.inv = LED_PINS;
  }
}
