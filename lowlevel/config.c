#include <stdint.h>

struct DeviceConfig {
    uint32_t devcfg3;
    uint32_t devcfg2;
    uint32_t devcfg1;
    uint32_t devcfg0;
    uint64_t cp_high, cp_low;
    uint64_t sign_high, sign_low;
};

#define DEVCFG0_EJTAGBEN (1<<30)
#define DEVCFG0_POSC_BOOST (1<<21)
#define DEVCFG0_POSC_GAIN(gain) ((gain)<<19)
#define DEVCFG0_SMCLR_NORMAL_RESET (1<<15)
#define DEVCFG0_FLASH_ECC_RECONFIGURABLE (3<<8)
#define DEVCFG0_BOOTISA_MIPS32 (1<<6)
#define DEVCFG0_TRACE_ENABLE (1<<5)
#define DEVCFG0_ICESEL_PAIR1 (3<<3)
#define DEVCFG0_JTAGEN (1<<2)
#define DEVCFG0_DEBUG_ENABLED (0<<0)
#define DEVCFG0_RESERVED ((1<<7) | (0xff<<22))

#define DEVCFG1_ENABLE_CLOCK_SWITCHING_AND_MONITORING (3<<14)
#define DEVCFG1_POSC_HS_MODE (2<<8)
#define DEVCFG1_POSC_EC_MODE (0<<8)
#define DEVCFG1_POSC_DISABLED (3<<8)
#define DEVCFG1_IESO (1<<7)
#define DEVCFG1_CLOCK_FRC (0<<0)
#define DEVCFG1_CLOCK_POSC (2<<0)

#define DEVCFG2_USB_12MHZ (0<<30)
#define DEVCFG2_FPLLMULT(value) ((value-1)<<8)
#define DEVCFG2_FPLLRNG_8_16MHZ (2<<4)
#define DEVCFG2_RESERVED ((1<<31) | (0x7ff<<19) | (1<<15) | (1<<3))

__attribute__((section(".config")))
const struct DeviceConfig _device_config = {
    .devcfg0 = 0
        | DEVCFG0_EJTAGBEN
        | DEVCFG0_POSC_GAIN(1)
        | DEVCFG0_SMCLR_NORMAL_RESET
        | DEVCFG0_FLASH_ECC_RECONFIGURABLE
        | DEVCFG0_BOOTISA_MIPS32
        | DEVCFG0_JTAGEN
        | DEVCFG0_TRACE_ENABLE
        | DEVCFG0_ICESEL_PAIR1
        | DEVCFG0_RESERVED
        | 0,
    .devcfg1 = 0
        | DEVCFG1_ENABLE_CLOCK_SWITCHING_AND_MONITORING
        | DEVCFG1_POSC_EC_MODE
        | DEVCFG1_CLOCK_FRC
        | 0,
    .devcfg2 = 0
        | DEVCFG2_USB_12MHZ
        | DEVCFG2_RESERVED
        | 0,
    .devcfg3 = 0x43000000,
    // no signature.
    .cp_high = -1, .cp_low = -1,
    .sign_high = -1, .sign_low = -1,
};
