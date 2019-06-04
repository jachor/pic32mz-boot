/**
 * Last preparations to run main code.
 */
#include <stdint.h>
#include "lowlevel/printk.h"
#include "lowlevel/ports.h"
#include "lowlevel/memory.h"

// values provided by linker script
extern void _bss_start();
extern void _bss_end();
extern void _data_src_start();
extern void _data_start();
extern void _data_end();

static void _describe_cache(const char *name, uint32_t desc_bits) {
  int s = (desc_bits >> 6) & 0x7;
  int l = (desc_bits >> 3) & 0x7;
  int a = (desc_bits >> 0) & 0x7;
  
  if (l != 0) {
    int line_size = 2 << l;
    int sets_per_way = 64 << s;
    int number_of_ways = a + 1;
    printk("%s: %d byte line size, %d sets, %d-associative.\r\n",
            name, line_size, sets_per_way, number_of_ways);
  } else {
    printk("%s: not present.\r\n", name);
  }
}

static void _describe_system() {
    printk("\r\n\r\nBooted\r\n");
    uint32_t config1 = read_c0_reg(16, 1);
    _describe_cache("D-cache", config1 >> 7);
    _describe_cache("I-cache", config1 >> 16);
    uint32_t config0 = read_c0_reg(16, 0);
    static const char* const cache_desc[] = {
        "write-through, no write-allocate",
        "write-through, write-allocate",
        "cache disabled",
        "write-back, write-allocate",
    };
    printk("KSEG0 cache coherency: %s.\r\n", cache_desc[config0&3]);
    printk("\r\n");
}

static void _zero_mem(void *start, void *end) {
  for (uint32_t *p = start; (void*)p<end; p++) {
    *p = 0;
  }
}

static void _copy_mem(void *dest, void *dest_end, void *src) {
  for (uint32_t *s=src, *d=dest; (void*)d<dest_end; s++, d++) {
    *d = *s;
  }
}

void _initialize() {
  _zero_mem(cached_addr(_bss_start), cached_addr(_bss_end));
  _copy_mem(
      cached_addr(_data_start),
      cached_addr(_data_end),
      cached_addr(_data_src_start));

  printk_init();
  _describe_system();
}

