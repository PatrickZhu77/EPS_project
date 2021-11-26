################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../source/sys_link.cmd 

ASM_SRCS += \
../source/dabort.asm \
../source/sys_core.asm \
../source/sys_intvecs.asm \
../source/sys_mpu.asm \
../source/sys_pmu.asm 

C_SRCS += \
../source/adc.c \
../source/can.c \
../source/crc.c \
../source/dcc.c \
../source/ecap.c \
../source/emif.c \
../source/eqep.c \
../source/errata_SSWF021_45.c \
../source/esm.c \
../source/etpwm.c \
../source/gio.c \
../source/het.c \
../source/i2c.c \
../source/lin.c \
../source/mibspi.c \
../source/notification.c \
../source/pinmux.c \
../source/pom.c \
../source/rti.c \
../source/sci.c \
../source/spi.c \
../source/sys_dma.c \
../source/sys_main.c \
../source/sys_pcr.c \
../source/sys_phantom.c \
../source/sys_pmm.c \
../source/sys_selftest.c \
../source/sys_startup.c \
../source/sys_vim.c \
../source/system.c 

C_DEPS += \
./source/adc.d \
./source/can.d \
./source/crc.d \
./source/dcc.d \
./source/ecap.d \
./source/emif.d \
./source/eqep.d \
./source/errata_SSWF021_45.d \
./source/esm.d \
./source/etpwm.d \
./source/gio.d \
./source/het.d \
./source/i2c.d \
./source/lin.d \
./source/mibspi.d \
./source/notification.d \
./source/pinmux.d \
./source/pom.d \
./source/rti.d \
./source/sci.d \
./source/spi.d \
./source/sys_dma.d \
./source/sys_main.d \
./source/sys_pcr.d \
./source/sys_phantom.d \
./source/sys_pmm.d \
./source/sys_selftest.d \
./source/sys_startup.d \
./source/sys_vim.d \
./source/system.d 

OBJS += \
./source/adc.o \
./source/can.o \
./source/crc.o \
./source/dabort.o \
./source/dcc.o \
./source/ecap.o \
./source/emif.o \
./source/eqep.o \
./source/errata_SSWF021_45.o \
./source/esm.o \
./source/etpwm.o \
./source/gio.o \
./source/het.o \
./source/i2c.o \
./source/lin.o \
./source/mibspi.o \
./source/notification.o \
./source/pinmux.o \
./source/pom.o \
./source/rti.o \
./source/sci.o \
./source/spi.o \
./source/sys_core.o \
./source/sys_dma.o \
./source/sys_intvecs.o \
./source/sys_main.o \
./source/sys_mpu.o \
./source/sys_pcr.o \
./source/sys_phantom.o \
./source/sys_pmm.o \
./source/sys_pmu.o \
./source/sys_selftest.o \
./source/sys_startup.o \
./source/sys_vim.o \
./source/system.o 

ASM_DEPS += \
./source/dabort.d \
./source/sys_core.d \
./source/sys_intvecs.d \
./source/sys_mpu.d \
./source/sys_pmu.d 

OBJS__QUOTED += \
"source\adc.o" \
"source\can.o" \
"source\crc.o" \
"source\dabort.o" \
"source\dcc.o" \
"source\ecap.o" \
"source\emif.o" \
"source\eqep.o" \
"source\errata_SSWF021_45.o" \
"source\esm.o" \
"source\etpwm.o" \
"source\gio.o" \
"source\het.o" \
"source\i2c.o" \
"source\lin.o" \
"source\mibspi.o" \
"source\notification.o" \
"source\pinmux.o" \
"source\pom.o" \
"source\rti.o" \
"source\sci.o" \
"source\spi.o" \
"source\sys_core.o" \
"source\sys_dma.o" \
"source\sys_intvecs.o" \
"source\sys_main.o" \
"source\sys_mpu.o" \
"source\sys_pcr.o" \
"source\sys_phantom.o" \
"source\sys_pmm.o" \
"source\sys_pmu.o" \
"source\sys_selftest.o" \
"source\sys_startup.o" \
"source\sys_vim.o" \
"source\system.o" 

C_DEPS__QUOTED += \
"source\adc.d" \
"source\can.d" \
"source\crc.d" \
"source\dcc.d" \
"source\ecap.d" \
"source\emif.d" \
"source\eqep.d" \
"source\errata_SSWF021_45.d" \
"source\esm.d" \
"source\etpwm.d" \
"source\gio.d" \
"source\het.d" \
"source\i2c.d" \
"source\lin.d" \
"source\mibspi.d" \
"source\notification.d" \
"source\pinmux.d" \
"source\pom.d" \
"source\rti.d" \
"source\sci.d" \
"source\spi.d" \
"source\sys_dma.d" \
"source\sys_main.d" \
"source\sys_pcr.d" \
"source\sys_phantom.d" \
"source\sys_pmm.d" \
"source\sys_selftest.d" \
"source\sys_startup.d" \
"source\sys_vim.d" \
"source\system.d" 

ASM_DEPS__QUOTED += \
"source\dabort.d" \
"source\sys_core.d" \
"source\sys_intvecs.d" \
"source\sys_mpu.d" \
"source\sys_pmu.d" 

C_SRCS__QUOTED += \
"../source/adc.c" \
"../source/can.c" \
"../source/crc.c" \
"../source/dcc.c" \
"../source/ecap.c" \
"../source/emif.c" \
"../source/eqep.c" \
"../source/errata_SSWF021_45.c" \
"../source/esm.c" \
"../source/etpwm.c" \
"../source/gio.c" \
"../source/het.c" \
"../source/i2c.c" \
"../source/lin.c" \
"../source/mibspi.c" \
"../source/notification.c" \
"../source/pinmux.c" \
"../source/pom.c" \
"../source/rti.c" \
"../source/sci.c" \
"../source/spi.c" \
"../source/sys_dma.c" \
"../source/sys_main.c" \
"../source/sys_pcr.c" \
"../source/sys_phantom.c" \
"../source/sys_pmm.c" \
"../source/sys_selftest.c" \
"../source/sys_startup.c" \
"../source/sys_vim.c" \
"../source/system.c" 

ASM_SRCS__QUOTED += \
"../source/dabort.asm" \
"../source/sys_core.asm" \
"../source/sys_intvecs.asm" \
"../source/sys_mpu.asm" \
"../source/sys_pmu.asm" 


