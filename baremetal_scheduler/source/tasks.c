#include "tasks.h"
#include "scheduler.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

static bool ledState = false;
static uint32_t stopwatch = 0;

/*------------------------------------------------*/
void LED_Task(void)
/*------------------------------------------------*/
{
    ledState = !ledState;

    GPIO_PinWrite(BOARD_LED_RED_GPIO,
                  BOARD_LED_RED_GPIO_PIN,
                  ledState ? LOGIC_LED_ON : LOGIC_LED_OFF);
}

/*------------------------------------------------*/
void UART_Task(void)
/*------------------------------------------------*/
{
    PRINTF("UART Task Running\r\n");
}

/*------------------------------------------------*/
void Stopwatch_Task(void)
/*------------------------------------------------*/
{
    stopwatch++;

    PRINTF("Stopwatch = %lu sec\r\n", stopwatch);
}

/*------------------------------------------------*/
void Button_Task(void)
/*------------------------------------------------*/
{
    if(GPIO_PinRead(BOARD_SW3_GPIO,
                    BOARD_SW3_GPIO_PIN) == 0)
    {
        PRINTF("SW3 Pressed\r\n");
    }
}
void Monitor_Task(void)
{
    Scheduler_PrintTasks();
}
