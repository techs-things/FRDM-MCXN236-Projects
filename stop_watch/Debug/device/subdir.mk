################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MCXN236.c 

C_DEPS += \
./device/system_MCXN236.d 

OBJS += \
./device/system_MCXN236.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXN236VDF -DCPU_MCXN236VDF_cm33 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN236_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\source" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\drivers" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\CMSIS" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\CMSIS\m-profile" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\CMSIS_driver\Include" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\CMSIS_driver\Include\GPIO" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\device" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\device\periph1" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\utilities" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\utilities\str" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\utilities\debug_console_lite" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\component\uart" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\board" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\source\mcux_config.h" -imacros "C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\frdmmcxn236_cmsis_button_toggle_led\source\mcuxsdk_version.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_MCXN236.d ./device/system_MCXN236.o

.PHONY: clean-device

