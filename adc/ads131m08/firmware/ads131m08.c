#include "ads131m08.h"
#include "string.h"

// Private function prototypes
static void ADS131M08_CS_Enable(ADS131M08_HandleTypeDef *hads);
static void ADS131M08_CS_Disable(ADS131M08_HandleTypeDef *hads);
//static uint8_t ADS131M08_CalculateCRC(uint8_t *data, uint8_t len);


HAL_StatusTypeDef ADS131M08_Init(ADS131M08_HandleTypeDef *hads, SPI_HandleTypeDef *hspi,
                                GPIO_TypeDef *cs_port, uint16_t cs_pin, float vref) {
    hads->hspi = hspi;
    hads->cs_port = cs_port;
    hads->cs_pin = cs_pin;
    hads->vref = vref;
    hads->pga = 1;

    if (ADS131M08_Reset(hads) != HAL_OK) {
        return HAL_ERROR;
    }

    uint16_t id;
    if (ADS131M08_ReadReg(hads, ADS131M08_REG_ID, &id) != HAL_OK) {
        return HAL_ERROR;
    }

	if (((id >> 8) != 0x28) & ((id >> 4) != 0x28)) {
		return HAL_ERROR;
	}

	if (ADS131M08_Wakeup(hads) != HAL_OK) {
        return HAL_ERROR;
    }

    // Configure device settings here
    // Example: Set mode register
    // ADS131M08_WriteReg(hads, ADS131M08_REG_MODE, 0x0000);

    return HAL_OK;
}

HAL_StatusTypeDef ADS131M08_Reset(ADS131M08_HandleTypeDef *hads) {
	uint8_t reset_cmd[3] = {0x00, 0x11, 0x00};
	uint8_t rx_buf[3] = {0};
    ADS131M08_CS_Enable(hads);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hads->hspi, reset_cmd, rx_buf, 3, 100);
    ADS131M08_CS_Disable(hads);

//    if (0xFF28 != ((rx_buf[0] << 8) | rx_buf[1])) {
//        return HAL_ERROR;
//    }

    HAL_Delay(1); // Wait for reset to complete

    return status;
}

HAL_StatusTypeDef ADS131M08_Standby(ADS131M08_HandleTypeDef *hads) {
	uint8_t standby_cmd[3] = {0x00, 0x22, 0x00};
    ADS131M08_CS_Enable(hads);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hads->hspi, standby_cmd, 3, 100);
    ADS131M08_CS_Disable(hads);

    HAL_Delay(1); // Wait for reset to complete

    return status;

}

HAL_StatusTypeDef ADS131M08_Wakeup(ADS131M08_HandleTypeDef *hads) {
	uint8_t wake_cmd[3] = {0x00, 0x33, 0x00};
    ADS131M08_CS_Enable(hads);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hads->hspi, wake_cmd, 3, 100);
    ADS131M08_CS_Disable(hads);

    HAL_Delay(1); // Wait for reset to complete

    return status;
}

HAL_StatusTypeDef ADS131M08_ReadReg(ADS131M08_HandleTypeDef *hads, uint8_t reg, uint16_t *data) {
    uint8_t tx_buf[3] = {0};
    uint8_t rx_buf[3] = {0};

    uint16_t cmd = ADS131M08_READ_REG_CMD | (reg << 7);

    tx_buf[0] = cmd >> 8;   // 101a aaaa (address bits a4-a0)
    tx_buf[1] = (cmd & 0x00FF);                  // annn nnnn (a5=0 for 1 register, nnn nnnn=0)

    HAL_StatusTypeDef status = ADS131M08_Receive(hads, tx_buf, rx_buf);

    if (status == HAL_OK) {
        *data = (rx_buf[0] << 8) | rx_buf[1];
    }

    return status;
}

HAL_StatusTypeDef ADS131M08_WriteReg(ADS131M08_HandleTypeDef *hads, uint8_t reg, uint16_t data) {
    uint8_t tx_buf[6] = {0};

    uint16_t cmd = ADS131M08_WRITE_REG_CMD | (reg << 7);

    tx_buf[0] = cmd >> 8;         // 101a aaaa (address bits a4-a0)
    tx_buf[1] = (cmd & 0x00FF);   // annn nnnn (a5=0 for 1 register, nnn nnnn=0)
    tx_buf[2] = 0x00;
    tx_buf[3] = (data >> 8) & 0xFF;     // DATA_HI
    tx_buf[4] = data & 0xFF;           // DATA_LO
    tx_buf[5] = 0x00;

    ADS131M08_CS_Enable(hads);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hads->hspi, tx_buf, 6, 100);
    ADS131M08_CS_Disable(hads);

    return status;
}

HAL_StatusTypeDef ADS131M08_EnableGlobalChopMode(ADS131M08_HandleTypeDef *hads) {
	uint16_t rx_buf[3] = {0};

	HAL_StatusTypeDef status = ADS131M08_WriteReg(hads, 0x6, 0x0100);
	return status;
	if (status == HAL_OK)
			ADS131M08_ReadReg(hads, 0x6, rx_buf);

	if (0x7 == rx_buf[0]) return HAL_ERROR;

	return HAL_OK;
}

HAL_StatusTypeDef ADS131M08_ReadData(ADS131M08_HandleTypeDef *hads) {
	int32_t upperByte, middleByte, lowerByte;
    uint8_t tx_buf[3 + (3 * ADS131M08_NUM_CHANNELS) + 3] = {0};
    uint8_t rx_buf[3 + (3 * ADS131M08_NUM_CHANNELS) + 3] = {0};


    ADS131M08_CS_Enable(hads);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hads->hspi, tx_buf, rx_buf,
                                      sizeof(rx_buf), 100);
    ADS131M08_CS_Disable(hads);

    if (status == HAL_OK) {
        // Parse channel data (each channel is 3 bytes)

        for (int i = 0; i < ADS131M08_NUM_CHANNELS; i++) {
        	upperByte   = ((int32_t) rx_buf[3 + (i*3)] << 24);
			middleByte  = ((int32_t) rx_buf[4 + (i*3)] << 16);
			lowerByte   = ((int32_t) rx_buf[5 + (i*3)] << 8);

            hads->raw_voltage[i] = (((int32_t) (upperByte | middleByte | lowerByte)) >> 8);
        }
    }

    return status;
}

HAL_StatusTypeDef ADS131M08_Receive(ADS131M08_HandleTypeDef *hads, uint8_t *tx_buf, uint8_t *rx_buf) {
	uint8_t buf[3] = {0};
	SPI_HandleTypeDef *hspi = hads->hspi;
	uint32_t junk;
	while (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)) {
		junk = hspi->Instance->DR;  // Read to clear RXNE flag
		UNUSED(junk);  // Prevent compiler warnings
	}

    ADS131M08_CS_Enable(hads);
    HAL_SPI_Transmit(hads->hspi, tx_buf, 3, 100);
    ADS131M08_CS_Disable(hads);
    asm("nop");
    ADS131M08_CS_Enable(hads);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hads->hspi, buf, rx_buf, 3, 100);
    ADS131M08_CS_Disable(hads);
    return status;
}

void ADS131M08_ConvertToVoltage(ADS131M08_HandleTypeDef *hads, uint8_t channel) {
    // Calculate voltage based on reference voltage and PGA setting
    float pga_gain = 1 << hads->pga; // Assuming PGA is set in powers of 2
    hads->conv_voltage[channel] = ((hads->raw_voltage[channel] * hads->vref) / (8388608.0f * pga_gain)) *2 ; // 8388608 = 2^23
}

void ADS131M08_ConvertAllToVoltage(ADS131M08_HandleTypeDef *hads) {
	//float pga_gain = 1 << hads->pga; // Assuming PGA is set in powers of 2
	for (int i = 0; i < ADS131M08_NUM_CHANNELS; i++) {
		ADS131M08_ConvertToVoltage(hads, i);
	}
}

// Private functions
static void ADS131M08_CS_Enable(ADS131M08_HandleTypeDef *hads) {
    HAL_GPIO_WritePin(hads->cs_port, hads->cs_pin, GPIO_PIN_RESET);
}

static void ADS131M08_CS_Disable(ADS131M08_HandleTypeDef *hads) {
    HAL_GPIO_WritePin(hads->cs_port, hads->cs_pin, GPIO_PIN_SET);
}


