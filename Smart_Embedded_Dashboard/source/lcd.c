#include "lcd.h"
#include "peripherals.h"
#include "fsl_lpi2c.h"
#include <stdio.h>
#include <stdarg.h>

static status_t PCF8574_Write(uint8_t data);
static status_t LCD_PulseEnable(uint8_t data);
static status_t LCD_WriteNibble(uint8_t nibble, uint8_t control);
static status_t LCD_SendByte(uint8_t byte, uint8_t control);

#define LCD_RS          (1U << 0)
#define LCD_RW          (1U << 1)
#define LCD_EN          (1U << 2)
#define LCD_BACKLIGHT   (1U << 3)

#define LCD_D4          (1U << 4)
#define LCD_D5          (1U << 5)
#define LCD_D6          (1U << 6)
#define LCD_D7          (1U << 7)

static status_t PCF8574_Write(uint8_t data)
{
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = LCD_I2C_ADDRESS;
    xfer.direction = kLPI2C_Write;
    xfer.subaddress = 0;
    xfer.subaddressSize = 0;
    xfer.data = &data;
    xfer.dataSize = 1;

    return LPI2C_MasterTransferBlocking(
        LP_FLEXCOMM0_PERIPHERAL,
        &xfer);
}

status_t LCD_Test(void)
{
    return PCF8574_Write(0x08);
}

static status_t LCD_WriteNibble(uint8_t nibble, uint8_t control)
{
    uint8_t data;

    data = (nibble << 4);          // Move nibble to D4-D7
    data |= control;               // RS / RW bits
    data |= LCD_BACKLIGHT;         // Keep backlight ON

    return LCD_PulseEnable(data);
}

static status_t LCD_SendByte(uint8_t byte, uint8_t control)
{
    status_t status;

    /* Upper nibble */

    status = LCD_WriteNibble((byte >> 4), control);

    if(status != kStatus_Success)
        return status;

    /* Lower nibble */

    status = LCD_WriteNibble((byte & 0x0F), control);

    return status;
}

static status_t LCD_PulseEnable(uint8_t data)
{
    status_t status;

    /* EN = 1 */
    status = PCF8574_Write(data | LCD_EN);

    if(status != kStatus_Success)
        return status;

    SDK_DelayAtLeastUs(10U, SystemCoreClock);

    /* EN = 0 */
    status = PCF8574_Write(data & ~LCD_EN);

    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    return status;
}

static status_t LCD_SendCommand(uint8_t cmd)
{
    return LCD_SendByte(cmd, 0);
}

static status_t LCD_SendData(uint8_t data)
{
    return LCD_SendByte(data, LCD_RS);
}

status_t LCD_Init(void)
{
    SDK_DelayAtLeastUs(50000U, SystemCoreClock);

    /* Force 8-bit mode */
    LCD_WriteNibble(0x03, 0);
    SDK_DelayAtLeastUs(5000U, SystemCoreClock);

    LCD_WriteNibble(0x03, 0);
    SDK_DelayAtLeastUs(150U, SystemCoreClock);

    LCD_WriteNibble(0x03, 0);
    SDK_DelayAtLeastUs(150U, SystemCoreClock);

    /* Switch to 4-bit mode */
    LCD_WriteNibble(0x02, 0);
    SDK_DelayAtLeastUs(150U, SystemCoreClock);

    /* Function Set */
    LCD_SendCommand(0x28);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
    /* Display OFF */
    LCD_SendCommand(0x08);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
    /* Clear Display */
    LCD_SendCommand(0x01);
    SDK_DelayAtLeastUs(3000U, SystemCoreClock);

    /* Entry Mode */
    LCD_SendCommand(0x06);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
    /* Display ON */
    LCD_SendCommand(0x0C);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
    return kStatus_Success;
}

void LCD_WriteChar(char c)
{
    LCD_SendData((uint8_t)c);
}

void LCD_WriteString(const char *str)
{
    while(*str)
    {
        LCD_WriteChar(*str++);
    }
}


void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address;

    switch(row)
    {
        case 0:
            address = 0x00 + col;
            break;

        case 1:
            address = 0x40 + col;
            break;

        default:
            return;     // Invalid row
    }

    LCD_SendCommand(0x80 | address);
}

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);

    SDK_DelayAtLeastUs(3000U, SystemCoreClock);
}

void LCD_Home(void)
{
    LCD_SendCommand(0x02);

    SDK_DelayAtLeastUs(3000U, SystemCoreClock);
}

void LCD_DisplayOn(void)
{
    LCD_SendCommand(0x0C);
}

void LCD_DisplayOff(void)
{
    LCD_SendCommand(0x08);
}

void LCD_CursorOn(void)
{
    LCD_SendCommand(0x0E);
}

void LCD_CursorOff(void)
{
    LCD_SendCommand(0x0C);
}

void LCD_BlinkOn(void)
{
    LCD_SendCommand(0x0F);
}

void LCD_BlinkOff(void)
{
    LCD_SendCommand(0x0E);
}


void LCD_PrintUInt(uint32_t number)
{
    char buffer[11];
    uint8_t i = 0;

    if(number == 0)
    {
        LCD_WriteChar('0');
        return;
    }

    while(number > 0)
    {
        buffer[i++] = (number % 10) + '0';

        number /= 10;
    }

    while(i > 0)
    {
        LCD_WriteChar(buffer[--i]);
    }
}

void LCD_PrintInt(int32_t number)
{
    char buffer[12];      // Enough for -2147483648
    uint8_t i = 0;

    /* Special case: Zero */
    if(number == 0)
    {
        LCD_WriteChar('0');
        return;
    }

    /* Handle negative numbers */
    if(number < 0)
    {
        LCD_WriteChar('-');
        number = -number;
    }

    /* Convert digits into buffer (stored in reverse order) */
    while(number > 0)
    {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }

    /* Print digits in correct order */
    while(i > 0)
    {
        LCD_WriteChar(buffer[--i]);
    }
}

void LCD_ClearLine(uint8_t row)
{
    LCD_SetCursor(row, 0);

    for(uint8_t i = 0; i < 16; i++)
    {
        LCD_WriteChar(' ');
    }

    LCD_SetCursor(row, 0);
}

void LCD_PrintAt(uint8_t row,
                 uint8_t col,
                 const char *str)
{
    LCD_SetCursor(row, col);

    LCD_WriteString(str);
}

void LCD_PrintIntAt(uint8_t row,
                    uint8_t col,
                    int32_t number)
{
    LCD_SetCursor(row,col);

    LCD_PrintInt(number);
}

void LCD_Printf(const char *format, ...)
{
    char buffer[33];      // 16x2 LCD -> 32 chars maximum

    va_list args;

    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    LCD_WriteString(buffer);
}

