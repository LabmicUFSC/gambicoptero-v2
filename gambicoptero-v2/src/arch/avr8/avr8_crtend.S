        .file   "avr8_crtend.s"

	.section .text
	.global exit
exit:
	jmp	_exit
