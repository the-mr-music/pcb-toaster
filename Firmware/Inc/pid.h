
#ifndef __PID_H
#define __PID_H

#include "stm32f1xx_hal.h"

void pid_setKp(uint32_t value);
void pid_setKi(uint32_t value);
void pid_setKd(uint32_t value);
void pid_setTargetTemperature(uint16_t value);

uint16_t pid_getCurOutputPower(void);


void pid_init(void);
void pid_enable(void);
void pid_disable(void);
uint8_t pid_getStatus(void);
uint32_t pid_calculate(void);
uint16_t pid_getTargetTemperature(void);

uint32_t pid_getKp(void);
uint32_t pid_getKi(void);
uint32_t pid_getKd(void);
int32_t pid_getCurError(void);

#endif
