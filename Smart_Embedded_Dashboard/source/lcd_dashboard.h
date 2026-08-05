#ifndef LCD_DASHBOARD_H
#define LCD_DASHBOARD_H

#include <stdint.h>

typedef enum
{
    DASHBOARD_PAGE_SYSTEM = 0,
    DASHBOARD_PAGE_SCHEDULER,
    DASHBOARD_PAGE_SENSOR,
    DASHBOARD_PAGE_STATUS,
    DASHBOARD_PAGE_COUNT,
	DASHBOARD_PAGE_DHT


}DashboardPage_t;

void LCD_Dashboard_Init(void);

void LCD_Dashboard_Update(void);

void LCD_Dashboard_NextPage(void);

void LCD_Dashboard_PreviousPage(void);

#endif
