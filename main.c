#include <stdint.h>
#include "lowlevel/printk.h"
#include "lowlevel/ports.h"
#include "lowlevel/clock.h"

#define LED_PINS    ((1UL << 13) | (1UL << 14))

void main() {
    printk("Hello, world\r\n");

    PortC->TRIS.clr = LED_PINS;
    PortC->ODC.clr = LED_PINS;
    for(;;) {
        clock_sleep_usec(1000000);
        PortC->LAT.inv = LED_PINS;
    }
}
