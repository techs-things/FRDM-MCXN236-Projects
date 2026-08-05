#include "scheduler.h"
#include "fsl_debug_console.h"

/*---------------------------------------------------------
 * Global Tick Counter
 *--------------------------------------------------------*/
volatile uint32_t g_msTicks = 0;

/*---------------------------------------------------------
 * Task Table
 *--------------------------------------------------------*/
SchedulerTask_t taskTable[MAX_TASKS];

/* Number of registered tasks */
static uint8_t taskCount = 0;

/*---------------------------------------------------------
 * Scheduler Initialization
 *--------------------------------------------------------*/
void Scheduler_Init(void)
{
    taskCount = 0;

    for(uint8_t i = 0; i < MAX_TASKS; i++)
    {
        taskTable[i].task = NULL;
        taskTable[i].name = NULL;
        taskTable[i].period = 0;
        taskTable[i].lastRun = 0;
        taskTable[i].nextRun = 0;
        taskTable[i].priority = 0;
        taskTable[i].state = TASK_READY;
        taskTable[i].runCount = 0;
        taskTable[i].enabled = false;
    }
}

/*---------------------------------------------------------
 * Register a Task
 *--------------------------------------------------------*/
bool Scheduler_AddTask(TaskFunction_t task,
                       char *name,
                       uint32_t period,
                       uint8_t priority)
{
    if(taskCount >= MAX_TASKS)
    {
        return false;
    }

    taskTable[taskCount].task = task;
    taskTable[taskCount].name = name;
    taskTable[taskCount].period = period;
    taskTable[taskCount].lastRun = 0;
    taskTable[taskCount].nextRun = period;
    taskTable[taskCount].priority = priority;
    taskTable[taskCount].state = TASK_READY;
    taskTable[taskCount].runCount = 0;
    taskTable[taskCount].enabled = true;

    taskCount++;

    return true;
}

/*---------------------------------------------------------
 * Scheduler
 *--------------------------------------------------------*/
void Scheduler_Run(void)
{
    uint32_t currentTick = g_msTicks;

    /* Highest priority first */
    for(int8_t p = 5; p >= 0; p--)
    {
        for(uint8_t i = 0; i < taskCount; i++)
        {
            if(taskTable[i].enabled &&
               taskTable[i].priority == p)
            {
                if(currentTick >= taskTable[i].nextRun)
                {
                    taskTable[i].state = TASK_RUNNING;

                    taskTable[i].task();

                    taskTable[i].runCount++;

                    taskTable[i].lastRun = currentTick;

                    taskTable[i].nextRun =
                        currentTick + taskTable[i].period;

                    taskTable[i].state = TASK_READY;
                }
            }
        }
    }
}

/*---------------------------------------------------------
 * Print Scheduler Information
 *--------------------------------------------------------*/
void Scheduler_PrintTasks(void)
{
    PRINTF("\r\n");
    PRINTF("==============================================\r\n");
    PRINTF("Name\t\tPrio\tRuns\tState\r\n");
    PRINTF("==============================================\r\n");

    for(uint8_t i = 0; i < taskCount; i++)
    {
        const char *state;

        switch(taskTable[i].state)
        {
            case TASK_READY:
                state = "READY";
                break;

            case TASK_RUNNING:
                state = "RUNNING";
                break;

            case TASK_BLOCKED:
                state = "BLOCKED";
                break;

            case TASK_SUSPENDED:
                state = "SUSPENDED";
                break;

            default:
                state = "UNKNOWN";
                break;
        }

        PRINTF("%s\t\t%d\t%lu\t%s\r\n",
               taskTable[i].name,
               taskTable[i].priority,
               taskTable[i].runCount,
               state);
    }

    PRINTF("==============================================\r\n");
}

/*---------------------------------------------------------
 * Enable Task
 *--------------------------------------------------------*/
void Scheduler_EnableTask(uint8_t id)
{
    if(id < taskCount)
    {
        taskTable[id].enabled = true;
    }
}

/*---------------------------------------------------------
 * Disable Task
 *--------------------------------------------------------*/
void Scheduler_DisableTask(uint8_t id)
{
    if(id < taskCount)
    {
        taskTable[id].enabled = false;
    }
}
