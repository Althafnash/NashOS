#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_init();

void pic_send_eio(uint8_t irq);

#endif 