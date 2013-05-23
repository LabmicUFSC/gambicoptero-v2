	.file	"crt1.s"

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

	.text
	.align	4
	.globl	_start
	.type	_start,@function
_start:
	la      $gp,_gp
	jal		_init
	.align	4
	.globl	__epos_library_app_entry
__epos_library_app_entry:
	jal		main
/*	add		$4, $2, $0 */
/*	add		$5, $3, $0 */
	jal		_fini
