#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include "fsl_irtc.h"
#include "fsl_debug_console.h"
void RTC_Update(void);
void RTC_DriverInit(void);
void RTC_PrintTime(void);
irtc_datetime_t RTC_GetTime(void);

#endif
