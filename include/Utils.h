#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

int strncmp(const char *str1, const char *str2, size_t num);
size_t strlen(const char* str);
char* custom_strncpy(char* dest, const char* src, size_t n);
int custom_snprintf(char* buffer, size_t size, const char* format, ...);
char* itoa(int value, char* str, int base);

// Inline I/O functions
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif
