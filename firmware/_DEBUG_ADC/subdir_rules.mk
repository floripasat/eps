################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ADC.obj: ../ADC.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="ADC.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ADS1248.obj: ../ADS1248.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="ADS1248.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

I2C.obj: ../I2C.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="I2C.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

SPI.obj: ../SPI.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="SPI.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

clock.obj: ../clock.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="clock.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

crc.obj: ../crc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="crc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

energy_level_algorithm.obj: ../energy_level_algorithm.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="energy_level_algorithm.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

misc.obj: ../misc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="misc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

mppt.obj: ../mppt.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mppt.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

onewire.obj: ../onewire.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="onewire.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pid.obj: ../pid.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="pid.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer.obj: ../timer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="timer.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="uart.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

watchdog.obj: ../watchdog.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="C:/Code/eps/firmware" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="watchdog.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


