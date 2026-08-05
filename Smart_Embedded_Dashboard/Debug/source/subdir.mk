################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/dashboard.c \
../source/dht.c \
../source/ir.c \
../source/lcd.c \
../source/lcd_dashboard.c \
../source/main.c \
../source/performance.c \
../source/rtc.c \
../source/scheduler.c \
../source/semihost_hardfault.c \
../source/sensor_data.c \
../source/servo.c \
../source/tasks.c \
../source/timer.c \
../source/ultrasonic.c 

C_DEPS += \
./source/dashboard.d \
./source/dht.d \
./source/ir.d \
./source/lcd.d \
./source/lcd_dashboard.d \
./source/main.d \
./source/performance.d \
./source/rtc.d \
./source/scheduler.d \
./source/semihost_hardfault.d \
./source/sensor_data.d \
./source/servo.d \
./source/tasks.d \
./source/timer.d \
./source/ultrasonic.d 

OBJS += \
./source/dashboard.o \
./source/dht.o \
./source/ir.o \
./source/lcd.o \
./source/lcd_dashboard.o \
./source/main.o \
./source/performance.o \
./source/rtc.o \
./source/scheduler.o \
./source/semihost_hardfault.o \
./source/sensor_data.o \
./source/servo.o \
./source/tasks.o \
./source/timer.o \
./source/ultrasonic.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN236VDF -DCPU_MCXN236VDF_cm33 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities\str" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\drivers" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\component\serial_manager" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\device" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\device\periph1" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities\debug_console" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\CMSIS" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\CMSIS\m-profile" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\component\lists" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities\debug_console\config" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\component\uart" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\board" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/dashboard.d ./source/dashboard.o ./source/dht.d ./source/dht.o ./source/ir.d ./source/ir.o ./source/lcd.d ./source/lcd.o ./source/lcd_dashboard.d ./source/lcd_dashboard.o ./source/main.d ./source/main.o ./source/performance.d ./source/performance.o ./source/rtc.d ./source/rtc.o ./source/scheduler.d ./source/scheduler.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sensor_data.d ./source/sensor_data.o ./source/servo.d ./source/servo.o ./source/tasks.d ./source/tasks.o ./source/timer.d ./source/timer.o ./source/ultrasonic.d ./source/ultrasonic.o

.PHONY: clean-source

