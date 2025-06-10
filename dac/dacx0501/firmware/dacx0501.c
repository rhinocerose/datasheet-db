/*
 * dacx0501.c
 *
 *  Created on: Jan 18, 2023
 *      Author: ashar
 */

#include "dacx0501.h"

uint8_t dacx0501_tx_buf[3] = { 0, 0, 0 };

void dacx0501_init(DACx0501_HandleTypeDef *dac, I2C_HandleTypeDef *hi2c, DACx0501_Type type, DACx0501_Address address) {

	dac->i2cAddress = (uint8_t)address;
	dac->hi2c = hi2c;
	dac->type = type;
	dac->default_val = 0;
	switch (type) {
		case DAC60501:
			dac->max_val = 4095;
			break;
		case DAC70501:
			dac->max_val = 16383;
			break;
		case DAC80501:
			dac->max_val = 65535;
			break;
		default: break;
	}
	return;
}


void dacx0501_setGain(DACx0501_HandleTypeDef *dac, DACx0501_VRef_Gain refmode) {
	dacx0501_tx_buf[DACx0501_COMMAND_BYTE] = DACx0501_GAIN_CMD;
	dacx0501_tx_buf[DACx0501_MSB] = ((uint16_t) refmode & 0xFF00) >> 8;
	dacx0501_tx_buf[DACx0501_LSB] = (uint8_t)((uint16_t) refmode & 0x00FF);

	dacx0501_write(dac);
}

void dacx0501_setDefault(DACx0501_HandleTypeDef *dac, uint16_t value) {
	if (value > dac->max_val) value = dac->max_val;
	dac->default_val = value;

}

void dacx0501_write(DACx0501_HandleTypeDef *dac) {
	HAL_I2C_Master_Transmit(dac->hi2c, dac->i2cAddress, dacx0501_tx_buf, 3, HAL_MAX_DELAY);
}

void dacx0501_setValue(DACx0501_HandleTypeDef *dac, uint16_t value) {

	dacx0501_tx_buf[DACx0501_COMMAND_BYTE] = DACx0501_DATA_CMD;
	if (value > dac->max_val) value = dac->max_val;

	switch(dac->type) {
		case DAC60501:
			dacx0501_tx_buf[DACx0501_MSB] = 0xFF00u & (value << 4);
			dacx0501_tx_buf[DACx0501_LSB] = 0x00F0u & (value << 4);
			break;
		case DAC70501:
			uint16_t temp = value << 2;
			dacx0501_tx_buf[DACx0501_MSB] = (0xFF00u & temp) >> 8;
			dacx0501_tx_buf[DACx0501_LSB] = 0x00FCu & temp;
			break;
		case DAC80501:
			dacx0501_tx_buf[DACx0501_MSB] = 0xFF00u & (value);
			dacx0501_tx_buf[DACx0501_LSB] = 0x00FFu & (value);
			break;
		default: break;
	}

	dacx0501_write(dac);
}

void dacx0501_readRegister(DACx0501_HandleTypeDef *dac, uint8_t command) {
	uint8_t buf[1] = {command};
	HAL_I2C_Master_Transmit(dac->hi2c, dac->i2cAddress, buf, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(dac->hi2c, dac->i2cAddress, dacx0501_tx_buf, 3, HAL_MAX_DELAY);
}
