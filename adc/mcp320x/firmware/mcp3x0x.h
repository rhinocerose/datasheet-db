/*
 * mcp3x0x.h
 *
 *  Created on: Nov 6, 2024
 *      Author: AsharLatif
 */

#ifndef INC_MCP3X0X_H_
#define INC_MCP3X0X_H_

#include "stm32f0xx.h"

#if defined (STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32U5)
#include "stm32u5xx_hal.h"
#endif


typedef enum {
	MCP3001,
	MCP3002,
	MCP3004,
	MCP3008,
	MCP3201,
	MCP3202,
	MCP3204,
	MCP3208
} MCP3x0x_Type;


typedef enum {
	Differential,
	SingleEnded = (1 << 3)
} MCP3x0x_Input_Type;


typedef enum {
	MCP3x0x_Channel_1,
	MCP3x0x_Channel_2,
	MCP3x0x_Channel_3,
	MCP3x0x_Channel_4,
	MCP3x0x_Channel_5,
	MCP3x0x_Channel_6,
	MCP3x0x_Channel_7,
	MCP3x0x_Channel_8,
} MCP3x0x_Channels;


typedef struct {
	SPI_HandleTypeDef*   hspi;
	MCP3x0x_Type         type;
	GPIO_TypeDef*        cs_port;
	uint16_t             cs_pin;
	uint8_t              num_channels;
	uint16_t			 max_val;
	uint16_t             adc_values[8];
	uint8_t              input_type;
} MCP3x0x_HandleTypeDef;


void mcp3x0x_init(MCP3x0x_HandleTypeDef *mcp3x0x, SPI_HandleTypeDef *hspi, MCP3x0x_Type type, GPIO_TypeDef* cs_port, uint16_t cs_pin);
void mcp3x0x_set_input_type(MCP3x0x_HandleTypeDef *mcp3x0x, MCP3x0x_Input_Type type);
void mcp3x0x_read_single_adc(MCP3x0x_HandleTypeDef *mcp3x0x, MCP3x0x_Channels channel);
void mcp3x0x_read_all_adc(MCP3x0x_HandleTypeDef *mcp3x0x);

#endif /* INC_MCP3X0X_H_ */
