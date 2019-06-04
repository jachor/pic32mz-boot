/**
 * Low level initialization code.
 *
 * Sets up caches and C execution environment.
 */
#include <stdint.h>

#define NO_CACHE  __attribute__((section(".text.nocache")))
#define read_c0_reg(reg, selector) ({ \
  uint32_t result; \
  asm volatile("mfc0 %0, $%1, %2": "=r" (result): "K" (reg), "K" (selector)); \
  result; \
})

#define write_c0_reg(reg, selector, value) ({ \
  asm volatile("mtc0 %z0, $%1, %2":: "r" ((uint32_t) (value)), "K" (reg), "K" (selector)); \
})

extern void _bss_start();
extern void _bss_end();

extern void _data_src_start();
extern void _data_start();
extern void _data_end();

NO_CACHE
static uint32_t read_cp0_config1() {
  return read_c0_reg(16, 1);
}

NO_CACHE
static void write_cp0_tag(uint32_t hi, uint32_t lo) {
  write_c0_reg(28, 0, lo);
  write_c0_reg(29, 0, hi);
}

NO_CACHE
static void write_l1_icache_tag(uint32_t ptr) {
  asm volatile("cache 0x8, 0(%0); nop":: "r" (ptr));
}

NO_CACHE
static void write_l1_dcache_tag(uint32_t ptr) {
  asm volatile("cache 0x9, 0(%0); nop":: "r" (ptr));
}

NO_CACHE
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

NO_CACHE
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

NO_CACHE
static void _enable_cache() {
  uint32_t config = read_c0_reg(16, 0);
  // 2=uncached, 3=write back,write allocate
  config = (config & ~3) | 3;
  write_c0_reg(16, 0, config);
}

NO_CACHE
static void* _kseg1(void *input) {
  uint32_t addr = (uint32_t)input;
  // assume it is in lower 1/2GB
  addr = addr & ~0xE0000000;
  // use KSEG1, uncached
  addr |= 0xa0000000;
  return (void*) addr;
}

NO_CACHE
static void _zero_mem(void *start, void *end) {
  for (uint32_t *p = start; (void*)p<end; p++) {
    *p = 0;
  }
}

NO_CACHE
static void _copy_mem(void *dest, void *dest_end, void *src) {
  for (uint32_t *s=src, *d=dest; (void*)d<dest_end; s++, d++) {
    *d = *s;
  }
}

NO_CACHE
void _setup_environment() {
  _zero_mem(_kseg1(_bss_start), _kseg1(_bss_end));
  _copy_mem(_kseg1(_data_start), _kseg1(_data_end), _kseg1(_data_src_start));

  _invalidate_icache();
  _invalidate_dcache();

  _enable_cache();
}

