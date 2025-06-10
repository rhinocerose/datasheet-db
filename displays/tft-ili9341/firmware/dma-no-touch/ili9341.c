/*
 * ili9341.c
 *
 *  Created on: Nov 28, 2019
 *      Author: andrew
 *      TG modifies for touch panel on I2C (and not SPI as here)
 */

// ----------------------------------------------------------------- includes --
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ili9341.h"
#include "ili9341_gfx.h"
#include "ili9341_font.h"

#include "main.h"	// refers to extern i2c handle



// ---------------------------------------------------------- private defines --

// ----------------------------------------------------------- private macros --


// ------------------------------------------------------------ private types --

/* nothing */

// ------------------------------------------------------- exported variables --

/* nothing */

// -------------------------------------------------------- private variables --

/* nothing */

// ---------------------------------------------- private function prototypes --

static void ili9341_reset(ili9341_t *lcd);
static void ili9341_initialize(ili9341_t *lcd);
static ili9341_two_dimension_t ili9341_screen_size(
    ili9341_screen_orientation_t orientation);
static uint8_t ili9341_screen_rotation(
    ili9341_screen_orientation_t orientation);


// ------------------------------------------------------- exported functions --

ili9341_t *ili9341_new(

    SPI_HandleTypeDef *spi_hal,

    GPIO_TypeDef *reset_port,        uint16_t reset_pin,
    GPIO_TypeDef *tft_select_port,   uint16_t tft_select_pin,
    GPIO_TypeDef *data_command_port, uint16_t data_command_pin,

    ili9341_screen_orientation_t orientation)
{
  ili9341_t *lcd = NULL;

  if (NULL != spi_hal) {

    if ( (NULL != reset_port)        && IS_GPIO_PIN(reset_pin)         &&
         (NULL != tft_select_port)   && IS_GPIO_PIN(tft_select_pin)    &&
         (NULL != data_command_port) && IS_GPIO_PIN(data_command_pin)  &&
         (orientation > isoNONE)     && (orientation < isoCOUNT)       ) {

      if (NULL != (lcd = malloc(sizeof(ili9341_t)))) {

          lcd->spi_hal              = spi_hal;

          lcd->reset_port           = reset_port;
          lcd->reset_pin            = reset_pin;
          lcd->tft_select_port      = tft_select_port;
          lcd->tft_select_pin       = tft_select_pin;
          lcd->data_command_port    = data_command_port;
          lcd->data_command_pin     = data_command_pin;

          lcd->orientation          = orientation;
          lcd->screen_size          = ili9341_screen_size(orientation);

          ili9341_initialize(lcd);
      }
    }
  }

  return lcd;
}


void ili9341_spi_tft_select(ili9341_t *lcd)
{
  HAL_GPIO_WritePin(lcd->tft_select_port, lcd->tft_select_pin, GPIO_PIN_RESET);
}


void ili9341_spi_tft_release(ili9341_t *lcd) {
  HAL_GPIO_WritePin(lcd->tft_select_port, lcd->tft_select_pin, GPIO_PIN_SET);
}


void ili9341_spi_slave_select(ili9341_t *lcd) {
  ili9341_spi_tft_select(lcd);
}


void ili9341_spi_slave_release(ili9341_t *lcd) {
    ili9341_spi_tft_release(lcd);
}


void ili9341_spi_write_command(ili9341_t *lcd, uint8_t command) {
  ili9341_spi_slave_select(lcd);

  HAL_GPIO_WritePin(lcd->data_command_port, lcd->data_command_pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(lcd->spi_hal, &command, sizeof(command), HAL_MAX_DELAY);

  ili9341_spi_slave_release(lcd);
}


void ili9341_spi_write_data(ili9341_t *lcd, uint16_t data_sz, uint8_t data[]) {
  ili9341_spi_slave_select(lcd);

  HAL_GPIO_WritePin(lcd->data_command_port, lcd->data_command_pin, GPIO_PIN_SET);
  HAL_SPI_Transmit(lcd->spi_hal, data, data_sz, HAL_MAX_DELAY);

  ili9341_spi_slave_release(lcd);
}


void ili9341_spi_write_data_read(ili9341_t *lcd, uint16_t data_sz, uint8_t tx_data[], uint8_t rx_data[]) {
  ili9341_spi_slave_select(lcd);

  HAL_GPIO_WritePin(lcd->data_command_port, lcd->data_command_pin, GPIO_PIN_SET);
  HAL_SPI_TransmitReceive(lcd->spi_hal, tx_data, rx_data, data_sz, HAL_MAX_DELAY);

  ili9341_spi_slave_release(lcd);
}


void ili9341_spi_write_command_data(ili9341_t *lcd, uint8_t command, uint16_t data_sz, uint8_t data[]) {
  ili9341_spi_slave_select(lcd);

  ili9341_spi_write_command(lcd, command);
  ili9341_spi_write_data(lcd, data_sz, data);

  ili9341_spi_slave_release(lcd);
}

// -------------------------------------------------------- private functions --

static void ili9341_reset(ili9341_t *lcd) {
  HAL_GPIO_WritePin(lcd->reset_port, lcd->reset_pin, GPIO_PIN_RESET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(lcd->reset_port, lcd->reset_pin, GPIO_PIN_SET);
  ili9341_spi_tft_release(lcd);
}


static void ili9341_initialize(ili9341_t *lcd) {
  ili9341_reset(lcd);
  ili9341_spi_tft_select(lcd);

  // command list is based on https://github.com/martnak/STM32-ILI9341

  ili9341_spi_write_command(lcd, LCD_SW_RESET);
  HAL_Delay(1000);

  ili9341_spi_write_command_data(lcd, LCD_POWER_CTRL_A,       5, (uint8_t[]){ 0x39, 0x2C, 0x00, 0x34, 0x02 });
  ili9341_spi_write_command_data(lcd, LCD_POWER_CTRL_B,       3, (uint8_t[]){ 0x00, 0xC1, 0x30 });
  ili9341_spi_write_command_data(lcd, LCD_DRIVER_TIME_CTRL_A, 3, (uint8_t[]){ 0x85, 0x00, 0x78 });
  ili9341_spi_write_command_data(lcd, LCD_DRIVER_TIME_CTRL_B, 2, (uint8_t[]){ 0x00, 0x00 });
  ili9341_spi_write_command_data(lcd, LCD_POWER_ON_SEQ,       4, (uint8_t[]){ 0x64, 0x03, 0x12, 0x81 });
  ili9341_spi_write_command_data(lcd, LCD_PUMP_RATIO_CTRL,    1, (uint8_t[]){ 0x20 });
  ili9341_spi_write_command_data(lcd, LCD_POWER_CTRL_1,       1, (uint8_t[]){ 0x23 });
  ili9341_spi_write_command_data(lcd, LCD_POWER_CTRL_2,       1, (uint8_t[]){ 0x10 });
  ili9341_spi_write_command_data(lcd, LCD_VCOM_CTRL_1,        2, (uint8_t[]){ 0x3E, 0x28 });
  ili9341_spi_write_command_data(lcd, LCD_VCOM_CTRL_2,        1, (uint8_t[]){ 0x86 });
  ili9341_spi_write_command_data(lcd, LCD_MEMORY_ACC_CTRL,    1, (uint8_t[]){ 0x48 });
  ili9341_spi_write_command_data(lcd, LCD_PIXEL_FORMAT,       1, (uint8_t[]){ 0x55 });
  ili9341_spi_write_command_data(lcd, LCD_FRAME_RATE_CTRL_1,  2, (uint8_t[]){ 0x00, 0x18 });
  ili9341_spi_write_command_data(lcd, LCD_DISPLAY_FUNC_CTRL,  3, (uint8_t[]){ 0x08, 0x82, 0x27 });
  ili9341_spi_write_command_data(lcd, LCD_3_GAMMA_ENABLE,     1, (uint8_t[]){ 0x00 });
  ili9341_spi_write_command_data(lcd, LCD_GAMMA_CURVE_SEL,    1, (uint8_t[]){ 0x01 });
  ili9341_spi_write_command_data(lcd, LCD_POS_GAMMA_CORRECT, 15, (uint8_t[]){ 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                             	 	 	 	 	 	 	 	 	 	 	 	  0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 });
  ili9341_spi_write_command_data(lcd, LCD_NEG_GAMMA_CORRECT, 15, (uint8_t[]){ 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                             	 	 	 	 	 	 	 	 	 	 	 	  0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F });

  ili9341_spi_write_command(lcd, LCD_SLEEP_OUT);
  HAL_Delay(120);

  ili9341_spi_write_command(lcd, LCD_DISPLAY_ON);
  ili9341_spi_write_command_data(lcd, LCD_MEMORY_ACC_CTRL,    1, (uint8_t[]){ ili9341_screen_rotation(lcd->orientation) });

  ili9341_spi_tft_release(lcd);
}


static ili9341_two_dimension_t ili9341_screen_size(ili9341_screen_orientation_t orientation) {
  switch (orientation) {
    default:
    case isoDown:
      return (ili9341_two_dimension_t){ { .width = 240U }, { .height = 320U } };
    case isoRight:
      return (ili9341_two_dimension_t){ { .width = 320U }, { .height = 240U } };
    case isoUp:
      return (ili9341_two_dimension_t){ { .width = 240U }, { .height = 320U } };
    case isoLeft:
      return (ili9341_two_dimension_t){ { .width = 320U }, { .height = 240U } };
  }
}


static uint8_t ili9341_screen_rotation(ili9341_screen_orientation_t orientation) {
  switch (orientation) {
    default:
    case isoDown:
      return 0x40 | 0x08;
    case isoRight:
      return 0x40 | 0x80 | 0x20 | 0x08;
    case isoUp:
      return 0x80 | 0x08;
    case isoLeft:
      return 0x20 | 0x08;
  }
}
