################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include gecko_sdk_4.3.0/util/third_party/segger/systemview/SEGGER/subdir.mk
-include gecko_sdk_4.3.0/util/third_party/printf/src/subdir.mk
-include gecko_sdk_4.3.0/util/third_party/printf/subdir.mk
-include gecko_sdk_4.3.0/platform/service/udelay/src/subdir.mk
-include gecko_sdk_4.3.0/platform/service/system/src/subdir.mk
-include gecko_sdk_4.3.0/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_4.3.0/platform/service/iostream/src/subdir.mk
-include gecko_sdk_4.3.0/platform/service/device_init/src/subdir.mk
-include gecko_sdk_4.3.0/platform/emlib/src/subdir.mk
-include gecko_sdk_4.3.0/platform/driver/i2cspm/src/subdir.mk
-include gecko_sdk_4.3.0/platform/driver/debug/src/subdir.mk
-include gecko_sdk_4.3.0/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_4.3.0/platform/common/src/subdir.mk
-include gecko_sdk_4.3.0/platform/Device/SiliconLabs/BGM22/Source/subdir.mk
-include gecko_sdk_4.3.0/hardware/board/src/subdir.mk
-include gecko_sdk_4.3.0/app/common/util/app_log/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := AHT20V4
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: AHT20V4.axf

# Tool invocations
AHT20V4.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\Users\ADMIN\SimplicityStudio\v5_workspace\AHT20V4\autogen\linkerfile.ld" -Xlinker --gc-sections -Xlinker -Map="AHT20V4.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o AHT20V4.axf -Wl,--start-group "./AHT20.o" "./app.o" "./blink.o" "./main.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_emlib_gpio_simple_init.o" "./autogen/sl_event_handler.o" "./autogen/sl_i2cspm_init.o" "./autogen/sl_iostream_handles.o" "./gecko_sdk_4.3.0/app/common/util/app_log/app_log.o" "./gecko_sdk_4.3.0/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_4.3.0/hardware/board/src/sl_board_init.o" "./gecko_sdk_4.3.0/platform/Device/SiliconLabs/BGM22/Source/startup_bgm22.o" "./gecko_sdk_4.3.0/platform/Device/SiliconLabs/BGM22/Source/system_bgm22.o" "./gecko_sdk_4.3.0/platform/common/src/sl_assert.o" "./gecko_sdk_4.3.0/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_4.3.0/platform/driver/debug/src/sl_debug_swo.o" "./gecko_sdk_4.3.0/platform/driver/i2cspm/src/sl_i2cspm.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_burtc.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_cmu.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_core.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_emu.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_gpio.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_i2c.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_msc.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_rtcc.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_system.o" "./gecko_sdk_4.3.0/platform/emlib/src/em_timer.o" "./gecko_sdk_4.3.0/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./gecko_sdk_4.3.0/platform/service/device_init/src/sl_device_init_emu_s2.o" "./gecko_sdk_4.3.0/platform/service/device_init/src/sl_device_init_hfxo_s2.o" "./gecko_sdk_4.3.0/platform/service/device_init/src/sl_device_init_lfxo_s2.o" "./gecko_sdk_4.3.0/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_4.3.0/platform/service/iostream/src/sl_iostream.o" "./gecko_sdk_4.3.0/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./gecko_sdk_4.3.0/platform/service/iostream/src/sl_iostream_rtt.o" "./gecko_sdk_4.3.0/platform/service/iostream/src/sl_iostream_stdlib_config.o" "./gecko_sdk_4.3.0/platform/service/iostream/src/sl_iostream_swo.o" "./gecko_sdk_4.3.0/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_4.3.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./gecko_sdk_4.3.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_prortc.o" "./gecko_sdk_4.3.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_4.3.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./gecko_sdk_4.3.0/platform/service/system/src/sl_system_init.o" "./gecko_sdk_4.3.0/platform/service/system/src/sl_system_process_action.o" "./gecko_sdk_4.3.0/platform/service/udelay/src/sl_udelay.o" "./gecko_sdk_4.3.0/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" "./gecko_sdk_4.3.0/util/third_party/printf/printf.o" "./gecko_sdk_4.3.0/util/third_party/printf/src/iostream_printf.o" "./gecko_sdk_4.3.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.o" "./gecko_sdk_4.3.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: AHT20V4.out'
	arm-none-eabi-objcopy "AHT20V4.axf" "AHT20V4.out"
	@echo ' '

	@echo 'Building hex file: AHT20V4.hex'
	arm-none-eabi-objcopy -O ihex "AHT20V4.axf" "AHT20V4.hex"
	@echo ' '

	@echo 'Building bin file: AHT20V4.bin'
	arm-none-eabi-objcopy -O binary "AHT20V4.axf" "AHT20V4.bin"
	@echo ' '

	@echo 'Building s37 file: AHT20V4.s37'
	arm-none-eabi-objcopy -O srec "AHT20V4.axf" "AHT20V4.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "AHT20V4.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) AHT20V4.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
