	.file	"mem.cpp"
	.text
	.section	.text.unlikely,"ax",@progbits
	.type	_ZN6onejit3memL15throw_bad_allocEv, @function
_ZN6onejit3memL15throw_bad_allocEv:
.LFB93:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$8, %edi
	call	__cxa_allocate_exception@PLT
	movq	_ZNSt9bad_allocD1Ev@GOTPCREL(%rip), %rdx
	leaq	_ZTISt9bad_alloc(%rip), %rsi
	movq	%rax, %rdi
	leaq	16+_ZTVSt9bad_alloc(%rip), %rax
	movq	%rax, (%rdi)
	call	__cxa_throw@PLT
	.cfi_endproc
.LFE93:
	.size	_ZN6onejit3memL15throw_bad_allocEv, .-_ZN6onejit3memL15throw_bad_allocEv
.LCOLDB0:
	.text
.LHOTB0:
	.p2align 4
	.globl	_ZN6onejit3mem11alloc_bytesEm
	.type	_ZN6onejit3mem11alloc_bytesEm, @function
_ZN6onejit3mem11alloc_bytesEm:
.LFB100:
	.cfi_startproc
	testq	%rdi, %rdi
	je	.L5
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	malloc@PLT
	testq	%rax, %rax
	je	.L6
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	xorl	%eax, %eax
	ret
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.type	_ZN6onejit3mem11alloc_bytesEm.cold, @function
_ZN6onejit3mem11alloc_bytesEm.cold:
.LFSB100:
.L6:
	.cfi_def_cfa_offset 16
	call	_ZN6onejit3memL15throw_bad_allocEv
	.cfi_endproc
.LFE100:
	.text
	.size	_ZN6onejit3mem11alloc_bytesEm, .-_ZN6onejit3mem11alloc_bytesEm
	.section	.text.unlikely
	.size	_ZN6onejit3mem11alloc_bytesEm.cold, .-_ZN6onejit3mem11alloc_bytesEm.cold
.LCOLDE0:
	.text
.LHOTE0:
	.section	.text.unlikely
.LCOLDB1:
	.text
.LHOTB1:
	.p2align 4
	.globl	_ZN6onejit3mem13realloc_bytesEPvm
	.type	_ZN6onejit3mem13realloc_bytesEPvm, @function
_ZN6onejit3mem13realloc_bytesEPvm:
.LFB101:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	testq	%rdi, %rdi
	je	.L22
	testq	%rsi, %rsi
	je	.L23
	call	realloc@PLT
	testq	%rax, %rax
	je	.L24
.L12:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L22:
	.cfi_restore_state
	testq	%rsi, %rsi
	je	.L17
	movq	%rsi, %rdi
	call	malloc@PLT
	testq	%rax, %rax
	je	.L16
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L23:
	.cfi_restore_state
	call	free@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L17:
	.cfi_restore_state
	xorl	%eax, %eax
	jmp	.L12
.L24:
	jmp	.L16
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.type	_ZN6onejit3mem13realloc_bytesEPvm.cold, @function
_ZN6onejit3mem13realloc_bytesEPvm.cold:
.LFSB101:
.L16:
	.cfi_def_cfa_offset 16
	call	_ZN6onejit3memL15throw_bad_allocEv
	.cfi_endproc
.LFE101:
	.text
	.size	_ZN6onejit3mem13realloc_bytesEPvm, .-_ZN6onejit3mem13realloc_bytesEPvm
	.section	.text.unlikely
	.size	_ZN6onejit3mem13realloc_bytesEPvm.cold, .-_ZN6onejit3mem13realloc_bytesEPvm.cold
.LCOLDE1:
	.text
.LHOTE1:
	.p2align 4
	.globl	_ZN6onejit3mem10free_bytesEPv
	.type	_ZN6onejit3mem10free_bytesEPv, @function
_ZN6onejit3mem10free_bytesEPv:
.LFB102:
	.cfi_startproc
	testq	%rdi, %rdi
	je	.L25
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L25:
	ret
	.cfi_endproc
.LFE102:
	.size	_ZN6onejit3mem10free_bytesEPv, .-_ZN6onejit3mem10free_bytesEPv
	.ident	"GCC: (Debian 10.2.1-1) 10.2.1 20201207"
	.section	.note.GNU-stack,"",@progbits
