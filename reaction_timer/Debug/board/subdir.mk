################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/hardware_init.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/hardware_init.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/hardware_init.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXN236VDF -DCPU_MCXN236VDF_cm33 -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN236_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\source" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\drivers" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\CMSIS" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\CMSIS\m-profile" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\CMSIS_driver\Include" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\CMSIS_driver\Include\GPIO" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\device" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\device\periph1" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\utilities" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\utilities\str" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\utilities\debug_console_lite" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\component\uart" -I"C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\board" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\source\mcux_config.h" -imacros "C:\Users\G SHIVA\Documents\MCUXpressoIDE_25.6.136\workspace\Embedded_Project\reaction timer\source\mcuxsdk_version.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/hardware_init.d ./board/hardware_init.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

