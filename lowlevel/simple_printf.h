#ifndef lowlevel_simple_printf_h__
#define lowlevel_simple_printf_h__

#include <stdarg.h>
 
typedef void (*append_func_t)(char);
void simple_print_formatted(append_func_t append, const char *fmt, va_list args);

#endif
