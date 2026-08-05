#include "performance.h"

#include "fsl_device_registers.h"

void Performance_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CYCCNT = 0;

    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t Performance_GetCycles(void)
{
    return DWT->CYCCNT;
}
