################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F5529PocketBoard/F5xx_F6xx_Core_Lib/%.obj: ../F5529PocketBoard/F5xx_F6xx_Core_Lib/%.cpp $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/home/yukacr/Workspace/ti/ccs901/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/home/yukacr/Workspace/ti/ccs901/ccs/ccs_base/msp430/include" --include_path="/home/yukacr/Workspace/ti/workspace_v9/msp430_framework" --include_path="/home/yukacr/Workspace/ti/ccs901/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="F5529PocketBoard/F5xx_F6xx_Core_Lib/$(basename $(<F)).d_raw" --obj_directory="F5529PocketBoard/F5xx_F6xx_Core_Lib" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

