
_fload_store.o:     file format elf64-littleaarch64


Disassembly of section .text:

0000000000000000 <fload32>:
   0:	bd400000 	ldr	s0, [x0]
   4:	bd400001 	ldr	s1, [x0]
   8:	bd400002 	ldr	s2, [x0]
   c:	bd40000f 	ldr	s15, [x0]
  10:	bd40001f 	ldr	s31, [x0]
  14:	bd400000 	ldr	s0, [x0]
  18:	bd400020 	ldr	s0, [x1]
  1c:	bd400040 	ldr	s0, [x2]
  20:	bd4001e0 	ldr	s0, [x15]
  24:	bd4003e0 	ldr	s0, [sp]
  28:	d503201f 	nop
  2c:	bd4003ff 	ldr	s31, [sp]
  30:	d65f03c0 	ret

0000000000000034 <fload64>:
  34:	fd400000 	ldr	d0, [x0]
  38:	fd400001 	ldr	d1, [x0]
  3c:	fd400002 	ldr	d2, [x0]
  40:	fd40000f 	ldr	d15, [x0]
  44:	fd40001f 	ldr	d31, [x0]
  48:	fd400000 	ldr	d0, [x0]
  4c:	fd400020 	ldr	d0, [x1]
  50:	fd400040 	ldr	d0, [x2]
  54:	fd4001e0 	ldr	d0, [x15]
  58:	fd4003e0 	ldr	d0, [sp]
  5c:	d503201f 	nop
  60:	fd4003ff 	ldr	d31, [sp]
  64:	d65f03c0 	ret

0000000000000068 <fstore32>:
  68:	bd000000 	str	s0, [x0]
  6c:	bd000001 	str	s1, [x0]
  70:	bd000002 	str	s2, [x0]
  74:	bd00000f 	str	s15, [x0]
  78:	bd00001f 	str	s31, [x0]
  7c:	bd000000 	str	s0, [x0]
  80:	bd000020 	str	s0, [x1]
  84:	bd000040 	str	s0, [x2]
  88:	bd0001e0 	str	s0, [x15]
  8c:	bd0003e0 	str	s0, [sp]
  90:	d503201f 	nop
  94:	bd0003ff 	str	s31, [sp]
  98:	d65f03c0 	ret

000000000000009c <fstore64>:
  9c:	fd000000 	str	d0, [x0]
  a0:	fd000001 	str	d1, [x0]
  a4:	fd000002 	str	d2, [x0]
  a8:	fd00000f 	str	d15, [x0]
  ac:	fd00001f 	str	d31, [x0]
  b0:	fd000000 	str	d0, [x0]
  b4:	fd000020 	str	d0, [x1]
  b8:	fd000040 	str	d0, [x2]
  bc:	fd0001e0 	str	d0, [x15]
  c0:	fd0003e0 	str	d0, [sp]
  c4:	d503201f 	nop
  c8:	fd0003ff 	str	d31, [sp]
  cc:	d65f03c0 	ret
