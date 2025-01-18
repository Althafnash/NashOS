; idt_load.asm
; Assembly function to load the IDT

section .text
global idt_load       ; Export the `idt_load` function for external linkage
extern idt_ptr        ; Declare the `idt_ptr` symbol, which is defined in C

idt_load:
    lidt [idt_ptr]    ; Load the IDT pointer into the IDTR register
    ret               ; Return to the caller
