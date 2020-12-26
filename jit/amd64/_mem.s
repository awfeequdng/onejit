
.globl main
.type main,@function
main:
	lea main,      %rax
	lea main(%rip),%rbx
	mov main,      %rcx
	ret
