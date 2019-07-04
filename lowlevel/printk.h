#ifndef lowlevel_printk_h__
#define lowlevel_printk_h__

//#define PRINTK_UART_BAUDRATE (9600)
#define PRINTK_UART_BAUDRATE (115200)
//#define PRINTK_UART_BAUDRATE (800000)

extern void printk_init();
extern void printk_flush();
extern void printk(const char *fmt, ...);

#endif
