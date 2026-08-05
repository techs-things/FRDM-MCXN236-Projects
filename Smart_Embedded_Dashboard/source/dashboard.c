#include "dashboard.h"

#include "scheduler.h"
#include "rtc.h"
#include "timer.h"
#include "fsl_debug_console.h"
#include "dht.h"

static const char *StateToString(TaskState_t state)
{
    switch(state)
    {
        case TASK_READY:     return "READY";
        case TASK_RUNNING:   return "RUNNING";
        case TASK_BLOCKED:   return "BLOCKED";
        case TASK_SUSPENDED: return "SUSPENDED";
        default:             return "UNKNOWN";
    }
}

static const char *TimerMode(bool autoReload)
{
    return autoReload ? "AUTO" : "ONCE";
}

static const char *TimerState(bool running)
{
    return running ? "RUNNING" : "STOPPED";
}

static const char *YesNo(bool value)
{
    return value ? "YES" : "NO";
}

void Dashboard_Print(void)
{
    irtc_datetime_t now = RTC_GetTime();

    SchedulerStats_t stats = Scheduler_GetStatistics();

    TimerStatistics_t timerStats = Timer_GetStatistics();

    SchedulerTask_t *tasks = Scheduler_GetTaskTable();
    uint8_t taskCount = Scheduler_GetTaskCount();

    PRINTF("\r\n");
    PRINTF("==============================================================\r\n");
    PRINTF("               SMART EMBEDDED DASHBOARD\r\n");
    PRINTF("==============================================================\r\n");

    PRINTF("Date : %02d/%02d/%04d\r\n",
           now.day,
           now.month,
           now.year);

    PRINTF("Time : %02d:%02d:%02d\r\n",
           now.hour,
           now.minute,
           now.second);

    PRINTF("System Tick : %lu ms\r\n",
           g_msTicks);

    PRINTF("CPU Load : %lu %%\r\n",
           Scheduler_GetCPULoad());

    PRINTF("\r\n");

    PRINTF("\r\n");
    PRINTF("---------------------------------------------------------\r\n");
    PRINTF("SCHEDULER STATISTICS\r\n");
    PRINTF("---------------------------------------------------------\r\n");

    PRINTF("Total Tasks      : %d\r\n", stats.totalTasks);

    PRINTF("Ready Tasks      : %d\r\n", stats.readyTasks);

    PRINTF("Running Tasks    : %d\r\n", stats.runningTasks);

    PRINTF("Blocked Tasks    : %d\r\n", stats.blockedTasks);

    PRINTF("Suspended Tasks  : %d\r\n", stats.suspendedTasks);

    PRINTF("Missed Deadlines : %lu\r\n",
           stats.totalMissedDeadlines);

    PRINTF("---------------------------------------------------------\r\n");
    PRINTF("Task\t\tPri\tRuns\tMiss\tState\r\n");
    PRINTF("---------------------------------------------------------\r\n");




    for(uint8_t i = 0; i < taskCount; i++)
    {
        PRINTF("%s\t\t", tasks[i].name);
        PRINTF("%d\t", tasks[i].priority);
        PRINTF("%lu\t", tasks[i].runCount);
        PRINTF("%lu\t", tasks[i].missedDeadlines);
        PRINTF("%s\r\n", StateToString(tasks[i].state));

    }

    PRINTF("\r\n");
    PRINTF("---------------------------------------------------------\r\n");
    PRINTF("TASK PERFORMANCE\r\n");
    PRINTF("---------------------------------------------------------\r\n");
    PRINTF("Task\t\tAvg(Cycles)\tMax(Cycles)\r\n");

    for(uint8_t i = 0; i < taskCount; i++)
    {
        uint32_t avg = 0;

        if(tasks[i].runCount > 0)
        {
            avg = tasks[i].totalExecutionTime / tasks[i].runCount;
        }

        PRINTF("%s\t\t", tasks[i].name);
        PRINTF("%lu\t\t", avg);
        PRINTF("%lu\r\n", tasks[i].maxExecutionTime);
    }

    PRINTF("\r\n");
    PRINTF("---------------------------------------------------------\r\n");
    PRINTF("              SOFTWARE TIMER MANAGER\r\n");
    PRINTF("---------------------------------------------------------\r\n");

    PRINTF("Total Timers    : %d\r\n", Timer_GetCount());
    PRINTF("Running Timers  : %d\r\n", timerStats.runningTimers);
    PRINTF("Stopped Timers  : %d\r\n", timerStats.stoppedTimers);
    PRINTF("Periodic Timers : %d\r\n", timerStats.periodicTimers);
    PRINTF("One Shot Timers : %d\r\n", timerStats.oneShotTimers);

    PRINTF("\r\n");
    PRINTF("----------------------------------------------------------------------------\r\n");
    PRINTF("Name        Period    Remain    Starts       Expired    Mode      State\r\n");
    PRINTF("----------------------------------------------------------------------------\r\n");

    Timer_t *timers = Timer_GetTable();

    for(uint8_t i = 0; i < Timer_GetCount(); i++)
    {
        PRINTF("%s\t\t", timers[i].name);

        PRINTF("%lu\t ", timers[i].period);

        PRINTF("%lu\t ", timers[i].remaining);

        PRINTF("%lu\t\t ", timers[i].startCount);

        PRINTF("%lu\t", timers[i].expireCount);

        PRINTF("%s\t ",
               timers[i].autoReload ? "AUTO" : "ONCE");

        PRINTF("%s\r\n",
               timers[i].running ? "RUNNING" : "STOPPED");
    }


    PRINTF("=========================================================================\r\n");

    int temp = (int)(DHT_GetTemperature() * 10);
    int hum  = (int)(DHT_GetHumidity() * 10);

    PRINTF("Temperature = %d.%d C\r\n",
               temp / 10,
               abs(temp % 10));

        PRINTF("Humidity    = %d.%d %%\r\n",
               hum / 10,
               abs(hum % 10));

}
