################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver_M/Src/stm32f407xx_gpio_driver.c 

OBJS += \
./Driver_M/Src/stm32f407xx_gpio_driver.o 

C_DEPS += \
./Driver_M/Src/stm32f407xx_gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Driver_M/Src/%.o Driver_M/Src/%.su Driver_M/Src/%.cyclo: ../Driver_M/Src/%.c Driver_M/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I"C:/Users/Minh/Documents/ArmMCU1/24_M_STM32F4xx_GPIO_Diver/Driver_M/Inc" -I"C:/Users/Minh/Documents/ArmMCU1/24_M_STM32F4xx_GPIO_Diver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Driver_M-2f-Src

clean-Driver_M-2f-Src:
	-$(RM) ./Driver_M/Src/stm32f407xx_gpio_driver.cyclo ./Driver_M/Src/stm32f407xx_gpio_driver.d ./Driver_M/Src/stm32f407xx_gpio_driver.o ./Driver_M/Src/stm32f407xx_gpio_driver.su

.PHONY: clean-Driver_M-2f-Src

