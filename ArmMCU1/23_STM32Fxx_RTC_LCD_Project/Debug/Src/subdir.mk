################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/15_rtc_ds1307_lcd_testing.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tutorial_rtc_project.c 

OBJS += \
./Src/15_rtc_ds1307_lcd_testing.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tutorial_rtc_project.o 

C_DEPS += \
./Src/15_rtc_ds1307_lcd_testing.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tutorial_rtc_project.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Minh/Documents/ArmMCU1/23_STM32Fxx_RTC_LCD_Project/bsp" -I"C:/Users/Minh/Documents/ArmMCU1/23_STM32Fxx_RTC_LCD_Project/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/15_rtc_ds1307_lcd_testing.cyclo ./Src/15_rtc_ds1307_lcd_testing.d ./Src/15_rtc_ds1307_lcd_testing.o ./Src/15_rtc_ds1307_lcd_testing.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/tutorial_rtc_project.cyclo ./Src/tutorial_rtc_project.d ./Src/tutorial_rtc_project.o ./Src/tutorial_rtc_project.su

.PHONY: clean-Src

