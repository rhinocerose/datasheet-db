/*
 * ili9341.h
 *
 *  Created on: Nov 28, 2019
 *      Author: andrew
 */

#ifndef __ILI9341_H
#define __ILI9341_H

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------- includes --

#define STM32H7

#if defined (STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#endif

// ------------------------------------------------------------------ defines --

#define __SPI_TX_BLOCK_MAX__ (1U * 1024U) // 1024 16-bit words (2 KiB)

#define FT6206_MSB_MASK                 0x0F
#define FT6206_MSB_SHIFT                0

  /* Values Pn_XL and Pn_YL related */
#define FT6206_LSB_MASK                 0xFF
#define FT6206_LSB_SHIFT                0
// ------------------------------------------------------------------- macros --

#define __MSBYTEu16(u) (uint8_t)(((uint16_t)(u) >> 8U) & 0xFF)
#define __LSBYTEu16(u) (uint8_t)(((uint16_t)(u)      ) & 0xFF)

// convert value at addr to little-endian (16-bit)
#define __LEu16(addr)                                      \
    ( ( (((uint16_t)(*(((uint8_t *)(addr)) + 1)))      ) | \
        (((uint16_t)(*(((uint8_t *)(addr)) + 0))) << 8U) ) )

// convert value at addr to little-endian (32-bit)
#define __LEu32(addr)                                       \
    ( ( (((uint32_t)(*(((uint8_t *)(addr)) + 3)))       ) | \
        (((uint32_t)(*(((uint8_t *)(addr)) + 2))) <<  8U) | \
        (((uint32_t)(*(((uint8_t *)(addr)) + 1))) << 16U) | \
        (((uint32_t)(*(((uint8_t *)(addr)) + 0))) << 24U) ) )

#define __SWAP(t, a, b) { t s; s = a; a = b; b = s; }

// resulting integer width given as t, e.g. __FROUND(uint16_t, -1.3)
#define __FROUND(t, x) ((x) < 0.0F ? -((t)((-(x)) + 0.5F)) : (t)((x) + 0.5F))
#define __FABS(v) ((v) < 0.0F ? -(v) : (v))

#define ibOK(x)  (0 != !!(x))
#define ibNOT(x) (0 == !!(x))

#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240


/* Level 1 Commands */
#define LCD_SW_RESET            0x01   /* Software Reset */
#define LCD_READ_DISPLAY_ID     0x04   /* Read display identification information */
#define LCD_RDDST               0x09   /* Read Display Status */
#define LCD_RDDPM               0x0A   /* Read Display Power Mode */
#define LCD_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define LCD_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define LCD_RDDIM               0x0D   /* Read Display Image Format */
#define LCD_RDDSM               0x0E   /* Read Display Signal Mode */
#define LCD_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define LCD_SPLIN               0x10   /* Enter Sleep Mode */
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_PTLON               0x12   /* Partial Mode ON */
#define LCD_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define LCD_DINVOFF             0x20   /* Display Inversion OFF */
#define LCD_DINVON              0x21   /* Display Inversion ON */
#define LCD_GAMMA_CURVE_SEL     0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Colomn address register */
#define LCD_PAGE_ADDR           0x2B   /* Page address register */
#define LCD_GRAM                0x2C   /* GRAM register */
#define LCD_RGBSET              0x2D   /* Color SET */
#define LCD_RAMRD               0x2E   /* Memory Read */
#define LCD_PLTAR               0x30   /* Partial Area */
#define LCD_VSCRDEF             0x33   /* Vertical Scrolling Definition */
#define LCD_TEOFF               0x34   /* Tearing Effect Line OFF */
#define LCD_TEON                0x35   /* Tearing Effect Line ON */
#define LCD_MEMORY_ACC_CTRL     0x36   /* Memory Access Control register*/
#define LCD_VSCRSADD            0x37   /* Vertical Scrolling Start Address */
#define LCD_IDMOFF              0x38   /* Idle Mode OFF */
#define LCD_IDMON               0x39   /* Idle Mode ON */
#define LCD_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define LCD_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */
#define LCD_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */
#define LCD_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */
#define LCD_GET_SCANLINE        0x45   /* Get Scanline */
#define LCD_WDB                 0x51   /* Write Brightness Display register */
#define LCD_RDDISBV             0x52   /* Read Display Brightness */
#define LCD_WCD                 0x53   /* Write Control Display register*/
#define LCD_RDCTRLD             0x54   /* Read CTRL Display */
#define LCD_WRCABC              0x55   /* Write Content Adaptive Brightness Control */
#define LCD_RDCABC              0x56   /* Read Content Adaptive Brightness Control */
#define LCD_WRITE_CABC          0x5E   /* Write CABC Minimum Brightness */
#define LCD_READ_CABC           0x5F   /* Read CABC Minimum Brightness */
#define LCD_READ_ID1            0xDA   /* Read ID1 */
#define LCD_READ_ID2            0xDB   /* Read ID2 */
#define LCD_READ_ID3            0xDC   /* Read ID3 */

/* Level 2 Commands */
#define LCD_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define LCD_FRAME_RATE_CTRL_1   0xB1   /* Frame Rate Control (In Normal Mode) */
#define LCD_FRAME_RATE_CTRL_2   0xB2   /* Frame Rate Control (In Idle Mode) */
#define LCD_FRAME_RATE_CTRL_3   0xB3   /* Frame Rate Control (In Partial Mode) */
#define LCD_INVTR               0xB4   /* Display Inversion Control */
#define LCD_BPC                 0xB5   /* Blanking Porch Control register */
#define LCD_DISPLAY_FUNC_CTRL   0xB6   /* Display Function Control register */
#define LCD_ETMOD               0xB7   /* Entry Mode Set */
#define LCD_BACKLIGHT1          0xB8   /* Backlight Control 1 */
#define LCD_BACKLIGHT2          0xB9   /* Backlight Control 2 */
#define LCD_BACKLIGHT3          0xBA   /* Backlight Control 3 */
#define LCD_BACKLIGHT4          0xBB   /* Backlight Control 4 */
#define LCD_BACKLIGHT5          0xBC   /* Backlight Control 5 */
#define LCD_BACKLIGHT7          0xBE   /* Backlight Control 7 */
#define LCD_BACKLIGHT8          0xBF   /* Backlight Control 8 */
#define LCD_POWER_CTRL_1        0xC0   /* Power Control 1 register */
#define LCD_POWER_CTRL_2        0xC1   /* Power Control 2 register */
#define LCD_VCOM_CTRL_1         0xC5   /* VCOM Control 1 register */
#define LCD_VCOM_CTRL_2         0xC7   /* VCOM Control 2 register */
#define LCD_NVMWR               0xD0   /* NV Memory Write */
#define LCD_NVMPKEY             0xD1   /* NV Memory Protection Key */
#define LCD_RDNVM               0xD2   /* NV Memory Status Read */
#define LCD_READ_ID4            0xD3   /* Read ID4 */
#define LCD_POS_GAMMA_CORRECT   0xE0   /* Positive Gamma Correction register */
#define LCD_NEG_GAMMA_CORRECT   0xE1   /* Negative Gamma Correction register */
#define LCD_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define LCD_DGAMCTRL2           0xE3   /* Digital Gamma Control 2 */
#define LCD_INTERFACE           0xF6   /* Interface control register */

/* Extend register commands */
#define LCD_POWER_CTRL_A        0xCB   /* Power control A register */
#define LCD_POWER_CTRL_B        0xCF   /* Power control B register */
#define LCD_DRIVER_TIME_CTRL_A  0xE8   /* Driver timing control A */
#define LCD_DRIVER_TIME_CTRL_B  0xEA   /* Driver timing control B */
#define LCD_POWER_ON_SEQ        0xED   /* Power on sequence register */
#define LCD_3_GAMMA_ENABLE      0xF2   /* 3 Gamma enable register */
#define LCD_PUMP_RATIO_CTRL     0xF7   /* Pump ratio control register */
// ----------------------------------------------------------- exported types --

typedef struct ili9341 ili9341_t;

typedef enum
{
  ibFalse = 0, ibNo  = ibFalse,
  ibTrue  = 1, ibYes = ibTrue,
}
ili9341_bool_t;

typedef ili9341_bool_t ibool_t; // short-hand alias

typedef struct
{
  union
  {
    uint16_t x;
    uint16_t width;
  };
  union
  {
    uint16_t y;
    uint16_t height;
  };
}
ili9341_two_dimension_t;

typedef enum
{
  // orientation is based on position of board pins when looking at the screen
  isoNONE                     = -1,
  isoDown,   isoPortrait      = isoDown,  // = 0
  isoRight,  isoLandscape     = isoRight, // = 1
  isoUp,     isoPortraitFlip  = isoUp,    // = 2
  isoLeft,   isoLandscapeFlip = isoLeft,  // = 3
  isoCOUNT                                // = 4
}
ili9341_screen_orientation_t;

typedef void (*ili9341_touch_callback_t)(ili9341_t *, uint16_t, uint16_t);

typedef HAL_StatusTypeDef ili9341_status_t;

struct ili9341  {
  SPI_HandleTypeDef *spi_hal;

  GPIO_TypeDef *reset_port;
  uint16_t      reset_pin;
  GPIO_TypeDef *tft_select_port;
  uint16_t      tft_select_pin;
  GPIO_TypeDef *data_command_port;
  uint16_t      data_command_pin;

  ili9341_screen_orientation_t orientation;
  ili9341_two_dimension_t      screen_size;
};

// ------------------------------------------------------- exported variables --

/* nothing */

// ------------------------------------------------------- exported functions --

ili9341_t *ili9341_new(

    SPI_HandleTypeDef *spi_hal,

    GPIO_TypeDef *reset_port,        uint16_t reset_pin,
    GPIO_TypeDef *tft_select_port,   uint16_t tft_select_pin,
    GPIO_TypeDef *data_command_port, uint16_t data_command_pin,

    ili9341_screen_orientation_t orientation);

void ili9341_spi_tft_select(ili9341_t *lcd);
void ili9341_spi_tft_release(ili9341_t *lcd);
void ili9341_spi_touch_select(ili9341_t *lcd);
void ili9341_spi_touch_release(ili9341_t *lcd);
void ili9341_spi_slave_select(ili9341_t *lcd);
void ili9341_spi_slave_release(ili9341_t *lcd);

void ili9341_spi_write_command(ili9341_t *lcd, uint8_t command);
void ili9341_spi_write_data(ili9341_t *lcd, uint16_t data_sz, uint8_t data[]);
void ili9341_spi_write_data_read(ili9341_t *lcd, uint16_t data_sz, uint8_t tx_data[], uint8_t rx_data[]);
void ili9341_spi_write_command_data(ili9341_t *lcd, uint8_t command, uint16_t data_sz, uint8_t data[]);

#ifdef __cplusplus
}
#endif

#endif /* __ILI9341_H */
