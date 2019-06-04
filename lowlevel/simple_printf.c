#include "lowlevel/simple_printf.h"
#include <stdint.h>

void simple_print_formatted(append_func_t append, const char *fmt, va_list args) {    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            
            char leading_zero = 0;
            char num_digits = 0;
            char len_mods = 0;
            
            char mod_done = 0;
            while (*fmt && !mod_done) {
                switch (*fmt) {
                    case '0':
                        if (num_digits==0) {
                            leading_zero = 1;
                        }
                        // fallthrough intended
                    case '1': case '2': case '3': case '4': case '5':
                    case '6': case '7': case '8': case '9':
                        num_digits = num_digits * 10 + (*fmt - '0');
                        break;

                    case 'l': len_mods++; break;
                    default: mod_done=1; continue;
                }
                fmt++;
            }
            
            char op = *fmt;
            switch (op) {
            case 'u': // decimal, unsigned
            case 'd': // decimal, signed
            case 'x': // hex
            {
                // only int supported for now
                int signed_val = va_arg(args, int);
                
                char sign = 0;
                uint32_t unsigned_val;
                if (op == 'd') {
                    if (signed_val < 0) {
                        sign = '-';
                        unsigned_val = -signed_val;
                    } else {
                        // todo: handle optional '+'?
                        unsigned_val = signed_val;
                    }
                } else {
                    unsigned_val = signed_val;
                }
                
                // produce number in buffer
                char buffer[40];
                char *ptr = buffer + 0;
                int div = (op == 'x')?16:10;
                do {
                    *(ptr++) = "0123456789ABCDEF"[unsigned_val%div];
                    unsigned_val /= div;
                } while(unsigned_val);
                int num_len = ptr-buffer;
                
                // figure out length of padding
                int padding = num_digits - num_len;
                if (sign) padding--;
                
                // leading sign
                if (sign && leading_zero)
                    append('-');
                
                // padding
                while (padding-- > 0) {
                    append(leading_zero ? '0' : ' ');
                }
                
                // optional sign
                if (sign && !leading_zero)
                    append('-');
                
                // number
                while (ptr > buffer) {
                    append(*(--ptr));
                }
                break;
            }
            case 's': // string
            {
                // TODO: padding?
                const char *str = va_arg(args, const char *);
                while (*str) {
                    append(*(str++));
                }
                break;
            }
            case 'c':
            {
                int z = va_arg(args, int);
                append((char)z);
                break;
            }
            case '%': // escape
                append('%');
            default: // unsupported, eos
                continue;
            }
        } else {
            append(*fmt);
        }
        
        fmt++;
    }    
}
