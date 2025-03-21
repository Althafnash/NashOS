#ifndef IDT_H
#define IDT_H

#include<stdint.h>

typedef struct 
{
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
}__attribute__((packed)) idt_entry_t;

typedef struct 
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) idt_ptr_t;

extern idt_entry_t idt[256];
extern idt_ptr_t idt_ptr;

void idt_init();
void idt_set_entry(int num, uint32_t base , uint16_t sel, uint8_t flags);

#endif
