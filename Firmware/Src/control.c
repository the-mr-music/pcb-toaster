
#include "control.h"
#include "main.h"
#include "cmsis_os.h"
#include "pid.h"
#include "thermocouple.h"
#include "oled.h"

extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;


// State Machine

#define STATE_IDLE 0
#define STATE_HEAT 1
#define STATE_COOL 2
#define STATE_SETTINGS 3

volatile uint8_t state;

// Time elapsed in 1/10s
volatile uint16_t sTime;
volatile uint8_t  uBuffer[32];
volatile uint8_t btnStateOld;
volatile uint8_t btnState;
volatile uint32_t aval;

void StartControlTask(void const * argument) {

    state = STATE_IDLE;

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 20; // 20 Ticks is 200 ms
    xLastWakeTime = xTaskGetTickCount();

    char displaybuf[32];
    uint32_t kp, ki, kd;
    uint16_t temp, fracTemp;
    uint8_t setpointIndex;

    int32_t pidErrorOld;
    int32_t pidError;

    btnStateOld = 0;
    uint32_t delaycount;

    // If continue is used -> no sleep
    // If break is used -> delay routine kicks in

    while(1) {
        //HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_SET);

        switch(state) {
            case STATE_IDLE:
                u8g2_ClearBuffer(&u8g2);
                u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
                u8g2_DrawStr(&u8g2, 55, 15, "Idle");
                temp = tc_getAverageTemperature();
                fracTemp = temp;
                temp /= 100;
                fracTemp -= temp * 100;
                sprintf(displaybuf, "CTemp %d.%d C", temp, fracTemp);
                u8g2_DrawStr(&u8g2, 0, 30, displaybuf);
                u8g2_SendBuffer(&u8g2);

                btnState = HAL_GPIO_ReadPin(BTN_START_GPIO_Port,BTN_START_Pin);
                if ( btnState == btnStateOld )
                    break;

                if ( btnStateOld == 0 && btnState == 1 ) {
                    btnStateOld = btnState;
                    break;
                }

                btnStateOld = btnState;

                kp = pid_getKp();
                ki = pid_getKi();
                kd = pid_getKd();

                uBuffer[0]  = 0xAA; // ID -> Is temperature log start
                uBuffer[1]  = (kp >> 24) & 0xFF;
                uBuffer[2]  = (kp >> 16) & 0xFF;
                uBuffer[3]  = (kp >> 8) & 0xFF;
                uBuffer[4]  =  kp & 0xFF;
                uBuffer[5]  = (ki >> 24) & 0xFF;
                uBuffer[6]  = (ki >> 16) & 0xFF;
                uBuffer[7]  = (ki >> 8) & 0xFF;
                uBuffer[8]  =  ki & 0xFF;
                uBuffer[9]  = (kd >> 24) & 0xFF;
                uBuffer[10] = (kd >> 16) & 0xFF;
                uBuffer[11] = (kd >> 8) & 0xFF;
                uBuffer[12] =  kd & 0xFF;
                HAL_UART_Transmit_IT(&huart1, &uBuffer, 13);

                // Before switching to heat initialise the solder timer
                sTime = 0;
                setpointIndex = 0;
                pidErrorOld = 0;
                delaycount = 600;
                pid_setTargetTemperature(tc_getAverageTemperature()/10);
                state = STATE_HEAT;
                pid_enable();
            break;

            case STATE_HEAT:
                HAL_ADC_Start(&hadc1);
                if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) {
                    aval = HAL_ADC_GetValue(&hadc1);
                }

                if (aval < 2000) {
                    pid_disable();
                    state = STATE_IDLE;
                    break;
                }

                // If needed adjust PID
                if ( (sTime % 10) == 0 ) {
                    uint16_t cSec = sTime / 10;
                    // If there is a temperature for the current time apply it
                    if ( cSec < (sizeof(tempProfile) / sizeof(tempProfile[0])) ) {
                        pid_setTargetTemperature(tempProfile[cSec]);
                    } else {
                        pid_disable();
                        state = STATE_IDLE;
                        break;
                    }
                    HAL_GPIO_TogglePin(GPIOB, LED2_Pin);
                }


                /*pidError = pid_getCurError();
                delaycount++;
                if ( (pidError != pidErrorOld) && (pidError < 0) && (delaycount > 149)) {
                    pidErrorOld = pidError;
                    pid_setTargetTemperature(pid_getTargetTemperature() + 500);
                    delaycount = 0;
                }*/

                // Log values
                uBuffer[0] = 0xAB; // ID -> Is temperature log
                uBuffer[1] = (sTime >> 8) & 0xFF;
                uBuffer[2] = sTime & 0xFF;
                temp = pid_getTargetTemperature();
                uBuffer[3] = (temp >> 8) & 0xFF;
                uBuffer[4] = temp & 0xFF;
                temp = tc_getAverageTemperature();
                uBuffer[5] = (temp >> 8) & 0xFF;
                uBuffer[6] = temp & 0xFF;
                uBuffer[7] = pid_getCurOutputPower() & 0xFF;

                HAL_UART_Transmit_IT(&huart1, &uBuffer, 8);

                // OLED
                u8g2_ClearBuffer(&u8g2);
                u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);
                temp = tc_getAverageTemperature();
                fracTemp = temp;
                temp /= 100;
                fracTemp -= temp * 100;
                sprintf(displaybuf, "CTemp %d.%d C", temp, fracTemp);
                u8g2_DrawStr(&u8g2, 0, 15, displaybuf);

                // Display Target Temperature
                temp = pid_getTargetTemperature();
                fracTemp = temp;
                temp /= 10;
                fracTemp -= temp * 10;
                sprintf(displaybuf, "TTemp %d.%d C", temp, fracTemp);
                u8g2_DrawStr(&u8g2, 0, 30, displaybuf);

                //sprintf(displaybuf, "%d.%d C - %d %%", temp, fracTemp, pid_getCurOutputPower());
                sprintf(displaybuf, "Time %d", sTime);
                u8g2_DrawStr(&u8g2, 0, 45, displaybuf);
                u8g2_SendBuffer(&u8g2);

                sprintf(displaybuf, "Power %d %%", pid_getCurOutputPower());
                u8g2_DrawStr(&u8g2, 0, 64, displaybuf);
                u8g2_SendBuffer(&u8g2);

                sTime += 2; // Add 200 ms to the timer
            break;

            default:
                // If an illegal state would occur, revert back to idle
                state = STATE_IDLE;
                pid_disable();
        }

        //HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_RESET);
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }

    return;
}


/*
      uint8_t cbuf[8];
      char displaybuf[24];
      uint16_t temp = tc_getAverageTemperature();
      uint32_t ctime = HAL_GetTick()      cbuf[0] = (ctime >> 24) & 0xFF;
      cbuf[1] = (ctime >> 16) & 0xFF;
      cbuf[2] = (ctime >> 8) & 0xFF;
      cbuf[3] = (ctime & 0xFF);
      uint8_t t_msb = temp >> 8;
      cbuf[4] = t_msb & 0xFF; //(char) (temp >> 8) & 0xFF;
      cbuf[5] = temp & 0xFF;
      cbuf[6] = pid_getCurOutputPower() & 0xFF;
      cbuf[7] = '\n';

      HAL_UART_Transmit_IT(&huart1, &cbuf, 8);

      u8g2_ClearBuffer(&u8g2);
      u8g2_SetFont(&u8g2, u8g2_font_helvB10_tr);

      uint16_t fracTemp = temp;
      temp /= 100;
      fracTemp -= temp * 100;
      sprintf(displaybuf, "%d.%d C - %d %%", temp, fracTemp, pid_getCurOutputPower());
      u8g2_DrawStr(&u8g2, 0, 64, displaybuf);

      uint32_t aval = 0;

      HAL_ADC_Start(&hadc1);
      if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) {
        aval = HAL_ADC_GetValue(&hadc1);
      }

      if (aval > 2000 && pid_getStatus() == 0)
          pid_enable();

      if (aval < 1000 && pid_getStatus() == 1)
          pid_disable();

      sprintf(displaybuf, "%d", aval);
      u8g2_DrawStr(&u8g2, 0, 32, displaybuf);

      if (pid_getStatus() == 0)
        u8g2_DrawStr(&u8g2, 40, 15, "PID off");
      else
        u8g2_DrawStr(&u8g2, 40, 15, "PID on");


      u8g2_SendBuffer(&u8g2);

      vTaskDelayUntil( &xLastWakeTime, xFrequency );
*/

