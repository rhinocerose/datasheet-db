// Copyright (c) 2018 Rudá Moura <ruda.moura@gmail.com>
// License: The MIT License (MIT)

#include "mcp23017.h"

// Registers
#define REGISTER_IODIRA		0x00
#define REGISTER_IODIRB		0x01
#define REGISTER_IPOLA		0x02
#define REGISTER_IPOLB		0x03
#define REGISTER_GPINTENA	0x04
#define REGISTER_GPINTENB	0x05
#define REGISTER_DEFVALA	0x06
#define REGISTER_DEFVALB	0x07
#define REGISTER_INTCONA	0x08
#define REGISTER_INTCONB	0x09
#define REGISTER_IOCON		0x0A
//	IOCON			0x0B
#define REGISTER_GPPUA		0x0C
#define REGISTER_GPPUB		0x0D
#define REGISTER_INTFA		0x0E
#define REGISTER_INTFB		0x0F
#define REGISTER_INTCAPA	0x10
#define REGISTER_INTCAPB	0x11
#define REGISTER_GPIOA		0x12
#define REGISTER_GPIOB		0x13
#define REGISTER_OLATA		0x14
#define REGISTER_OLATB		0x15

void mcp23017_init(MCP23017_HandleTypeDef *device, I2C_HandleTypeDef *hi2c, MCP23017_Address addr)
{
	device->hi2c = hi2c;
	device->addr = addr << 1;
}

HAL_StatusTypeDef mcp23017_read(MCP23017_HandleTypeDef *device, uint16_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Read(device->hi2c, device->addr, reg, 1, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef mcp23017_write(MCP23017_HandleTypeDef *device, uint16_t reg, uint8_t *data)
{
	return HAL_I2C_Mem_Write(device->hi2c, device->addr, reg, 1, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef mcp23017_iodir(MCP23017_HandleTypeDef *device, MCP23017_Port port, MCP23017_IO_Dir iodir)
{
	uint8_t data[1] = {iodir};
	return mcp23017_write(device, REGISTER_IODIRA|port, data);
}

HAL_StatusTypeDef mcp23017_ipol(MCP23017_HandleTypeDef *device, MCP23017_Port port, uint8_t ipol)
{
	uint8_t data[1] = {ipol};
	return mcp23017_write(device, REGISTER_IPOLA|port, data);
}

HAL_StatusTypeDef mcp23017_ggpu(MCP23017_HandleTypeDef *device, MCP23017_Port port, uint8_t pu)
{
	uint8_t data[1] = {pu};
	return mcp23017_write(device, REGISTER_GPPUA|port, data);
}

HAL_StatusTypeDef mcp23017_read_gpio(MCP23017_HandleTypeDef *device, MCP23017_Port port)
{
	uint8_t data[1];
	HAL_StatusTypeDef status;
	status = mcp23017_read(device, REGISTER_GPIOA|port, data);
	if (status == HAL_OK)
		device->gpio[port] = data[0];
	return status;
}

HAL_StatusTypeDef mcp23017_write_gpio(MCP23017_HandleTypeDef *device, MCP23017_Port port)
{
	uint8_t data[1] = {device->gpio[port]};
	return mcp23017_write(device, REGISTER_GPIOA|port, data);
}

HAL_StatusTypeDef mcp23017_set_interrupt_mode(MCP23017_HandleTypeDef *device, MCP23017_Port port, MCP23017_Int_Mode mode, MCP23017_Int_Dir dir) {
	uint8_t iocon[1];
	uint8_t data[1];
	HAL_StatusTypeDef status;
	uint8_t defval = REGISTER_DEFVALA + port;
	uint8_t intcon = REGISTER_INTCONA + port;
	status = mcp23017_read(device, REGISTER_IOCON, iocon);
	if (status == HAL_OK) {
		if (mode == INTERRUPT_MIRRORED) {
			iocon[0] |= INTERRUPT_MIRRORED;
		} else iocon[0] &= ~(INTERRUPT_MIRRORED);
	}

	status = mcp23017_write(device, REGISTER_IOCON, iocon);
	if (status == HAL_OK) {
		switch (dir) {
		case INTERRUPT_CHANGE:
			data[0] = 0;
			status = mcp23017_write(device, intcon, data);
			break;
		case INTERRUPT_FALLING:
			data[0] = 0xFF;
			status = mcp23017_write(device, intcon, data);
			if (status == HAL_OK) status = mcp23017_write(device, defval, data);
			break;
		case INTERRUPT_RISING:
			data[0] = 0xFF;
			status = mcp23017_write(device, intcon, data);
			data[0] = 0;
			if (status == HAL_OK) status = mcp23017_write(device, defval, data);
			break;
		}
	}
	return status;
}

HAL_StatusTypeDef mcp23017_clear_interrupts(MCP23017_HandleTypeDef *device, MCP23017_Port port) {
	uint8_t data[1];
	HAL_StatusTypeDef status;
	status = mcp23017_read(device, REGISTER_INTCAPA + port, data);
	if (status == HAL_OK)
			device->gpio[port] = data[0];
	return status;
}

HAL_StatusTypeDef mcp23017_read_interrupts(MCP23017_HandleTypeDef *device, MCP23017_Port port) {
	uint8_t data[1];
	HAL_StatusTypeDef status;
	status = mcp23017_read(device, REGISTER_INTCAPA + port, data);
	if (status == HAL_OK) device->gpio[port] = data[0];
	return status;
}
