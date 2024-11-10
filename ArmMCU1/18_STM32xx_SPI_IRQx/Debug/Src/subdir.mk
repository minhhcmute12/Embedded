################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/007_spi_message_rcv_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tutorial_spi_interrupt.c 

OBJS += \
./Src/007_spi_message_rcv_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tutorial_spi_interrupt.o 

C_DEPS += \
./Src/007_spi_message_rcv_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tutorial_spi_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Minh/Documents/ArmMCU1/18_STM32xx_SPI_IRQx/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/007_spi_message_rcv_it.cyclo ./Src/007_spi_message_rcv_it.d ./Src/007_spi_message_rcv_it.o ./Src/007_spi_message_rcv_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/tutorial_spi_interrupt.cyclo ./Src/tutorial_spi_interrupt.d ./Src/tutorial_spi_interrupt.o ./Src/tutorial_spi_interrupt.su

.PHONY: clean-Src

