#include "terminal.h"
#include "VGA.h"
#include "RCT.h"
#include "Keyboard.h"
#include "Utils.h"
#include "ISR.h"
#include "PIC.h"
#include "IDT.h"
#include <stddef.h>
#include <stdbool.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with an ix86-elf compiler"
#endif

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
    pic_init();
    idt_init();
    isr_init();
    terminal_initialize();
    terminal_writestring("$NASHOS$- : ");
    while (true) {
        keyboard_handler();
    }
}
