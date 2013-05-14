rem @echo off
set MY_TARGET=project
set MY_SPECS=--specs=nano.specs
set PREFIX=arm-none-eabi-
set INCLUDE=^
 -I..^
 -I../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/CoreSupport^
 -I../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/DeviceSupport/Nuvoton/M051Series^
 -I../../../../M051SeriesBSP/LibM051Series/StdDriver/inc

set CFLAGS=%MY_SPECS% -mcpu=cortex-m0 -mthumb -Wall -ffunction-sections -Os -std=c99 -gdwarf-2 

set LDFLAGS=%MY_SPECS% -Os -nostartfiles -Wl,-Map=%MY_TARGET%.map -mcpu=cortex-m0 -mthumb -L. -Wl,--gc-sections -Wl,-Tlink.ld

%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/CoreSupport/core_cm0.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/DeviceSupport/Nuvoton/M051Series/system_M051Series.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/DeviceSupport/Nuvoton/M051Series/startup/gcc/startup_M051Series.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/DeviceSupport/Nuvoton/M051Series/startup/gcc/printf.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../M051SeriesBSP/LibM051Series/CMSIS/CM0/DeviceSupport/Nuvoton/M051Series/startup/gcc/syscalls.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../M051SeriesBSP/LibM051Series/StdDriver/src/retarget.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../Src/tt.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../Main.c

%PREFIX%gcc %LDFLAGS% -o %MY_TARGET%.elf^
 core_cm0.o^
 system_M051Series.o^
 startup_M051Series.o^
 retarget.o^
 syscalls.o^
 printf.o^
 tt.o^
 Main.o^
 
 %PREFIX%objcopy -O binary -S %MY_TARGET%.elf %MY_TARGET%.bin
 dir %MY_TARGET%.bin
