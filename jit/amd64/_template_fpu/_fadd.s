
_fadd.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <add(fnum, fnum)>:
   0:	66 0f 6e c7          	movd   %edi,%xmm0
   4:	66 0f 6e ce          	movd   %esi,%xmm1
   8:	f3 0f 58 c1          	addss  %xmm1,%xmm0
   c:	66 0f 7e c0          	movd   %xmm0,%eax
  10:	c3                   	retq   
  11:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  18:	00 00 00 00 
  1c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000020 <add(fnum&, fnum&)>:
  20:	f3 0f 10 07          	movss  (%rdi),%xmm0
  24:	f3 0f 58 06          	addss  (%rsi),%xmm0
  28:	66 0f 7e c0          	movd   %xmm0,%eax
  2c:	c3                   	retq   
  2d:	0f 1f 00             	nopl   (%rax)

0000000000000030 <add(dnum, dnum)>:
  30:	66 48 0f 6e c7       	movq   %rdi,%xmm0
  35:	66 48 0f 6e ce       	movq   %rsi,%xmm1
  3a:	f2 0f 58 c1          	addsd  %xmm1,%xmm0
  3e:	66 48 0f 7e c0       	movq   %xmm0,%rax
  43:	c3                   	retq   
  44:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  4b:	00 00 00 00 
  4f:	90                   	nop

0000000000000050 <add(dnum&, dnum&)>:
  50:	f2 0f 10 07          	movsd  (%rdi),%xmm0
  54:	f2 0f 58 06          	addsd  (%rsi),%xmm0
  58:	66 48 0f 7e c0       	movq   %xmm0,%rax
  5d:	c3                   	retq   
