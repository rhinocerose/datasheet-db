/*
 * digipot.h
 *
 *  Created on: Mar. 30, 2023
 *      Author: AsharLatif
 */

#ifndef INC_DIGIPOT_H_
#define INC_DIGIPOT_H_

#include "main.h"


// Main i2c address
#define MCP44xx_I2C_ADDR 0b0101100 // 0b01011 + A0(0) + A1(0)

// 4-bit memory addresses
#define MCP44xx_TCON0 0x4
#define MCP44xx_TCON1 0xa
#define MCP44xx_WIPER0 0x0
#define MCP44xx_WIPER1 0x1
#define MCP44xx_WIPER2 0x6
#define MCP44xx_WIPER3 0x7

// 2 bit commands
#define MCP44xx_WRITE 0b00
#define MCP44xx_INCREMENT 0b01
#define MCP44xx_DECREMENT 0b10
#define MCP44xx_READ 0b11

// Command byte = 4bit address + 2bit command + 2bit (MSB) of 10bit data (0x00)
#define MCP44xx_COMMAND(address, command, data) (address << 4 | command << 2 | data)



typedef enum {
	MCP44xx_COMMAND_BYTE,
	MCP44xx_DATA_BYTE
} MCP44xx_Bytes;

typedef enum  {
	MCP44xx_ADDR_0 = 0x58,  // A0 tied to GND and A1 tied to GND
	MCP44xx_ADDR_1 = 0x5A,  // A0 tied to VCC and A1 tied to GND
	MCP44xx_ADDR_2 = 0x5C,  // A0 tied to GND and A1 tied to VCC
	MCP44xx_ADDR_3 = 0x5E,  // A0 tied to VCC and A1 tied to VCC
} MCP44xx_Address;

typedef struct {
	I2C_HandleTypeDef*   hi2c;
	MCP44xx_Address      i2cAddress;
	uint16_t             max_val;
} MCP44xx_HandleTypeDef;

void DigipotInit(MCP44xx_HandleTypeDef *MCP4xx, I2C_HandleTypeDef *hi2c, MCP44xx_Address address);
void DigipotWrite(MCP44xx_HandleTypeDef *MCP4xx, uint8_t pot, uint8_t val);
void DigipotTestSweep(MCP44xx_HandleTypeDef *MCP4xx);


#endif /* INC_DIGIPOT_H_ */
