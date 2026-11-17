/*
 * lcd_i2c.c
 *
 *  Created on: Oct 6, 2026
 *      Author: ACER
 */
#include "lcd_i2c.h"

#define LCD_RS         0x01U
#define LCD_ENABLE     0x04U
#define LCD_BACKLIGHT  0x08U

static I2C_HandleTypeDef *lcd_i2c_handle;
static uint16_t lcd_device_address;

static void LCD_WriteNibble(uint8_t nibble, uint8_t rs)
{
  uint8_t data = (uint8_t)((nibble << 4) | LCD_BACKLIGHT);
  uint8_t pulse[2];

  if (rs != 0U)
  {
    data |= LCD_RS;
  }

  pulse[0] = (uint8_t)(data | LCD_ENABLE);
  pulse[1] = data;

  (void)HAL_I2C_Master_Transmit(lcd_i2c_handle,
                                lcd_device_address,
                                pulse,
                                2U,
                                100U);
}

static void LCD_SendByte(uint8_t value, uint8_t rs)
{
  LCD_WriteNibble((uint8_t)(value >> 4), rs);
  LCD_WriteNibble((uint8_t)(value & 0x0FU), rs);
}

static void LCD_Command(uint8_t command)
{
  LCD_SendByte(command, 0U);
  HAL_Delay(((command == 0x01U) || (command == 0x02U)) ? 2U : 1U);
}

static void LCD_Data(uint8_t data)
{
  LCD_SendByte(data, 1U);
  HAL_Delay(1U);
}

void LCD_Init(I2C_HandleTypeDef *hi2c, uint8_t address_7bit)
{
  lcd_i2c_handle = hi2c;
  lcd_device_address = (uint16_t)(address_7bit << 1);

  HAL_Delay(50U);
  LCD_WriteNibble(0x03U, 0U); HAL_Delay(5U);
  LCD_WriteNibble(0x03U, 0U); HAL_Delay(1U);
  LCD_WriteNibble(0x03U, 0U); HAL_Delay(1U);
  LCD_WriteNibble(0x02U, 0U); HAL_Delay(1U);

  LCD_Command(0x28U);
  LCD_Command(0x0CU);
  LCD_Command(0x06U);
  LCD_Command(0x01U);
}

void LCD_Clear(void)
{
  LCD_Command(0x01U);
}

void LCD_SetCursor(uint8_t row, uint8_t column)
{
  uint8_t address;
  if (column > 15U) column = 15U;
  address = (row == 0U) ? column : (uint8_t)(0x40U + column);
  LCD_Command((uint8_t)(0x80U | address));
}

void LCD_Print(const char *text)
{
  while (*text != '\0')
  {
    LCD_Data((uint8_t)*text);
    text++;
  }
}


