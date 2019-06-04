/**
 * Low level initialization code.
 *
 * Sets up caches and C execution environment.
 */
#include <stdint.h>
#include "lowlevel/memory.h"

extern void _bss_start();
extern void _bss_end();

extern void _data_src_start();
extern void _data_start();
extern void _data_end();

CODE_CACHELESS
static uint32_t read_cp0_config1() {
  return read_c0_reg(16, 1);
}

CODE_CACHELESS
static void write_cp0_tag(uint32_t hi, uint32_t lo) {
  write_c0_reg(28, 0, lo);
  write_c0_reg(29, 0, hi);
}

CODE_CACHELESS
static void write_l1_icache_tag(uint32_t ptr) {
  asm volatile("cache 0x8, 0(%0); nop":: "r" (ptr));
}

CODE_CACHELESS
static void write_l1_dcache_tag(uint32_t ptr) {
  asm volatile("cache 0x9, 0(%0); nop":: "r" (ptr));
}

CODE_CACHELESS
static void _invalidate_icache() {
  uint32_t cp0_config1 = read_cp0_config1();
  int is = (cp0_config1 >> 22) & 0x7;
  int il = (cp0_config1 >> 19) & 0x7;
  int ia = (cp0_config1 >> 16) & 0x7;
  
  // I-cache present
  if (il != 0) {
    int line_size = 2 << il;
    int sets_per_way = 64 << is;
    int number_of_ways = ia + 1;
    
    int total_sets_in_cache = sets_per_way * number_of_ways;
    
    // tag 0 => invalid entry
    write_cp0_tag(0, 0);
    
    uint32_t ptr = 0x80000000; /* KSEG0 -> phy=0 */
    for (int i=0; i<total_sets_in_cache; i++) {
      write_l1_icache_tag(ptr);
      ptr += line_size;
    }
  }
}

CODE_CACHELESS
static void _invalidate_dcache() {
  uint32_t cp0_config1 = read_cp0_config1();
  int ds = (cp0_config1 >> 13) & 0x7;
  int dl = (cp0_config1 >> 10) & 0x7;
  int da = (cp0_config1 >>  7) & 0x7;
  
  // I-cache present
  if (dl != 0) {
    int line_size = 2 << dl;
    int sets_per_way = 64 << ds;
    int number_of_ways = da + 1;
    
    int total_sets_in_cache = sets_per_way * number_of_ways;
    
    // tag 0 => invalid entry
    write_cp0_tag(0, 0);
    
    uint32_t ptr = 0x80000000; /* KSEG0 -> phy=0 */
    for (int i=0; i<total_sets_in_cache; i++) {
      write_l1_dcache_tag(ptr);
      ptr += line_size;
    }
  }
}

CODE_CACHELESS
static void _enable_cache() {
  uint32_t config = read_c0_reg(16, 0);
  // 2=uncached, 3=write back,write allocate
  config = (config & ~3) | 3;
  write_c0_reg(16, 0, config);
}

CODE_CACHELESS
static void _zero_mem(void *start, void *end) {
  for (uint32_t *p = start; (void*)p<end; p++) {
    *p = 0;
  }
}

CODE_CACHELESS
static void _copy_mem(void *dest, void *dest_end, void *src) {
  for (uint32_t *s=src, *d=dest; (void*)d<dest_end; s++, d++) {
    *d = *s;
  }
}

CODE_CACHELESS
void _setup_environment() {
  _zero_mem(cacheless_addr(_bss_start), cacheless_addr(_bss_end));
  _copy_mem(
      cacheless_addr(_data_start),
      cacheless_addr(_data_end),
      cacheless_addr(_data_src_start));

  _invalidate_icache();
  _invalidate_dcache();

  _enable_cache();
}

