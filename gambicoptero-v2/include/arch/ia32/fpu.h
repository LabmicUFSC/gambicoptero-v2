// EPOS IA32 FPU Mediator Declarations

#ifndef __ia32_fpu_h
#define __ia32_fpu_h

#include <fpu.h>

__BEGIN_SYS

class IA32_FPU: public FPU_Common
{
public:

	class Context {
    public:
		Context() { }
		Context(Reg32 cw, Reg32 sw, Reg32 tag, Reg32 ipoff, Reg32 cssel,
				Reg32 dataoff, Reg32 datasel)
		: _cw(cw), _sw(sw), _tag(tag), _ipoff(ipoff), _cssel(cssel),
			_dataoff(dataoff), _datasel(datasel) { }

		// FPU registers structure
		struct _fpreg {
			Reg16 significand[4];
			Reg16 exponent;
		};

    private:
		Reg32   _cw;				/* FPU Control Word     */
		Reg32   _sw;				/* FPU Status Word      */
		Reg32   _tag;				/* FPU Tag Word         */
		Reg32   _ipoff;				/* FPU IP Offset        */
		Reg32   _cssel;				/* FPU IP Selector      */
		Reg32   _dataoff;			/* FPU Operand Pointer Offset */
		Reg32   _datasel;			/* FPU Operand Pointer Selector */
		struct _fpreg   _st[8];		/* 8*10 bytes for each FP-reg = 80 bytes */
		Reg32 _padding[8];			/* padding                */
	}; 
    
public:
    IA32_FPU() {}

    static void init() { ASMV("fninit"); };

	static void save(volatile Context *context);
	static void load(volatile Context *context);
	
};

__END_SYS

#endif
