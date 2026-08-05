#include "timer.h"

#include <string.h>

Timer_t timerTable[MAX_TIMERS];

static uint8_t timerCount = 0;

/*----------------------------------------------------------*/

void Timer_Init(void)
{
    timerCount = 0;

    for(uint8_t i = 0; i < MAX_TIMERS; i++)
    {
        timerTable[i].id = i;

        timerTable[i].running = false;

        timerTable[i].expired = false;

        timerTable[i].remaining = 0;

        timerTable[i].period = 0;

        timerTable[i].autoReload = false;

        timerTable[i].expireCount = 0;

        timerTable[i].startCount = 0;

        timerTable[i].name[0] = '\0';
    }
}

/*----------------------------------------------------------*/

uint8_t Timer_Create(const char *name,
                     uint32_t period,
                     bool autoReload)
{
    if(timerCount >= MAX_TIMERS)
    {
        return 0xFF;
    }

    uint8_t id = timerCount;

    timerTable[id].id = id;

    strcpy(timerTable[id].name, name);

    timerTable[id].period = period;

    timerTable[id].remaining = period;

    timerTable[id].running = false;

    timerTable[id].expired = false;

    timerTable[id].autoReload = autoReload;

    timerTable[id].expireCount = 0;

    timerTable[id].startCount = 0;

    timerCount++;

    return id;
}

/*----------------------------------------------------------*/

void Timer_Start(uint8_t id)
{
    if(id >= timerCount)
        return;

    timerTable[id].running = true;

    timerTable[id].expired = false;

    timerTable[id].remaining = timerTable[id].period;

    timerTable[id].startCount++;
}

/*----------------------------------------------------------*/

void Timer_Stop(uint8_t id)
{
    if(id >= timerCount)
        return;

    timerTable[id].running = false;
}

/*----------------------------------------------------------*/

void Timer_Reset(uint8_t id)
{
    if(id >= timerCount)
        return;

    timerTable[id].remaining = timerTable[id].period;

    timerTable[id].expired = false;
}

/*----------------------------------------------------------*/

void Timer_Update(void)
{
    for(uint8_t i = 0; i < timerCount; i++)
    {
        if(!timerTable[i].running)
            continue;

        if(timerTable[i].remaining > 0)
        {
            timerTable[i].remaining--;
        }

        if(timerTable[i].remaining == 0)
        {
            timerTable[i].expired = true;

            timerTable[i].expireCount++;

            if(timerTable[i].autoReload)
            {
                timerTable[i].remaining = timerTable[i].period;
            }
            else
            {
                timerTable[i].running = false;
            }
        }
    }
}

/*----------------------------------------------------------*/

bool Timer_Expired(uint8_t id)
{
    if(id >= timerCount)
        return false;

    if(timerTable[id].expired)
    {
        timerTable[id].expired = false;

        return true;
    }

    return false;
}

/*----------------------------------------------------------*/

TimerStatistics_t Timer_GetStatistics(void)
{
    TimerStatistics_t stats = {0};

    stats.totalTimers = timerCount;

    for(uint8_t i = 0; i < timerCount; i++)
    {
        if(timerTable[i].running)
        {
            stats.runningTimers++;
        }
        else
        {
            stats.stoppedTimers++;
        }

        if(timerTable[i].autoReload)
        {
            stats.periodicTimers++;
        }
        else
        {
            stats.oneShotTimers++;
        }
    }

    return stats;
}

/*----------------------------------------------------------*/

Timer_t *Timer_GetTable(void)
{
    return timerTable;
}

/*----------------------------------------------------------*/

uint8_t Timer_GetCount(void)
{
    return timerCount;
}
