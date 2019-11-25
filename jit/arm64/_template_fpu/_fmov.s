	.text
	.file	"_fmov.cpp"

	.globl	fmovs
	.p2align	2
	.type	fmovs,@function
fmovs:
	fmov	s0, s0
	fmov	s1, s0
	fmov	s2, s0
	fmov	s15, s0
	fmov	s31, s0
	fmov	s0, s0
	fmov	s0, s1
	fmov	s0, s2
	fmov	s0, s15
	fmov	s0, s31
	nop
	fmov	s0, w0
	fmov	s1, w0
	fmov	s2, w0
	fmov	s15, w0
	fmov	s31, w0
	fmov	s0, w1
	fmov	s0, w2
	fmov	s0, w15
	fmov	s0, wzr
	fmov	s31, wzr
	nop
	fmov	w0, s0
	fmov	w0, s1
	fmov	w0, s2
	fmov	w0, s15
	fmov	w0, s31
	fmov	w0, s0
	fmov	w1, s0
	fmov	w2, s0
	fmov	w15, s0
	fmov	wzr, s0
	fmov	wzr, s31
	ret
.Lfmovs_end:
	.size	fmovs, .Lfmovs_end-fmovs


	.globl	fmovd
	.p2align	2
	.type	fmovd,@function
fmovd:
	fmov	d0, d0
	fmov	d1, d0
	fmov	d2, d0
	fmov	d15, d0
	fmov	d31, d0
	fmov	d0, d0
	fmov	d0, d1
	fmov	d0, d2
	fmov	d0, d15
	fmov	d0, d31
	nop
	fmov	d0, x0
	fmov	d1, x0
	fmov	d2, x0
	fmov	d15, x0
	fmov	d31, x0
	fmov	d0, x1
	fmov	d0, x2
	fmov	d0, x15
	fmov	d0, xzr
	fmov	d31, xzr
	nop
	fmov	x0, d0
	fmov	x0, d1
	fmov	x0, d2
	fmov	x0, d15
	fmov	x0, d31
	fmov	x0, d0
	fmov	x1, d0
	fmov	x2, d0
	fmov	x15, d0
	fmov	xzr, d0
	fmov	xzr, d31
	ret
.Lfmovd_end:
	.size	fmovd, .Lfmovd_end-fmovd

	.section	".note.GNU-stack","",@progbits
