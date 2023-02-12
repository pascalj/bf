# bf
An ongoing experiment to build a simple MLIR dialect for Brainfuck.

The plan is to build:

1. A C interpreter for Brainfuck programs. It works, but must be implemented pretty naively, since it handles loops token by token.
2. A "flat" LLVM IR variant that is very similar to the C interpreter. It will probably perform not very well, but should work. It *might* use the same "stack" mechanic to handle loops, rather than proper labels.
3. A more sophisticated AST version should allow a much better IR and optimization possibilities.
4. An own MLIR dialect for brainfuck could express high-level constructs like loops that might be lifted and at least theoretically allow much better analysis of the data flow.

It's just a project for me to learn and have fun 8)
