#include "digipot.h"

const uint8_t digipots[4] = {
    MCP44xx_COMMAND(MCP44xx_WIPER0, MCP44xx_WRITE, 0b00),
    MCP44xx_COMMAND(MCP44xx_WIPER1, MCP44xx_WRITE, 0b00),
    MCP44xx_COMMAND(MCP44xx_WIPER2, MCP44xx_WRITE, 0b00),
    MCP44xx_COMMAND(MCP44xx_WIPER3, MCP44xx_WRITE, 0b00)};

//const char digipotNames[] = {'A','B','C','D'};

uint8_t mcp44xx_buf[2] = {0, 0};

void i2c_send(MCP44xx_HandleTypeDef *MCP44xx, uint8_t a, uint8_t b) //wrapper for I2C routines
{
	mcp44xx_buf[MCP44xx_COMMAND_BYTE] = a;
	mcp44xx_buf[MCP44xx_DATA_BYTE] = b;

	HAL_I2C_Master_Transmit(MCP44xx->hi2c, MCP44xx->i2cAddress, mcp44xx_buf, 2, HAL_MAX_DELAY);

}

void DigipotInit(MCP44xx_HandleTypeDef *MCP44xx, I2C_HandleTypeDef *hi2c, MCP44xx_Address address)
{
	MCP44xx->hi2c = hi2c;
	MCP44xx->i2cAddress = (uint8_t)address;
	MCP44xx->max_val = 256;
    // enable all digipots
    i2c_send(MCP44xx, MCP44xx_COMMAND(MCP44xx_TCON0, MCP44xx_WRITE, 0b00), 0xff);
    i2c_send(MCP44xx, MCP44xx_COMMAND(MCP44xx_TCON1, MCP44xx_WRITE, 0b00), 0xff);
}

//write a value on one of the four digipots in the IC
void DigipotWrite(MCP44xx_HandleTypeDef *MCP4xx, uint8_t pot, uint8_t val)
{
    // set specific digipot value
    i2c_send(MCP4xx, digipots[pot], val);
}

void DigipotTestSweep(MCP44xx_HandleTypeDef *MCP4xx)
{
    for (int pot = 0; pot < 1; pot++)
    {
        for (uint16_t value = 0; value < 0xff; value++)
        {
            DigipotWrite(MCP4xx, pot, value);
        }
    }
}
