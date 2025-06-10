/*
 * mcp41hvx1.h
 *
 *  Created on: Mar 29, 2023
 *      Author: AsharLatif
 */

#ifndef MCP41HVX1_H
#define MCP41HVX1_H


#define STM32H7

#if defined (STM32L4)
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_gpio.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#endif


typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
} MCP41HVX1_HandleTypeDef;

void MCP41HVX1_Init(MCP41HVX1_HandleTypeDef *hdev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);
void MCP41HVX1_SetResistance(MCP41HVX1_HandleTypeDef *hdev, uint8_t value);
void MCP41HVX1_Shutdown(MCP41HVX1_HandleTypeDef *hdev);

#endif /* INC_MCP41HVX1_H */
