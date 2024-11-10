################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/tutorial_volatile.c \
../Src/volatile_interrupt.c 

OBJS += \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/tutorial_volatile.o \
./Src/volatile_interrupt.o 

C_DEPS += \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/tutorial_volatile.d \
./Src/volatile_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/tutorial_volatile.cyclo ./Src/tutorial_volatile.d ./Src/tutorial_volatile.o ./Src/tutorial_volatile.su ./Src/volatile_interrupt.cyclo ./Src/volatile_interrupt.d ./Src/volatile_interrupt.o ./Src/volatile_interrupt.su

.PHONY: clean-Src

