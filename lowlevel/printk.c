#include "lowlevel/printk.h"
#include "lowlevel/clock.h"
#include "lowlevel/ports.h"
#include "lowlevel/simple_printf.h"
#include <stdarg.h>

#define buffer_size (1024 * 16)
static struct {
    int head, tail;
    char data[buffer_size];
} buffer;

static void uart_init(struct UartSfrs *uart, uint32_t pbclk2_freq, uint32_t baudrate) {
  uint32_t brg = pbclk2_freq / (4 * baudrate) - 1;
  uart->BRG.reg = brg;
  // uart disabled, high speed mode, no parity, 1 stop bit
  uart->MODE.reg = 0 | 1<<3;
  // reset, disable tx and rx
  uart->STA.reg = 0;
  uart->STA.set = 1<<10; // transmitter enabled
  uart->MODE.set = 1<<15; // uart enabled
}

static char uart_is_tx_ready(struct UartSfrs *uart) {
    return !(uart->STA.reg & (1<<9));
}

static void uart_blocking_write(struct UartSfrs *uart, uint8_t data) {
  // wait for space in TX buffer
  while (!uart_is_tx_ready(uart)) {}
  uart->TXREG = data;
}

static void uart_blocking_flush(struct UartSfrs *uart) {
  // wait for Transmit shift register is empty bit
  while (!(uart->STA.reg & (1<<8))) {}
}

void printk_init() {
  // RPB2 -> UART TX
  PortPps->output.RPB2R = 0x2; /* Uart2 TX */
  PortB->TRIS.clr = 1<<2;
  PortB->ODC.clr = 1<<2;

  uart_init(Uart2, clock_get_peripheral_bus_clock_hz(2), PRINTK_UART_BAUDRATE);

  buffer.head = buffer.tail = 0;
}

static void _debug_putc(const char z) {
  //uart_blocking_write(Uart2, z); return;
  
  int ntail = (buffer.tail + 1) % buffer_size;
  if (ntail == buffer.head) {
      // overrun
      return;
  }
  buffer.data[buffer.tail] = z;
  buffer.tail = ntail;
}

void printk(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  simple_print_formatted(_debug_putc, fmt, args);
  va_end(args);
}

void printk_flush() {
  uart_blocking_flush(Uart2);
}

void printk_loop() {
    while(buffer.head != buffer.tail && uart_is_tx_ready(Uart2)) {
        uart_blocking_write(Uart2, buffer.data[buffer.head]);
        buffer.head = (buffer.head + 1) % buffer_size;
    }
}

