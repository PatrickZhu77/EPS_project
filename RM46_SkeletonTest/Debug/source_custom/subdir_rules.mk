################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source_custom/%.obj: ../source_custom/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"F:/Program Files/ti/ccs8/ccsv8/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7R4 --code_state=32 --float_support=VFPv3D16 -me --include_path="E:/TortoiseSVN/EPS2/EPS System Test/software/RM46_SkeletonTest/include_custom" --include_path="C:/ti/Hercules/F021 Flash API/02.01.01/include" --include_path="E:/TortoiseSVN/EPS2/EPS System Test/software/RM46_SkeletonTest/include" --include_path="F:/Program Files/ti/ccs8/ccsv8/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --enum_type=packed --abi=eabi --preproc_with_compile --preproc_dependency="source_custom/$(basename $(<F)).d_raw" --obj_directory="source_custom" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


