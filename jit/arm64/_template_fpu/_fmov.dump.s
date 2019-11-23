
_fmov.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <fmov>:
   0:	1e270000 	fmov	s0, w0
   4:	1e270001 	fmov	s1, w0
   8:	1e270002 	fmov	s2, w0
   c:	1e27000f 	fmov	s15, w0
  10:	1e27001f 	fmov	s31, w0
  14:	1e270020 	fmov	s0, w1
  18:	1e270040 	fmov	s0, w2
  1c:	1e2701e0 	fmov	s0, w15
  20:	1e2703e0 	fmov	s0, wzr
  24:	1e2703ff 	fmov	s31, wzr
  28:	1e212800 	fadd	s0, s0, s1
  2c:	1e260000 	fmov	w0, s0
  30:	1e260020 	fmov	w0, s1
  34:	1e260040 	fmov	w0, s2
  38:	1e2601e0 	fmov	w0, s15
  3c:	1e2603e0 	fmov	w0, s31
  40:	1e260000 	fmov	w0, s0
  44:	1e260001 	fmov	w1, s0
  48:	1e260002 	fmov	w2, s0
  4c:	1e26000f 	fmov	w15, s0
  50:	1e26001f 	fmov	wzr, s0
  54:	1e2603ff 	fmov	wzr, s31
  58:	d65f03c0 	ret
