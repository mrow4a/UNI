#lepiej tego nie modyfikowac
CROSS_COMPILE=arm-elf-
CC=$(CROSS_COMPILE)gcc
AS=$(CROSS_COMPILE)gcc
ASFLAGS=-D__ASSEMBLY__ -g -mcpu=arm9 -c -O0 -Wall
CCFLAGS=-g -mcpu=arm9 -O0 -Wall
#LDFLAGS+=-nostartfiles -Wl,--cref -lc -lgcc -T /opt/arm_user/elf32-littlearm.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
LDFLAGS+=-nostartfiles -Wl -lc -lgcc -T /opt/arm_user/elf32-littlearm.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
OBJS=/opt/arm_user/include/cstartup.o /opt/arm_user/include/lowlevel.o
#koniec, dalej mozna dowolnie zmieniac

PROGRAM_NAME=term.elf

all: $(PROGRAM_NAME)

rebuild: clean $(PROGRAM_NAME)

$(PROGRAM_NAME): main.o
	arm-elf-gcc $(LDFLAGS) $(OBJS) $< -o $@

#main.o: main.S 
#	$(AS) $(ASFLAGS) $< -o $@

#dbg_print.o: dbg_print.c
#	$(CC) -c $(CCFLAGS) $< -o $@

main.o: main.c 
	arm-elf-gcc -c $(CCFLAGS) $< -o $@

clean:
	rm -f *.o *.elf 
