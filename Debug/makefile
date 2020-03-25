################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS

GEN_OPTS__FLAG := --cmd_file="C:/Users/iron/workspace_v9/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs/Debug/configPkg/compiler.opt"
GEN_CMDS__FLAG := -l"C:/Users/iron/workspace_v9/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs/Debug/configPkg/linker.cmd"

ORDERED_OBJS += \
"./syscfg/ti_devices_config.obj" \
"./syscfg/ti_radio_config.obj" \
"./syscfg/ti_drivers_config.obj" \
"./usb1352.obj" \
"./Queue/queue.obj" \
"./RF/RFQueue.obj" \
"./RF/usb1352_amp.obj" \
"./RF/usb1352_rf.obj" \
"./SPI/usb1352_intr_thread.obj" \
"./SPI/usb1352_spi.obj" \
"../CC1352P1_LAUNCHXL_TIRTOS.cmd" \
-l"ti/drivers/rf/lib/rf_multiMode_cc13x2.aem4f" \
-l"ti/display/lib/display.aem4f" \
-l"ti/grlib/lib/ccs/m4f/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4f/spiffs_cc26xx.a" \
-l"ti/drivers/lib/drivers_cc13x2.aem4f" \
-l"ti/dpl/lib/dpl_cc13x2.aem4f" \
$(GEN_CMDS__FLAG) \
-l"ti/devices/cc13x2_cc26x2/driverlib/bin/ccs/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Queue/subdir_vars.mk
-include RF/subdir_vars.mk
-include SPI/subdir_vars.mk
-include subdir_rules.mk
-include Queue/subdir_rules.mk
-include RF/subdir_rules.mk
-include SPI/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
USB1352_S.out \

EXE_OUTPUTS__QUOTED += \
"USB1352_S.out" \

BIN_OUTPUTS += \
USB1352_S.hex \

BIN_OUTPUTS__QUOTED += \
"USB1352_S.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "USB1352_S.out"

dependents:
	-cd C:/Users/iron/workspace_v9/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs/Debug && $(MAKE) all

# Tool invocations
USB1352_S.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) C:/Users/iron/workspace_v9/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs/Debug/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --define=DeviceFamily_CC13X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"USB1352_S.map" -i"C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/source" -i"C:/ti/simplelink_cc13x2_26x2_sdk_3_40_00_02/kernel/tirtos/packages" -i"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="USB1352_S_linkInfo.xml" --rom_model -o "USB1352_S.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

USB1352_S.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs930/ccs/tools/compiler/ti-cgt-arm_18.12.4.LTS/bin/armhex"  -o "USB1352_S.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "syscfg\ti_devices_config.obj" "syscfg\ti_radio_config.obj" "syscfg\ti_drivers_config.obj" "usb1352.obj" "Queue\queue.obj" "RF\RFQueue.obj" "RF\usb1352_amp.obj" "RF\usb1352_rf.obj" "SPI\usb1352_intr_thread.obj" "SPI\usb1352_spi.obj" 
	-$(RM) "syscfg\ti_devices_config.d" "syscfg\ti_radio_config.d" "syscfg\ti_drivers_config.d" "usb1352.d" "Queue\queue.d" "RF\RFQueue.d" "RF\usb1352_amp.d" "RF\usb1352_rf.d" "SPI\usb1352_intr_thread.d" "SPI\usb1352_spi.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/Users/iron/workspace_v9/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs/Debug/tirtos_builds_CC1352P1_LAUNCHXL_release_ccs.lib:

-include ../makefile.targets

