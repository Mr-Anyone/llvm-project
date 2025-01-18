# The CPEN211 Backend 

This is the dead simple backend of CPEN211 ISA:

[CPEN211 ISA](ISA.png)

The entire instruction set is self explanatory

# Calling Conventions

Here we are going to define the calling convention: 

**R7** is link register, **R6** is stack pointer.

Callee saved: 
- R4
- R5
- R6
- R7
