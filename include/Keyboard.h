#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stddef.h>
#include <stdint.h>

static inline uint8_t inb(uint16_t port);
static inline void outb(uint16_t port, uint8_t val);
uint8_t keyboard_read_scan_code(void);
char scan_code_to_char(uint8_t scan_code);
void keyboard_handler(void);

#endif
