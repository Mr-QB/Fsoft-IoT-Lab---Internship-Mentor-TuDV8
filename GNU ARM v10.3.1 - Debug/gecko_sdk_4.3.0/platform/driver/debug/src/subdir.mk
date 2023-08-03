################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk/platform/driver/debug/src/sl_debug_swo.c 

OBJS += \
./gecko_sdk_4.3.0/platform/driver/debug/src/sl_debug_swo.o 

C_DEPS += \
./gecko_sdk_4.3.0/platform/driver/debug/src/sl_debug_swo.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.3.0/platform/driver/debug/src/sl_debug_swo.o: C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk/platform/driver/debug/src/sl_debug_swo.c gecko_sdk_4.3.0/platform/driver/debug/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DDEBUG=1' '-DDEBUG_EFM=1' '-DBGM220PC22HNA=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DSL_BOARD_NAME="BRD4314A"' '-DSL_BOARD_REV="A02"' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DRTT_USE_ASM=0' '-DSEGGER_RTT_SECTION="SEGGER_RTT"' -I"C:\Users\ADMIN\SimplicityStudio\v5_workspace\AHT20V4\config" -I"C:\Users\ADMIN\SimplicityStudio\v5_workspace\AHT20V4\autogen" -I"C:\Users\ADMIN\SimplicityStudio\v5_workspace\AHT20V4" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/BGM22/Include" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//app/common/util/app_log" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/driver/debug/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/init/gpio_simple" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/driver/i2cspm/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/service/iostream/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//util/third_party/printf" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//util/third_party/printf/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//util/third_party/segger/systemview/SEGGER" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/service/sleeptimer/inc" -I"C:/Users/ADMIN/SimplicityStudio/SDKs/gecko_sdk//platform/service/udelay/inc" -Os -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse -fno-builtin-printf -fno-builtin-sprintf --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.3.0/platform/driver/debug/src/sl_debug_swo.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


