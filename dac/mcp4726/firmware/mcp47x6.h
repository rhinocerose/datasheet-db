/*
 * MCP47x6.h
 *
 *  Created on: Mar 29, 2023
 *      Author: AsharLatif
 */

#ifndef INC_MCP47x6_H_
#define INC_MCP47x6_H_

#include <stddef.h>
#include <_ansi.h>


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


typedef enum {
	MCP4706,
	MCP4716,
	MCP4726
} MCP47x6_Type;

typedef enum {
	MCP47x6_Addr_A0 = 0xC0,
	MCP47x6_Addr_A1 = 0xC2,
	MCP47x6_Addr_A2 = 0xC4,
	MCP47x6_Addr_A3 = 0xC6,
	MCP47x6_Addr_A4 = 0xC8,
	MCP47x6_Addr_A5 = 0xCA,
	MCP47x6_Addr_A6 = 0xCC,
	MCP47x6_Addr_A7 = 0xCE,
} MCP47x6_Address;

/* MCP47x6_VREF
 * Resistor Ladder Voltage Reference (VRL) Selection bits
 * 0x00 = VDD (Unbuffered)
 * 0x02 = VREF pin (Unbuffered)
 * 0x03 = VREF pin (Buffered)
 */
typedef enum {
	MCP47x6_VDD_UNBUFF  = 0x00,
	MCP47x6_VREF_UNBUFF = 0x02,
	MCP47x6_VREF_BUFF   = 0x03,
} MCP47x6_VRL_Type;


/* MCP47x6_PD
 * Power-Down Selection bits
 * When the DAC is powered down, most of the internal circuits are powered off and the op amp is
 * disconnected from the VOUT pin.
 * 0x00 = Not Powered Down (Normal operation)
 * 0x01 = Powered Down – VOUT is loaded with 1 kΩ resistor to ground.
 * 0x02 = Powered Down – VOUT is loaded with 100 kΩ resistor to ground.
 * 0x03 = Powered Down – VOUT is loaded with 500 kΩ resistor to ground.
 */
typedef enum {
	MCP47x6_PD_ON       = 0x00,
	MCP47x6_PD_OFF_1K   = 0x01,
	MCP47x6_PD_OFF_100K = 0x02,
	MCP47x6_PD_OFF_500K = 0x03,
} MCP47x6_PD_Type;


/* MCP47x6_GAIN
 * Gain Selection bit
 * 0x00 = 1x (gain of 1)
 * 0x01 = 2x (gain of 2). Not applicable when VDD is used as VRL
 */
typedef enum {
	MCP47x6_GAIN1 = 0x00,
	MCP47x6_GAIN2 = 0x01,
} MCP47x6_Gain_Type;

typedef struct {
	I2C_HandleTypeDef*   hi2c;
	MCP47x6_Address      i2cAddress;
	MCP47x6_Type         type;
	uint16_t			 max_val;
	uint16_t			 default_val;
} MCP47x6_HandleTypeDef;


void mcp47x6_init(MCP47x6_HandleTypeDef *MCP47x6, I2C_HandleTypeDef *hi2c, MCP47x6_Type type, MCP47x6_Address address);
void mcp47x6_reset(MCP47x6_HandleTypeDef *MCP47x6, MCP47x6_VRL_Type vref, MCP47x6_PD_Type power_down, MCP47x6_Gain_Type gain);
void mcp47x6_setGain(MCP47x6_HandleTypeDef *MCP47x6, MCP47x6_Gain_Type gain);
void mcp47x6_setDefault(MCP47x6_HandleTypeDef *MCP47x6, uint16_t value);

HAL_StatusTypeDef mcp47x6_WriteConfiguration(MCP47x6_HandleTypeDef *MCP47x6, uint8_t byte);
HAL_StatusTypeDef mcp47x6_WriteOutput(MCP47x6_HandleTypeDef *MCP47x6, uint16_t data);

#endif /* INC_MCP47x6_H_ */
