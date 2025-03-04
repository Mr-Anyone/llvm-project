# The CPEN211 Backend 

This is the dead simple backend of CPEN211 ISA:

[CPEN211 ISA](ISA.png)

The entire instruction set is self explanatory

# Calling Conventions

Here we are going to define the calling convention: 

**R7** is link register, **R6** is stack pointer.

Argument register are R0, and R1.

Return happens at R0.

```
int add(int a, int b, int c, int d){
    return a + b + c;
}
```

Would be that the stack frame would look something like this!
Note: variable must be pushed in reverse order to make sense!
```
R0:         int a
R1:         int b
SP  :       int d
SP+1:       int c
```
