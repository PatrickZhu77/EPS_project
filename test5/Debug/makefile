################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ad5324.o" \
"./channel_control.o" \
"./current_protection.o" \
"./i2c_common.o" \
"./ina226.o" \
"./ina226_2.o" \
"./ina3221.o" \
"./ina3221_2.o" \
"./max6698.o" \
"./mppt.o" \
"./Sensor Drivers/ina226.o" \
"./Sensor Drivers/ina3221.o" \
"./Sensor Drivers/max6698.o" \
"./source/adc.o" \
"./source/can.o" \
"./source/crc.o" \
"./source/dabort.o" \
"./source/dcc.o" \
"./source/ecap.o" \
"./source/emif.o" \
"./source/eqep.o" \
"./source/errata_SSWF021_45.o" \
"./source/esm.o" \
"./source/etpwm.o" \
"./source/gio.o" \
"./source/het.o" \
"./source/i2c.o" \
"./source/lin.o" \
"./source/mibspi.o" \
"./source/notification.o" \
"./source/pinmux.o" \
"./source/pom.o" \
"./source/rti.o" \
"./source/sci.o" \
"./source/spi.o" \
"./source/sys_core.o" \
"./source/sys_dma.o" \
"./source/sys_intvecs.o" \
"./source/sys_main.o" \
"./source/sys_mpu.o" \
"./source/sys_pcr.o" \
"./source/sys_phantom.o" \
"./source/sys_pmm.o" \
"./source/sys_pmu.o" \
"./source/sys_selftest.o" \
"./source/sys_startup.o" \
"./source/sys_vim.o" \
"./source/system.o" \
-Wl,-T"../source/sys_link.cmd" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Sensor\ Drivers/subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include Sensor\ Drivers/subdir_rules.mk
-include source/subdir_rules.mk
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
test5.out \

EXE_OUTPUTS__QUOTED += \
"test5.out" \

BIN_OUTPUTS += \
test5.hex \

BIN_OUTPUTS__QUOTED += \
"test5.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "test5.out"

# Tool invocations
test5.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"" -Wl,-Map,"test5.map" -o"test5.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

test5.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	""  $(EXE_OUTPUTS__QUOTED) "test5.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ad5324.o" "channel_control.o" "current_protection.o" "i2c_common.o" "ina226.o" "ina226_2.o" "ina3221.o" "ina3221_2.o" "max6698.o" "mppt.o" "Sensor Drivers\ina226.o" "Sensor Drivers\ina3221.o" "Sensor Drivers\max6698.o" "source\adc.o" "source\can.o" "source\crc.o" "source\dabort.o" "source\dcc.o" "source\ecap.o" "source\emif.o" "source\eqep.o" "source\errata_SSWF021_45.o" "source\esm.o" "source\etpwm.o" "source\gio.o" "source\het.o" "source\i2c.o" "source\lin.o" "source\mibspi.o" "source\notification.o" "source\pinmux.o" "source\pom.o" "source\rti.o" "source\sci.o" "source\spi.o" "source\sys_core.o" "source\sys_dma.o" "source\sys_intvecs.o" "source\sys_main.o" "source\sys_mpu.o" "source\sys_pcr.o" "source\sys_phantom.o" "source\sys_pmm.o" "source\sys_pmu.o" "source\sys_selftest.o" "source\sys_startup.o" "source\sys_vim.o" "source\system.o" 
	-$(RM) "ad5324.d" "channel_control.d" "current_protection.d" "i2c_common.d" "ina226.d" "ina226_2.d" "ina3221.d" "ina3221_2.d" "max6698.d" "mppt.d" "Sensor Drivers\ina226.d" "Sensor Drivers\ina3221.d" "Sensor Drivers\max6698.d" "source\adc.d" "source\can.d" "source\crc.d" "source\dcc.d" "source\ecap.d" "source\emif.d" "source\eqep.d" "source\errata_SSWF021_45.d" "source\esm.d" "source\etpwm.d" "source\gio.d" "source\het.d" "source\i2c.d" "source\lin.d" "source\mibspi.d" "source\notification.d" "source\pinmux.d" "source\pom.d" "source\rti.d" "source\sci.d" "source\spi.d" "source\sys_dma.d" "source\sys_main.d" "source\sys_pcr.d" "source\sys_phantom.d" "source\sys_pmm.d" "source\sys_selftest.d" "source\sys_startup.d" "source\sys_vim.d" "source\system.d" 
	-$(RM) "source\dabort.d" "source\sys_core.d" "source\sys_intvecs.d" "source\sys_mpu.d" "source\sys_pmu.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

