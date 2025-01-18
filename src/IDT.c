#include "IDT.h"
#include <stdint.h>
#include <stddef.h>

idt_entry_t idt[256];
idt_ptr_t idt_ptr;

// Extern declaration for assembly function
extern void idt_load();

void idt_init() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Initialize all IDT entries
    for (int i = 0; i < 256; i++) {
        idt_set_entry(i, 0, 0, 0); // Nullify all entries
    }

    idt_load(); // Load the IDT
}

void idt_set_entry(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}
