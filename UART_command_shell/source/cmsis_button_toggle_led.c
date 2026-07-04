/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio_cmsis.h"
#include "app.h"
#include <string.h>
#include "fsl_lpuart.h"

bool ledState = false;

uint32_t LEDLevel = LOGIC_LED_OFF;

char command[32];
uint8_t index = 0;

bool stopwatchRunning = false;
uint32_t stopwatchSeconds = 0;

bool trafficRunning = false;
bool emergencyMode = false;

volatile uint32_t msTicks = 0;


typedef enum
{
    TRAFFIC_RED,
    TRAFFIC_GREEN,
    TRAFFIC_BLUE
} traffic_state_t;

traffic_state_t trafficState = TRAFFIC_RED;

void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

void Stopwatch_Start(void);
void Stopwatch_Stop(void);
void Stopwatch_Reset(void);

void Traffic_Start(void);
void Traffic_Stop(void);
void Traffic_Emergency(void);
void UART_Task(void);
void ProcessCommand(char *cmd);
void Print_Status(void);

void UART_Task(void)
{
    if (LPUART_GetStatusFlags(LPUART4) & kLPUART_RxDataRegFullFlag)
    {
        char rxChar = LPUART_ReadByte(LPUART4);

        PUTCHAR(rxChar);   // Echo back to Putty

        if(rxChar == '\r' || rxChar == '\n')
        {
            command[index] = '\0';

            ProcessCommand(command);

            index = 0;

            PRINTF("\r\n> ");
        }
        else
        {
            if(index < sizeof(command) - 1)
            {
                command[index++] = rxChar;
            }
        }
    }
}






void LED_On(void)
{
    EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
            EXAMPLE_LED_PIN,
            LOGIC_LED_ON);

    PRINTF("LED ON\r\n");
}

void LED_Off(void)
{
    EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
            EXAMPLE_LED_PIN,
            LOGIC_LED_OFF);

    PRINTF("LED OFF\r\n");
}

void LED_Toggle(void)
{
    if(LEDLevel == LOGIC_LED_OFF)
    {
        LED_On();
        LEDLevel = LOGIC_LED_ON;
    }
    else
    {
        LED_Off();
        LEDLevel = LOGIC_LED_OFF;
    }
}

void Stopwatch_Start(void)
{
    stopwatchRunning = true;
    PRINTF("Stopwatch Started\r\n");
}

void Stopwatch_Stop(void)
{
    stopwatchRunning = false;
    PRINTF("Stopwatch Stopped\r\n");
}

void Stopwatch_Reset(void)
{
    stopwatchSeconds = 0;
    PRINTF("Stopwatch Reset\r\n");
}

void Traffic_UpdateLED(void)
{
    /* Turn all LEDs OFF first */
    GPIO_PinWrite(GPIO4,18U,1U); // Red OFF
    GPIO_PinWrite(GPIO4,19U,1U); // Green OFF
    GPIO_PinWrite(GPIO4,17U,1U); // Blue OFF

    switch(trafficState)
    {
        case TRAFFIC_RED:
            GPIO_PinWrite(GPIO4,18U,0U);
            break;

        case TRAFFIC_GREEN:
            GPIO_PinWrite(GPIO4,19U,0U);
            break;

        case TRAFFIC_BLUE:
            GPIO_PinWrite(GPIO4,17U,0U);
            break;
    }
}

void Traffic_Start(void)
{
    trafficRunning = true;
    PRINTF("Traffic Controller Started\r\n");
}

void Traffic_Stop(void)
{
    trafficRunning = false;
    PRINTF("Traffic Controller Stopped\r\n");
}

void Traffic_Emergency(void)
{
    emergencyMode = !emergencyMode;

    if(emergencyMode)
        PRINTF("Emergency Mode Activated\r\n");
    else
        PRINTF("Emergency Mode Cleared\r\n");
}

void Print_Status(void)
{
    PRINTF("\r\n========== STATUS ==========\r\n");

    PRINTF("LED          : %s\r\n",
           LEDLevel == LOGIC_LED_ON ? "ON" : "OFF");

    PRINTF("Stopwatch    : %s\r\n",
           stopwatchRunning ? "RUNNING" : "STOPPED");

    PRINTF("Traffic Mode : %s\r\n",
           trafficRunning ? "RUNNING" : "STOPPED");

    PRINTF("Emergency    : %s\r\n",
           emergencyMode ? "ACTIVE" : "INACTIVE");
}



void ProcessCommand(char *cmd)
{
    if(strcmp(cmd, "led on") == 0)
        LED_On();

    else if(strcmp(cmd, "led off") == 0)
        LED_Off();

    else if(strcmp(cmd, "toggle") == 0)
        LED_Toggle();

    else if(strcmp(cmd, "stopwatch start") == 0)
        Stopwatch_Start();

    else if(strcmp(cmd, "stopwatch stop") == 0)
        Stopwatch_Stop();

    else if(strcmp(cmd, "stopwatch reset") == 0)
        Stopwatch_Reset();

    else if(strcmp(cmd, "traffic start") == 0)
        Traffic_Start();

    else if(strcmp(cmd, "traffic stop") == 0)
        Traffic_Stop();

    else if(strcmp(cmd, "emergency") == 0)
        Traffic_Emergency();

    else if(strcmp(cmd, "status") == 0)
        Print_Status();
    else if(strcmp(cmd, "help") == 0)
    {
		PRINTF("status\r\n");
		PRINTF("Emergency\r\n");
		PRINTF("traffic start\r\n");
		PRINTF("traffic stop\r\n");
		PRINTF("led on\r\n");
		PRINTF("led off\r\n");
		PRINTF("toggle\r\n");
		PRINTF("stopwatch start\r\n");
		PRINTF("stopwatch stop\r\n");
		PRINTF("stopwatch reset\r\n");
   }
    else
        PRINTF("Unknown Command\r\n");
}


void SysTick_Handler(void)
{
    msTicks++;
}

int main(void)
{
    BOARD_InitHardware();
    SysTick_Config(SystemCoreClock / 1000);

    PRINTF("\r\nUART Command Interface\r\n");
    PRINTF("Type 'help' for available commands\r\n");
    PRINTF("> ");


    /* LED pin set up */
    EXAMPLE_LED_GPIO_INTERFACE.Setup(EXAMPLE_LED_PIN, NULL);
    EXAMPLE_LED_GPIO_INTERFACE.SetDirection(EXAMPLE_LED_PIN, ARM_GPIO_OUTPUT);
    EXAMPLE_LED_GPIO_INTERFACE.SetOutput(EXAMPLE_LED_PIN, LOGIC_LED_OFF);

    /* Configure RGB LED pins as outputs */
    GPIO_PinWrite(GPIO4, 17U, 1U);   // Blue OFF
    GPIO_PinWrite(GPIO4, 18U, 1U);   // Red OFF
    GPIO_PinWrite(GPIO4, 19U, 1U);   // Green OFF

    GPIO4->PDDR |= (1U << 17U);      // Blue output
    GPIO4->PDDR |= (1U << 18U);      // Red output
    GPIO4->PDDR |= (1U << 19U);      // Green output

    while(1)
    {
        /* ---------------- Stopwatch Task ---------------- */
        static uint32_t lastStopwatchTick = 0;

        if(stopwatchRunning)
        {
            if(msTicks - lastStopwatchTick >= 1000)
            {
                stopwatchSeconds++;
                lastStopwatchTick = msTicks;
            }
        }

        /* ---------------- Traffic Task ---------------- */
        static uint32_t lastTrafficTick = 0;

        if(trafficRunning && !emergencyMode)
        {
            switch(trafficState)
            {
                case TRAFFIC_RED:

                    if(msTicks - lastTrafficTick >= 5000)
                    {
                        trafficState = TRAFFIC_GREEN;
                        lastTrafficTick = msTicks;
                        Traffic_UpdateLED();

                        PRINTF("GREEN\r\n");
                    }
                    break;

                case TRAFFIC_GREEN:

                    if(msTicks - lastTrafficTick >= 5000)
                    {
                        trafficState = TRAFFIC_BLUE;
                        lastTrafficTick = msTicks;
                        Traffic_UpdateLED();

                        PRINTF("BLUE\r\n");
                    }
                    break;

                case TRAFFIC_BLUE:

                    if(msTicks - lastTrafficTick >= 2000)
                    {
                        trafficState = TRAFFIC_RED;
                        lastTrafficTick = msTicks;
                        Traffic_UpdateLED();

                        PRINTF("RED\r\n");
                    }
                    break;
            }
        }

        /* ---------------- UART Task ---------------- */

        UART_Task();
    }
}
