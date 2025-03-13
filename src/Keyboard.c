#include "Keyboard.h"
#include "terminal.h"
#include "VGA.h"
#include "Utils.h"

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define BUFFER_SIZE 256

char input_buffer[BUFFER_SIZE];
size_t buffer_index = 0;

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

/* Declare process_command before using it */
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
