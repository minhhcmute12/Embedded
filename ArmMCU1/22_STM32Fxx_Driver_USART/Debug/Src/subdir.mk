################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/12_usart_txtesting.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/usart_tutorial.c 

OBJS += \
./Src/12_usart_txtesting.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/usart_tutorial.o 

C_DEPS += \
./Src/12_usart_txtesting.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/usart_tutorial.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Minh/Documents/ArmMCU1/22_STM32Fxx_Driver_USART/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/12_usart_txtesting.cyclo ./Src/12_usart_txtesting.d ./Src/12_usart_txtesting.o ./Src/12_usart_txtesting.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/usart_tutorial.cyclo ./Src/usart_tutorial.d ./Src/usart_tutorial.o ./Src/usart_tutorial.su

.PHONY: clean-Src

