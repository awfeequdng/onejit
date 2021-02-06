	.file	"_call.s"
	.text

	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
	.cfi_startproc
        xor	%eax, %eax
	ret
	.cfi_endproc



	.p2align 4,,15
	.globl	call_imm
	.type	call_imm, @function
call_imm:
	.cfi_startproc
	call	call_imm+0
	call	call_imm+1
	call	call_imm+2
	call	call_imm+3
	call	call_imm+4
	call	call_imm+5
	call	call_imm+6
	call	call_imm+7
	call	call_imm+0x7f
	call	call_imm+0x80
	call	call_imm+0x81
	call	call_imm+0x82
	call	call_imm+0x83
	call	call_imm+0x7fffffff
        ret
	.cfi_endproc



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
	.globl	call_base
	.type	call_base, @function
call_base:
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
	.globl	call_off8_base
	.type	call_off8_base, @function
call_off8_base:
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
	.globl	call_off32_base
	.type	call_off32_base, @function
call_off32_base:
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


	.p2align 4,,15
	.globl	call_base_index0
	.type	call_base_index0, @function
call_base_index0:
	.cfi_startproc
	call	*(%rax,%rax,1)
	call	*(%rcx,%rax,1)
	call	*(%rdx,%rax,1)
	call	*(%rbx,%rax,1)
	call	*(%rsp,%rax,1)
	call	*(%rbp,%rax,1)
	call	*(%rsi,%rax,1)
	call	*(%rdi,%rax,1)
	call	*(%r8,%rax,1)
	call	*(%r9,%rax,1)
	call	*(%r10,%rax,1)
	call	*(%r11,%rax,1)
	call	*(%r12,%rax,1)
	call	*(%r13,%rax,1)
	call	*(%r14,%rax,1)
	call	*(%r15,%rax,1)
	nop
	call	*(%rax,%rax,2)
	call	*(%rcx,%rax,2)
	call	*(%rdx,%rax,2)
	call	*(%rbx,%rax,2)
	call	*(%rsp,%rax,2)
	call	*(%rbp,%rax,2)
	call	*(%rsi,%rax,2)
	call	*(%rdi,%rax,2)
	call	*(%r8,%rax,2)
	call	*(%r9,%rax,2)
	call	*(%r10,%rax,2)
	call	*(%r11,%rax,2)
	call	*(%r12,%rax,2)
	call	*(%r13,%rax,2)
	call	*(%r14,%rax,2)
	call	*(%r15,%rax,2)
	nop
	call	*(%rax,%rax,4)
	call	*(%rcx,%rax,4)
	call	*(%rdx,%rax,4)
	call	*(%rbx,%rax,4)
	call	*(%rsp,%rax,4)
	call	*(%rbp,%rax,4)
	call	*(%rsi,%rax,4)
	call	*(%rdi,%rax,4)
	call	*(%r8,%rax,4)
	call	*(%r9,%rax,4)
	call	*(%r10,%rax,4)
	call	*(%r11,%rax,4)
	call	*(%r12,%rax,4)
	call	*(%r13,%rax,4)
	call	*(%r14,%rax,4)
	call	*(%r15,%rax,4)
	nop
	call	*(%rax,%rax,8)
	call	*(%rcx,%rax,8)
	call	*(%rdx,%rax,8)
	call	*(%rbx,%rax,8)
	call	*(%rbp,%rax,8)
	call	*(%rsp,%rax,8)
	call	*(%rsi,%rax,8)
	call	*(%rdi,%rax,8)
	call	*(%r8,%rax,8)
	call	*(%r9,%rax,8)
	call	*(%r10,%rax,8)
	call	*(%r11,%rax,8)
	call	*(%r12,%rax,8)
	call	*(%r13,%rax,8)
	call	*(%r14,%rax,8)
	call	*(%r15,%rax,8)
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	call_base0_index
	.type	call_base0_index, @function
call_base0_index:
	.cfi_startproc
	call	*(%rax,%rax,1)
	call	*(%rax,%rcx,1)
	call	*(%rax,%rdx,1)
	call	*(%rax,%rbx,1)
	ud2
	call	*(%rax,%rbp,1)
	call	*(%rax,%rsi,1)
	call	*(%rax,%rdi,1)
	call	*(%rax,%r8,1)
	call	*(%rax,%r9,1)
	call	*(%rax,%r10,1)
	call	*(%rax,%r11,1)
	call	*(%rax,%r12,1)
	call	*(%rax,%r13,1)
	call	*(%rax,%r14,1)
	call	*(%rax,%r15,1)
	nop
	call	*(%rax,%rax,2)
	call	*(%rax,%rcx,2)
	call	*(%rax,%rdx,2)
	call	*(%rax,%rbx,2)
	ud2
	call	*(%rax,%rbp,2)
	call	*(%rax,%rsi,2)
	call	*(%rax,%rdi,2)
	call	*(%rax,%r8,2)
	call	*(%rax,%r9,2)
	call	*(%rax,%r10,2)
	call	*(%rax,%r11,2)
	call	*(%rax,%r12,2)
	call	*(%rax,%r13,2)
	call	*(%rax,%r14,2)
	call	*(%rax,%r15,2)
	nop
	call	*(%rax,%rax,4)
	call	*(%rax,%rcx,4)
	call	*(%rax,%rdx,4)
	call	*(%rax,%rbx,4)
	nop
	call	*(%rax,%rbp,4)
	call	*(%rax,%rsi,4)
	call	*(%rax,%rdi,4)
	call	*(%rax,%r8,4)
	call	*(%rax,%r9,4)
	call	*(%rax,%r10,4)
	call	*(%rax,%r11,4)
	call	*(%rax,%r12,4)
	call	*(%rax,%r13,4)
	call	*(%rax,%r14,4)
	call	*(%rax,%r15,4)
	nop
	call	*(%rax,%rax,8)
	call	*(%rax,%rcx,8)
	call	*(%rax,%rdx,8)
	call	*(%rax,%rbx,8)
	ud2
	call	*(%rax,%rbp,8)
	call	*(%rax,%rsi,8)
	call	*(%rax,%rdi,8)
	call	*(%rax,%r8,8)
	call	*(%rax,%r9,8)
	call	*(%rax,%r10,8)
	call	*(%rax,%r11,8)
	call	*(%rax,%r12,8)
	call	*(%rax,%r13,8)
	call	*(%rax,%r14,8)
	call	*(%rax,%r15,8)
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	call_index
	.type	call_index, @function
call_index:
	.cfi_startproc
	call	*(,%rax,1)
	call	*(,%rcx,1)
	call	*(,%rdx,1)
	call	*(,%rbx,1)
	nop
	call	*(,%rbp,1)
	call	*(,%rsi,1)
	call	*(,%rdi,1)
	call	*(,%r8,1)
	call	*(,%r9,1)
	call	*(,%r10,1)
	call	*(,%r11,1)
	call	*(,%r12,1)
	call	*(,%r13,1)
	call	*(,%r14,1)
	call	*(,%r15,1)
	call	*(,%rax,2)
	call	*(,%rcx,2)
	call	*(,%rdx,2)
	call	*(,%rbx,2)
	nop
	call	*(,%rbp,2)
	call	*(,%rsi,2)
	call	*(,%rdi,2)
	call	*(,%r8,2)
	call	*(,%r9,2)
	call	*(,%r10,2)
	call	*(,%r11,2)
	call	*(,%r12,2)
	call	*(,%r13,2)
	call	*(,%r14,2)
	call	*(,%r15,2)
	call	*(,%rax,4)
	call	*(,%rcx,4)
	call	*(,%rdx,4)
	call	*(,%rbx,4)
	nop
	call	*(,%rbp,4)
	call	*(,%rsi,4)
	call	*(,%rdi,4)
	call	*(,%r8,4)
	call	*(,%r9,4)
	call	*(,%r10,4)
	call	*(,%r11,4)
	call	*(,%r12,4)
	call	*(,%r13,4)
	call	*(,%r14,4)
	call	*(,%r15,4)
	call	*(,%rax,8)
	call	*(,%rcx,8)
	call	*(,%rdx,8)
	call	*(,%rbx,8)
	nop
	call	*(,%rbp,8)
	call	*(,%rsi,8)
	call	*(,%rdi,8)
	call	*(,%r8,8)
	call	*(,%r9,8)
	call	*(,%r10,8)
	call	*(,%r11,8)
	call	*(,%r12,8)
	call	*(,%r13,8)
	call	*(,%r14,8)
	call	*(,%r15,8)
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	call_off32_index
	.type	call_off32_index, @function
call_off32_index:
	.cfi_startproc
	call	*0x67452301(,%rax,1)
	call	*0x67452301(,%rcx,1)
	call	*0x67452301(,%rdx,1)
	call	*0x67452301(,%rbx,1)
	nop
	call	*0x67452301(,%rbp,1)
	call	*0x67452301(,%rsi,1)
	call	*0x67452301(,%rdi,1)
	call	*0x67452301(,%r8,1)
	call	*0x67452301(,%r9,1)
	call	*0x67452301(,%r10,1)
	call	*0x67452301(,%r11,1)
	call	*0x67452301(,%r12,1)
	call	*0x67452301(,%r13,1)
	call	*0x67452301(,%r14,1)
	call	*0x67452301(,%r15,1)
	call	*0x67452301(,%rax,2)
	call	*0x67452301(,%rcx,2)
	call	*0x67452301(,%rdx,2)
	call	*0x67452301(,%rbx,2)
	nop
	call	*0x67452301(,%rbp,2)
	call	*0x67452301(,%rsi,2)
	call	*0x67452301(,%rdi,2)
	call	*0x67452301(,%r8,2)
	call	*0x67452301(,%r9,2)
	call	*0x67452301(,%r10,2)
	call	*0x67452301(,%r11,2)
	call	*0x67452301(,%r12,2)
	call	*0x67452301(,%r13,2)
	call	*0x67452301(,%r14,2)
	call	*0x67452301(,%r15,2)
	call	*0x67452301(,%rax,4)
	call	*0x67452301(,%rcx,4)
	call	*0x67452301(,%rdx,4)
	call	*0x67452301(,%rbx,4)
	nop
	call	*0x67452301(,%rbp,4)
	call	*0x67452301(,%rsi,4)
	call	*0x67452301(,%rdi,4)
	call	*0x67452301(,%r8,4)
	call	*0x67452301(,%r9,4)
	call	*0x67452301(,%r10,4)
	call	*0x67452301(,%r11,4)
	call	*0x67452301(,%r12,4)
	call	*0x67452301(,%r13,4)
	call	*0x67452301(,%r14,4)
	call	*0x67452301(,%r15,4)
	call	*0x67452301(,%rax,8)
	call	*0x67452301(,%rcx,8)
	call	*0x67452301(,%rdx,8)
	call	*0x67452301(,%rbx,8)
	nop
	call	*0x67452301(,%rbp,8)
	call	*0x67452301(,%rsi,8)
	call	*0x67452301(,%rdi,8)
	call	*0x67452301(,%r8,8)
	call	*0x67452301(,%r9,8)
	call	*0x67452301(,%r10,8)
	call	*0x67452301(,%r11,8)
	call	*0x67452301(,%r12,8)
	call	*0x67452301(,%r13,8)
	call	*0x67452301(,%r14,8)
	call	*0x67452301(,%r15,8)
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	call_off32_base_index0
	.type	call_off32_base_index0, @function
call_off32_base_index0:
	.cfi_startproc
	call	*0x67452301(%rax,%rax,1)
	call	*0x67452301(%rcx,%rax,1)
	call	*0x67452301(%rdx,%rax,1)
	call	*0x67452301(%rbx,%rax,1)
	call	*0x67452301(%rsp,%rax,1)
	call	*0x67452301(%rbp,%rax,1)
	call	*0x67452301(%rsi,%rax,1)
	call	*0x67452301(%rdi,%rax,1)
	call	*0x67452301(%r8,%rax,1)
	call	*0x67452301(%r9,%rax,1)
	call	*0x67452301(%r10,%rax,1)
	call	*0x67452301(%r11,%rax,1)
	call	*0x67452301(%r12,%rax,1)
	call	*0x67452301(%r13,%rax,1)
	call	*0x67452301(%r14,%rax,1)
	call	*0x67452301(%r15,%rax,1)
	nop
	call	*0x67452301(%rax,%rax,2)
	call	*0x67452301(%rcx,%rax,2)
	call	*0x67452301(%rdx,%rax,2)
	call	*0x67452301(%rbx,%rax,2)
	call	*0x67452301(%rsp,%rax,2)
	call	*0x67452301(%rbp,%rax,2)
	call	*0x67452301(%rsi,%rax,2)
	call	*0x67452301(%rdi,%rax,2)
	call	*0x67452301(%r8,%rax,2)
	call	*0x67452301(%r9,%rax,2)
	call	*0x67452301(%r10,%rax,2)
	call	*0x67452301(%r11,%rax,2)
	call	*0x67452301(%r12,%rax,2)
	call	*0x67452301(%r13,%rax,2)
	call	*0x67452301(%r14,%rax,2)
	call	*0x67452301(%r15,%rax,2)
	nop
	call	*0x67452301(%rax,%rax,4)
	call	*0x67452301(%rcx,%rax,4)
	call	*0x67452301(%rdx,%rax,4)
	call	*0x67452301(%rbx,%rax,4)
	call	*0x67452301(%rsp,%rax,4)
	call	*0x67452301(%rbp,%rax,4)
	call	*0x67452301(%rsi,%rax,4)
	call	*0x67452301(%rdi,%rax,4)
	call	*0x67452301(%r8,%rax,4)
	call	*0x67452301(%r9,%rax,4)
	call	*0x67452301(%r10,%rax,4)
	call	*0x67452301(%r11,%rax,4)
	call	*0x67452301(%r12,%rax,4)
	call	*0x67452301(%r13,%rax,4)
	call	*0x67452301(%r14,%rax,4)
	call	*0x67452301(%r15,%rax,4)
	nop
	call	*0x67452301(%rax,%rax,8)
	call	*0x67452301(%rcx,%rax,8)
	call	*0x67452301(%rdx,%rax,8)
	call	*0x67452301(%rbx,%rax,8)
	call	*0x67452301(%rsp,%rax,8)
	call	*0x67452301(%rbp,%rax,8)
	call	*0x67452301(%rsi,%rax,8)
	call	*0x67452301(%rdi,%rax,8)
	call	*0x67452301(%r8,%rax,8)
	call	*0x67452301(%r9,%rax,8)
	call	*0x67452301(%r10,%rax,8)
	call	*0x67452301(%r11,%rax,8)
	call	*0x67452301(%r12,%rax,8)
	call	*0x67452301(%r13,%rax,8)
	call	*0x67452301(%r14,%rax,8)
	call	*0x67452301(%r15,%rax,8)
        ret
	.cfi_endproc


	.p2align 4,,15
	.globl	call_misc
	.type	call_misc, @function
call_misc:
	.cfi_startproc
	call	*(%rip)
	call	*0x7f(%rip)
	call	*0x67452301(%rip)
	nop
	call	*0x67452301
        ret
	.cfi_endproc
