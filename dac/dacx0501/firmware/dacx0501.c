/*
 * dacx0501.c
 *
 *  Created on: Jan 18, 2023
 *      Author: ashar
 */

#include "dacx0501.h"

uint8_t dacx0501_tx_buf[3] = { 0, 0, 0 };

void dacx0501_init(DACx0501_HandleTypeDef *DACx0501, I2C_HandleTypeDef *hi2c, DACx0501_Type type, DACx0501_Address address) {

	DACx0501->i2cAddress = (uint8_t)address;
	DACx0501->hi2c = hi2c;
	DACx0501->type = type;
	switch (type) {
	case DAC60501:
		DACx0501->max_val = 4095;
		break;
	case DAC70501:
		DACx0501->max_val = 16383;
		break;
	case DAC80501:
		DACx0501->max_val = 65535;
		break;
	default:
		break;
	}
	return;
}


void dacx0501_setGain(DACx0501_HandleTypeDef *DACx0501, DACx0501_VRef_Gain refmode) {
	dacx0501_tx_buf[DACx0501_COMMAND_BYTE] = DACx0501_CLRCODE_COMMAND;
	dacx0501_tx_buf[DACx0501_MSB] = ((uint16_t)refmode & 0xFF00) >> 8;
	dacx0501_tx_buf[DACx0501_LSB] = (uint8_t)((uint16_t)refmode & 0x00FF);

	dacx0501_write(DACx0501);
}


void dacx0501_write(DACx0501_HandleTypeDef *DACx0501) {
	HAL_I2C_Master_Transmit(DACx0501->hi2c, DACx0501->i2cAddress, dacx0501_tx_buf, 3, HAL_MAX_DELAY);
}

void dacx0501_setValue(DACx0501_HandleTypeDef *DACx0501, uint16_t value) {

	dacx0501_tx_buf[DACx0501_COMMAND_BYTE] = DACx0501_DATA_CMD;
	if (value > DACx0501->max_val) value = DACx0501->max_val;
	switch(DACx0501->type) {
		case DAC60501:
			dacx0501_tx_buf[DACx0501_MSB] = 0xFF00 | (value << 4);
			dacx0501_tx_buf[DACx0501_LSB] = 0x00F0 | (value << 4);
			break;
		case DAC70501:
			dacx0501_tx_buf[DACx0501_MSB] = (0xFF00 | (value << 2) >> 8);
			dacx0501_tx_buf[DACx0501_LSB] = 0x00FC | (value << 2);
			break;
		case DAC80501:
			dacx0501_tx_buf[DACx0501_MSB] = 0xFF00 | (value);
			dacx0501_tx_buf[DACx0501_LSB] = 0x00FF | (value);
			break;
		default:
			break;
	}

	dacx0501_write(DACx0501);
}
