.SUFFIXES : .x .o .c .s

MY_TARGET = project

MY_SPECS = --specs=nano.specs
MY_LDFLAG = -Os -nostartfiles -Wl,-Map=$(MY_TARGET).map $(MY_SPECS) -mcpu=cortex-m0 -mthumb -L. -Wl,--gc-sections -Wl,-Tlink.ld

MY_CFLAG = $(MY_SPECS) -mcpu=cortex-m0 -mthumb -Wall -ffunction-sections -Os -std=c99 -gdwarf-2 \
	-I .. \
	-I ../../../../Inc \
	-I ../../../../NANO100BSeriesBSP/Libraries/CMSIS/Include \
	-I ../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Include \
	-I ../../../../NANO100BSeriesBSP/Libraries/NANO1xx_Drivers/Include \

MY_SOURCE = \
	../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Source/gcc/syscalls.c \
	../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Source/gcc/printf.c \
	../../../../NANO100BSeriesBSP/Libraries/Device/Nuvoton/NANO1xx/Source/gcc/startup_nano1xx.c \
	../../../../NANO100BSeriesBSP/Samples/Driver/COMMON/nano1xx_retarget.c \
	../../../../NANO100BSeriesBSP/Samples/Driver/COMMON/system_nano1xx.c \
	../../../../NANO100BSeriesBSP/Libraries/NANO1xx_Drivers/Source/nano1xx_gpio.c \
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
