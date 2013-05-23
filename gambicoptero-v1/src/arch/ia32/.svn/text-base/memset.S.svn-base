 .global memset

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
      

memset:

 pushl %ebp
 movl %esp,%ebp
 pushl %edi
 movl 8(%ebp),%edi
 movzbl 12(%ebp),%eax
 movl 16(%ebp),%ecx
 cld



 cmpl $16,%ecx
 jbe .L19
 testl $7,%edi
 je .L10



 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx
 testl $7,%edi
 je .L10

 movb %al,(%edi)
 incl %edi
 decl %ecx


.L10:
 movb %al,%ah
 movl %eax,%edx
 sall $16,%edx
 orl %edx,%eax

 movl %ecx,%edx
 shrl $2,%ecx
 andl $3,%edx
 rep
 stosl
 movl %edx,%ecx


.L19:
 rep
 stosb

 movl 8(%ebp),%eax

 leal -4(%ebp),%esp
 popl %edi
 leave
 ret
