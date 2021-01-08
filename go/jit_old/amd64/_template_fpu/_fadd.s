	.file	"_fadd.cpp"
	.text
	.p2align 4
	.globl	_Z3add4fnumS_
	.type	_Z3add4fnumS_, @function
_Z3add4fnumS_:
.LFB0:
	.cfi_startproc
	movd	%edi, %xmm0
	movd	%esi, %xmm1
	addss	%xmm1, %xmm0
	movd	%xmm0, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	_Z3add4fnumS_, .-_Z3add4fnumS_
	.p2align 4
	.globl	_Z3addR4fnumS0_
	.type	_Z3addR4fnumS0_, @function
_Z3addR4fnumS0_:
.LFB1:
	.cfi_startproc
	movss	(%rdi), %xmm0
	addss	(%rsi), %xmm0
	movd	%xmm0, %eax
	ret
	.cfi_endproc
.LFE1:
	.size	_Z3addR4fnumS0_, .-_Z3addR4fnumS0_
	.p2align 4
	.globl	_Z3add4dnumS_
	.type	_Z3add4dnumS_, @function
_Z3add4dnumS_:
.LFB2:
	.cfi_startproc
	movq	%rdi, %xmm0
	movq	%rsi, %xmm1
	addsd	%xmm1, %xmm0
	movq	%xmm0, %rax
	ret
	.cfi_endproc
.LFE2:
	.size	_Z3add4dnumS_, .-_Z3add4dnumS_
	.p2align 4
	.globl	_Z3addR4dnumS0_
	.type	_Z3addR4dnumS0_, @function
_Z3addR4dnumS0_:
.LFB3:
	.cfi_startproc
	movsd	(%rdi), %xmm0
	addsd	(%rsi), %xmm0
	movq	%xmm0, %rax
	ret
	.cfi_endproc
.LFE3:
	.size	_Z3addR4dnumS0_, .-_Z3addR4dnumS0_
	.ident	"GCC: (Debian 9.2.1-16) 9.2.1 20191030"
	.section	.note.GNU-stack,"",@progbits
