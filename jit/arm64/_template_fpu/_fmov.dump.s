
_fmov.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <fmovs>:
   0:	1e204000 	fmov	s0, s0
   4:	1e204001 	fmov	s1, s0
   8:	1e204002 	fmov	s2, s0
   c:	1e20400f 	fmov	s15, s0
  10:	1e20401f 	fmov	s31, s0
  14:	1e204000 	fmov	s0, s0
  18:	1e204020 	fmov	s0, s1
  1c:	1e204040 	fmov	s0, s2
  20:	1e2041e0 	fmov	s0, s15
  24:	1e2043e0 	fmov	s0, s31
  28:	d503201f 	nop
  2c:	1e270000 	fmov	s0, w0
  30:	1e270001 	fmov	s1, w0
  34:	1e270002 	fmov	s2, w0
  38:	1e27000f 	fmov	s15, w0
  3c:	1e27001f 	fmov	s31, w0
  40:	1e270020 	fmov	s0, w1
  44:	1e270040 	fmov	s0, w2
  48:	1e2701e0 	fmov	s0, w15
  4c:	1e2703e0 	fmov	s0, wzr
  50:	1e2703ff 	fmov	s31, wzr
  54:	d503201f 	nop
  58:	1e260000 	fmov	w0, s0
  5c:	1e260020 	fmov	w0, s1
  60:	1e260040 	fmov	w0, s2
  64:	1e2601e0 	fmov	w0, s15
  68:	1e2603e0 	fmov	w0, s31
  6c:	1e260000 	fmov	w0, s0
  70:	1e260001 	fmov	w1, s0
  74:	1e260002 	fmov	w2, s0
  78:	1e26000f 	fmov	w15, s0
  7c:	1e26001f 	fmov	wzr, s0
  80:	1e2603ff 	fmov	wzr, s31
  84:	d65f03c0 	ret

0000000000000088 <fmovd>:
  88:	1e604000 	fmov	d0, d0
  8c:	1e604001 	fmov	d1, d0
  90:	1e604002 	fmov	d2, d0
  94:	1e60400f 	fmov	d15, d0
  98:	1e60401f 	fmov	d31, d0
  9c:	1e604000 	fmov	d0, d0
  a0:	1e604020 	fmov	d0, d1
  a4:	1e604040 	fmov	d0, d2
  a8:	1e6041e0 	fmov	d0, d15
  ac:	1e6043e0 	fmov	d0, d31
  b0:	d503201f 	nop
  b4:	9e670000 	fmov	d0, x0
  b8:	9e670001 	fmov	d1, x0
  bc:	9e670002 	fmov	d2, x0
  c0:	9e67000f 	fmov	d15, x0
  c4:	9e67001f 	fmov	d31, x0
  c8:	9e670020 	fmov	d0, x1
  cc:	9e670040 	fmov	d0, x2
  d0:	9e6701e0 	fmov	d0, x15
  d4:	9e6703e0 	fmov	d0, xzr
  d8:	9e6703ff 	fmov	d31, xzr
  dc:	d503201f 	nop
  e0:	9e660000 	fmov	x0, d0
  e4:	9e660020 	fmov	x0, d1
  e8:	9e660040 	fmov	x0, d2
  ec:	9e6601e0 	fmov	x0, d15
  f0:	9e6603e0 	fmov	x0, d31
  f4:	9e660000 	fmov	x0, d0
  f8:	9e660001 	fmov	x1, d0
  fc:	9e660002 	fmov	x2, d0
 100:	9e66000f 	fmov	x15, d0
 104:	9e66001f 	fmov	xzr, d0
 108:	9e6603ff 	fmov	xzr, d31
 10c:	d65f03c0 	ret
