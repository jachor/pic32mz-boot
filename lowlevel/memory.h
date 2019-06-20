#ifndef lowlevel_memory_h__
#define lowlevel_memory_h__

#include <stdint.h>

#define PACKED_STRUCT __attribute__((packed))

#define CODE_CACHELESS  __attribute__((section(".text.nocache")))

#define read_c0_reg(reg, selector) ({ \
  uint32_t result; \
  asm volatile("mfc0 %0, $%1, %2": "=r" (result): "K" (reg), "K" (selector)); \
  result; \
})

#define write_c0_reg(reg, selector, value) ({ \
  asm volatile("mtc0 %z0, $%1, %2":: "r" ((uint32_t) (value)), "K" (reg), "K" (selector)); \
})

// Returns address in uncached KSEG1.
__attribute__((always_inline))
static inline void *cacheless_addr(void *ptr) {
  uint32_t addr = (uint32_t) ptr;
  // assume it is in lower 0.5GiB
  addr = addr & ~0xE0000000;
  // use KSEG1, uncached
  addr |= 0xa0000000;
  return (void*) addr;
}

// Returns address in (potentially) cached KSEG0
__attribute__((always_inline))
static inline void *cached_addr(void *ptr) {
  uint32_t addr = (uint32_t) ptr;
  // assume it is in lower 0.5GiB
  addr = addr & ~0xE0000000;
  // use KSEG0, cached
  addr |= 0x80000000;
  return (void*) addr;
}

#endif
