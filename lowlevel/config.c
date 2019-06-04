#include <stdint.h>

struct DeviceConfig {
    uint32_t devcfg3;
    uint32_t devcfg2;
    uint32_t devcfg1;
    uint32_t devcfg0;
    uint64_t cp_high, cp_low;
    uint64_t sign_high, sign_low;
};

__attribute__((section(".config")))
const struct DeviceConfig _device_config = {
    .devcfg0 = 0x8000877f,
    .devcfg1 = 0x003fc78f,
    .devcfg2 = 0x00000080,
    .devcfg3 = 0x43000000,
    // no signature.
    .cp_high = -1, .cp_low = -1,
    .sign_high = -1, .sign_low = -1,
};
