#include "lowlevel/clock.h"
#include "lowlevel/printk.h"
#include "lowlevel/ports.h"
#include "lowlevel/memory.h"

#define FRC_CLOCK_HZ (8 * 1000000)
#define POSC_CLOCK_HZ (12 * 1000000)

static int _pll_hz;
static int _sysclk_hz = FRC_CLOCK_HZ;

/* peripheral clock dividers (sysclk/x) */
const int _pb_divider[8+1] = {
  0, /* PB0 --> N/A */
  2, /* PB1 --> Flash, Deadman, Watchdog */
  2, /* PB2 --> UART, I2C, PMP */
  2, /* PB3 --> ADC, Timers */
  2, /* PB4 --> PORTS */
  2, /* PB5 --> Crypro, Rng, USB */
  0, /* PB6 --> N/A? */
  1, /* PB7 --> CPU */
  2, /* PB8 --> EBI */
};


int clock_get_peripheral_bus_clock_hz(int index) {
  if (index < 0 || index >= (sizeof(_pb_divider)/sizeof(_pb_divider[0])))
    return 0;
  return _sysclk_hz / _pb_divider[index];
}

static void _unlock_syskey() {
  DeviceIdAndConfig->SYSKEY.reg = 0xaa996655;
  DeviceIdAndConfig->SYSKEY.reg = 0x556699aa;
}

static void _lock_syskey() {
  DeviceIdAndConfig->SYSKEY.reg = 0x0;
}

static void _wait_for_primary_oscilator_lock() {
  while (!(OscillatorConfig->CLKSTAT.reg & (1<<2))) {}
}

static void _wait_for_pll_lock() {
  while (!(OscillatorConfig->CLKSTAT.reg & (1<<1))) {}
}

static void _setup_pll() {
  _unlock_syskey();
  // input = 12MHz
  int idiv = 1; // input divider, 1-8
  int iclk = 0; // 0=use primary osc, 1=use frc
  int range = 2; // pll input range, 1->5-10, 2->8-16
  // pllinput = 12MHz
  int mult = 33; // multiplier, 1-128
  // plloutput = 396MHz
  int odiv = 1; // output divider, 1->2, 2->4, 3->8, 4->16.
  // out = 198Mhz
  OscillatorConfig->SPLLCON.reg = 0
      | (odiv<<24)
      | ((mult-1)<<16)
      | ((idiv-1)<<8)
      | (iclk<<7)
      | (range<<0)
      | 0;
  _pll_hz = (iclk?FRC_CLOCK_HZ:POSC_CLOCK_HZ) / idiv * mult / (1<<odiv);
  _lock_syskey();
}

static void _switch_to_pll() {
  int new_source = 1; // SPLL
  _unlock_syskey();
  OscillatorConfig->OSCCON.reg = new_source<<8; // new source=SPLL
  OscillatorConfig->OSCCON.reg = (new_source<<8) | (1<<0); // new source=SPLL, commit
  _lock_syskey();
  // wait for OSWEN=0
  while(OscillatorConfig->OSCCON.reg & (1<<0)) {}
  _sysclk_hz = _pll_hz;
}

void clock_init() {
  _wait_for_primary_oscilator_lock();
  _setup_pll();
  _switch_to_pll();
  _wait_for_pll_lock();
}

static int _clock_usec_to_ticks(int usec) {
  // counter counts at _sysclk_hz / 2.
  // NOTE: This only works because _sysclk_hz is integer MHz. I don't want to
  // implement __divdi and friends yet.
  return usec * (_sysclk_hz / 1000000) / 2;
}

void clock_sleep_usec(int usec) {
  if (usec < 0) usec = 0;
  uint32_t target = read_c0_reg(9,0) + _clock_usec_to_ticks(usec);
  while ((int)(read_c0_reg(9,0) - target) < 0) ;
}

