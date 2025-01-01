################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/08_i2c_master_tx_testing.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/08_i2c_master_tx_testing.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/08_i2c_master_tx_testing.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Users/Minh/Documents/ArmMCU1/28_M_STM32F4xx_I2C/drivers/Inc" -I"C:/Users/Minh/Documents/ArmMCU1/28_M_STM32F4xx_I2C/Driver_M/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/08_i2c_master_tx_testing.cyclo ./Src/08_i2c_master_tx_testing.d ./Src/08_i2c_master_tx_testing.o ./Src/08_i2c_master_tx_testing.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

