#ifndef j_ports_h__
#define j_ports_h__

#include <stdint.h>

__attribute__((packed))
struct CombinedCommandRegister {
    uint32_t reg;
    uint32_t clr;
    uint32_t set;
    uint32_t inv;
};

__attribute__((packed))
struct IoPort {
    struct CombinedCommandRegister ANSEL;
    struct CombinedCommandRegister TRIS;
    struct CombinedCommandRegister PORT;
    struct CombinedCommandRegister LAT;
    struct CombinedCommandRegister ODC;
    struct CombinedCommandRegister CNPU;
    struct CombinedCommandRegister CNPD;
    struct CombinedCommandRegister CNCON;
    struct CombinedCommandRegister CNEN;
    struct CombinedCommandRegister CNSTAT;
    struct CombinedCommandRegister CNNE;
    struct CombinedCommandRegister CNF;
    struct CombinedCommandRegister SRCON0;
    struct CombinedCommandRegister SRCON1;
};

#define PortA ((volatile struct IoPort*)0xbf860000)
#define PortB ((volatile struct IoPort*)0xbf860100)
#define PortC ((volatile struct IoPort*)0xbf860200)
#define PortD ((volatile struct IoPort*)0xbf860300)

#endif
