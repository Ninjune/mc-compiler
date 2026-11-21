Make sure to run tests after 
make sure to disable graphviz
make sure to detailed errors off

plan out part 4 before coding it
will have to translate what comes out to the next stage

Part 5:

generated pseudocode (queue of strings?)

->

patterns: match the most IR code to risc v code (greedy)

call stack 
need to track in memory variables, store old values work with new ones

for 8 vars: might need to store result in memory/one of the vars for the result
to be a register

stack frame stores:
- spilled variables
- arameters
- return address to resume after the call
- non-local variables
- local arrays and records

stack grows downward (to lower addresses) and SP points to the last used word

https://shd.mit.edu/2024/recitations/riscv.html

M: risc-v method
framesize_f: f's frame size 

### Prelogue to function call

LABEL f p1 := M[SP+8
pm := M[SP+8*m]
SP := SP - framesize_f

### epilogue to function call 

SP := SP + framesize_f
M[SP+8] := result
GOTO M[SP] 

see mini c slides should cover it
