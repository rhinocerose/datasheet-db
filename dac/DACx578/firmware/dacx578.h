/*
 * dacx578.h
 *
 *  Created on: Jan 18, 2023
 *      Author: ashar
 */

#ifndef INC_DACX578_H_
#define INC_DACX578_H_

#include "stm32l0xx_hal.h"

// Registers, commands, etc. Don't cares are set high unless theyre intersecting with a channel code.
#define DACx578_DEFAULT_ADDRESS     0b1001000 // address when all ADDR pins are pulled low

#define DACx578_WRITE_CHANNEL       0x00      // 0000 XXXX - access a channel's input register - OR with Channel
#define DACx578_UPDATE_CHANNEL      0x10      // 0001 XXXX - update or read from a channel - OR with Channel
#define DACx578_WRITE_CH_UPDATE_ALL 0x20      // 0010 XXXX - access a channel, then update all channels - OR with Channel
#define DACx578_WRITE_CH_UPDATE_CH  0x30      // 0011 XXXX - access a channel, then update only that channel - OR with Channel
#define DACx578_PDMODE_COMMAND      0x4F      // 0100 XXXX - access PDMode register
#define DACx578_CLRCODE_COMMAND     0x5F      // 0101 XXXX - access ClrCode register
#define DACx578_LDAC_COMMAND        0x6F      // 0110 XXXX - access LDAC register
#define DACx578_RESET_COMMAND       0x7F      // 0111 XXXX - software reset
#define DACx578_STATICREF_COMMAND   0x8F      // 1000 XXXX - access internal reference static mode register
#define DACx578_FLEXIBLEREF_COMMAND 0x9F      // 1001 XXXX - access internal reference flexible mode register

#define DACx578_STATICMODE_SW       0xBFFF    // (1001) - X0XX XXXX XXXX XXXX - switch to static mode from flexible mode

#define DACx578_POWER_ON_ALL        0x1FFF
#define DAC_I2C_TIMEOUT             10u


// Register codes

typedef enum DACx578_Type {
	DAC5578,
	DAC6578,
	DAC7578
} DACx578_Type;

/* dac addresses */
typedef enum DACx578_Address {
	DACx578_ADDR_A0_LOW      = 0x90,   //i2c address, A0 connected to GND
	DACx578_ADDR_A0_HIGH     = 0x94,   //i2c address, A0 connected to VCC
	DACx578_ADDR_A0_FLOAT    = 0x98,   //i2c address, A0 floating
} DACx578_Address;

typedef enum DACx578_Bytes {
	COMMAND_BYTE,
	MSB,
	LSB
} DACx578_Bytes;

/**
 * @brief Used when writing to a channel (setting its output voltage).
 *
 * @param A 0
 * @param B 1
 * @param C 2
 * @param D 3
 * @param E 4
 * @param F 5
 * @param G 6
 * @param H 7
 * @param ALL 15
 * @param NONE -1
 */
typedef enum DACx578_Channel {
	CHANN_A    = 0b0000,
	CHANN_B    = 0b0001,
	CHANN_C    = 0b0010,
	CHANN_D    = 0b0011,
	CHANN_E    = 0b0100,
	CHANN_F    = 0b0101,
	CHANN_G    = 0b0110,
	CHANN_H    = 0b0111,
	CHANN_ALL  = 0b1111,
	CHANN_NONE = -1
} DACx578_Channel;


/**
 * @brief Codes written to ClrMode register that decide what value to set DAC channels to when they're cleared.
 *
 * @param ZERO_SCALE 0xFFCF
 * @param MID_SCALE 0xFFDF
 * @param FULL_SCALE 0xFFEF
 * @param CLR_DISABLE 0xFFFF
 */
typedef enum DACx578_ClrMode {
	ZERO_SCALE  = 0xFFCF,
	MID_SCALE   = 0xFFDF,
	FULL_SCALE  = 0xFFEF,
	CLR_DISABLE = 0xFFFF
} DACx578_ClrMode;

/**
 * @brief Codes written to PDMode register - powers on registers, or powers them down and determines what impedance to pull pins to GND with.
 *
 * @param POWER_ON 0b00
 * @param PD_1K 0b01 - 1k pulldown
 * @param PD_100K 0b10 - 100k pulldown
 * @param PD_HIGHZ 0b11 - no pulldown, just disconnects pin
 */
typedef enum DACx578_PDMode {
	POWER_ON = 0b00,
	PD_1K    = 0b01,
	PD_100K  = 0b10,
	PD_HIGHZ = 0b11
} DACx578_PDMode;

/**
 * @brief Codes written to SpeedMode register - sets I2C communication speed (requires reset).
 *
 * @param LOW_SPEED 0x3FFF
 * @param HIGH_SPEED 0x7FFF
 * @param MAINTAIN_SPEED 0xBFFF
 */
typedef enum DACx578_SpeedMode {
	LOW_SPEED      = 0x3FFF,
	HIGH_SPEED     = 0x7FFF,
	MAINTAIN_SPEED = 0xBFFF
} DACx578_SpeedMode;

/**
 * @brief Choose between static mode or flexible mode for the DAC7678's internal reference.
 *
 * Static mode has two options - reference on, reference off.
 * Flexible mode has four options - powered on (acts same as static case),
 * ALWAYS powered on, or ALWAYS powered off.
 *
 * @param ON_STATIC 0xFFFF
 * @param OFF_STATIC 0xFFEF
 * @param ON_FLEXIBLE 0xCFFF
 * @param ALWAYS_ON 0xDFFF
 * @param ALWAYS_OFF 0xEFFF
 */
typedef enum DACx578_ReferenceMode {
	ON_STATIC   = 0xFFFF,
	OFF_STATIC  = 0xFFEF,
	ON_FLEXIBLE = 0xCFFF,
	ALWAYS_ON   = 0xDFFF,
	ALWAYS_OFF  = 0xEFFF
} DACx578_ReferenceMode;


/**
 * @brief Not a register code; determines behavior of this class when writing to DAC registers.
 *
 * @param UPDATE_ONE
 * @param UPDATE_ALL
 * @param UPDATE_NONE
 */
typedef enum DACx578_UpdateMode {
	UPDATE_ONE,
	UPDATE_ALL,
	UPDATE_NONE
} DACx578_UpdateMode;

typedef struct DACx578_HandleTypeDef {
	I2C_HandleTypeDef*  hi2c;
	DACx578_Address     i2cAddress;
	DACx578_UpdateMode  update_mode;
	DACx578_Type        type;
} DACx578_HandleTypeDef;

/* ~ Function documentation is in .cpp ~ */

// Constructors
void dacx578_init(DACx578_HandleTypeDef *DACx578, I2C_HandleTypeDef *hi2c, DACx578_Type type, DACx578_Address address, DACx578_UpdateMode update_mode);

void dacx578_powerOnAll(DACx578_HandleTypeDef *DACx578);
void dacx578_setDAC(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value);
void dacx578_write(DACx578_HandleTypeDef *DACx578);

/**
 * @brief Sets the CLRCODE register, which determines the chip's response to an input from the CLR pin.
 * @details Pulling the CLR pin to GND will send the chip a signal to clear the DAC's outputs.
 *          If set to ZERO_SCALE, the CLR pin will set each channel to 0V.
 *          MID_SCALE will set each channel to VDD/2, and FULL_SCALE will set each channel to VDD.
 *          CLR_DISABLE will disable the CLR pin, and the chip won't respond to a CLR signal.
 *
 * @param mode CLR setting to configure the DAC7678 chip with
 */
void dacx578_setClearMode(DACx578_HandleTypeDef *DACx578, DACx578_ClrMode clear_mode);
void dacx578_setUpdateMode(DACx578_HandleTypeDef *DACx578, DACx578_UpdateMode update_mode);

void dacx578_setAndUpdate(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value);
void dacx578_setAndUpdateAll(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value);
void dacx578_setWithoutUpdating(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value);
void dacx578_writeToChannel(DACx578_HandleTypeDef *DACx578, DACx578_Channel channel, uint16_t value, uint16_t command);

// Class configuration functions - doesn't do anything to communicate with DAC,
//                                 just determines behavior of this class
void setDefaults();
void setAddress(uint8_t address);
//void connectWire(TwoWire &wire);

// I2C functions
void write(uint8_t command, uint16_t data);
uint16_t read(uint8_t command);

// Command functions
void update(int chan);
//void reset(enum DACx578_SpeedMode mode = LOW_SPEED);
void setLDACMode(uint8_t chan_code);
void setPDMode(DACx578_PDMode mode, uint8_t chan_code);
void setReferenceMode(DACx578_ReferenceMode ref_mode);
void setToStaticMode();

// Read functions
uint16_t readChannelInput(int chan);
uint16_t readChannelOutput(int chan);
uint16_t readPDRegister();
uint16_t readClrModeRegister();
uint16_t readLDACRegister();
uint16_t readStaticReference();
uint16_t readFlexibleReference();

// Misc functions
//bool isValidAddress(uint8_t address);
//uint8_t getChannelCode(int chan1, int chan2 = NONE, int chan3 = NONE, int chan4 = NONE,
//					int chan5 = NONE, int chan6 = NONE, int chan7 = NONE, int chan8 = NONE);

// Access private variables
//uint8_t getAddressVar() { return address; }
//UpdateMode getUpdateModeVar() { return update_mode; }
//ReferenceMode getReferenceModeVar() { return ref_mode; }

uint8_t chanCode(int chan);
//bool isStaticMode(DACx578_ReferenceMode mode)   { return mode == ON_STATIC   || mode == OFF_STATIC; }
//bool isFlexibleMode(DACx578_ReferenceMode mode) { return mode == ON_FLEXIBLE || mode == ALWAYS_ON || mode == ALWAYS_OFF; }

//TwoWire * wire; // TwoWire Arduino object used for I2C communication

#endif /* INC_DACX578_H_ */
