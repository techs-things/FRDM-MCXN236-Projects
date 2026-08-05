#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"


#include "scheduler.h"
#include "tasks.h"

void SysTick_Handler(void)
{
    g_msTicks++;
}

int main(void)
{
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

    SysTick_Config(SystemCoreClock / 1000);

    Scheduler_Init();

    Scheduler_AddTask(
            LED_Task,
            "LED",
            500,
            1);

    Scheduler_AddTask(
            UART_Task,
            "UART",
            1000,
            3);

    Scheduler_AddTask(
            Stopwatch_Task,
            "STOPWATCH",
            1000,
            2);

    Scheduler_AddTask(
            Button_Task,
            "BUTTON",
            50,
            4);

    Scheduler_AddTask(
        Monitor_Task,
        "MONITOR",
        5000,
        0);

    PRINTF("\r\nBare Metal Scheduler Started\r\n");

    gpio_pin_config_t sw3Config = {
        kGPIO_DigitalInput,
        0
    };

    GPIO_PinInit(BOARD_SW3_GPIO,
                 BOARD_SW3_GPIO_PIN,
                 &sw3Config);
    gpio_pin_config_t ledConfig =
    {
        kGPIO_DigitalOutput, 1U          // LED OFF initially (active LOW)
    };

    GPIO_PinInit(BOARD_LED_RED_GPIO,
                 BOARD_LED_RED_GPIO_PIN,
                 &ledConfig);
    while(1)
    {
        Scheduler_Run();
    }
}
