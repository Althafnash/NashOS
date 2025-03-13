// pic.c
#include "PIC.h"
#include <stdint.h>
#include "Utils.h"

// Initialize the PICs by sending the remapping commands
void pic_init() {
    // Remap the PICs to use IRQs starting from 0x20
    outb(0x20, 0x11);  // ICW1: Initialize the PIC
    outb(0xA0, 0x11);  // Slave PIC needs initialization

    outb(0x21, 0x20);  // ICW2: Master PIC offset
    outb(0xA1, 0x28);  // ICW2: Slave PIC offset

    outb(0x21, 0x04);  // ICW3: Master PIC connects to slave on IRQ2
    outb(0xA1, 0x02);  // ICW3: Slave PIC is connected to IRQ2

    outb(0x21, 0x01);  // ICW4: 8086 mode
    outb(0xA1, 0x01);  // ICW4: 8086 mode

    outb(0x21, 0x00);  // Mask all interrupts on the master PIC
    outb(0xA1, 0x00);  // Mask all interrupts on the slave PIC
}

// Send an EOI (End Of Interrupt) signal to the PIC
void pic_send_eio(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20);  // Send EOI to slave PIC
    }
    outb(0x20, 0x20);  // Send EOI to master PIC
}
