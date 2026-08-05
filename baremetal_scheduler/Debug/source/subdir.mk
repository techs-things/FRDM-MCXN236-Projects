################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c \
../source/scheduler.c \
../source/semihost_hardfault.c \
../source/tasks.c 

C_DEPS += \
./source/main.d \
./source/scheduler.d \
./source/semihost_hardfault.d \
./source/tasks.d 

OBJS += \
./source/main.o \
./source/scheduler.o \
./source/semihost_hardfault.o \
./source/tasks.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXN236VDF -DCPU_MCXN236VDF_cm33 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\board" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\source" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\utilities\str" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\drivers" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\component\serial_manager" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\device" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\device\periph1" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\utilities\debug_console" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\CMSIS" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\CMSIS\m-profile" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\component\lists" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\utilities" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\utilities\debug_console\config" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\component\uart" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\freertos\freertos-kernel\include" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\freertos\freertos-kernel\template" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\baremetal_scheduler\freertos\freertos-kernel\template\ARM_CM33_3_priority_bits" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/main.d ./source/main.o ./source/scheduler.d ./source/scheduler.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/tasks.d ./source/tasks.o

.PHONY: clean-source

