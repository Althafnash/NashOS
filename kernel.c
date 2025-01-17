#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "utils.h" 

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with an ix86-elf compiler"
#endif

#define BUFFER_SIZE 256

char input_buffer[BUFFER_SIZE];
size_t buffer_index = 0;

/* Hardware text mode color constants. */
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll(void) {
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
    }
    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
        }
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_putentryat(' ', terminal_color, --terminal_column, terminal_row);
        } else if (terminal_row > 0) {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
            terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
        }
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();
            }
        }
    }
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t keyboard_read_scan_code(void) {
    while (!(inb(KEYBOARD_STATUS_PORT) & 0x01));
    return inb(KEYBOARD_DATA_PORT);
}

static const char keymap[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ',
};

char scan_code_to_char(uint8_t scan_code) {
    return keymap[scan_code];
}

void process_command(const char* command);
int custom_snprintf(char* buffer, size_t size, const char* format, ...);
char* itoa(int value, char* str, int base);

void keyboard_handler(void) {
    uint8_t scan_code = keyboard_read_scan_code();
    if (!(scan_code & 0x80)) { // Key press
        char character = scan_code_to_char(scan_code);
        if (character != 0) {
            if (character == '\b' && buffer_index > 0) {
                // Handle backspace
                buffer_index--;
                input_buffer[buffer_index] = '\0';
                terminal_writestring("\b \b"); // Erase the last character on screen
            } else if (character == '\n') {
                // Process the command
                terminal_putchar(character);
                process_command(input_buffer);
                // Clear the buffer
                buffer_index = 0;
                input_buffer[0] = '\0';
                terminal_writestring("$NASHOS$- : ");
            } else if (character != '\b') {
                // Add the character to the buffer
                if (buffer_index < BUFFER_SIZE - 1) {
                    input_buffer[buffer_index++] = character;
                    input_buffer[buffer_index] = '\0'; // Null-terminate the string
                }
                // Display the character
                terminal_putchar(character);
            }
        }
    }
}

uint8_t read_rtc_reg(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}

uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd & 0x0F) + (bcd / 16) * 10);
}

void read_rtc(char* date_time) {
    uint8_t second = read_rtc_reg(0x00);
    uint8_t minute = read_rtc_reg(0x02);
    uint8_t hour = read_rtc_reg(0x04);
    uint8_t day = read_rtc_reg(0x07);
    uint8_t month = read_rtc_reg(0x08);
    uint8_t year = read_rtc_reg(0x09);

    second = bcd_to_bin(second);
    minute = bcd_to_bin(minute);
    hour = bcd_to_bin(hour);
    day = bcd_to_bin(day);
    month = bcd_to_bin(month);
    year = bcd_to_bin(year);

    // Custom snprintf implementation for compatibility
    custom_snprintf(date_time, 30, "Date: %02d/%02d/20%02d Time: %02d:%02d:%02d",
                    day, month, year, hour, minute, second);
}

void date_command() {
    char date_time[30];
    read_rtc(date_time);
    terminal_writestring(date_time);
    terminal_writestring("\n");
    terminal_writestring("\n");
}

void welcome() {
    terminal_writestring("\n");
    terminal_writestring("\n");
    terminal_writestring("WELCOME TO NASHOS\n");
    terminal_writestring("\n");
    terminal_writestring("NashOS is a custom-built operating system developed by Althaf\n");
    terminal_writestring("\n");
    terminal_writestring("-_- -_-");
    terminal_writestring("\n");
    terminal_writestring("\n");
}

static inline void outw(uint16_t port, uint16_t val) {
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void process_command(const char* command) {
    if (strncmp(command, "help", 4) == 0 && (command[4] == '\0' || command[4] == ' ')) {
        welcome();
    } else if (strncmp(command, "cls", 3) == 0 && (command[3] == '\0' || command[3] == ' ')) {
        terminal_initialize();
    } else if (strncmp(command, "echo ", 5) == 0) {
        terminal_writestring(command + 5);  // Print the string following the "echo " command
        terminal_writestring("\n");
    } else if (strncmp(command, "date", 4) == 0 && (command[4] == '\0' || command[4] == ' ')) {
        date_command();
    } else if (strncmp(command, "shutdown", 8) == 0 && (command[8] == '\0' || command[8] == ' ')) {
        outw(0x604, 0x2000);
    } else {
        terminal_writestring("\nUnknown command\n");
    }
}


void kernel_main(void) {
    terminal_initialize();
    terminal_writestring("$NASHOS$-{{Version:Beta}} : ");
    while (true) {
        keyboard_handler();
    }
}
