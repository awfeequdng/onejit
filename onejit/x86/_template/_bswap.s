	.file	"_bswap.s"
	.text

	.p2align 4,,15
	.globl	bswap_32
	.type	bswap_32, @function
bswap_32:
	.cfi_startproc
	bswap	%eax
	bswap	%ecx
	bswap	%edx
	bswap	%ebx
	bswap	%esp
	bswap	%ebp
	bswap	%esi
	bswap	%edi
	bswap	%r8d
	bswap	%r9d
	bswap	%r10d
	bswap	%r11d
	bswap	%r12d
	bswap	%r13d
	bswap	%r14d
	bswap	%r15d
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	bswap_64
	.type	bswap_64, @function
bswap_64:
	.cfi_startproc
	bswap	%rax
	bswap	%rcx
	bswap	%rdx
	bswap	%rbx
	bswap	%rsp
	bswap	%rbp
	bswap	%rsi
	bswap	%rdi
	bswap	%r8
	bswap	%r9
	bswap	%r10
	bswap	%r11
	bswap	%r12
	bswap	%r13
	bswap	%r14
	bswap	%r15
        ret
	.cfi_endproc

