################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
COECSL_edma3.obj: C:/Team_5/mylabs/Final_team5_V1/src/COECSL_edma3.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="COECSL_edma3.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

COECSL_mcbsp.obj: C:/Team_5/mylabs/Final_team5_V1/src/COECSL_mcbsp.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="COECSL_mcbsp.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ColorLCD.obj: C:/Team_5/mylabs/Final_team5_V1/src/ColorLCD.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ColorLCD.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ColorVision.obj: C:/Team_5/mylabs/Final_team5_V1/src/ColorVision.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ColorVision.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-2090520385:
	@$(MAKE) -Onone -f subdir_rules.mk build-2090520385-inproc

build-2090520385-inproc: C:/Team_5/mylabs/Final_team5_V1/DSPBIOS/Final_team5_V1.tcf
	@echo 'Building file: $<'
	@echo 'Invoking: TConf'
	"C:/CCStudio_v7/bios_5_42_02_10/xdctools/tconf" -b -Dconfig.importPath="C:/CCStudio_v7/bios_5_42_02_10/packages;C:/Team_5/mylabs/Final_team5_V1/DSPBIOS;" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Final_team5_V1cfg.cmd: build-2090520385 C:/Team_5/mylabs/Final_team5_V1/DSPBIOS/Final_team5_V1.tcf
Final_team5_V1cfg.s??: build-2090520385
Final_team5_V1cfg_c.c: build-2090520385
Final_team5_V1cfg.h: build-2090520385
Final_team5_V1cfg.h??: build-2090520385
Final_team5_V1.cdb: build-2090520385

Final_team5_V1cfg.obj: ./Final_team5_V1cfg.s?? $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Final_team5_V1cfg.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Final_team5_V1cfg_c.obj: ./Final_team5_V1cfg_c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Final_team5_V1cfg_c.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

LCDprintf.obj: C:/Team_5/mylabs/Final_team5_V1/src/LCDprintf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="LCDprintf.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Ladar.obj: C:/Team_5/mylabs/Final_team5_V1/src/Ladar.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="Ladar.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

MatrixMath.obj: C:/Team_5/mylabs/Final_team5_V1/src/MatrixMath.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="MatrixMath.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pru.obj: C:/Team_5/mylabs/Final_team5_V1/src/pru.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="pru.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

user_Final_team5_V1.obj: C:/Team_5/mylabs/Final_team5_V1/src/user_Final_team5_V1.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="user_Final_team5_V1.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

user_xy.obj: C:/Team_5/mylabs/Final_team5_V1/src/user_xy.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/bin/cl6x" -mv6740 --abi=coffabi -O3 -g --include_path="C:/CCStudio_v7/ccsv7/tools/compiler/c6000_7.4.22/include" --include_path="C:/Team_5/_shared/bsl/inc" --include_path="C:/Team_5/mylabs/Final_team5_V1" --include_path="C:/Team_5/_shared/c67xmathlib_2_01_00_00/inc" --include_path="C:/Team_5/_shared/mcbsp_com" --include_path="C:/Team_5/_shared/sharedmem_com" --include_path="C:/Team_5/mylabs/Final_team5_V1/include" --include_path="C:/Team_5/mylabs/Final_team5_V1/Final_team5_V1Project/Debug" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/bios/include" --include_path="C:/CCStudio_v7/bios_5_42_02_10/packages/ti/rtdx/include/c6000" --define=RUNNING_ON_OMAPL138 --define=_DEBUG --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="user_xy.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


