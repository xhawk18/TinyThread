rem @echo off
set MY_TARGET=project
set MY_SPECS=--specs=nano.specs
set PREFIX=arm-none-eabi-
set INCLUDE=^
 -I ..^
 -I ../../../../Inc^
 -I ../../../../NUC100SeriesBSP/CMSIS/CM0/CoreSupport^
 -I ../../../../NUC100SeriesBSP/CMSIS/CM0/DeviceSupport/Nuvoton/NUC1xx^
 -I ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Include^
 -I ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Include/Driver

set CFLAGS=%MY_SPECS% -mcpu=cortex-m0 -mthumb -Wall -ffunction-sections -Os -std=c99 -gdwarf-2 

set LDFLAGS=%MY_SPECS% -Os -nostartfiles -Wl,-Map=%MY_TARGET%.map -mcpu=cortex-m0 -mthumb -L. -Wl,--gc-sections -Wl,-Tlink.ld

%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/CMSIS/CM0/CoreSupport/core_cm0.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/CMSIS/CM0/DeviceSupport/Nuvoton/NUC1xx/system_NUC1xx.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/CMSIS/CM0/DeviceSupport/Nuvoton/NUC1xx/startup/gcc/startup_NUC1xx.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/DrvGPIO.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/DrvSYS.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/DrvUART.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/retarget.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/NuvotonPlatform_gcc/syscalls.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NUC100SeriesBSP/NuvotonPlatform_gcc/printf.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../Src/tt.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../Main.c

%PREFIX%gcc %LDFLAGS% -o %MY_TARGET%.elf^
 core_cm0.o^
 system_NUC1xx.o^
 startup_NUC1xx.o^
 DrvGPIO.o^
 DrvSYS.o^
 DrvUART.o^
 retarget.o^
 syscalls.o^
 printf.o^
 tt.o^
 Main.o^
 
 %PREFIX%objcopy -O binary -S %MY_TARGET%.elf %MY_TARGET%.bin
 dir %MY_TARGET%.bin
