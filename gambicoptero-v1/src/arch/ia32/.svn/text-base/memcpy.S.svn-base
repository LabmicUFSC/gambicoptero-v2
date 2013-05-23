 .global memcpy

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
      

memcpy:

 pushl %ebp
 movl %esp,%ebp
 pushl %esi
 pushl %edi
 pushl %ebx
 movl 8(%ebp),%edi
 movl 16(%ebp),%ecx
 movl 12(%ebp),%esi
 cld



 cmpl $8,%ecx
 jbe .L3



 movl %edi,%edx
 movl %ecx,%ebx
 andl $3,%edx
 jz .L11
 movl $4,%ecx
 subl %edx,%ecx
 andl $3,%ecx
 subl %ecx,%ebx
 rep
 movsb

 mov %ebx,%ecx



.L11:
 shrl $2,%ecx
 .p2align 2
 rep
 movsl

 movl %ebx,%ecx
 andl $3,%ecx





.L3:
 rep
 movsb
.L5:
 movl 8(%ebp),%eax

 leal -12(%ebp),%esp
 popl %ebx
 popl %edi
 popl %esi
 leave
 ret
