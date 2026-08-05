#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "fsl_common.h"
#include <stdint.h>
#include <stdbool.h>

/* Maximum number of tasks */
#define MAX_TASKS    10

/* Task Function Pointer */
typedef void (*TaskFunction_t)(void);

/*---------------------------------------------------------
 * Task States
 *--------------------------------------------------------*/
typedef enum
{
    TASK_READY = 0,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED

} TaskState_t;

/*---------------------------------------------------------
 * Task Control Block (TCB)
 *--------------------------------------------------------*/
typedef struct
{
    /* Task Function */
    TaskFunction_t task;

    /* Task Name */
    char *name;

    /* Scheduler Timing */
    uint32_t period;
    uint32_t lastRun;
    uint32_t nextRun;

    /* Task Priority */
    uint8_t priority;

    /* Current Task State */
    TaskState_t state;

    /* Statistics */
    uint32_t runCount;

    /* Enable / Disable */
    bool enabled;

} SchedulerTask_t;

/*---------------------------------------------------------
 * Global Variables
 *--------------------------------------------------------*/

/* Global millisecond tick */
extern volatile uint32_t g_msTicks;

/* Task Table */
extern SchedulerTask_t taskTable[MAX_TASKS];

/*---------------------------------------------------------
 * Scheduler API
 *--------------------------------------------------------*/

/* Initialize Scheduler */
void Scheduler_Init(void);

/* Add a Task */
bool Scheduler_AddTask(TaskFunction_t task,
                       char *name,
                       uint32_t period,
                       uint8_t priority);

/* Run Scheduler */
void Scheduler_Run(void);

/* Print Scheduler Information */
void Scheduler_PrintTasks(void);

/* Enable Task */
void Scheduler_EnableTask(uint8_t id);

/* Disable Task */
void Scheduler_DisableTask(uint8_t id);

#endif
