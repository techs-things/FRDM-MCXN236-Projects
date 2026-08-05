#include "ultrasonic.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "performance.h"

void Ultrasonic_Init(void)
{
    GPIO_PinWrite(
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO,
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO_PIN,
        0);
}

void Ultrasonic_Trigger(void)
{
    GPIO_PinWrite(
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO,
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO_PIN,
        0);

    SDK_DelayAtLeastUs(2, SystemCoreClock);

    GPIO_PinWrite(
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO,
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO_PIN,
        1);

    SDK_DelayAtLeastUs(10, SystemCoreClock);

    GPIO_PinWrite(
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO,
        BOARD_INITULTRASONICPINS_ULTRASONIC_TRIG_GPIO_PIN,
        0);


}





bool Ultrasonic_ReadEcho(void)
{
    return GPIO_PinRead(
        BOARD_INITULTRASONICPINS_ULTRASONIC_ECHO_GPIO,
        BOARD_INITULTRASONICPINS_ULTRASONIC_ECHO_GPIO_PIN);
}



uint32_t Ultrasonic_GetDistanceCM(void)
{
    uint32_t startCycles;
    uint32_t endCycles;
    uint32_t pulseCycles;
    uint32_t pulseUs;
    uint32_t distance;

    Ultrasonic_Trigger();

    startCycles = Performance_GetCycles();

    while(!Ultrasonic_ReadEcho())
    {
        if((Performance_GetCycles() - startCycles) >
            (SystemCoreClock/1000000U)*30000U)
        {
            return 0;
        }
    }

    startCycles = Performance_GetCycles();

    while(Ultrasonic_ReadEcho())
    {
        if((Performance_GetCycles() - startCycles) >
            (SystemCoreClock/1000000U)*30000U)
        {
            return 0;
        }
    }

    endCycles = Performance_GetCycles();

    pulseCycles = endCycles - startCycles;

    pulseUs = pulseCycles /
              (SystemCoreClock/1000000U);

    distance = pulseUs / 58U;

    if(distance < 2 || distance > 400)
    {
        return 0;
    }

    return distance;
}

uint32_t Ultrasonic_GetAverageDistance(void)
{
    uint32_t sum = 0;
    uint32_t distance;
    uint8_t count = 0;

    for(uint8_t i = 0; i < 5; i++)
    {
        distance = Ultrasonic_GetDistanceCM();

        if((distance != 0xFFFFFFFF) && (distance <= 80))
        {
            sum += distance;
            count++;
        }

        /* Small delay between measurements */
        SDK_DelayAtLeastUs(60000, SystemCoreClock);
    }

    if(count == 0)
    {
        return 0xFFFFFFFF;
    }

    return sum / count;
}
