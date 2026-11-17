/*
 * lcd_i2c.h
 *
 *  Created on: Oct 6, 2026
 *      Author: ACER
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

#include "stm32f1xx_hal.h"

void LCD_Init(I2C_HandleTypeDef *hi2c, uint8_t address_7bit);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_Print(const char *text);

#endif /* INC_LCD_I2C_H_ */
