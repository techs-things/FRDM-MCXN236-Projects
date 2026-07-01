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
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Whether the SW button is pressed */
volatile bool g_ButtonPress = false;
uint32_t LEDLevel = LOGIC_LED_OFF;

volatile uint32_t g_msTicks = 0;
volatile bool gameStarted = false;
volatile bool waitingForReaction = false;

uint32_t randomDelay = 0;
uint32_t reactionStartTime = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BUTTON_EventCallback(uint32_t pin, uint32_t event)
{
    if (pin == EXAMPLE_BUTTON_PIN && event == ARM_GPIO_TRIGGER_FALLING_EDGE)
    {
        g_ButtonPress = true;
        PRINTF("\r\nBUTTON Pressed! \r\n");
    }
}

void SysTick_Handler(void)
{
    g_msTicks++;

    if (gameStarted && !waitingForReaction)
    {
        if (g_msTicks >= randomDelay)
        {
            EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
                    EXAMPLE_LED_PIN,
                    LOGIC_LED_ON);

            PRINTF("\r\nGO! Press the button NOW!\r\n");

            waitingForReaction = true;
            reactionStartTime = g_msTicks;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();
    SysTick_Config(SystemCoreClock / 1000U);

    PRINTF("\r\n=== Reaction Timer Game ===\r\n");
    PRINTF("Press button to start.\r\n");

    /* BUTTON pin set up */
    EXAMPLE_BUTTON_GPIO_INTERFACE.Setup(EXAMPLE_BUTTON_PIN, BUTTON_EventCallback);
    EXAMPLE_BUTTON_GPIO_INTERFACE.SetEventTrigger(EXAMPLE_BUTTON_PIN, ARM_GPIO_TRIGGER_FALLING_EDGE);
    /* LED pin set up */
    EXAMPLE_LED_GPIO_INTERFACE.Setup(EXAMPLE_LED_PIN, NULL);
    EXAMPLE_LED_GPIO_INTERFACE.SetDirection(EXAMPLE_LED_PIN, ARM_GPIO_OUTPUT);
    EXAMPLE_LED_GPIO_INTERFACE.SetOutput(EXAMPLE_LED_PIN, LOGIC_LED_OFF);

    while (1)
    {
        if (g_ButtonPress)
        {
            g_ButtonPress = false;

            if (!gameStarted)
            {
                gameStarted = true;
                waitingForReaction = false;

                g_msTicks = 0;

                static uint32_t seed = 1234;
                seed = seed * 1103515245 + 12345;

                randomDelay = 2000 + (seed % 3000);

                PRINTF("\r\nGet Ready...\r\n");
            }
            else if (waitingForReaction)
            {
                uint32_t reactionTime =
                        g_msTicks - reactionStartTime;

                EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
                        EXAMPLE_LED_PIN,
                        LOGIC_LED_OFF);

                PRINTF("\r\nReaction Time = %lu ms\r\n",
                       reactionTime);

                if (reactionTime < 250)
                {
                    PRINTF("Excellent!\r\n");
                }
                else if (reactionTime < 500)
                {
                    PRINTF("Good!\r\n");
                }
                else
                {
                    PRINTF("Too Slow!\r\n");
                }

                PRINTF("\r\nPress button to play again.\r\n");

                gameStarted = false;
                waitingForReaction = false;
            }
            else
            {
                PRINTF("\r\nToo Early! Wait for GO signal.\r\n");

                gameStarted = false;
                waitingForReaction = false;

                EXAMPLE_LED_GPIO_INTERFACE.SetOutput(
                        EXAMPLE_LED_PIN,
                        LOGIC_LED_OFF);
            }
        }
    }
}
