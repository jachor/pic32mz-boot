#ifndef j_ports_h__
#define j_ports_h__

#include <stdint.h>
#include "lowlevel/memory.h"

PACKED_STRUCT
struct CombinedCommandRegister {
    volatile uint32_t reg;
    volatile uint32_t clr;
    volatile uint32_t set;
    volatile uint32_t inv;
};

PACKED_STRUCT
struct IoPortSfrs {
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

PACKED_STRUCT
struct IoPortPpsInputSfrs {
    /* base = 0xbf80_1400 */
    uint32_t __unused1;
    volatile uint32_t INT1R;
    volatile uint32_t INT2R;
    volatile uint32_t INT3R;
    volatile uint32_t INT4R;
    uint32_t __unused2;
    volatile uint32_t T2CKR;
    volatile uint32_t T3CKR;
    volatile uint32_t T4CKR;
    volatile uint32_t T5CKR;
    volatile uint32_t T6CKR;
    volatile uint32_t T7CKR;
    volatile uint32_t T8CKR;
    volatile uint32_t T9CKR;
    volatile uint32_t IC1R;
    volatile uint32_t IC2R;
    volatile uint32_t IC3R;
    volatile uint32_t IC4R;
    volatile uint32_t IC5R;
    volatile uint32_t IC6R;
    volatile uint32_t IC7R;
    volatile uint32_t IC8R;
    volatile uint32_t IC9R;
    uint32_t __unused3;
    volatile uint32_t OCFAR;
    uint32_t __unused4;
    volatile uint32_t U1RXR;
    volatile uint32_t U1CTSR;
    volatile uint32_t U2RXR;
    volatile uint32_t U2CTSR;
    volatile uint32_t U3RXR;
    volatile uint32_t U3CTSR;
    volatile uint32_t U4RXR;
    volatile uint32_t U4CTSR;
    volatile uint32_t U5RXR;
    volatile uint32_t U5CTSR;
    volatile uint32_t U6RXR;
    volatile uint32_t U6CTSR;
    uint32_t __unused5;
    volatile uint32_t SDI1R;
    volatile uint32_t SS1R;
    uint32_t __unused6;
    volatile uint32_t SDI2R;
    volatile uint32_t SS2R;
    uint32_t __unused7;
    volatile uint32_t SDI3R;
    volatile uint32_t SS3R;
    uint32_t __unused8;
    volatile uint32_t SDI4R;
    volatile uint32_t SS4R;
    uint32_t __unused9;
    volatile uint32_t SDI5R;
    volatile uint32_t SS5R;
    uint32_t __unused10;
    volatile uint32_t SDI6R;
    volatile uint32_t SS6R;
    volatile uint32_t C1RXR;
    volatile uint32_t C2RXR;
    volatile uint32_t REFCLKI1R;
    volatile uint32_t REFCLKI3R;
    volatile uint32_t REFCLKI4R;
};

PACKED_STRUCT
struct IoPortPpsOutputSfrs {
    /* base = 0xbf80_1500 */
    volatile uint32_t __unused1[0x38/4];
    volatile uint32_t RPA14R;
    volatile uint32_t RPA15R;
    volatile uint32_t RPB0R;
    volatile uint32_t RPB1R;
    volatile uint32_t RPB2R;
    volatile uint32_t RPB3R;
    uint32_t __unused2;
    volatile uint32_t RPB5R;
    volatile uint32_t RPB6R;
    volatile uint32_t RPB7R;
    volatile uint32_t RPB8R;
    volatile uint32_t RPB9R;
    volatile uint32_t RPB10R;
    uint32_t __unused3[4];
    volatile uint32_t RPB14R;
    volatile uint32_t RPB15R;
    // TODO: more regs
};

PACKED_STRUCT
struct IoPortPpsSfrs {
    union {
        struct IoPortPpsInputSfrs input;
        uint8_t __padding0[0x100];
    };
    union {
        struct IoPortPpsOutputSfrs output;
    };
};

PACKED_STRUCT
struct UartSfrs {
    struct CombinedCommandRegister MODE;
    struct CombinedCommandRegister STA;
    volatile uint32_t TXREG;
    uint32_t __padding0[3];
    volatile uint32_t RXREG;
    uint32_t __padding1[3];
    struct CombinedCommandRegister BRG;
};

PACKED_STRUCT
struct OscillatorConfigSfrs {
  // addr=0xbf80_1200
  struct CombinedCommandRegister OSCCON;
  struct CombinedCommandRegister OSCTUN;
  struct CombinedCommandRegister SPLLCON;
  uint8_t __padding0[0x50];
  struct CombinedCommandRegister REFO1CON;
  struct CombinedCommandRegister REFO1TRIM;
  struct CombinedCommandRegister REFO2CON;
  struct CombinedCommandRegister REFO2TRIM;
  struct CombinedCommandRegister REFO3CON;
  struct CombinedCommandRegister REFO3TRIM;
  struct CombinedCommandRegister REFO4CON;
  struct CombinedCommandRegister REFO4TRIM;
  struct CombinedCommandRegister PB1DIV;
  struct CombinedCommandRegister PB2DIV;
  struct CombinedCommandRegister PB3DIV;
  struct CombinedCommandRegister PB4DIV;
  struct CombinedCommandRegister PB5DIV;
  struct CombinedCommandRegister __padding1;
  struct CombinedCommandRegister PB7DIV;
  struct CombinedCommandRegister PB8DIV;
  uint8_t __padding2[0x40];
  struct CombinedCommandRegister SLEWCON;
  struct CombinedCommandRegister CLKSTAT;
};

PACKED_STRUCT
struct DeviceIdAndConfigSfrs {
  // addr=0xbf80_0000
  struct CombinedCommandRegister CFGCON;
  struct CombinedCommandRegister __padding0[1];
  struct CombinedCommandRegister DEVID;
  struct CombinedCommandRegister SYSKEY;
  struct CombinedCommandRegister __padding1[8];
  struct CombinedCommandRegister CFGEBIA;
  struct CombinedCommandRegister CFGEBIC;
  struct CombinedCommandRegister CFGPG;
};

PACKED_STRUCT
struct UsbControllerTxRxAddrSfrs {
  volatile uint32_t TXA;
  volatile uint32_t RXA;
};

PACKED_STRUCT
struct UsbControllerEndpointCsrSfrs {
  volatile uint32_t CSR0;
  volatile uint32_t CSR1;
  volatile uint32_t CSR2;
  volatile uint32_t CSR3;
};

PACKED_STRUCT
struct UsbControllerEndpointDmaSfrs {
  volatile uint32_t __pading;
  volatile uint32_t CONTROL;
  volatile uint32_t ADDRESS;
  volatile uint32_t COUNT;
};

PACKED_STRUCT
struct UsbControllerEndpointFifoSfrs {
  union {
    volatile uint32_t r32;
    struct { // doesn't work
      volatile uint8_t r8;
    };
    volatile uint8_t indexed8[4];
  };
};

PACKED_STRUCT
struct UsbControllerSfrs {
  // addr=0xbf8e_3000
  // Control Status Register
  volatile uint32_t USBCSR0;
  volatile uint32_t USBCSR1;
  volatile uint32_t USBCSR2;
  volatile uint32_t USBCSR3;
  // indexed endpoint CSR, index=USBCSR3<19:16>
  struct UsbControllerEndpointCsrSfrs USBIE_CSR;
  struct UsbControllerEndpointFifoSfrs USBEP_FIFO[16];
  volatile uint32_t USBOTG;
  volatile uint32_t USBFIFOA;
  uint32_t __padding0;
  volatile uint32_t USBHWVER;
  uint32_t __padding1[2];
  volatile uint32_t USBINFO;
  volatile uint32_t USBEOFRST;
  struct UsbControllerTxRxAddrSfrs USBEP_ADDR[16];
  struct UsbControllerEndpointCsrSfrs USBEP_CSR[16];

  // DMA
  union {
    volatile uint32_t USBDMAINT;
    struct UsbControllerEndpointDmaSfrs USBDMA[16];
  };

  volatile uint32_t USBEP_RPC[16];
  volatile uint32_t USBDPBFD;
  volatile uint32_t USBTMCON1;
  volatile uint32_t USBTMCON2;
  uint32_t __padding2[5];
  volatile uint32_t USBLPMR1;
  volatile uint32_t USBLPMR2;
};

PACKED_STRUCT
struct UsbControllerSfrs2 {
  // addr=0xbf88_4000
  volatile uint32_t USBCRCON;
};

#define PortA ((struct IoPortSfrs*)0xbf860000)
#define PortB ((struct IoPortSfrs*)0xbf860100)
#define PortC ((struct IoPortSfrs*)0xbf860200)
#define PortD ((struct IoPortSfrs*)0xbf860300)

#define PortPps ((struct IoPortPpsSfrs*)0xbf801400)

#define Uart1 ((struct UartSfrs*)0xbf822000)
#define Uart2 ((struct UartSfrs*)0xbf822200)
#define Uart3 ((struct UartSfrs*)0xbf822400)
#define Uart4 ((struct UartSfrs*)0xbf822600)
#define Uart5 ((struct UartSfrs*)0xbf822800)
#define Uart6 ((struct UartSfrs*)0xbf822a00)

#define OscillatorConfig ((struct OscillatorConfigSfrs*)0xbf801200)

#define DeviceIdAndConfig ((struct DeviceIdAndConfigSfrs*)0xbf800000)

#define UsbController ((struct UsbControllerSfrs*)0xbf8e3000)
#define UsbController2 ((struct UsbControllerSfrs2*)0xbf884000)

#endif
