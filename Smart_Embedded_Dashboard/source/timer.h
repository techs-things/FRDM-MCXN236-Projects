#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TIMERS 10

typedef struct
{
    uint8_t totalTimers;
    uint8_t runningTimers;
    uint8_t stoppedTimers;

    uint8_t periodicTimers;
    uint8_t oneShotTimers;

} TimerStatistics_t;

typedef struct
{
    uint8_t id;

    char name[20];

    uint32_t period;

    uint32_t remaining;

    bool autoReload;

    bool running;

    uint32_t expireCount;

    uint32_t startCount;

    uint8_t expired;

} Timer_t;

extern Timer_t timerTable[MAX_TIMERS];

void Timer_Init(void);

uint8_t Timer_Create(const char *name,
                     uint32_t period,
                     bool autoReload);

void Timer_Start(uint8_t id);
void Timer_Stop(uint8_t id);
void Timer_Reset(uint8_t id);

bool Timer_Expired(uint8_t id);

void Timer_Update(void);

TimerStatistics_t Timer_GetStatistics(void);

Timer_t *Timer_GetTable(void);

uint8_t Timer_GetCount(void);

#endif
