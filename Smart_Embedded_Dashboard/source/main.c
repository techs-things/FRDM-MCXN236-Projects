#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "timer.h"
#include "rtc.h"
#include "performance.h"
#include "peripherals.h"
#include "scheduler.h"
#include "tasks.h"
#include "lcd.h"
#include "lcd_dashboard.h"
#include "ultrasonic.h"
#include "ir.h"
#include "dht.h"
#include <math.h>
void SysTick_Handler(void)
{
    g_msTicks++;

    Timer_Update();
}

int main(void)
{


  	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	Performance_Init();
	BOARD_InitBootPeripherals();
	BOARD_InitULTRASONICPins();
	BOARD_InitIRPins();
	BOARD_InitDHTPins();

	float t = 29.4f;
	float h = 76.4f;

	PRINTF("T = %.1f\r\n", t);
	PRINTF("H = %.1f\r\n", h);

	PRINTF("SystemCoreClock = %lu\r\n", SystemCoreClock);

	PRINTF("Main Started\r\n");
	IR_Init();

	LCD_Init();

	LCD_Dashboard_Update();


	DHT_Init();

	CLOCK_SetupClk16KClocking(
	    kCLOCK_Clk16KToVbat |
	    kCLOCK_Clk16KToMain);

	RTC_DriverInit();

    SysTick_Config(SystemCoreClock / 1000);

    Scheduler_Init();

    Ultrasonic_Init();
    Timer_Init();
    //RTC_Init();
    uint8_t ledTimer;
    uint8_t buttonTimer;
    uint8_t monitorTimer;
    uint8_t idleTimer;
    uint8_t rtcTimer;
    uint8_t lcdTimer;
    uint8_t usTimer;
    uint8_t irTimer;
    uint8_t dhtTimer;

    dhtTimer = Timer_Create("DHT", 2000, true);

    irTimer = Timer_Create("IR",100,true);

    ledTimer = Timer_Create("LED",500,true);

    buttonTimer = Timer_Create("BUTTON",20,true);

    monitorTimer = Timer_Create("MONITOR",10000,true);

    idleTimer = Timer_Create("IDLE",1,true);

    rtcTimer = Timer_Create("RTC", 1000, true);

    lcdTimer= Timer_Create("LCD",1000,true);

    usTimer= Timer_Create("us",60 ,true );

    Timer_Start(rtcTimer);
    Timer_Start(ledTimer);
    Timer_Start(buttonTimer);
    Timer_Start(monitorTimer);
    Timer_Start(idleTimer);
    Timer_Start(lcdTimer);
    Timer_Start(usTimer);
    Timer_Start(irTimer);
    Timer_Start(dhtTimer);

    Scheduler_AddTask(DHT_Task,"DHT", 2000,2,dhtTimer);
    Scheduler_AddTask(LED_Task,"LED",500,2,ledTimer);

    Scheduler_AddTask(Button_Task,"BUTTON",20,4,buttonTimer);

    Scheduler_AddTask( Monitor_Task,"MONITOR", 10000,0,monitorTimer);

    Scheduler_AddTask( Idle_Task,"IDLE",1,0,idleTimer);

    Scheduler_AddTask( LCD_Task,"LCD",1000,3,lcdTimer);

    Scheduler_AddTask( RTC_Task,"RTC",1000,5,rtcTimer);

    Scheduler_AddTask(Ultrasonic_Task,"us",60,3,usTimer);

    Scheduler_AddTask( IR_Task,"IR",100,2,irTimer);

    PRINTF("\r\nBare Metal Scheduler Started\r\n");

    gpio_pin_config_t sw3Config = {
        kGPIO_DigitalInput,
        0
    };
    gpio_pin_config_t sw2Config = {
            kGPIO_DigitalInput,
            0
        };

    GPIO_PinInit(BOARD_SW3_GPIO,
                 BOARD_SW3_GPIO_PIN,
                 &sw3Config);
    GPIO_PinInit(BOARD_SW2_GPIO,
                     BOARD_SW2_GPIO_PIN,
                     &sw2Config);
    gpio_pin_config_t ledConfig =
    {
        kGPIO_DigitalOutput, 1U          // LED OFF initially (active LOW)
    };

    GPIO_PinInit(BOARD_LED_RED_GPIO,
                 BOARD_LED_RED_GPIO_PIN,
                 &ledConfig);

    while(1)
    {
    	Scheduler_Run();
    }

}




