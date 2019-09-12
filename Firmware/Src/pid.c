
#include "pid.h"
#include "main.h"
#include "thermocouple.h"

extern SPI_HandleTypeDef hspi1;

// Fixed comma calculations -> 1000 = 1.0
volatile uint32_t kp, ki, kd;  // Coefficients
volatile int32_t ki_acc;       // Accumulated Integral Value

volatile uint16_t targetTemp;
volatile uint32_t prevTemp;

// Maximum output range
const uint16_t p_IronMin = 0;
const uint16_t p_IronMax = 100;

const int32_t out_ul = p_IronMax * 1000;
const int32_t out_ll = p_IronMin * 1000;

// For statistics, can only be accessed readonly by functions
volatile uint16_t cur_outputPower = 0;
volatile uint16_t cur_ironTemperature = 0;

void pid_setKp(uint32_t value) {
  kp = value;
}

void pid_setKi(uint32_t value) {
  ki = value;
}

void pid_setKd(uint32_t value) {
  kd = value;
}

void pid_setTargetTemperature(uint16_t value) {
  targetTemp = value;
}

uint16_t pid_getCurOutputPower() {
    return cur_outputPower;
}

void pid_init() {
    tc_readTemperature(&cur_ironTemperature);
    prevTemp = cur_ironTemperature;
    prevTemp /= 10;
    // Not ki_acc = output, maybe if necessary
    kp = 150;
    ki = 1;
    kd = 0;
    ki_acc = 0;
    targetTemp = 2000;
    return;
}

uint32_t pid_calculate() {
    uint16_t measTemp;
    int32_t out;

    //tc_readTemperature(&measTemp);
    measTemp = rc_getAverageTemperature();
    measTemp /= 10;

    int32_t error = targetTemp - measTemp;
    ki_acc += ki * error;

    if( ki_acc > out_ul)
        ki_acc = out_ul;
    else
        if( ki_acc < out_ll)
            ki_acc = out_ll;

    int16_t dTemp = measTemp - prevTemp;
    out = kp * error + ki_acc - kd * dTemp;

    if( out > out_ul)
        out = out_ul;
    else
        if( out < out_ll)
            out = out_ll;

    prevTemp = measTemp;
    cur_outputPower = out / 1000;
    return cur_outputPower;
}

