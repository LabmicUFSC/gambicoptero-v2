##################################################################
# TITLE: Boot Up Code
# AUTHOR: Steve Rhoads (rhoadss@yahoo.com)
# DATE CREATED: 1/12/02
# FILENAME: boot.asm
# PROJECT: Plasma CPU core
# COPYRIGHT: Software placed into the public domain by the author.
#    Software 'as is' without warranty.  Author liable for nothing.
# DESCRIPTION:
#    Initializes the stack pointer and jumps to main2().
##################################################################
	.text
	.align	2
	.globl	entry
	.ent	entry
entry:
   .set noreorder

   #These eight instructions must be the first instructions.
   #convert.exe will correctly initialize $gp, .sbss_start, .bss_end, $sp
   lui   $gp, 0
   ori   $gp, $gp, 0          #initialize $gp
   lui   $4, 0
   ori   $4, $4, 0            #$4 = .sbss_start
   lui   $5, 0
   ori   $5, $5, 0            #$5 = .bss_end
   lui   $sp, 0
   ori   $sp, $sp, 0xfff0     #initialize stack pointer

$BSS_CLEAR:
   sw    $0, 0($4)
   slt   $3, $4, $5
   bnez  $3, $BSS_CLEAR
   addiu $4, $4, 4

   jal   main
   nop
$L1:
   j $L1

   .end entry

