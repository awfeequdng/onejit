
_get_set_idx.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <get(float*, unsigned long)>:
   0:	bc617800 	ldr	s0, [x0, x1, lsl #2]
   4:	d65f03c0 	ret

0000000000000008 <set(float*, unsigned long, float)>:
   8:	bc217800 	str	s0, [x0, x1, lsl #2]
   c:	d65f03c0 	ret

0000000000000010 <get_c1(float*)>:
  10:	bd408000 	ldr	s0, [x0, #128]
  14:	d65f03c0 	ret

0000000000000018 <get_c2(float*)>:
  18:	bd7ffc00 	ldr	s0, [x0, #16380]
  1c:	d65f03c0 	ret

0000000000000020 <set_c1(float*, float)>:
  20:	bd000400 	str	s0, [x0, #4]
  24:	d65f03c0 	ret

0000000000000028 <set_c2(float*, float)>:
  28:	bd008000 	str	s0, [x0, #128]
  2c:	d65f03c0 	ret

0000000000000030 <set_c3(float*, float)>:
  30:	bd3ffc00 	str	s0, [x0, #16380]
  34:	d65f03c0 	ret

0000000000000038 <set_c4(float*, float)>:
  38:	91401000 	add	x0, x0, #0x4, lsl #12
  3c:	bd3ffc00 	str	s0, [x0, #16380]
  40:	d65f03c0 	ret
  44:	d503201f 	nop

0000000000000048 <get(double*, unsigned long)>:
  48:	fc617800 	ldr	d0, [x0, x1, lsl #3]
  4c:	d65f03c0 	ret

0000000000000050 <set(double*, unsigned long, double)>:
  50:	fc217800 	str	d0, [x0, x1, lsl #3]
  54:	d65f03c0 	ret

0000000000000058 <get_c1(double*)>:
  58:	fd408000 	ldr	d0, [x0, #256]
  5c:	d65f03c0 	ret

0000000000000060 <get_c2(double*)>:
  60:	fd7ffc00 	ldr	d0, [x0, #32760]
  64:	d65f03c0 	ret

0000000000000068 <set_c1(double*, double)>:
  68:	fd000400 	str	d0, [x0, #8]
  6c:	d65f03c0 	ret

0000000000000070 <set_c2(double*, double)>:
  70:	fd008000 	str	d0, [x0, #256]
  74:	d65f03c0 	ret

0000000000000078 <set_c3(double*, double)>:
  78:	fd3ffc00 	str	d0, [x0, #32760]
  7c:	d65f03c0 	ret

0000000000000080 <set_c4(double*, double)>:
  80:	91402000 	add	x0, x0, #0x8, lsl #12
  84:	fd3ffc00 	str	d0, [x0, #32760]
  88:	d65f03c0 	ret
