	.text
	.file	"_fmov.cpp"

	.globl	fmov
	.p2align	2
	.type	fmov,@function
fmov:
	fmov	s0, w0
	fmov	s1, w0
	fmov	s2, w0
	fmov	s15, w0
	fmov	s31, w0
	fmov	s0, w1
	fmov	s0, w2
	fmov	s0, w15
	fmov	s0, w31
	fmov	s31, w31
	fadd	s0, s0, s1
	fmov	w0, s0
	fmov	w0, s1
	fmov	w0, s2
	fmov	w0, s15
	fmov	w0, s31
	fmov	w0, s0
	fmov	w1, s0
	fmov	w2, s0
	fmov	w15, s0
	fmov	w31, s0
	fmov	w31, s31
	ret
.Lfunc_end0:
	.size	fmov, .Lfunc_end0-fmov

	.section	".note.GNU-stack","",@progbits
