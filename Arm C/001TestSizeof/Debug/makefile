################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
001TestSizeof.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
001TestSizeof.list \

OBJCOPY_BIN += \
001TestSizeof.bin \


# All Target
all: 001TestSizeof.elf secondary-outputs

# Tool invocations
001TestSizeof.elf: $(OBJS) $(USER_OBJS) C:\Users\Minh\Documents\Arm\ C\001TestSizeof\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "001TestSizeof.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Minh\Documents\Arm C\001TestSizeof\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="001TestSizeof.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

001TestSizeof.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "001TestSizeof.list"
	@echo 'Finished building: $@'
	@echo ' '

001TestSizeof.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "001TestSizeof.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
