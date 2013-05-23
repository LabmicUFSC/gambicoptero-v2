// EPOS Configuration Engine

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#ifndef __config_h
#define __config_h

//============================================================================
// DEFINITIONS
//============================================================================
#define __SYS_NS		System
#define __BEGIN_SYS		namespace __SYS_NS {
#define __END_SYS		}
#define __USING_SYS		using namespace __SYS_NS;
#define __SYS(X)		::__SYS_NS::X

#define ASM			__asm__
#define ASMV			__asm__ __volatile__

#define __HEADER_ARCH(X)	<arch/ARCH/X.h>
#define __HEADER_MACH(X)	<mach/MACH/X.h>

//============================================================================
// ARCHITECTURE AND MACHINE SELECTION
//============================================================================
#define ARCH arm7
#define __ARCH_TRAITS_H	 __HEADER_ARCH(traits)

#define MACH mc13224v
#define __MACH_TRAITS_H	 __HEADER_MACH(traits)

//============================================================================
// CONFIGURATION
//============================================================================
#include <system/types.h>
#include <system/meta.h>
#include <traits.h>
#include __HEADER_MACH(config)
#include <system/info.h>

//============================================================================
// THINGS EVERBODY NEEDS
//============================================================================
#include <utility/ostream.h>
#include <utility/debug.h>

#endif
