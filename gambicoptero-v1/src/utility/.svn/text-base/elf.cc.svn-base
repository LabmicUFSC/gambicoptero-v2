// EPOS ELF Utility Implementation

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/elf.h>
#include <utility/string.h>

__BEGIN_SYS

int ELF::load_segment(int i, Elf32_Addr addr)
{
    if((i > segments()) || (segment_type(i) != PT_LOAD))
	return 0;
    
    char * src = (char *)(unsigned(this) + seg(i)->p_offset);
    char * dst = (char *)((addr) ? addr : segment_address(i));
    
    memcpy(dst, src, seg(i)->p_filesz);
    memset(dst + seg(i)->p_filesz, 0,
	   seg(i)->p_memsz - seg(i)->p_filesz);
    
    return seg(i)->p_memsz;
}

__END_SYS
