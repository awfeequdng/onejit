	.file	"_call.s"
	.text

	.p2align 4,,15
	.globl	call_reg
	.type	call_reg, @function
call_reg:
	.cfi_startproc
	call	*%rax
	call	*%rcx
	call	*%rdx
	call	*%rbx
	call	*%rsp
	call	*%rbp
	call	*%rsi
	call	*%rdi
	call	*%r8
	call	*%r9
	call	*%r10
	call	*%r11
	call	*%r12
	call	*%r13
	call	*%r14
	call	*%r15
        ret
	.cfi_endproc



	.p2align 4,,15
	.globl	call_mem
	.type	call_mem, @function
call_mem:
	.cfi_startproc
	call	*(%rax)
	call	*(%rcx)
	call	*(%rdx)
	call	*(%rbx)
	call	*(%rsp)
	call	*(%rbp)
	call	*(%rsi)
	call	*(%rdi)
	call	*(%r8)
	call	*(%r9)
	call	*(%r10)
	call	*(%r11)
	call	*(%r12)
	call	*(%r13)
	call	*(%r14)
	call	*(%r15)
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	call_mem_off8
	.type	call_mem_off8, @function
call_mem_off8:
	.cfi_startproc
	call	*0x17(%rax)
	call	*0x17(%rcx)
	call	*0x17(%rdx)
	call	*0x17(%rbx)
	call	*0x17(%rsp)
	call	*0x17(%rbp)
	call	*0x17(%rsi)
	call	*0x17(%rdi)
	call	*0x17(%r8)
	call	*0x17(%r9)
	call	*0x17(%r10)
	call	*0x17(%r11)
	call	*0x17(%r12)
	call	*0x17(%r13)
	call	*0x17(%r14)
	call	*0x17(%r15)
        ret
	.cfi_endproc

	.p2align 4,,15
	.globl	call_mem_off32
	.type	call_mem_off32, @function
call_mem_off32:
	.cfi_startproc
	call	*0x78563412(%rax)
	call	*0x78563412(%rcx)
	call	*0x78563412(%rdx)
	call	*0x78563412(%rbx)
	call	*0x78563412(%rsp)
	call	*0x78563412(%rbp)
	call	*0x78563412(%rsi)
	call	*0x78563412(%rdi)
	call	*0x78563412(%r8)
	call	*0x78563412(%r9)
	call	*0x78563412(%r10)
	call	*0x78563412(%r11)
	call	*0x78563412(%r12)
	call	*0x78563412(%r13)
	call	*0x78563412(%r14)
	call	*0x78563412(%r15)
        ret
	.cfi_endproc

