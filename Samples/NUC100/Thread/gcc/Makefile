.SUFFIXES : .x .o .c .s

MY_TARGET = project

MY_SPECS = --specs=nano.specs
MY_LDFLAG = -Os -nostartfiles -Wl,-Map=$(MY_TARGET).map $(MY_SPECS) -mcpu=cortex-m0 -mthumb -L. -Wl,--gc-sections -Wl,-Tlink.ld

MY_CFLAG = $(MY_SPECS) -mcpu=cortex-m0 -mthumb -Wall -ffunction-sections -Os -std=c99 -gdwarf-2 \
	-I .. \
	-I ../../../../Inc \
	-I ../../../../NUC100SeriesBSP/CMSIS/CM0/CoreSupport \
	-I ../../../../NUC100SeriesBSP/CMSIS/CM0/DeviceSupport/Nuvoton/NUC1xx \
	-I ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Include \
	-I ../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Include/Driver \

MY_SOURCE = \
	../../../../NUC100SeriesBSP/CMSIS/CM0/CoreSupport/core_cm0.c \
	../../../../NUC100SeriesBSP/CMSIS/CM0/DeviceSupport/Nuvoton/NUC1xx/system_NUC1xx.c \
	../../../../NUC100SeriesBSP/CMSIS/CM0/DeviceSupport/Nuvoton/NUC1xx/startup/gcc/startup_NUC1xx.c \
	../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/DrvGPIO.c \
	../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/DrvSYS.c \
	../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/DrvUART.c \
	../../../../NUC100SeriesBSP/NuvotonPlatform_Keil/Src/Driver/retarget.c \
	../../../../NUC100SeriesBSP/NuvotonPlatform_gcc/syscalls.c \
	../../../../NUC100SeriesBSP/NuvotonPlatform_gcc/printf.c \
	../../../../Src/tt.c \
	../Main.c \


#PATH:=$(BUILD_ROOT)/bin:$(PATH)
PREFIX=arm-none-eabi-

OBJS = $(patsubst %.c,%.o,\
	$(patsubst %.cpp,%.o,\
	$(patsubst %.c++,%.o,\
	$(patsubst %.cc,%.o,\
	$(patsubst %.s,%.o,\
	$(MY_SOURCE))))))


all: $(OBJS)
	$(PREFIX)gcc $(MY_LDFLAG) -o $(MY_TARGET).elf $(OBJS)
	$(PREFIX)objcopy -O binary -S $(MY_TARGET).elf $(MY_TARGET).bin
	ls -la $(MY_TARGET).bin

.c.o:
	$(PREFIX)gcc $(MY_CFLAG) -o $*.o -c $*.c

.s.o:
	$(PREFIX)gcc $(MY_CFLAG) -o $*.o -c $*.s


clean:
	rm -f $(OBJS)
	rm -f $(MY_TARGET).elf
	rm -f $(MY_TARGET).map
	rm -f $(MY_TARGET).bin
