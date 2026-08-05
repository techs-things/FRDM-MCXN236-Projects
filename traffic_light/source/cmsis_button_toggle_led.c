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
	GPIO_PortClear(GPIO4, 1UL << 18U);
			    GPIO_PortSet(GPIO4, (1UL << 19U) | (1UL << 17U));   // Red OFF
			    GPIO_PortClear(GPIO4, 1UL << 19U);
			            			    GPIO_PortSet(GPIO4, (1UL << 18U) | (1UL << 17U));

    switch(state)
    {
        case TRAFFIC_RED:
        	GPIO_PortClear(GPIO4, 1UL << 18U);
        	GPIO_PortSet(GPIO4, (1UL << 19U) | (1UL << 17U));
            break;

        case TRAFFIC_GREEN:
        	GPIO_PortClear(GPIO4, 1UL << 19U);
        	GPIO_PortSet(GPIO4, (1UL << 18U) | (1UL << 17U));
            break;

        case TRAFFIC_YELLOW:
        	 GPIO_PortClear(GPIO4, (1UL << 18U) | (1UL << 19U));
        	 GPIO_PortSet(GPIO4, 1UL << 17U);
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

    EXAMPLE_BUTTON_GPIO_INTERFACE.Setup(EXAMPLE_BUTTON_PIN, BUTTON_EventCallback);
       EXAMPLE_BUTTON_GPIO_INTERFACE.SetEventTrigger(EXAMPLE_BUTTON_PIN, ARM_GPIO_TRIGGER_FALLING_EDGE);
    gpio_pin_config_t ledConfig =
      {
          kGPIO_DigitalOutput,
          1U      // Active LOW LEDs -> 1 means OFF initially
      };

      GPIO_PinInit(GPIO4, 18U, &ledConfig);   // RED
      GPIO_PinInit(GPIO4, 19U, &ledConfig);   // GREEN
      GPIO_PinInit(GPIO4, 17U, &ledConfig);   // BLUE



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

            GPIO_PortClear(GPIO4, 1UL << 18U);
            GPIO_PortSet(GPIO4, (1UL << 19U) | (1UL << 17U));

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




