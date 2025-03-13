#include "RCT.h"
#include "Utils.h"

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
