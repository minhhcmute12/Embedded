################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tutorial1_mcu_driver.c \
../Src/tutorial2_mcu_driver.c \
../Src/tutorial_mcu_driver.c 

OBJS += \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tutorial1_mcu_driver.o \
./Src/tutorial2_mcu_driver.o \
./Src/tutorial_mcu_driver.o 

C_DEPS += \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tutorial1_mcu_driver.d \
./Src/tutorial2_mcu_driver.d \
./Src/tutorial_mcu_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Minh/Documents/ArmMCU1/09_STM32Fxxx_Drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/tutorial1_mcu_driver.cyclo ./Src/tutorial1_mcu_driver.d ./Src/tutorial1_mcu_driver.o ./Src/tutorial1_mcu_driver.su ./Src/tutorial2_mcu_driver.cyclo ./Src/tutorial2_mcu_driver.d ./Src/tutorial2_mcu_driver.o ./Src/tutorial2_mcu_driver.su ./Src/tutorial_mcu_driver.cyclo ./Src/tutorial_mcu_driver.d ./Src/tutorial_mcu_driver.o ./Src/tutorial_mcu_driver.su

.PHONY: clean-Src

