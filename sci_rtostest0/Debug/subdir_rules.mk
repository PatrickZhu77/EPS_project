################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 -me --include_path="C:/Users/sdamkjar/Documents/EPS/EPS_project.git/trunk/sci_rtostest0" --include_path="C:/Users/sdamkjar/Documents/EPS/EPS_project.git/trunk/sci_rtostest0/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


