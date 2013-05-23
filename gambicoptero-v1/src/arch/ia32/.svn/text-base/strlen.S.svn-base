 .global strlen

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
      

strlen:

 pushl %ebp
 movl %esp,%ebp
 pushl %edi
 movl 8(%ebp),%edx
 movl %edx,%edi
 test $3,%edi
 je L5
 movb (%edi),%cl
 incl %edi
 testb %cl,%cl
 je L15

 test $3,%edi
 je L5
 movb (%edi),%cl
 incl %edi
 testb %cl,%cl
 je L15

 test $3,%edi
 je L5
 movb (%edi),%cl
 incl %edi
 testb %cl,%cl
 je L15

L5:
 subl $4,%edi


 .p2align 4,,7
L10:
 addl $4,%edi
 movl (%edi),%ecx
 leal -16843009(%ecx),%eax
 notl %ecx
 andl %ecx,%eax
 testl $-2139062144,%eax
 je L10


 notl %ecx
 incl %edi

 testb %cl,%cl
 je L15
 incl %edi
 shrl $8,%ecx

 testb %cl,%cl
 je L15
 incl %edi
 shrl $8,%ecx

 testb %cl,%cl
 je L15
 incl %edi



L15:
 subl %edx,%edi
 leal -1(%edi),%eax

 leal -4(%ebp),%esp
 popl %edi
 leave
 ret
