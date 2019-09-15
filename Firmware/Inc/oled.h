
#ifndef __OLED_H
#define __OLED_H

#include "u8g2.h"
#include "stm32f1xx_hal.h"

volatile u8g2_t u8g2;

void oled_init(void);

#endif