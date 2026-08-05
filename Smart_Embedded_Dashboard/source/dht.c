#include "dht.h"

#include "board.h"
#include "pin_mux.h"

#include "fsl_gpio.h"
#include "fsl_common.h"

#include <math.h>

#include "performance.h"

static uint8_t DHT_ReadBit(void);
static uint8_t DHT_ReadByte(void);

static float g_temperature = 0.0f;
static float g_humidity = 0.0f;

static void DHT_SetInput(void);
static void DHT_SetOutput(void);

static void DHT_Write(bool state);
static bool DHT_ReadPin(void);

static void DHT_StartSignal(void);
static bool DHT_WaitForResponse(void);
bool DHT_Read(void);
bool DHT_ReadReliable(void);

void DHT_Init(void)
{
    DHT_SetInput();
}

static void DHT_SetInput(void)
{
    BOARD_INITDHTPINS_DHT_DATA_GPIO->PDDR &=
        ~(1UL << BOARD_INITDHTPINS_DHT_DATA_PIN);
}

static void DHT_SetOutput(void)
{
    BOARD_INITDHTPINS_DHT_DATA_GPIO->PDDR |=
        (1UL << BOARD_INITDHTPINS_DHT_DATA_PIN);
}


static void DHT_Write(bool state)
{
    if(state)
    {
        GPIO_PortSet(
            BOARD_INITDHTPINS_DHT_DATA_GPIO,
            1UL << BOARD_INITDHTPINS_DHT_DATA_PIN);
    }
    else
    {
        GPIO_PortClear(
            BOARD_INITDHTPINS_DHT_DATA_GPIO,
            1UL << BOARD_INITDHTPINS_DHT_DATA_PIN);
    }
}

static bool DHT_ReadPin(void)
{
    return (bool)((BOARD_INITDHTPINS_DHT_DATA_GPIO->PDIR >>
                   BOARD_INITDHTPINS_DHT_DATA_PIN) & 0x01U);
}

float DHT_GetTemperature(void)
{
    return g_temperature;
}

float DHT_GetHumidity(void)
{
    return g_humidity;
}

static void DHT_StartSignal(void)
{
    /* MCU drives the line */
    DHT_SetOutput();

    /* Pull DATA LOW for at least 1 ms */
    DHT_Write(false);

    SDK_DelayAtLeastUs(1200, SystemCoreClock);

    /* Release DATA line */
    DHT_Write(true);

    /* Wait 30 us */
    SDK_DelayAtLeastUs(30, SystemCoreClock);

    /* Give control to the sensor */
    DHT_SetInput();
}

static bool DHT_WaitForResponse(void)
{
    uint32_t timeout;

    /* Sensor pulls DATA LOW (≈80 us) */
    timeout = 100000;

    while(DHT_ReadPin())
    {
        if(--timeout == 0)
        {
            return false;
        }
    }

    /* Sensor pulls DATA HIGH (≈80 us) */
    timeout = 100000;

    while(!DHT_ReadPin())
    {
        if(--timeout == 0)
        {
            return false;
        }
    }

    /* Wait until HIGH pulse ends */
    timeout = 100000;

    while(DHT_ReadPin())
    {
        if(--timeout == 0)
        {
            return false;
        }
    }

    /* Ready to receive the first data bit */
    return true;
}

static uint8_t DHT_ReadBit(void)
{
    uint32_t timeout;
    uint32_t startCycles;
    uint32_t endCycles;
    uint32_t pulseCycles;

    /* Wait until DATA goes HIGH */
    timeout = 100000;

    while(!DHT_ReadPin())
    {
        if(--timeout == 0)
        {
            return 0;
        }
    }

    /* Start measuring HIGH pulse */
    startCycles = Performance_GetCycles();

    timeout = 100000;

    while(DHT_ReadPin())
    {
        if(--timeout == 0)
        {
            return 0;
        }
    }

    endCycles = Performance_GetCycles();

    pulseCycles = endCycles - startCycles;

    /*
     * 12 MHz
     * 0 bit ≈ 320 cycles
     * 1 bit ≈ 840 cycles
     */

    if(pulseCycles > 570U)
    {
        return 1U;
    }

    return 0U;
}

static uint8_t DHT_ReadByte(void)
{
    uint8_t value = 0;

    for(uint8_t i = 0; i < 8; i++)
    {
        value <<= 1;

        value |= DHT_ReadBit();
    }

    return value;
}

bool DHT_Read(void)
{
    uint8_t humidityHigh;
    uint8_t humidityLow;
    uint8_t temperatureHigh;
    uint8_t temperatureLow;
    uint8_t receivedChecksum;
    uint8_t calculatedChecksum;

    uint16_t rawHumidity;
    uint16_t rawTemperature;

    float temperature;
    float humidity;

    /* Disable interrupts during timing-critical communication */
    __disable_irq();

    /* Send start signal */
    DHT_StartSignal();

    /* Wait for sensor response */
    if(!DHT_WaitForResponse())
    {
        __enable_irq();
        return false;
    }

    /* Read 40 bits */
    humidityHigh     = DHT_ReadByte();
    humidityLow      = DHT_ReadByte();

    temperatureHigh  = DHT_ReadByte();
    temperatureLow   = DHT_ReadByte();

    receivedChecksum = DHT_ReadByte();

    /* Communication finished */
    __enable_irq();

    /* Calculate checksum */
    calculatedChecksum =
            humidityHigh +
            humidityLow +
            temperatureHigh +
            temperatureLow;

    /* Verify checksum */
    if(receivedChecksum != calculatedChecksum)
    {
        return false;
    }

    /* Combine humidity bytes */
    rawHumidity =
            ((uint16_t)humidityHigh << 8) |
             humidityLow;

    /* Combine temperature bytes */
    rawTemperature =
            ((uint16_t)temperatureHigh << 8) |
             temperatureLow;

    /* Negative temperature */
    if(rawTemperature & 0x8000)
    {
        rawTemperature &= 0x7FFF;

        temperature = -((float)rawTemperature / 10.0f);
    }
    else
    {
        temperature = (float)rawTemperature / 10.0f;
    }

    /* Humidity */
    humidity = (float)rawHumidity / 10.0f;

    /* Sensor range validation */
    if(temperature < -40.0f || temperature > 80.0f)
    {
        return false;
    }

    if(humidity < 0.0f || humidity > 100.0f)
    {
        return false;
    }

    /* Update latest readings */
    g_temperature = temperature;
    g_humidity    = humidity;

    return true;
}

bool DHT_ReadReliable(void)
{
    uint8_t retry;

    for(retry = 0; retry < 5; retry++)
    {
        if(DHT_Read())
        {
            return true;
        }

        /* Small delay before retrying */
        SDK_DelayAtLeastUs(5000U, SystemCoreClock);
    }

    return false;
}
