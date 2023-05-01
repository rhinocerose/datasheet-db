/*
 * mcp23017.h
 *
 *  Created on: Jan 18, 2023
 *      Author: ashar
 */

#ifndef INC_MCP23017_H_
#define INC_MCP23017_H_

#include "stm32l0xx_hal.h"

typedef enum MCP23017_Port {
	MCP23017_PORTA,
	MCP23017_PORTB
} MCP23017_Port;

typedef enum MCP23017_Address {
	MCP23017_DEVICE_1 = 0x20,
	MCP23017_DEVICE_2 = 0x21,
	MCP23017_DEVICE_3 = 0x22,
	MCP23017_DEVICE_4 = 0x23,
	MCP23017_DEVICE_5 = 0x24,
	MCP23017_DEVICE_6 = 0x25,
	MCP23017_DEVICE_7 = 0x26,
	MCP23017_DEVICE_8 = 0x27,
} MCP23017_Address;

typedef enum MCP23017_IO_Dir {
	MCP23017_IODIR_ALL_OUTPUT = 0x00,
	MCP23017_IODIR_ALL_INPUT  = 0xFF,
	MCP23017_IODIR_IO0_INPUT  = 0x01,
	MCP23017_IODIR_IO1_INPUT  = 0x02,
	MCP23017_IODIR_IO2_INPUT  = 0x04,
	MCP23017_IODIR_IO3_INPUT  = 0x08,
	MCP23017_IODIR_IO4_INPUT  = 0x10,
	MCP23017_IODIR_IO5_INPUT  = 0x20,
	MCP23017_IODIR_IO6_INPUT  = 0x40,
	MCP23017_IODIR_IO7_INPUT  = 0x80,
} MCP23017_IO_Dir;

// I/O Direction
// Default state: MCP23017_IODIR_ALL_INPUT
#define MCP23017_IODIR_ALL_OUTPUT	0x00
#define MCP23017_IODIR_ALL_INPUT	0xFF
#define MCP23017_IODIR_IO0_INPUT	0x01
#define MCP23017_IODIR_IO1_INPUT	0x02
#define MCP23017_IODIR_IO2_INPUT	0x04
#define MCP23017_IODIR_IO3_INPUT	0x08
#define MCP23017_IODIR_IO4_INPUT	0x10
#define MCP23017_IODIR_IO5_INPUT	0x20
#define MCP23017_IODIR_IO6_INPUT	0x40
#define MCP23017_IODIR_IO7_INPUT	0x80

// Input Polarity
// Default state: MCP23017_IPOL_ALL_NORMAL
#define MCP23017_IPOL_ALL_NORMAL	0x00
#define MCP23017_IPOL_ALL_INVERTED	0xFF
#define MCP23017_IPOL_IO0_INVERTED	0x01
#define MCP23017_IPOL_IO1_INVERTED	0x02
#define MCP23017_IPOL_IO2_INVERTED	0x04
#define MCP23017_IPOL_IO3_INVERTED	0x08
#define MCP23017_IPOL_IO4_INVERTED	0x10
#define MCP23017_IPOL_IO5_INVERTED	0x20
#define MCP23017_IPOL_IO6_INVERTED	0x40
#define MCP23017_IPOL_IO7_INVERTED	0x80

// Pull-Up Resistor
// Default state: MCP23017_GPPU_ALL_DISABLED
#define MCP23017_GPPU_ALL_DISABLED	0x00
#define MCP23017_GPPU_ALL_ENABLED	0xFF
#define MCP23017_GPPU_IO0_ENABLED	0x01
#define MCP23017_GPPU_IO1_ENABLED	0x02
#define MCP23017_GPPU_IO2_ENABLED	0x04
#define MCP23017_GPPU_IO3_ENABLED	0x08
#define MCP23017_GPPU_IO4_ENABLED	0x10
#define MCP23017_GPPU_IO5_ENABLED	0x20
#define MCP23017_GPPU_IO6_ENABLED	0x40
#define MCP23017_GPPU_IO7_ENABLED	0x80

// Interrupt Modes
typedef enum {
	INTERRUPT_SEPARATE,
	INTERRUPT_MIRRORED = 0b01000000
} MCP23017_Int_Mode;

// Interrupt Directions
typedef enum {
	INTERRUPT_CHANGE,
	INTERRUPT_FALLING,
	INTERRUPT_RISING
} MCP23017_Int_Dir;

typedef struct MCP23017_HandleTypeDef {
	I2C_HandleTypeDef*	hi2c;
	uint16_t		addr;
	uint8_t			gpio[2];
} MCP23017_HandleTypeDef;

void                mcp23017_init(MCP23017_HandleTypeDef *hdev, I2C_HandleTypeDef *hi2c, MCP23017_Address addr);
HAL_StatusTypeDef	mcp23017_read(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data);
HAL_StatusTypeDef	mcp23017_write(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data);
HAL_StatusTypeDef	mcp23017_iodir(MCP23017_HandleTypeDef *hdev, MCP23017_Port port, MCP23017_IO_Dir iodir);
HAL_StatusTypeDef	mcp23017_ipol(MCP23017_HandleTypeDef *hdev, MCP23017_Port port, uint8_t ipol);
HAL_StatusTypeDef	mcp23017_ggpu(MCP23017_HandleTypeDef *hdev, MCP23017_Port port, uint8_t pu);
HAL_StatusTypeDef	mcp23017_read_gpio(MCP23017_HandleTypeDef *hdev, MCP23017_Port port);
HAL_StatusTypeDef	mcp23017_write_gpio(MCP23017_HandleTypeDef *hdev, MCP23017_Port port);
HAL_StatusTypeDef   mcp23017_set_interrupt_mode(MCP23017_HandleTypeDef *device, MCP23017_Port port, MCP23017_Int_Mode mode, MCP23017_Int_Dir dir);
HAL_StatusTypeDef   mcp23017_clear_interrupts(MCP23017_HandleTypeDef *device, MCP23017_Port port);
uint8_t mcp23017_read_interrupts(MCP23017_HandleTypeDef *device, MCP23017_Port port);

#endif /* INC_MCP23017_H_ */
