#include "rtc.h"
#include "board.h"

static irtc_datetime_t g_rtcTime;

void RTC_DriverInit(void)
{
    irtc_config_t irtcConfig;
    irtc_datetime_t datetime;

    IRTC_GetDefaultConfig(&irtcConfig);

    if (IRTC_Init(RTC, &irtcConfig) == kStatus_Fail)
    {
        PRINTF("RTC Init Failed\r\n");
        return;
    }

#if !defined(FSL_FEATURE_RTC_HAS_NO_GP_DATA_REG) || (!FSL_FEATURE_RTC_HAS_NO_GP_DATA_REG)
    IRTC_Enable32kClkDuringRegisterWrite(RTC, true);
#endif

#if !defined(FSL_FEATURE_RTC_HAS_NO_TAMPER_FEATURE) || (!FSL_FEATURE_RTC_HAS_NO_TAMPER_FEATURE)
    IRTC_ClearTamperStatusFlag(RTC);
#endif

    datetime.year = 2026;
    datetime.month = 8;
    datetime.day = 3;
    datetime.weekDay = 6;
    datetime.hour = 20;
    datetime.minute = 50;
    datetime.second = 0;

    IRTC_SetDatetime(RTC, &datetime);

    /* Initialize cached time */
    RTC_Update();

    PRINTF("RTC Initialized\r\n");
}



void RTC_PrintTime(void)
{
    irtc_datetime_t time;

    IRTC_GetDatetime(RTC, &time);


}
void RTC_Update(void)
{
    IRTC_GetDatetime(RTC, &g_rtcTime);
}

irtc_datetime_t RTC_GetTime(void)
{
    return g_rtcTime;
}
