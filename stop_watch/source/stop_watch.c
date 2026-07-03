#include <stdio.h>
#include <stdbool.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"

volatile uint32_t stopwatch_ms = 0;

typedef enum
{
    STOPWATCH_RESET,
    STOPWATCH_RUNNING,
    STOPWATCH_STOPPED
} stopwatch_state_t;

volatile stopwatch_state_t stopwatchState =
        STOPWATCH_RESET;

uint32_t lastPrintedSecond = 0;

void SysTick_Handler(void)
{
    if(stopwatchState == STOPWATCH_RUNNING)
    {
        stopwatch_ms++;
    }
}

int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    /* Configure SysTick for 1ms interrupts */
    SysTick_Config(SystemCoreClock / 1000U);

    /* LED Configuration */
    gpio_pin_config_t led_config =
    {
        kGPIO_DigitalOutput,
        1
    };

    /* Button Configuration */
    gpio_pin_config_t button_config =
    {
        kGPIO_DigitalInput,
        0
    };

    GPIO_PinInit(GPIO4,18,&led_config);
    GPIO_PinInit(GPIO0,6,&button_config);

    PRINTF("\r\n===== STOPWATCH =====\r\n");
    PRINTF("Press SW3 to START\r\n");

    bool previousButton = true;

    while(1)
    {
        bool currentButton =
                GPIO_PinRead(GPIO0,6);

        /* Detect button press */
        if(previousButton && !currentButton)
        {
            switch(stopwatchState)
            {
                case STOPWATCH_RESET:

                    stopwatchState =
                            STOPWATCH_RUNNING;

                    GPIO_PinWrite(
                            GPIO4,
                            18,
                            0);

                    PRINTF(
                            "\r\nSTOPWATCH STARTED\r\n");

                    break;

                case STOPWATCH_RUNNING:

                    stopwatchState =
                            STOPWATCH_STOPPED;

                    GPIO_PinWrite(
                            GPIO4,
                            18,
                            1);

                    PRINTF(
                            "\r\nSTOPWATCH STOPPED\r\n");

                    PRINTF(
                        "Final Time = %d.%03d seconds\r\n",
                        stopwatch_ms/1000,
                        stopwatch_ms%1000);

                    break;

                case STOPWATCH_STOPPED:

                    stopwatch_ms = 0;
                    lastPrintedSecond = 0;

                    stopwatchState =
                            STOPWATCH_RESET;

                    PRINTF(
                            "\r\nSTOPWATCH RESET\r\n");

                    PRINTF(
                            "Press SW3 to START\r\n");

                    break;
            }

            SDK_DelayAtLeastUs(
                    200000,
                    CLOCK_GetCoreSysClkFreq());
        }

        previousButton = currentButton;

        /* Print elapsed time every second */
        if(stopwatchState ==
                STOPWATCH_RUNNING)
        {
            if((stopwatch_ms/1000)
                    != lastPrintedSecond)
            {
                lastPrintedSecond =
                        stopwatch_ms/1000;

                PRINTF(
                    "Time = %d seconds\r\n",
                    lastPrintedSecond);
            }
        }
    }
}
