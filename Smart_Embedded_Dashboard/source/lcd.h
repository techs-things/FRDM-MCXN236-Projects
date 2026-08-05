#ifndef LCD_H
#define LCD_H

#include "fsl_common.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>


#define LCD_I2C_ADDRESS    0x27

status_t LCD_Test(void);
status_t LCD_Init(void);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char ch);
void LCD_WriteString(const char *str);
void LCD_Clear(void);
void LCD_Home();
void LCD_DisplayOff(void);
void LCD_DisplayOn(void);
void LCD_CursorOff(void);
void LCD_CursorOn(void);
void LCD_BlinkOff(void);
void LCD_BlinkOn(void);
void LCD_PrintInt(int32_t number);
void LCD_PrintUInt(uint32_t number);
void LCD_PrintIntAt(uint8_t row,uint8_t col,int32_t number);
void LCD_PrintAt(uint8_t row,uint8_t col,const char *str);
void LCD_ClearLine(uint8_t row);
void LCD_Printf(const char *format, ...);
#endif
