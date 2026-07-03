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

volatile bool emergencyExitRequest = false;

typedef enum
{
    TRAFFIC_RED,
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW,
    PEDESTRIAN_MODE,
    EMERGENCY_MODE,
    NIGHT_MODE
} traffic_state_t;

volatile bool g_ButtonPress = false;

volatile traffic_state_t currentState = TRAFFIC_RED;

volatile uint32_t msTicks = 0;

volatile bool pedestrianRequest = false;
volatile bool emergencyMode = false;
volatile bool nightMode = false;

volatile uint32_t buttonPressTime = 0;
volatile uint32_t lastPressTime = 0;
volatile uint8_t pressCount = 0;

void SysTick_Handler(void)
{
    msTicks++;
}

static void BUTTON_EventCallback(uint32_t pin, uint32_t event)
{
    if(pin == EXAMPLE_BUTTON_PIN &&
       event == ARM_GPIO_TRIGGER_FALLING_EDGE)
    {
        uint32_t now = msTicks;

        if(emergencyMode)
        {
            emergencyExitRequest = true;
            return;
        }

        if((now - lastPressTime) < 500)
        {
            emergencyMode = true;
            PRINTF("Emergency Mode Requested\r\n");
        }
        else
        {
            pedestrianRequest = true;
            PRINTF("Pedestrian Request\r\n");
        }

        lastPressTime = now;
    }
}

void SetTrafficLight(traffic_state_t state)
{
    /* Turn OFF both LEDs (active low) */
    GPIO_PinWrite(GPIO4, 18U, 1U);   // Red OFF
    GPIO_PinWrite(GPIO4, 19U, 1U);   // Green OFF

    switch(state)
    {
        case TRAFFIC_RED:
            GPIO_PinWrite(GPIO4, 18U, 0U);
            break;

        case TRAFFIC_GREEN:
            GPIO_PinWrite(GPIO4, 19U, 0U);
            break;

        case TRAFFIC_YELLOW:
            GPIO_PinWrite(GPIO4, 18U, 0U);
            GPIO_PinWrite(GPIO4, 19U, 0U);
            break;

        default:
            break;
    }
}


int main(void)
{
    BOARD_InitHardware();
    SysTick_Config(SystemCoreClock / 1000);

    PRINTF("\r\nCMSIS GPIO Example! \r\n");
    PRINTF("\r\nUse Button to toggle LED! \r\n");

    /* BUTTON pin set up */
    EXAMPLE_BUTTON_GPIO_INTERFACE.Setup(EXAMPLE_BUTTON_PIN, BUTTON_EventCallback);
    EXAMPLE_BUTTON_GPIO_INTERFACE.SetEventTrigger(EXAMPLE_BUTTON_PIN, ARM_GPIO_TRIGGER_FALLING_EDGE);
    /* LED pin set up */
    EXAMPLE_LED_GPIO_INTERFACE.Setup(EXAMPLE_LED_PIN, NULL);
    EXAMPLE_LED_GPIO_INTERFACE.SetDirection(EXAMPLE_LED_PIN, ARM_GPIO_OUTPUT);
    EXAMPLE_LED_GPIO_INTERFACE.SetOutput(EXAMPLE_LED_PIN, LOGIC_LED_OFF);

    while(1)
    {
        /* Handle pedestrian crossing first */
        if(pedestrianRequest)
        {
            PRINTF("Pedestrian Crossing Active\r\n");

            currentState = PEDESTRIAN_MODE;

            SetTrafficLight(TRAFFIC_RED);

            SDK_DelayAtLeastUs(
                    5000000,
                    CLOCK_GetCoreSysClkFreq());

            pedestrianRequest = false;

            PRINTF("Returning to Normal Traffic\r\n");
        }

        /* Normal traffic operation */
        if(!emergencyMode && !nightMode)
        {
            currentState = TRAFFIC_RED;
            SetTrafficLight(currentState);

            PRINTF("RED\r\n");

            SDK_DelayAtLeastUs(
                    5000000,
                    CLOCK_GetCoreSysClkFreq());

            currentState = TRAFFIC_GREEN;
            SetTrafficLight(currentState);

            PRINTF("GREEN\r\n");

            SDK_DelayAtLeastUs(
                    5000000,
                    CLOCK_GetCoreSysClkFreq());

            currentState = TRAFFIC_YELLOW;
            SetTrafficLight(currentState);

            PRINTF("YELLOW\r\n");

            SDK_DelayAtLeastUs(
                    2000000,
                    CLOCK_GetCoreSysClkFreq());
        }

        /* Emergency mode */
        while(emergencyMode)
        {
            PRINTF("Emergency Mode Active\r\n");

            SetTrafficLight(TRAFFIC_YELLOW);

            SDK_DelayAtLeastUs(
                    500000,
                    CLOCK_GetCoreSysClkFreq());

            GPIO_PinWrite(GPIO4,18U,1U);
            GPIO_PinWrite(GPIO4,19U,1U);

            SDK_DelayAtLeastUs(
                    500000,
                    CLOCK_GetCoreSysClkFreq());

            if(emergencyExitRequest)
            {
                emergencyMode = false;
                emergencyExitRequest = false;

                PRINTF("Emergency Mode Cleared\r\n");
            }
        }
    }
}
