rem @echo off
set MY_TARGET=project
set MY_SPECS=--specs=nano.specs
set PREFIX=arm-none-eabi-
set INCLUDE=^
 -I ..^
 -I ../../../../Inc^
 -I ../../../../NANO100BSeriesBSP/Libraries/CMSIS/Include^
 -I ../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Include^
 -I ../../../../NANO100BSeriesBSP/Libraries/NANO1xx_Drivers/Include

set CFLAGS=%MY_SPECS% -mcpu=cortex-m0 -mthumb -Wall -ffunction-sections -Os -std=c99 -gdwarf-2 

set LDFLAGS=%MY_SPECS% -Os -nostartfiles -Wl,-Map=%MY_TARGET%.map -mcpu=cortex-m0 -mthumb -L. -Wl,--gc-sections -Wl,-Tlink.ld

%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Source/gcc/syscalls.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Source/gcc/printf.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Source/gcc/startup_nano1xx.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NANO100BSeriesBSP/Samples/Driver/COMMON/nano1xx_retarget.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NANO100BSeriesBSP/Samples/Driver/COMMON/system_nano1xx.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../NANO100BSeriesBSP/Libraries/NANO1xx_Drivers/Source/nano1xx_gpio.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../../../../Src/tt.c
%PREFIX%gcc %CFLAGS% %INCLUDE% -c ../Main.c

%PREFIX%gcc %LDFLAGS% -o %MY_TARGET%.elf^
 syscalls.o^
 printf.o^
 startup_nano1xx.o^
 nano1xx_retarget.o^
 system_nano1xx.o^
 nano1xx_gpio.o^
 tt.o^
 Main.o^
 
 %PREFIX%objcopy -O binary -S %MY_TARGET%.elf %MY_TARGET%.bin
 dir %MY_TARGET%.bin
