# The CC_X86 is for compiling tools on your PC.

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
# The GCC_MIPS is for compiling code on the target.
# Convert_bin changes test.axf into code.txt which is used by the VHDL.
# Convert_bin == "objcopy -I elf32-big -O binary test.axf test.bin"


ifeq ($(LANG),)

# Customize for Windows
# The MIPS gcc compiler must use the cygwin1.dll that came with the compiler.
CC_X86 = cl /O1 /nologo
CP = copy
RM = del
DWIN32 = -DWIN32
BIN_MIPS = ..\gccmips_elf
VHDL_DIR = ..\vhdl
LINUX_PWD =
GCC_MIPS  = $(BIN_MIPS)\gcc $(CFLAGS)
AS_MIPS   = $(BIN_MIPS)\as
LD_MIPS   = $(BIN_MIPS)\ld
DUMP_MIPS = $(BIN_MIPS)\objdump
CONVERT_BIN = $(LINUX_PWD)convert_bin.exe

else

# Customized for Linux
# See http://www.opencores.com/projects.cgi/web/mips/linux_tools.htm
CC_X86 = gcc -Wall -O -g
CP = cp
RM = rm -rf 
DWIN32 =
BIN_MIPS = 
VHDL_DIR = ../vhdl
LINUX_PWD = ./
GCC_MIPS  = $(BIN_MIPS)mips-gcc $(CFLAGS)
GPP_MIPS  = $(BIN_MIPS)mips-g++ $(CFLAGS)
AS_MIPS   = $(BIN_MIPS)mips-as
LD_MIPS   = $(BIN_MIPS)mips-ld
DUMP_MIPS = $(BIN_MIPS)mips-objdump
CONVERT_BIN = $(LINUX_PWD)convert_bin.exe
#CONVERT_BIN = $(BIN_MIPS)mips-elf-objcopy -I elf32-big -O binary test.axf test.bin

endif

CFLAGS = -O2 -Wall -c -s 

all: convert_bin.exe bintohex.exe ram_image.exe
	@echo make targets = count, opcodes, pi, test, run, tohex,\
	bootldr, toimage, eterm
	
clean:
	-$(RM) *.o *.obj *.map *.lst *.hex *.txt *.exe *.axf

#Same as "objcopy -I elf32-big -O binary test.axf test.bin"
convert_bin.exe: convert.c
	@$(CC_X86) -o convert_bin.exe convert.c

convert_le.exe: convert.c
	@$(CC_X86) -DLITTLE_ENDIAN -o convert_le.exe convert.c

mlite.exe: mlite.c
	@$(CC_X86) -o mlite.exe mlite.c $(DWIN32)

bintohex.exe: bintohex.c
	@$(CC_X86) -o bintohex.exe bintohex.c

ram_image.exe: ram_image.c
	@$(CC_X86) -o ram_image.exe ram_image.c

# Terminal program that will download a new image.
# For Linux, use any serial port program at 57600 baud 8-bits;1-stop bit
# This mode does not support Ethernet packet transfers.
eterm: etermip.c
	-@$(CC_X86) -o etermip.exe etermip.c

# Terminal program that will download a new image and supports Ethernet
# traffic over UART.  Get wpcap.lib from http://www.winpcap.org/.
# Requires Windows OS
# Uses wpcap.lib for receiving and transmitting raw Ethernet packets
# Get wpcap.lib from http://www.winpcap.org/.
etermip: etermip.c
	@echo Get wpcap.lib from http://www.winpcap.org/
	-@$(CC_X86) -DUSE_WPCAP -o etermip.exe etermip.c wpcap.lib

nomult.exe: nomult.c
	-@$(CC_X86) -o nomult.exe nomult.c

download: etermip
	@echo Reset board before downloading code
	etermip

opcodes: 
	$(AS_MIPS) -o opcodes.o opcodes.asm
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf opcodes.o
	-@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

opcodes2: 
	$(AS_MIPS) -o opcodes.o opcodes.asm
	$(LD_MIPS) -Ttext 0x10000000 -eentry -Map test.map -s -N -o test.axf opcodes.o
	-@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

test: 
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) test.c
	$(GCC_MIPS) no_os.c
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o test.o no_os.o
	-@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

count: 
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) count.c
	$(GCC_MIPS) no_os.c 
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o count.o no_os.o 
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)
	
mem_test: 
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) mem_test.c
	$(GCC_MIPS) no_os.c 
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o mem_test.o no_os.o 
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

amba_test:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) amba_test.c
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o amba_test.o
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

to_be_loaded:
	$(GPP_MIPS) to_be_loaded.c --no-exceptions --no-rtti --no-use-cxa-atexit -c -s
	$(LD_MIPS) -Ttext 0x10000000 -eentry -Map test.map -s -N -o test.axf to_be_loaded.o
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

dtmf:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) no_os.c
	$(GCC_MIPS) dtmf/dtmf_detector.c
	$(GCC_MIPS) dtmf/parameters.c
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf boot.o no_os.o dtmf_detector.o parameters.o 
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)
	
loader: 
	$(AS_MIPS) -o boot.o boot.asm
	$(GPP_MIPS) loader.cc --no-exceptions --no-rtti --no-use-cxa-atexit -c -s 
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf boot.o loader.o -lc
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)
	
loader2: 
	$(AS_MIPS) -o boot.o boot.asm
	$(GPP_MIPS) loader.cc --no-exceptions --no-rtti --no-use-cxa-atexit
	$(GCC_MIPS) no_os.c 
	$(LD_MIPS) -Ttext 0 -eentry -s -N -o test.axf -Map test.map boot.o loader.o no_os.o -lc
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

count_le: 
	$(AS_MIPS) -EL -o boot.o boot.asm
	$(GCC_MIPS) -EL count.c
	$(GCC_MIPS) -EL no_os.c 
	$(LD_MIPS) -EL -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o count.o no_os.o 
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

count2:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) count.c
	$(GCC_MIPS) no_os.c
	$(LD_MIPS) -Ttext 0x10000000 -eentry -Map test.map -s -N -o test.axf \
		boot.o count.o no_os.o
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

count3:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) count.c
	$(GCC_MIPS) no_os.c
	$(LD_MIPS) -Ttext 0x2000 -eentry -Map test.map -s -N -o test.axf \
		boot.o count.o no_os.o
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

count5: nomult.exe
	$(AS_MIPS) -o boot.o boot.asm
	$(AS_MIPS) -o mult_sim.o mult_sim.asm
	$(GCC_MIPS) count.c
	$(GCC_MIPS) no_os.c
	$(LD_MIPS) -Ttext 0x10000000 -eentry -Map test.map -s -N -o test.axf \
		boot.o mult_sim.o count.o no_os.o
	-$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CP) test.axf test_old.axf
	nomult test.axf test.lst test.map
	-$(DUMP_MIPS) --disassemble test.axf > test2.lst
	$(CONVERT_BIN)

pi: 
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) pi.c
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o pi.o 
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

pi2:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) pi.c
	$(LD_MIPS) -Ttext 0x10000000 -eentry -Map test.map -s -N -o test.axf \
		boot.o pi.o 
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

pi5: nomult.exe
	$(AS_MIPS) -o boot.o boot.asm
	$(AS_MIPS) -o mult_sim.o mult_sim.asm
	$(GCC_MIPS) pi.c
	$(LD_MIPS) -Ttext 0x0 -eentry -Map test.map -s -N -o test.axf \
		boot.o mult_sim.o pi.o 
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	nomult test.axf test.lst test.map
	@$(DUMP_MIPS) --disassemble test.axf > test2.lst
	$(CONVERT_BIN)

ddr_test:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) ddr_init.c -DDDR_TEST_MAIN
	$(GCC_MIPS) no_os.c
	$(LD_MIPS) -Ttext 0x0 -eentry -Map test.map -s -N -o test.axf \
		boot.o ddr_init.o no_os.o
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

memtest:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) memtest.c
	$(LD_MIPS) -Ttext 0x2000 -eentry -o test.axf boot.o memtest.o
	$(CONVERT_BIN)
	
memtest2:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) -DINIT_DDR memtest.c
	$(GCC_MIPS) ddr_init.c
	$(LD_MIPS) -Ttext 0x0 -eentry -o test.axf boot.o memtest.o ddr_init.o
	$(CONVERT_BIN)

memtest3:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) memtest.c
	$(LD_MIPS) -Ttext 0x10000000 -eentry -o test.axf boot.o memtest.o
	$(CONVERT_BIN)

bootldr:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) bootldr.c
	$(GCC_MIPS) no_os.c
	$(GCC_MIPS) -DDLL_DISABLE ddr_init.c
	$(LD_MIPS) -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o bootldr.o no_os.o ddr_init.o
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)
	@echo Next do "make toimage" for Xilinx or "make tohex" for Altera.

bootldr2:
	$(AS_MIPS) -o boot.o boot.asm
	$(GCC_MIPS) bootldr.c
	$(GCC_MIPS) no_os.c
	$(GCC_MIPS) -DDLL_DISABLE ddr_init.c
	$(LD_MIPS) -Ttext 0x1000 -eentry -Map test.map -s -N -o test.axf \
		boot.o bootldr.o no_os.o ddr_init.o
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(CONVERT_BIN)

bootldr_little_endian: convert_le.exe
	$(AS_MIPS) -EL -o boot.o boot.asm
	$(GCC_MIPS) -EL bootldr.c
	$(GCC_MIPS) -EL no_os.c
	$(GCC_MIPS) -EL ddr_init.c
	$(LD_MIPS) -EL -Ttext 0 -eentry -Map test.map -s -N -o test.axf \
		boot.o bootldr.o no_os.o ddr_init.o
	@$(DUMP_MIPS) --disassemble test.axf > test.lst
	$(LINUX_PWD)convert_le.exe

# Run a MIPS opcode simulator
run: mlite.exe
	@$(LINUX_PWD)mlite.exe test.bin 

run_little_endian: mlite.exe
	@$(LINUX_PWD)mlite.exe test.bin L

disassemble: mlite.exe
	$(LINUX_PWD)mlite.exe test.bin BD > test.txt

# Used by Altera FPGAs for ram image
tohex: bintohex.exe
	$(LINUX_PWD)bintohex.exe test.bin

# Used by Xilinx FPGAs for ram image
toimage:
	$(LINUX_PWD)ram_image.exe ../../src/mach/plasma/ram_xilinx.vhd code.txt ../../src/mach/plasma/ram_image.vhd
	
toimage_coe:
	java bin2coe 4 2048 test.bin 
	cp test.coe ../../src/mach/common/coregen/ram_amba_128k.coe
	rm ../../src/mach/common/coregen/ram_amba_128k.vhd

