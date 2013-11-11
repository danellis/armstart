#ifndef DEBUG_H
#define DEBUG_H

void debug_init(void);
void debug_putc(char c);
void debug_puts(const char *s);
void debug_putptr(void *ptr);
void debug_put8(uint8_t value);

#endif
