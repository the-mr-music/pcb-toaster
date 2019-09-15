
#ifndef __THERMOCOUPLE_H
#define __THERMOCOUPLE_H

#include "main.h"

void tc_init(void);
void tc_poll(void);
uint8_t tc_readTemperature(uint16_t *temp);
uint16_t tc_getAverageTemperature();


#endif
