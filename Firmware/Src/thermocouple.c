
#include "thermocouple.h"

#include "main.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef hspi1;

const uint16_t calibration = 800;

const uint8_t nAvg = 4; // Numbers of samples per average

volatile uint16_t cTemp;
volatile uint16_t lastMeasurements[nAvg];
volatile uint8_t  mIndex;

void tc_init() {
  // Pull CS high, the MAX6675 only samples temperature
  // when CS is not asserted
  HAL_GPIO_WritePin(TC_CS_GPIO_Port, TC_CS_Pin, GPIO_PIN_SET);
  HAL_Delay(250); // Let the IC perform a measurement

  // Perform one temperature measurement and initialise the array
  if (tc_readTemperature(&cTemp) != 0 ) {
    cTemp = 0; // If the thermocouple is not connected or not working
               // Set the output temperature to 0
  }

  for (uint8_t i = 0; i < nAvg; i++)
    lastMeasurements[nAvg] = cTemp;

  mIndex = 0;
  return;
}

// Returns the temperature of the thermocouple in centidegrees
uint8_t tc_readTemperature(uint16_t *temp) {
  uint8_t raw[2];

  HAL_GPIO_WritePin(TC_CS_GPIO_Port, TC_CS_Pin, GPIO_PIN_RESET);
  if ( HAL_SPI_Receive(&hspi1, &raw, 2, HAL_MAX_DELAY) != HAL_OK )
    return 1;
  HAL_GPIO_WritePin(TC_CS_GPIO_Port, TC_CS_Pin, GPIO_PIN_SET);

  uint16_t tmp =  raw[0] << 8 | raw[1];

  // Validity check -> Bits 15 and 1 have to be zero
  // Because of the circuit the thermocouple detection
  // cannot be used, so this is the only option
  if ( (tmp & 0x8002) > 0 )
    return 1;

  // See datasheet -> Shifting to the right by 3 bits gets rid of
  // status bits. 12 bit ADC value -> all bit 1's is 1024.75 °C
  // -> 1 bit is 0.25 °C

  tmp = (tmp >> 3) * 25;
  tmp -= calibration;
  *temp = tmp;

  return 0;
}

// Call this function every 250 ms in task
void tc_poll() {
  uint16_t tmp;
  uint32_t sum;

  if (tc_readTemperature(&tmp) != 0 ) {
    // Error handling
    return;
  }

  lastMeasurements[mIndex] = tmp;
  mIndex++;
  if (mIndex >= nAvg)
    mIndex = 0;

  sum = 0;
  for (uint8_t i = 0; i < nAvg; i++)
    sum += lastMeasurements[nAvg];

  cTemp = (sum / nAvg);
  return;
}

uint16_t tc_getAverageTemperature() {
  return cTemp;
}


