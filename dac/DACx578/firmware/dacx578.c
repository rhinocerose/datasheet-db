/*
 * dacx578.c
 *
 *  Created on: Jan 18, 2023
 *      Author: ashar
 */

#include "dacx578.h"

uint8_t tx_buf[3] = { 0, 0, 0 };

void dacx578_init(DACx578_HandleTypeDef *DACx578, I2C_HandleTypeDef *hi2c, DACx578_Type type, DACx578_Address address, DACx578_UpdateMode update_mode) {

	DACx578->i2cAddress = (uint8_t)address;
	DACx578->hi2c = hi2c;
	DACx578->update_mode = update_mode;
	DACx578->type = type;
	return;
}

void dacx578_setClearMode(DACx578_HandleTypeDef *DACx578, DACx578_ClrMode clear_mode) {
	tx_buf[COMMAND_BYTE] = DACx578_CLRCODE_COMMAND;
	tx_buf[MSB] = 0 | (clear_mode >> 8);
	tx_buf[LSB] = 0 | (clear_mode);

	dacx578_write(DACx578);
	//HAL_I2C_Master_Transmit(DACx578->hi2c, DACx578->i2cAddress, tx_buf, 3, DAC_I2C_TIMEOUT);
}

void dacx578_setUpdateMode(DACx578_HandleTypeDef *DACx578, DACx578_UpdateMode update_mode) {
	DACx578->update_mode = update_mode;
}

void dacx578_powerOnAll(DACx578_HandleTypeDef *DACx578) {
	tx_buf[COMMAND_BYTE] = 0 | DACx578_PDMODE_COMMAND;
	tx_buf[MSB] = 0 | (uint8_t)(DACx578_POWER_ON_ALL >> 8);
	tx_buf[LSB] = 0 | (uint8_t)(DACx578_POWER_ON_ALL);

	dacx578_write(DACx578);
	//HAL_I2C_Master_Transmit(DACx578->hi2c, DACx578->i2cAddress, tx_buf, 3, DAC_I2C_TIMEOUT);
}


void dacx578_write(DACx578_HandleTypeDef *DACx578) {
	HAL_I2C_Master_Transmit(DACx578->hi2c, DACx578->i2cAddress, tx_buf, 3, DAC_I2C_TIMEOUT);
}

void dacx578_writeToChannel(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value, uint16_t command) {
	tx_buf[COMMAND_BYTE] = 0 | command | channel;
	switch(DACx578->type) {
		case DAC5578:
			if (value > 255) value = 255;
			tx_buf[MSB] = 0 | value;
			tx_buf[LSB] = 0xFF;
			break;
		case DAC6578:
			if (value > 1023) value = 1023;
			tx_buf[MSB] = 0 | (value << 8);
			tx_buf[LSB] = 0xFF | value;
			break;
		case DAC7578:
			if (value > 4095) value = 4095;
			tx_buf[MSB] = 0 | (value >> 4);
			tx_buf[LSB] = ((0x0F & value) << 4) | 0x0F;
			break;
		default:
			break;
	}

	dacx578_write(DACx578);
	//HAL_I2C_Master_Transmit(DACx578->hi2c, DACx578->i2cAddress, tx_buf, 3, DAC_I2C_TIMEOUT);
}

void dacx578_setAndUpdate(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value) {
	dacx578_writeToChannel(DACx578, channel, value, DACx578_WRITE_CH_UPDATE_CH);
}

void dacx578_setAndUpdateAll(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value) {
	dacx578_writeToChannel(DACx578, channel, value, DACx578_WRITE_CH_UPDATE_ALL);
}

void dacx578_setWithoutUpdating(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value) {
	dacx578_writeToChannel(DACx578, channel, value, DACx578_WRITE_CHANNEL);
}

void dacx578_setDAC(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value) {
	switch(DACx578->update_mode) {
    case UPDATE_ONE:
    	dacx578_setAndUpdate(DACx578, channel, value);
        break;
    case UPDATE_ALL:
    	dacx578_setAndUpdateAll(DACx578, channel, value);
        break;
    case UPDATE_NONE:
    	dacx578_setWithoutUpdating(DACx578, channel, value);
        break;
    default:
        break;
	}
}
