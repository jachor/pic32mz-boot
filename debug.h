#ifndef j_debug_h__
#define j_debug_h__

#include <stdint.h>

extern void debug_init();
extern void debug_print(const char*);
extern void debug_printf(const char *fmt, ...);

#endif
