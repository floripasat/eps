################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ADC.obj: ../ADC.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="ADC.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

ADS1248.obj: ../ADS1248.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="ADS1248.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

I2C.obj: ../I2C.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="I2C.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

SPI.obj: ../SPI.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="SPI.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

clock.obj: ../clock.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="clock.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

crc16.obj: /home/bruno/Git/fsp/fsp/crc16.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="crc16.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

crc8.obj: ../crc8.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="crc8.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

energy_level_algorithm.obj: ../energy_level_algorithm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="energy_level_algorithm.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

fsp.obj: /home/bruno/Git/fsp/fsp/fsp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="fsp.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

misc.obj: ../misc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="misc.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

mppt.obj: ../mppt.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mppt.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

onewire.obj: ../onewire.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="onewire.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

pid.obj: ../pid.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="pid.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

timer.obj: ../timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="timer.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="uart.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

watchdog.obj: ../watchdog.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=none -Ooff --use_hw_mpy=F5 --include_path="/home/bruno/ti/ccsv7/ccs_base/msp430/include" --include_path="/home/fsat-integration/git/eps/firmware" --include_path="/home/bruno/Git/eps/firmware" --include_path="/home/bruno/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power="all" --define=__MSP430F6659__ --define=_DEBUG_ADC=1 -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="watchdog.d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


