#include "lowlevel/printk.h"
#include "lowlevel/ports.h"
#include "lowlevel/simple_printf.h"
#include <stdarg.h>

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

static void uart_blocking_write(struct UartSfrs *uart, uint8_t data) {
    // wait for space in TX buffer
    while (uart->STA.reg & (1<<9)) {}
    uart->TXREG = data;
}

void printk_init() {
    // RPB2 -> UART TX
    PortPps->output.RPB2R = 0x2; /* Uart2 TX */
    PortB->TRIS.clr = 1<<2;
    PortB->ODC.clr = 1<<2;

    uart_init(Uart2, 8000000 / 2, 9600);
}

static void _debug_putc(const char z) {
    uart_blocking_write(Uart2, z);
}

void printk(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    simple_print_formatted(_debug_putc, fmt, args);
    va_end(args);
}
