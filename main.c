#include <stdint.h>
#include "lowlevel/printk.h"
#include "lowlevel/ports.h"

#define LED_PINS    ((1UL << 13) | (1UL << 14))

static void sleep() {
    for (uint32_t i=0; i<1000000; i++) {
        asm volatile("nop");
    }
}

void main() {
    printk("Hello, world\r\n");
    
    PortC->TRIS.clr = LED_PINS;
    PortC->ODC.clr = LED_PINS;
    for(;;) {
        sleep();
        PortC->LAT.inv = LED_PINS;
    }
}
