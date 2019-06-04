#ifndef lowlevel_memory_h__
#define lowlevel_memory_h__

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

__attribute__((always_inline))
static inline void *cacheless_addr(void *ptr) {
  uint32_t addr = (uint32_t) ptr;
  // assume it is in lower 0.5GiB
  addr = addr & ~0xE0000000;
  // use KSEG1, uncached
  addr |= 0xa0000000;
  return (void*) addr;
}

#endif
