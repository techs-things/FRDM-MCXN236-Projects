#include "scheduler.h"
#include "fsl_debug_console.h"
#include <string.h>
#include "timer.h"
#include "performance.h"
/*---------------------------------------------------------
 * Global Tick Counter
 *--------------------------------------------------------*/
volatile uint32_t g_msTicks = 0;
static uint32_t previousIdleRuns = 0;
static int8_t idleTaskIndex = -1;
static uint32_t cpuLoad = 0;
/*---------------------------------------------------------
 * Task Table
 *--------------------------------------------------------*/
SchedulerTask_t taskTable[MAX_TASKS];

/* Number of registered tasks */
uint8_t taskCount = 0;

/*---------------------------------------------------------
 * Scheduler Initialization
 *--------------------------------------------------------*/
void Scheduler_Init(void)
{
    taskCount = 0;

    for(uint8_t i = 0; i < MAX_TASKS; i++)
    {
        taskTable[i].task = NULL;
        taskTable[i].name[0] = '\0';
        taskTable[i].period = 0;
        taskTable[i].priority = 0;
        taskTable[i].state = TASK_READY;
        taskTable[i].runCount = 0;
        taskTable[i].enabled = false;
        taskTable[i].missedDeadlines = 0;
        taskTable[i].totalExecutionTime = 0;
        taskTable[i].maxExecutionTime = 0;
    }
}

/*---------------------------------------------------------
 * Register a Task
 *--------------------------------------------------------*/
void Scheduler_AddTask(
        void (*task)(void),
        const char *name,
        uint32_t period,
        uint8_t priority,
        uint8_t timerID)
{
	if(taskCount >= MAX_TASKS)
	{
	    return;
	}

    taskTable[taskCount].task = task;
    strncpy(taskTable[taskCount].name,
            name,
            sizeof(taskTable[taskCount].name)-1);

    taskTable[taskCount].name[
    sizeof(taskTable[taskCount].name)-1] = '\0';
    taskTable[taskCount].period = period;
    taskTable[taskCount].priority = priority;
    taskTable[taskCount].state = TASK_READY;
    taskTable[taskCount].runCount = 0;
    taskTable[taskCount].timerID = timerID;
    taskTable[taskCount].enabled = true;

    if(strcmp(name, "IDLE") == 0)
    {
        idleTaskIndex = taskCount;
    }

    taskCount++;

}

uint32_t Scheduler_GetTicks(void)
{
    return g_msTicks;
}

/*---------------------------------------------------------
 * Scheduler
 *--------------------------------------------------------*/
void Scheduler_Run(void)
{
    /* Highest priority first */
    for(int8_t priority = 5; priority >= 0; priority--)
    {
        for(uint8_t i = 0; i < taskCount; i++)
        {
            if(taskTable[i].enabled &&
               taskTable[i].priority == priority &&
               taskTable[i].state != TASK_SUSPENDED)
            {
                if(Timer_Expired(taskTable[i].timerID))
                {
                    taskTable[i].state = TASK_RUNNING;

                    uint32_t startCycles;
                    uint32_t endCycles;
                    uint32_t executionCycles;

                    startCycles = Performance_GetCycles();

                    /* Execute Task */
                    taskTable[i].task();

                    endCycles = Performance_GetCycles();

                    executionCycles = endCycles - startCycles;

                    /* ======== ADD THESE LINES HERE ======== */

                    taskTable[i].totalExecutionTime += executionCycles;

                    if(executionCycles > taskTable[i].maxExecutionTime)
                    {
                        taskTable[i].maxExecutionTime = executionCycles;
                    }

                    /* ====================================== */

                    taskTable[i].runCount++;

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
    const char *state;

    PRINTF("\r\n");
    PRINTF("=====================================================================\r\n");
    PRINTF("Task\t taskid \t\tPri\tPeriod\tRuns\tMiss\tState\r\n");
    PRINTF("=====================================================================\r\n");

    for(uint8_t i = 0; i < taskCount; i++)
    {
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
            case TASK_DISABLED:
                state = "DISABLED";
                break;

            default:
                state = "UNKNOWN";
                break;
        }

        PRINTF("%s\t\t", taskTable[i].name);
        PRINTF("%d\t\t", taskTable[i].timerID);
        PRINTF("%d\t", taskTable[i].priority);
        PRINTF("%lu\t", taskTable[i].period);
        PRINTF("%lu\t", taskTable[i].runCount);
        PRINTF("%lu\t", taskTable[i].missedDeadlines);
        PRINTF("%s\r\n", state);
    }

    PRINTF("=====================================================================\r\n");
}




/*---------------------------------------------------------
 * Disable Task
 *--------------------------------------------------------*/
void Scheduler_DisableTask(uint8_t id)
{
    if(id >= taskCount)
        return;

    taskTable[id].enabled = false;
    taskTable[id].state = TASK_DISABLED;
}
void Scheduler_SuspendTask(uint8_t id)
{
    if(id >= taskCount)
        return;

    taskTable[id].state = TASK_SUSPENDED;
}

void Scheduler_ResumeTask(uint8_t id)
{
    if(id >= taskCount)
        return;

    taskTable[id].state = TASK_READY;
}

/*---------------------------------------------------------
 * Enable Task
 *--------------------------------------------------------*/

void Scheduler_EnableTask(uint8_t id)
{
    if(id >= taskCount)
        return;

    taskTable[id].enabled = true;
    taskTable[id].state = TASK_READY;
}

SchedulerTask_t *Scheduler_GetTaskTable(void)
{
    return taskTable;
}

uint8_t Scheduler_GetTaskCount(void)
{
    return taskCount;
}

uint32_t Scheduler_GetCPULoad(void)
{
    return cpuLoad;
}

void Scheduler_UpdateCPULoad(void)
{
    SchedulerTask_t *tasks = Scheduler_GetTaskTable();

    uint32_t idleRuns = 0;


    if(idleTaskIndex >= 0)
    {
        idleRuns = taskTable[idleTaskIndex].runCount;
    }

    uint32_t idleDifference = idleRuns - previousIdleRuns;

    previousIdleRuns = idleRuns;

    if(idleDifference > 5000)
    {
        idleDifference = 5000;
    }

    cpuLoad = 100 - ((idleDifference * 100) / 5000);
}

SchedulerStats_t Scheduler_GetStatistics(void)
{
    SchedulerStats_t stats = {0};

    stats.totalTasks = taskCount;

    for(uint8_t i = 0; i < taskCount; i++)
    {
        switch(taskTable[i].state)
        {
            case TASK_READY:
                stats.readyTasks++;
                break;

            case TASK_RUNNING:
                stats.runningTasks++;
                break;

            case TASK_BLOCKED:
                stats.blockedTasks++;
                break;

            case TASK_SUSPENDED:
                stats.suspendedTasks++;
                break;
        }

        stats.totalMissedDeadlines +=
                taskTable[i].missedDeadlines;
    }

    return stats;
}
