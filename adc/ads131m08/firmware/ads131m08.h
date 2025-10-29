/*
 * ads131m08.h
 *
 *  Created on: May 13, 2025
 *      Author: AsharLatif
 */

#ifndef INC_ADS131M08_H_
#define INC_ADS131M08_H_


#define STM32F1

#if defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined (STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#endif

#define ADS131M08_NUM_CHANNELS 8

// Register addresses
#define ADS131M08_REG_ID         0x00
#define ADS131M08_REG_STATUS     0x01
#define ADS131M08_REG_MODE       0x02
#define ADS131M08_REG_CLOCK      0x03
#define ADS131M08_REG_GAIN1      0x04
#define ADS131M08_REG_GAIN2      0x05
#define ADS131M08_REG_CONFIG     0x06

#define ADS131M08_READ_REG_CMD   0xA000
#define ADS131M08_WRITE_REG_CMD  0x6000
// Add other registers as needed

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef *rst_port;
    uint16_t rst_pin;
    uint8_t drdy_pin;
    uint8_t dev_num;
    int32_t raw_voltage[8];
    float conv_voltage[8];
    float vref;  // Reference voltage
    uint8_t pga;
} ADS131M08_HandleTypeDef;

// Function prototypes
HAL_StatusTypeDef ADS131M08_Init(ADS131M08_HandleTypeDef *hads, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin, float vref);
HAL_StatusTypeDef ADS131M08_Reset(ADS131M08_HandleTypeDef *hads);
HAL_StatusTypeDef ADS131M08_Standby(ADS131M08_HandleTypeDef *hads);
HAL_StatusTypeDef ADS131M08_Wakeup(ADS131M08_HandleTypeDef *hads);
HAL_StatusTypeDef ADS131M08_Receive(ADS131M08_HandleTypeDef *hads, uint8_t *tx_buf, uint8_t *rx_buf);
HAL_StatusTypeDef ADS131M08_ReadReg(ADS131M08_HandleTypeDef *hads, uint8_t reg, uint16_t *data);
HAL_StatusTypeDef ADS131M08_WriteReg(ADS131M08_HandleTypeDef *hads, uint8_t reg, uint16_t data);
HAL_StatusTypeDef ADS131M08_ReadData(ADS131M08_HandleTypeDef *hads);
HAL_StatusTypeDef ADS131M08_EnableGlobalChopMode(ADS131M08_HandleTypeDef *hads);
void ADS131M08_ConvertToVoltage(ADS131M08_HandleTypeDef *hads, uint8_t channel);
void ADS131M08_ConvertAllToVoltage(ADS131M08_HandleTypeDef *hads);


#endif /* INC_ADS131M08_H_ */
