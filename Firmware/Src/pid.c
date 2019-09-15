
#include "pid.h"
#include "main.h"
#include "thermocouple.h"

extern SPI_HandleTypeDef hspi1;

// Fixed comma calculations -> 1000 = 1.0
volatile uint32_t kp, ki, kd;  // Coefficients
volatile int32_t ki_acc;       // Accumulated Integral Value

volatile uint16_t targetTemp;
volatile uint32_t prevTemp;
volatile int32_t curError;

// Maximum output range
const uint16_t p_IronMin = 0;
const uint16_t p_IronMax = 100;

const int32_t out_ul = p_IronMax * 10000;
const int32_t out_ll = p_IronMin * 10000;

// For statistics, can only be accessed readonly by functions
volatile uint16_t cur_outputPower = 0;

volatile uint8_t enabled = 0;

void pid_setKp(uint32_t value) {
  kp = value;
}

void pid_setKi(uint32_t value) {
  ki = value;
}

void pid_setKd(uint32_t value) {
  kd = value;
}

uint32_t pid_getKp() {
  return kp;
}

uint32_t pid_getKi() {
  return ki;
}

uint32_t pid_getKd() {
  return kd;
}

int32_t pid_getCurError() {
  return curError;
}

void pid_setTargetTemperature(uint16_t value) {
  targetTemp = value;
}

uint16_t pid_getTargetTemperature() {
  return targetTemp;
}

uint16_t pid_getCurOutputPower() {
    return cur_outputPower;
}

void pid_init() {
    uint16_t cur_ironTemperature;
    tc_readTemperature(&cur_ironTemperature);
    prevTemp = cur_ironTemperature;
    prevTemp /= 10;
    // Not ki_acc = output, maybe if necessary
    kp = 10000;
    ki = 10;
    kd = 0; //10000;

    ki_acc = 0;
    targetTemp = 1500;
    enabled = 0;
    return;
}

void pid_enable() {
  enabled   = 1;
  prevTemp  = tc_getAverageTemperature();
  prevTemp /= 10;
  // Maybe also ki_acc = out
  ki_acc = 0;
}

void pid_disable() {
  enabled = 0;
}

uint8_t pid_getStatus() {
  return enabled;
}

uint32_t pid_calculate() {
    if (enabled == 0) {
      cur_outputPower = 0;
      return 0;
    }

    uint16_t measTemp;
    int32_t out;

    measTemp = tc_getAverageTemperature();
    measTemp /= 10;

    int32_t error = targetTemp - measTemp;
    curError = error;
    ki_acc += ki * error;

    if( ki_acc > out_ul)
        ki_acc = out_ul;
    else
        if( ki_acc < out_ll)
            ki_acc = out_ll;

    int32_t dTemp = measTemp - prevTemp;
    out = kp * error + ki_acc - kd * dTemp;

    if( out > out_ul)
        out = out_ul;
    else
        if( out < out_ll)
            out = out_ll;

    prevTemp = measTemp;
    cur_outputPower = out / 10000;
    return cur_outputPower;
}

