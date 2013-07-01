	.file	"crt1.s"

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
