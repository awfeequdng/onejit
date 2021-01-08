	.text
	.file	"_ldr.cpp"

	.globl	fload32
	.p2align	2
	.type	fload32,@function
fload32:
	ldr	s0, [x0]
	ldr	s1, [x0]
	ldr	s2, [x0]
	ldr	s15, [x0]
	ldr	s31, [x0]
	ldr	s0, [x0]
	ldr	s0, [x1]
	ldr	s0, [x2]
	ldr	s0, [x15]
	ldr	s0, [sp]
	nop
	ldr	s31, [sp]
	ret
.Lload32_end:
	.size	fload32, .Lload32_end-fload32


	.globl	fload64
	.p2align	2
	.type	fload64,@function
fload64:
	ldr	d0, [x0]
	ldr	d1, [x0]
	ldr	d2, [x0]
	ldr	d15, [x0]
	ldr	d31, [x0]
	ldr	d0, [x0]
	ldr	d0, [x1]
	ldr	d0, [x2]
	ldr	d0, [x15]
	ldr	d0, [sp]
	nop
	ldr	d31, [sp]
	ret
.Lfload64_end:
	.size	fload64, .Lfload64_end-fload64



	.globl	fstore32
	.p2align	2
	.type	fstore32,@function
fstore32:
	str	s0, [x0]
	str	s1, [x0]
	str	s2, [x0]
	str	s15, [x0]
	str	s31, [x0]
	str	s0, [x0]
	str	s0, [x1]
	str	s0, [x2]
	str	s0, [x15]
	str	s0, [sp]
	nop
	str	s31, [sp]
	ret
.Lstore32_end:
	.size	fstore32, .Lstore32_end-fstore32


	.globl	fstore64
	.p2align	2
	.type	fstore64,@function
fstore64:
	str	d0, [x0]
	str	d1, [x0]
	str	d2, [x0]
	str	d15, [x0]
	str	d31, [x0]
	str	d0, [x0]
	str	d0, [x1]
	str	d0, [x2]
	str	d0, [x15]
	str	d0, [sp]
	nop
	str	d31, [sp]
	ret
.Lfstore64_end:
	.size	fstore64, .Lfstore64_end-fstore64









	.section	".note.GNU-stack","",@progbits
