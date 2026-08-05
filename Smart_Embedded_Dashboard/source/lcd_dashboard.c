#include "lcd_dashboard.h"
#include "lcd.h"
#include "scheduler.h"
#include "rtc.h"
#include "ultrasonic.h"
#include "ir.h"
#include "dht.h"
#include <math.h>

static DashboardPage_t currentPage = DASHBOARD_PAGE_SYSTEM;

static void Dashboard_DrawHeader(void);
static void Dashboard_DrawSystem(void);
static void Dashboard_DrawScheduler(void);
static void Dashboard_DrawStatus(void);
static void Dashboard_DrawSensor();
static void Dashboard_DrawDHT(void);

static void Dashboard_DrawDHT(void)
{
	int temp = (int)(DHT_GetTemperature() * 10);
    int hum  = (int)(DHT_GetHumidity() * 10);

    LCD_SetCursor(1,0);

    LCD_Printf("T:%d.%dC H:%d.%d %%",
    		temp / 10,abs(temp % 10),
			 hum / 10,abs(hum % 10));
}

static void Dashboard_DrawHeader(void)
{
    irtc_datetime_t time = RTC_GetTime();

    LCD_SetCursor(0,0);

    LCD_Printf("%02d:%02d:%02d",
               time.hour,
               time.minute,
               time.second);
}

static void Dashboard_DrawSystem(void)
{
    LCD_SetCursor(1,0);

    LCD_Printf("CPU:%2lu%%",
               Scheduler_GetCPULoad());
}

static void Dashboard_DrawScheduler(void)
{
    LCD_SetCursor(1,0);

    LCD_Printf("Tick:%-6lu",
               Scheduler_GetTicks());
}

static void Dashboard_DrawSensor(void)
{
    uint32_t distance;

    LCD_SetCursor(1,0);

    if(IR_ObjectDetected())
    {
        distance = Ultrasonic_GetDistanceCM();

        if(distance > 0 && distance <= 400)
        {
            LCD_Printf("Detected:%3lucm", distance);
        }
        else
        {
            LCD_Printf("Detected: ---  ");
        }
    }
    else
    {
        LCD_Printf("No Object      ");
    }
}

static void Dashboard_DrawStatus(void)
{
    LCD_SetCursor(1,0);

    LCD_Printf("LED:ON ");
}
int refreshCounter=0;
void LCD_Dashboard_Update(void)
{
    /* Always update the RTC header */
    Dashboard_DrawHeader();



    switch(currentPage)
    {
        case DASHBOARD_PAGE_SYSTEM:
            Dashboard_DrawSystem();
            break;

        case DASHBOARD_PAGE_SCHEDULER:
            Dashboard_DrawScheduler();
            break;

        case DASHBOARD_PAGE_SENSOR:
            Dashboard_DrawSensor();
            break;

        case DASHBOARD_PAGE_STATUS:
            Dashboard_DrawStatus();
            break;

        case DASHBOARD_PAGE_DHT:
        	Dashboard_DrawDHT();
        	break;

        default:
            break;
    }
}

void LCD_Dashboard_NextPage(void)
{
    currentPage++;

    if(currentPage >= DASHBOARD_PAGE_COUNT)
    {
        currentPage = DASHBOARD_PAGE_SYSTEM;
    }

    LCD_ClearLine(1);
    LCD_Dashboard_Update();
}

void LCD_Dashboard_PreviousPage(void)
{
    if(currentPage == DASHBOARD_PAGE_SYSTEM)
    {
        currentPage = DASHBOARD_PAGE_DHT;
    }
    else
    {
        currentPage--;
    }

    LCD_ClearLine(1);
    LCD_Dashboard_Update();
}
