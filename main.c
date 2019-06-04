#include <stdint.h>
#include "debug.h"
#include "lowlevel/ports.h"
#include "lowlevel/memory.h"

#define LED_PINS    ((1UL << 13) | (1UL << 14))

static void sleep() {
    for (uint32_t i=0; i<1000000; i++) {
        asm volatile("nop");
    }
}

static void _describe_cache(const char *name, uint32_t desc_bits) {
  int s = (desc_bits >> 6) & 0x7;
  int l = (desc_bits >> 3) & 0x7;
  int a = (desc_bits >> 0) & 0x7;
  
  if (l != 0) {
    int line_size = 2 << l;
    int sets_per_way = 64 << s;
    int number_of_ways = a + 1;
    debug_printf("%s: %d byte line size, %d sets, %d-associative.\r\n", name, line_size, sets_per_way, number_of_ways);
  } else {
    debug_printf("%s: not present.\r\n", name);
  }
}

static void describe_system() {
    debug_printf("\r\n\r\nBooted\r\n");
    uint32_t config1 = read_c0_reg(16, 1);
    _describe_cache("D-cache", config1 >> 7);
    _describe_cache("I-cache", config1 >> 16);
    uint32_t config0 = read_c0_reg(16, 0);
    static const char* const cache_desc[] = {
        "write-through, no write-allocate",
        "write-through, write-allocate",
        "disabled",
        "write-back, write-allocate",
    };
    debug_printf("KSEG0 cache coherency: %s.\r\n", cache_desc[config0&3]);
    
    debug_printf("\r\n");
}

void enter_c_world() {
    debug_init();
    describe_system();
    
    debug_printf("Hello, world\r\n");
    
    PortC->TRIS.clr = LED_PINS;
    PortC->ODC.clr = LED_PINS;
    for(;;) {
        debug_printf("inverse\r\n");
        sleep();
        PortC->LAT.inv = LED_PINS;
    }
}
