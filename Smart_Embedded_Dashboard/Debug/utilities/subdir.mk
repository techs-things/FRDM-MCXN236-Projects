################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_assert.c 

C_DEPS += \
./utilities/fsl_assert.d 

OBJS += \
./utilities/fsl_assert.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MCXN236VDF -DCPU_MCXN236VDF_cm33 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities\str" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\drivers" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\component\serial_manager" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\device" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\device\periph1" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities\debug_console" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\CMSIS" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\CMSIS\m-profile" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\component\lists" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\utilities\debug_console\config" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\component\uart" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\board" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Smart_Embedded_Dashboard\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_assert.d ./utilities/fsl_assert.o

.PHONY: clean-utilities

