
_fadd.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <add(fnum, fnum)>:
   0:	1e270000 	fmov	s0, w0
   4:	1e270021 	fmov	s1, w1
   8:	1e212800 	fadd	s0, s0, s1
   c:	1e260000 	fmov	w0, s0
  10:	d65f03c0 	ret
  14:	d503201f 	nop

0000000000000018 <add(fnum&, fnum&, fnum&)>:
  18:	bd400000 	ldr	s0, [x0]
  1c:	bd400021 	ldr	s1, [x1]
  20:	1e212800 	fadd	s0, s0, s1
  24:	bd000040 	str	s0, [x2]
  28:	d65f03c0 	ret
  2c:	d503201f 	nop

0000000000000030 <add(dnum, dnum)>:
  30:	9e670000 	fmov	d0, x0
  34:	9e670021 	fmov	d1, x1
  38:	1e612800 	fadd	d0, d0, d1
  3c:	9e660000 	fmov	x0, d0
  40:	d65f03c0 	ret
  44:	d503201f 	nop

0000000000000048 <add(dnum&, dnum&, dnum&)>:
  48:	fd400000 	ldr	d0, [x0]
  4c:	fd400021 	ldr	d1, [x1]
  50:	1e612800 	fadd	d0, d0, d1
  54:	fd000040 	str	d0, [x2]
  58:	d65f03c0 	ret
