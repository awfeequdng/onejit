# onejit - Go just-in-time compiler

[PRE-ALPHA] Onejit is a Go just-in-time compiler being written from scratch.
It generates native code in memory.

## current status

- lexer:   100% complete, including generics
- parser:  100% complete, including generics
- types:   90% almost complete. missing hashing/equality. preliminary support for generics.
- typecheck: 20% work in progress
- IR code generation: 0% to be implemented
- IR code representation: 90% almost complete (written in C++)
- IR code optimization: 0% to be implemented
- register allocation: 80% almost complete (written in C++)
- native code generation: 10% work in progress on x86_64 (written in C++), other backends to be written
