#include "mcp41hvx1.h"
#include "main.h"

// Command definitions
#define MCP41HVX1_CMD_WRITE    (0b00 << 2)
#define MCP41HVX1_CMD_READ     (0b11 << 2)
#define MCP41HVX1_CMD_SHUTDOWN 0x20

void MCP41HVX1_Init(MCP41HVX1_HandleTypeDef *hdev, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin) {
    hdev->hspi = hspi;
    hdev->cs_port = cs_port;
    hdev->cs_pin = cs_pin;
    
    // Initialize CS pin
    HAL_GPIO_WritePin(hdev->cs_port, hdev->cs_pin, GPIO_PIN_SET);
}

void MCP41HVX1_SetResistance(MCP41HVX1_HandleTypeDef *hdev, uint8_t value) {
    uint8_t tx_data[2];
    
    // Prepare command and data
    tx_data[0] = MCP41HVX1_CMD_WRITE;
    tx_data[1] = value;
    
    // Pull CS low to select device
    HAL_GPIO_WritePin(hdev->cs_port, hdev->cs_pin, GPIO_PIN_RESET);
    
    // Transmit data
    HAL_SPI_Transmit(hdev->hspi, tx_data, 2, HAL_MAX_DELAY);
    
    // Pull CS high to deselect device
    HAL_GPIO_WritePin(hdev->cs_port, hdev->cs_pin, GPIO_PIN_SET);
}

void MCP41HVX1_Shutdown(MCP41HVX1_HandleTypeDef *hdev) {
    uint8_t tx_data[2];
    
    // Prepare shutdown command
    tx_data[0] = MCP41HVX1_CMD_SHUTDOWN;
    tx_data[1] = 0x00; // Don't care
    
    // Pull CS low to select device
    HAL_GPIO_WritePin(hdev->cs_port, hdev->cs_pin, GPIO_PIN_RESET);
    
    // Transmit data
    HAL_SPI_Transmit(hdev->hspi, tx_data, 2, HAL_MAX_DELAY);
    
    // Pull CS high to deselect device
    HAL_GPIO_WritePin(hdev->cs_port, hdev->cs_pin, GPIO_PIN_SET);
}