// ELF Utility Declarations

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __elf_h
#define	__elf_h

#include <string.h>
#include "elf-linux.h"

class ELF: private Elf32_Ehdr
{
public:
    ELF() {}

    bool valid() { // check for ELF magic number
	return (e_ident[EI_MAG0] == ELFMAG0) && (e_ident[EI_MAG1] == ELFMAG1)
	    && (e_ident[EI_MAG2] == ELFMAG2) && (e_ident[EI_MAG3] == ELFMAG3);
    }

    Elf32_Addr entry() { return e_entry; }

    int segments() { return e_phnum; }

    Elf32_Word segment_type(int i) {
 	return (i > segments()) ? PT_NULL : seg(i)->p_type;
    }

    Elf32_Addr segment_address(int i) {
	return (i > segments()) ? 0 :
	    (seg(i)->p_vaddr & ~(seg(i)->p_align - 1));
    }

    int segment_size(int i) {
	return (i > segments()) ? -1 : (int)(
	    ((seg(i)->p_offset % seg(i)->p_align)
	     + seg(i)->p_memsz
	     + seg(i)->p_align - 1)
	    & ~(seg(i)->p_align - 1));
    }

    int load_segment(int i, Elf32_Addr addr = 0) {
	 if((i > segments()) || (segment_type(i) != PT_LOAD))
           return 0;
         char * src = (char *)(unsigned(this) + seg(i)->p_offset);
         char * dst = (char *)((addr) ? addr : segment_address(i));

         memcpy(dst, src, seg(i)->p_filesz);
         memset(dst + seg(i)->p_filesz, 0,
         seg(i)->p_memsz - seg(i)->p_filesz);

         return seg(i)->p_memsz;
    }
			    
private:
    Elf32_Phdr * pht() { return (Elf32_Phdr *)(((char *) this) + e_phoff); }
    Elf32_Phdr * seg(int i) { return &pht()[i];  }

};

#endif

