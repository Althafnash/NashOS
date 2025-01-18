#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

char* custom_strncpy(char* dest, const char* src, size_t n);
int custom_snprintf(char* buffer, size_t size, const char* format, ...);
char* itoa(int value, char* str, int base);
size_t strlen(const char* str);
int strncmp(const char *str1, const char *str2, size_t num);

#endif /* UTILS_H */
