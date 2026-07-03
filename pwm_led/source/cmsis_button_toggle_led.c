

#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio_cmsis.h"
#include "app.h"



uint8_t brightness = 0;



void SetBrightness(uint8_t brightness)
{
    uint32_t period = 1000; // us

    uint32_t onTime =
        (period * brightness) / 100;

    uint32_t offTime =
        period - onTime;

    if(onTime > 0)
    {
        EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
                EXAMPLE_LED_PIN,
                LOGIC_LED_ON);

        SDK_DelayAtLeastUs(
                onTime,
                CLOCK_GetCoreSysClkFreq());
    }

    if(offTime > 0)
    {
        EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
                EXAMPLE_LED_PIN,
                LOGIC_LED_OFF);

        SDK_DelayAtLeastUs(
                offTime,
                CLOCK_GetCoreSysClkFreq());
    }
}

int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nCMSIS GPIO Example! \r\n");
    PRINTF("\r\nUse Button to toggle LED! \r\n");


    EXAMPLE_LED_GPIO_INTERFACE.Setup(EXAMPLE_LED_PIN, NULL);
    EXAMPLE_LED_GPIO_INTERFACE.SetDirection(EXAMPLE_LED_PIN, ARM_GPIO_OUTPUT);
    EXAMPLE_LED_GPIO_INTERFACE.SetOutput(EXAMPLE_LED_PIN, LOGIC_LED_OFF);

    while(1)
    {

        for(uint8_t brightness = 0;
            brightness <= 100;
            brightness++)
        {
            for(uint16_t i = 0; i < 50; i++)
            {
                SetBrightness(brightness);
            }

            PRINTF("Brightness = %d%%\r\n",
                   brightness);
        }


        for(int brightness = 100;
            brightness >= 0;
            brightness--)
        {
            for(uint16_t i = 0; i < 50; i++)
            {
                SetBrightness(brightness);
            }

            PRINTF("Brightness = %d%%\r\n",
                   brightness);
        }
    }
}
