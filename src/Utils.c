#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "Utils.h"

int strncmp(const char *str1, const char *str2, size_t num) {
    while (num && *str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
        --num;
    }
    if (num == 0) {
        return 0;
    } else {
        return *(unsigned char *)str1 - *(unsigned char *)str2;
    }
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

char* custom_strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for ( ; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
}

int custom_snprintf(char* buffer, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int length = 0;

    while (*format && length < (int)size - 1) {
        if (*format == '%' && *(format + 1)) {
            format++;
            switch (*format) {
                case 'd': {
                    int value = va_arg(args, int);
                    char temp[20];
                    itoa(value, temp, 10);
                    custom_strncpy(buffer + length, temp, size - length - 1);
                    length += strlen(temp);
                    break;
                }
                case 's': {
                    char* value = va_arg(args, char*);
                    custom_strncpy(buffer + length, value, size - length - 1);
                    length += strlen(value);
                    break;
                }
                case 'c': {
                    char value = (char)va_arg(args, int);
                    buffer[length++] = value;
                    break;
                }
                case '%': {
                    buffer[length++] = '%';
                    break;
                }
                default:
                    break;
            }
        } else {
            buffer[length++] = *format;
        }
        format++;
    }

    buffer[length] = '\0';
    va_end(args);
    return length;
}


// Custom itoa implementation
char* itoa(int value, char* str, int base) {
    char* rc;
    char* ptr;
    char* low;
    // Set the pointer to the end of the string
    ptr = str;
    if (value < 0 && base == 10) {
        value = -value;
        *ptr++ = '-';
    }
    rc = ptr;
    // Set the last character to be null
    *ptr = '\0';

    // Convert the number to string
    do {
        *ptr++ = "0123456789abcdef"[value % base];
        value /= base;
    } while (value);

    // Save the low address
    low = *rc == '-' ? rc + 1 : rc;
    // Reverse the string
    for (ptr--; low < ptr; low++, ptr--) {
        char tmp = *low;
        *low = *ptr;
        *ptr = tmp;
    }

    return rc;
}
