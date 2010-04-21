# Path to cross tools

MIPS_PREFIX=/it/sw/cross/mips-idt/bin/mips-idt-elf

EXECUTABLES=$(addprefix bin/, dv8)


# gcc flags for the MIPS architecture:
#  -EL     : Little endian
#  -G0     : Always use small data (using register gp)
#  -mips32 : Compile for a 32-bit MIPS architecture
#

ARCH=-EL -G0 -mips32

# Other gcc flags

CFLAGS	+= -ggdb -Wall -W -fno-builtin -I include

# Compiler and linker commands

CC=$(MIPS_PREFIX)-gcc

# Tell the linker where to start the text segment.
LD=$(MIPS_PREFIX)-ld -Ttext 80020000


# Path to Simics installation

SIMICS=/home/$(shell whoami)/simics-workspace




#### RULE USED TO START SIMICS

do_boot: bin/dv8
	./scripts/run.sh $(SIMICS) $<

#### RULES TO BUILD BINARIES FROM OBJECT FILES

bin/dv8: $(addprefix build/, boot.o kernel.o)
	$(LD) $(ARCH) -o $@ $^

#### Add dependency on headerfile of various tty.o files

#build/tty%.o: src/tty%.c include/tty%.h
#	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@


###### GENERIC BUILD PATTERNS ########

build/%.o: src/%.c
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@

build/%.o: src/%.S
	$(CC) $(ARCH) $(CFLAGS)  -c $< -o $@


clean:
	pwd
	rm -f build/*.o
	rm -f include/*~ include/#* include/*#
	rm -f src/*~ src/#* src/*#
	rm -f scripts/*~ scripts/#* scripts/*#
	rm -f ${EXECUTABLES}
