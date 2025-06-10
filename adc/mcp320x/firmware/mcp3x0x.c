/*
 * mcp3x0x.c
 *
 *  Created on: Nov 6, 2024
 *      Author: AsharLatif
 */

#include "mcp3x0x.h"

uint8_t txData[3] = { 0, 0, 0 };
uint8_t rxData[3] = { 0, 0, 0 };

void mcp3x0x_init(MCP3x0x_HandleTypeDef *mcp3x0x, SPI_HandleTypeDef *hspi, MCP3x0x_Type type, GPIO_TypeDef* cs_port, uint16_t cs_pin) {
	int i;

	mcp3x0x->hspi = hspi;
	mcp3x0x->type = type;
	mcp3x0x->cs_port = cs_port;
	mcp3x0x->cs_pin = cs_pin;
	mcp3x0x->input_type = SingleEnded;

	for (i = 0; i < 8; ++i) mcp3x0x->adc_values[i] = 0;

	if (type > MCP3008) mcp3x0x->max_val = 4095;
	else                mcp3x0x->max_val = 1023;

	switch (type) {
	case MCP3001:
		mcp3x0x->num_channels = 1;
		break;
	case MCP3002:
		mcp3x0x->num_channels = 2;
		break;
	case MCP3004:
		mcp3x0x->num_channels = 4;
		break;
	case MCP3008:
		mcp3x0x->num_channels = 8;
		break;
	case MCP3201:
		mcp3x0x->num_channels = 1;
		break;
	case MCP3202:
		mcp3x0x->num_channels = 2;
		break;
	case MCP3204:
		mcp3x0x->num_channels = 4;
		break;
	case MCP3208:
		mcp3x0x->num_channels = 8;
		break;
	default:
		break;
	}
	return;
}

void mcp3x0x_set_input_type(MCP3x0x_HandleTypeDef *mcp3x0x, MCP3x0x_Input_Type type) {
	mcp3x0x->input_type = type;
	return;
}

void mcp3x0x_read_single_adc(MCP3x0x_HandleTypeDef *mcp3x0x, MCP3x0x_Channels channel) {
	uint8_t data_size = 0;
	if ((mcp3x0x->num_channels == 4) || (mcp3x0x->num_channels == 8)) {
		txData[0] = mcp3x0x->input_type | channel;
		data_size = 3;
	} else if (mcp3x0x->num_channels == 2) {
		data_size = 2;
		txData[0] = mcp3x0x->input_type | channel;
	} else if (mcp3x0x->num_channels == 1) {
		data_size = 2;
		txData[0] = mcp3x0x->input_type | channel;
	}
    HAL_GPIO_WritePin(mcp3x0x->cs_port, mcp3x0x->cs_pin, GPIO_PIN_RESET); // Pull CS low
    HAL_SPI_TransmitReceive(mcp3x0x->hspi, txData, rxData, data_size, 100);
    HAL_GPIO_WritePin(mcp3x0x->cs_port, mcp3x0x->cs_pin, GPIO_PIN_SET);   // Pull CS high

    mcp3x0x->adc_values[channel] = ((rxData[1] & 0x0F) << 8) | rxData[2];

    return;
}

void mcp3x0x_read_all_adc(MCP3x0x_HandleTypeDef *mcp3x0x) {
	int channel;

	for (channel = 0; channel < mcp3x0x->num_channels; ++channel) {
		mcp3x0x_read_single_adc(mcp3x0x, channel);
	}

	return;

}


