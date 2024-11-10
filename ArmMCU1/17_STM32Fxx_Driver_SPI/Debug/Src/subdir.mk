################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/006_spi_cmd_handling.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tutorial_spi_driver.c 

OBJS += \
./Src/006_spi_cmd_handling.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tutorial_spi_driver.o 

C_DEPS += \
./Src/006_spi_cmd_handling.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tutorial_spi_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Minh/Documents/ArmMCU1/17_STM32Fxx_Driver_SPI/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/006_spi_cmd_handling.cyclo ./Src/006_spi_cmd_handling.d ./Src/006_spi_cmd_handling.o ./Src/006_spi_cmd_handling.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/tutorial_spi_driver.cyclo ./Src/tutorial_spi_driver.d ./Src/tutorial_spi_driver.o ./Src/tutorial_spi_driver.su

.PHONY: clean-Src

