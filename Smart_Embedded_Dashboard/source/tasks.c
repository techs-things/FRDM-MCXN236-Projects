#include "tasks.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "scheduler.h"
#include "rtc.h"
#include "dashboard.h"
#include "lcd.h"
#include "lcd_dashboard.h"
#include "ultrasonic.h"
#include "ir.h"
#include "dht.h"
#include <math.h>

void LED_Task(void)
{
    static bool ledState = false;

    ledState = !ledState;

    if(ledState)
    {
        GPIO_PinWrite(GPIO4, 18U, 0U);   // Red LED ON (active low)
    }
    else
    {
        GPIO_PinWrite(GPIO4, 18U, 1U);   // Red LED OFF
    }

   // PRINTF("LED Task\r\n");
}


void Monitor_Task(void)
{
	Scheduler_UpdateCPULoad();
    Dashboard_Print();
}

void Idle_Task(void)
{
    /* Do nothing for now */

    // Later we'll use this task to calculate CPU load
}




void RTC_Task(void)
{
    RTC_Update();


}

void LCD_Task(void)
{
    LCD_Dashboard_Update();
}



void Button_Task(void)
{
    static bool lastSW2 = false;
    static bool lastSW3 = false;

    bool sw2;
    bool sw3;

    sw2 = !GPIO_PinRead(BOARD_SW2_GPIO,
                        BOARD_SW2_GPIO_PIN);

    sw3 = !GPIO_PinRead(BOARD_SW3_GPIO,
                        BOARD_SW3_GPIO_PIN);

    if(sw2 && !lastSW2)
    {
        LCD_Dashboard_NextPage();
    }

    if(sw3 && !lastSW3)
    {
        LCD_Dashboard_PreviousPage();
    }

    lastSW2 = sw2;
    lastSW3 = sw3;
}


void Ultrasonic_Task(void)
{
    uint32_t distance;

    distance = Ultrasonic_GetDistanceCM();

    if(distance == 0)
    {
       // PRINTF("No Object\r\n");
    }
    else
    {
        PRINTF("Distance = %lu cm\r\n", distance);
    }
}

void IR_Task(void)
{
//    if(IR_ObjectDetected())
//    {
//        PRINTF("Object Detected\r\n");
//    }
//    else
//    {
//        PRINTF("No Object\r\n");
//    }
}

void DHT_Task(void)
{

    if(DHT_ReadReliable())
    {
        int temp = (int)(DHT_GetTemperature() * 10);
        int hum  = (int)(DHT_GetHumidity() * 10);

        //PRINTF("Temperature = %d.%d C\r\n",
//               temp / 10,
//               abs(temp % 10));
//
//        PRINTF("Humidity    = %d.%d %%\r\n",
//               hum / 10,
//               abs(hum % 10));
    }
    else
    {
        PRINTF("DHT Read Failed\r\n");
    }
}
