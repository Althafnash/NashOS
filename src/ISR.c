#include "ISR.h"
#include "PIC.h"
#include "IDT.h"

void isr_handler()
{
    pic_send_eio(0);
}

void isr_init()
{
    idt_set_entry(0, (uint32_t)isr_handler, 0x08, 0x8E);  
}