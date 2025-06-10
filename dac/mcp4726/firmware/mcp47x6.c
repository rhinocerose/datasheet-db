/*
 * MCP47x6.c
 *
 *  Created on: Mar 29, 2023
 *      Author: AsharLatif
 */


#include "mcp47x6.h"
#include <stdlib.h>


HAL_StatusTypeDef mcp47x6_WriteConfiguration(MCP47x6_HandleTypeDef *MCP47x6, uint8_t byte) {
	return HAL_I2C_Master_Transmit(MCP47x6->hi2c, MCP47x6->i2cAddress, &byte, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef mcp47x6_WriteOutput(MCP47x6_HandleTypeDef *MCP47x6, uint16_t data) { //MCP47x6_PD as 0b00
	uint8_t byte[2];
	if (MCP4726 == MCP47x6->type) {
		byte[0] = (uint8_t)((data >> 8) & 0x0F);
		byte[1] = (uint8_t)(data);
	}
	return HAL_I2C_Master_Transmit(MCP47x6->hi2c, MCP47x6->i2cAddress, byte, 2, HAL_MAX_DELAY);
}

void mcp47x6_init(MCP47x6_HandleTypeDef *MCP47x6, I2C_HandleTypeDef *hi2c, MCP47x6_Type type, MCP47x6_Address address) {
	MCP47x6->i2cAddress = address;
	MCP47x6->hi2c = hi2c;
	MCP47x6->type = type;
	MCP47x6->default_val = 0;
	if      (MCP4706 == type) MCP47x6->max_val = 255;
	else if (MCP4716 == type) MCP47x6->max_val = 1023;
	else if (MCP4726 == type) MCP47x6->max_val = 4095;
    HAL_Delay(100);

    mcp47x6_reset(MCP47x6, MCP47x6_VDD_UNBUFF, MCP47x6_PD_ON, MCP47x6_GAIN1);

}

 void mcp47x6_reset(MCP47x6_HandleTypeDef *MCP47x6, MCP47x6_VRL_Type vref, MCP47x6_PD_Type power_down, MCP47x6_Gain_Type gain) {
	 uint8_t conf = 0x80 | (vref << 3) | (power_down << 1) | gain;

	 mcp47x6_WriteConfiguration(MCP47x6, conf);

	 mcp47x6_WriteOutput(MCP47x6, 0x0100);
 }

 void mcp47x6_setGain(MCP47x6_HandleTypeDef *MCP47x6, MCP47x6_Gain_Type gain) {
	 mcp47x6_WriteConfiguration(MCP47x6, gain);
 }

 void mcp47x6_setDefault(MCP47x6_HandleTypeDef *MCP47x6, uint16_t value) {
	 if (value > MCP47x6->default_val) value = MCP47x6->max_val;
	 MCP47x6->default_val = value;
 }
