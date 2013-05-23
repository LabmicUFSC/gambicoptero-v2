// EPOS AVR Flash Mediator Common Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __avr_flash_h
#define __avr_flash_h

#include <flash.h>

__BEGIN_SYS

class AVR_Flash
{
protected:
    AVR_Flash() {}

private:
    typedef IO_Map<Machine> IO;
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg16 Address;
    static const unsigned int PAGE_SIZE = Traits<Flash>::PAGE_SIZE;

public:
    enum {
	// SPMCSR
	SPMIE  = 7,
	RWWSB  = 6,
	RWWSRE = 4,
	BLBSET = 3,
	PGWRT  = 2,
	PGERS  = 1,
	SPMEN  = 0,
	// EECR
	EEWE   = 1

    };

    unsigned char read(const Address addr) {
	return pgm_read_byte(addr);
    }

    int write(Address addr, unsigned char * data, unsigned int size) __attribute__ ((section (".bootloader"))) __attribute__ ((noinline)) {
	Reg8 buffer_tmp[PAGE_SIZE];
	Reg16 i, j, w, page_addr;
	Reg8 status_register, temp;

	page_addr = addr & ~(PAGE_SIZE - 1);

	spm_busy_wait();

	for (i = 0; i < PAGE_SIZE; i++)
	    buffer_tmp[i] = pgm_read_byte(page_addr + i);

	status_register = sreg();

	CPU::int_disable();

	page_erase_safe(page_addr);

	spm_busy_wait();

	j = 0;
	for (i = 0; i < PAGE_SIZE; i += 2) {
	    if ((((page_addr + i) == addr) || ((page_addr + i + 1) == addr)) && (j != size)) {
		if (j + 2 <= size) {
		    temp = *data++;
		    w = (*data++ << 8) | temp;
		    addr += 2;
		    j += 2;
		} else {
		    w = (buffer_tmp[i] << 8) | *data++;
		    addr += 2;
		    j += 1;
		}
	    } else {
		w = (buffer_tmp[i+1] << 8) | (buffer_tmp[i]);
	    }
	    
	    page_fill(page_addr + i, w);
	}

	page_write(addr);

	spm_busy_wait();

	rww_enable();

	sreg(status_register);

	return 1;
    }

private:
    Reg8 spmcsr() { return CPU::in8(IO::SPMCSR); }
    void spmcsr(Reg8 value) { CPU::out8(IO::SPMCSR, value); }
    Reg8 sreg() { return CPU::in8(IO::SREG); }
    void sreg(Reg8 value) { CPU::out8(IO::SREG, value); }
    Reg8 eecr() { return CPU::in8(IO::EECR); }
    void eecr(Reg8 value) { CPU::out8(IO::EECR, value); }

    void spm_busy_wait() {
	while (spmcsr() & (1 << SPMEN));
    }

    Reg8 pgm_read_byte(Reg16 addr) {
	return (__extension__({
     	  Reg16 __addr16 = (Reg16)(addr);
	  Reg8 __result;
	  ASMV(
	      "lpm" "\n\t"
	      "mov %0, r0" "\n\t"
	      : "=r" (__result)
	      : "z" (__addr16)
	      : "r0"
	  );
	  __result;
	}));
    }
    
    void eeprom_busy_wait() {
	while (!(!(eecr() & (1 << EEWE))));
    }

    void page_erase_safe(Reg16 addr) {
	spm_busy_wait();
	eeprom_busy_wait();
	page_erase(addr);
    }

    void page_erase(Reg16 addr) {
	ASMV(
		"movw r30, %A3\n\t"
		"sts  %1, %C3\n\t"
		"sts %0, %2\n\t"
		"spm\n\t"
		:
		: "i" ((Reg16)(IO::SPMCSR + 0x20)),
		  "i" ((Reg16)(IO::RAMPZ + 0x20)),
		  "r" ((Reg8)(1 << SPMEN)|(1 << PGERS)),
		  "r" ((Reg16)addr)
		: "r30", "r31"
	    );
    }

    void page_fill(Reg16 addr, Reg16 data) {
	ASMV(
		"movw  r0, %4\n\t"
		"movw r30, %A3\n\t"
		"sts %1, %C3\n\t"
		"sts %0, %2\n\t"
		"spm\n\t"
		"clr  r1\n\t"
		:
		: "i" ((Reg16)(IO::SPMCSR + 0x20)),
		  "i" ((Reg16)(IO::RAMPZ + 0x20)),
		  "r" ((Reg8)(1 << SPMEN)),
		  "r" ((Reg16)addr),
		  "r" ((Reg16)data)
		: "r0", "r30", "r31"
	    );
    }

    void page_write(Reg16 addr) {
	ASMV(
		"movw r30, %A3\n\t"
		"sts %1, %C3\n\t"
		"sts %0, %2\n\t"
		"spm\n\t"
		:
		: "i" ((Reg16)(IO::SPMCSR + 0x20)),
		  "i" ((Reg16)(IO::RAMPZ + 0x20)),
		  "r" ((Reg8)(1 << SPMEN) | (1 << PGWRT)),
		  "r" ((Reg16)addr)
		: "r30", "r31"
	    );  
    }

    void rww_enable() {
	ASMV(
		"sts %0, %1\n\t"
		"spm\n\t"
		:
		: "i" ((Reg16)(IO::SPMCSR + 0x20)),
		"r" ((Reg8)(1 << SPMEN) | (1 << RWWSRE))
	    );
    }
};

__END_SYS

#endif

