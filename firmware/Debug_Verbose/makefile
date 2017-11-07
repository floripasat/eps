################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS

ORDERED_OBJS += \
"./ADC.obj" \
"./ADS1248.obj" \
"./I2C.obj" \
"./SPI.obj" \
"./clock.obj" \
"./crc.obj" \
"./energy_level_algorithm.obj" \
"./main.obj" \
"./misc.obj" \
"./mppt.obj" \
"./onewire.obj" \
"./pid.obj" \
"./timer.obj" \
"./uart.obj" \
"./watchdog.obj" \
"../lnk_msp430f6659.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \
-l"libmath.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
fsat_eps.out \

EXE_OUTPUTS__QUOTED += \
"fsat_eps.out" \

BIN_OUTPUTS += \
fsat_eps.hex \

BIN_OUTPUTS__QUOTED += \
"fsat_eps.hex" \


# All Target
all: fsat_eps.out secondary-outputs

# Tool invocations
fsat_eps.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --advice:power="all" --define=__MSP430F6659__ --define=_VERBOSE_DEBUG --define=_DEBUG=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"fsat_eps.map" --heap_size=400 --stack_size=400 --cinit_hold_wdt=on -i"C:/ti/ccsv6/ccs_base/msp430/include" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" -i"C:/ti/ccsv6/ccs_base/msp430/lib/5xx_6xx_FRxx" --reread_libs --define=_DEBUG=1 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="fsat_eps_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "fsat_eps.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

fsat_eps.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "fsat_eps.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "ADC.pp" "ADS1248.pp" "I2C.pp" "SPI.pp" "clock.pp" "crc.pp" "energy_level_algorithm.pp" "main.pp" "misc.pp" "mppt.pp" "onewire.pp" "pid.pp" "timer.pp" "uart.pp" "watchdog.pp" 
	-$(RM) "ADC.obj" "ADS1248.obj" "I2C.obj" "SPI.obj" "clock.obj" "crc.obj" "energy_level_algorithm.obj" "main.obj" "misc.obj" "mppt.obj" "onewire.obj" "pid.obj" "timer.obj" "uart.obj" "watchdog.obj" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

