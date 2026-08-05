#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS   10

typedef struct
{
    uint8_t totalTasks;
    uint8_t readyTasks;
    uint8_t runningTasks;
    uint8_t blockedTasks;
    uint8_t suspendedTasks;

    uint32_t totalMissedDeadlines;

} SchedulerStats_t;

typedef enum
{
    TASK_LED = 0,
    TASK_BUTTON,
    TASK_MONITOR,
    TASK_IDLE,

    TASK_LCD,
    TASK_RTC,
    TASK_SERVO,
    TASK_ULTRASONIC,
    TASK_IR,
    TASK_DHT

} TaskID_t;

/*-------------------------------------------------------
 * Task States
 *------------------------------------------------------*/
typedef enum
{
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED,
    TASK_DISABLED
} TaskState_t;



/*-------------------------------------------------------
 * Task Control Block (TCB)
 *------------------------------------------------------*/
typedef struct
{
    void (*task)(void);

    char name[20];

    uint32_t period;

    uint32_t runCount;

    uint8_t priority;

    bool enabled;

    uint8_t timerID;

    TaskState_t state;

    /* New Fields */

    uint32_t missedDeadlines;

    uint32_t totalExecutionTime;

    uint32_t maxExecutionTime;

} SchedulerTask_t;

/*-------------------------------------------------------
 * Global Variables
 *------------------------------------------------------*/
extern volatile uint32_t g_msTicks;

extern SchedulerTask_t taskTable[MAX_TASKS];

extern uint8_t taskCount;


/*-------------------------------------------------------
 * Scheduler API
 *------------------------------------------------------*/
void Scheduler_Init(void);

void Scheduler_AddTask(
        void (*task)(void),
        const char *name,
        uint32_t period,
        uint8_t priority,
        uint8_t timerID);
SchedulerTask_t *Scheduler_GetTaskTable(void);

uint8_t Scheduler_GetTaskCount(void);

uint32_t Scheduler_GetCPULoad(void);

void Scheduler_UpdateCPULoad(void);
void Scheduler_Run(void);

void Scheduler_PrintTasks(void);

void Scheduler_SuspendTask(uint8_t id);

void Scheduler_EnableTask(uint8_t id);

void Scheduler_DisableTask(uint8_t id);

void Scheduler_ResumeTask(uint8_t id);

uint32_t Scheduler_GetTicks();

SchedulerStats_t Scheduler_GetStatistics(void);
#endif
