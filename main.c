#include <stdint.h>
#include "debug.h"
#include "ports.h"

#define LED_PINS    ((1UL << 13) | (1UL << 14))

void write_PORTC(uint32_t value) {
    *((volatile uint32_t*)0xbf860220) = value;
}

void sleep() {
    for (uint32_t i=0; i<100000; i++) {
        asm volatile("nop");
    }
}

void enter_c_world() {
    //debug_init();
    //debug_print("Hello, world\r\n");
    
    PortC->TRIS.clr = LED_PINS;
    PortC->ODC.clr = LED_PINS;
    for(;;) {
        sleep();
        PortC->LAT.inv = LED_PINS;
    }
}
