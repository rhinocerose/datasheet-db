/*
 * dacx0501.h
 *
 *  Created on: Jan 18, 2023
 *      Author: ashar
 */

#ifndef INC_DACx0501_H_
#define INC_DACx0501_H_

#include "stm32l0xx_hal.h"

// Registers, commands, etc. Don't cares are set high unless they're intersecting with a channel code.

#define DACx0501_NOOP_CMD            0x00
#define DACx0501_DEVID_CMD           0x01
#define DACx0501_SYNC_CMD            0x02
#define DACx0501_CONFIG_CMD          0x03
#define DACx0501_GAIN_CMD            0x04
#define DACx0501_TRIG_CMD            0x05
#define DACx0501_STATUS_CMD          0x07
#define DACx0501_DATA_CMD            0x08

#define DACx0501_WRITE_CHANNEL       0x00      // 0000 XXXX - access a channel's input register - OR with Channel
#define DACx0501_UPDATE_CHANNEL      0x10      // 0001 XXXX - update or read from a channel - OR with Channel
#define DACx0501_WRITE_CH_UPDATE_ALL 0x20      // 0010 XXXX - access a channel, then update all channels - OR with Channel
#define DACx0501_WRITE_CH_UPDATE_CH  0x30      // 0011 XXXX - access a channel, then update only that channel - OR with Channel
#define DACx0501_PDMODE_COMMAND      0x4F      // 0100 XXXX - access PDMode register
#define DACx0501_CLRCODE_COMMAND     0x5F      // 0101 XXXX - access ClrCode register
#define DACx0501_LDAC_COMMAND        0x6F      // 0110 XXXX - access LDAC register
#define DACx0501_RESET_COMMAND       0x7F      // 0111 XXXX - software reset
#define DACx0501_STATICREF_COMMAND   0x8F      // 1000 XXXX - access internal reference static mode register
#define DACx0501_FLEXIBLEREF_COMMAND 0x9F      // 1001 XXXX - access internal reference flexible mode register

#define DACx0501_STATICMODE_SW       0xBFFF    // (1001) - X0XX XXXX XXXX XXXX - switch to static mode from flexible mode

#define DACx0501_POWER_ON_ALL        0x1FFF
#define DAC_I2C_TIMEOUT             10u


// Register codes

typedef enum DACx0501_Type {
	DAC60501,
	DAC70501,
	DAC80501
} DACx0501_Type;

/* dac addresses */
typedef enum DACx0501_Address {
	DACx0501_ADDR_A0_LOW      = 0x90,   //i2c address, A0 connected to GND
	DACx0501_ADDR_A0_HIGH     = 0x92,   //i2c address, A0 connected to VCC
	DACx0501_ADDR_A0_SDA      = 0x94,   //i2c address, A0 connected to SDA
	DACx0501_ADDR_A0_SCL      = 0x96,   //i2c address, A0 connected to SCL
} DACx0501_Address;

typedef enum DACx0501_Bytes {
	DACx0501_COMMAND_BYTE,
	DACx0501_MSB,
	DACx0501_LSB
} DACx0501_Bytes;

typedef enum DACx0501_VRef_Gain {
	DACx0501_REF_DIV1_GAIN1 = 0x0000,
	DACx0501_REF_DIV1_GAIN2 = 0x0001,
	DACx0501_REF_DIV2_GAIN1 = 0x0100,
	DACx0501_REF_DIV2_GAIN2 = 0x0101,
} DACx0501_VRef_Gain;



typedef struct DACx0501_HandleTypeDef {
	I2C_HandleTypeDef*   hi2c;
	DACx0501_Address     i2cAddress;
	DACx0501_Type        type;
	uint16_t			 max_val;
} DACx0501_HandleTypeDef;

/* ~ Function documentation is in .cpp ~ */

// Constructors
void dacx0501_init(DACx0501_HandleTypeDef *DACx0501, I2C_HandleTypeDef *hi2c, DACx0501_Type type, DACx0501_Address address);

void dacx0501_setDAC(DACx0501_HandleTypeDef *DACx0501, uint16_t value);
void dacx0501_write(DACx0501_HandleTypeDef *DACx0501);
void dacx0501_setGain(DACx0501_HandleTypeDef *DACx0501, DACx0501_VRef_Gain refmode);

void dacx0501_setValue(DACx0501_HandleTypeDef *DACx0501, uint16_t value);


#endif /* INC_DACx0501_H_ */
