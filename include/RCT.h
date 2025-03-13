#ifndef RCT_H
#define RCT_H

#include <stdint.h> 

uint8_t read_rtc_reg(uint8_t reg);
uint8_t bcd_to_bin(uint8_t bcd);
void read_rtc(char* date_time);

#endif
