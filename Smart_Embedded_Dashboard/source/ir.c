#include "ir.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

/*---------------------------------------------------------
 * Initialize IR Sensor
 *--------------------------------------------------------*/

void IR_Init(void)
{
    /* Pins are already initialized by
       BOARD_InitIRPins() */
}

/*---------------------------------------------------------
 * Detect Object
 *--------------------------------------------------------*/

bool IR_ObjectDetected(void)
{
    /* Most LM393 modules are ACTIVE LOW */

    return !GPIO_PinRead(
            BOARD_INITIRPINS_IR_OUT_GPIO,
            BOARD_INITIRPINS_IR_OUT_PIN);
}

/*---------------------------------------------------------
 * Return Raw State
 *--------------------------------------------------------*/

uint8_t IR_GetState(void)
{
    return GPIO_PinRead(
            BOARD_INITIRPINS_IR_OUT_GPIO,
            BOARD_INITIRPINS_IR_OUT_PIN);
}
