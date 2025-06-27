# The CPEN211 Backend 

This is the dead simple backend of CPEN211 ISA:

![CPEN211 ISA](ISA.png)

The entire instruction set is basically self explanatory. 

## Counterintuitive Behaviors

- `long` is 16 bit. Almost everything is 16 bit.

## Bugs

- Currently, signed and unsigned comparison is just incorrect. This is because we pretend that the
ISD::SETLT and ISD::SETULT are the same thing.
- Moving a really large 16 bit constant. This requires ISA changes to be done so effectively. 
- Char doesn't work, because it is hard coded to be 8 bits.

# TODO 

## Instructions

General guidance  and necessarily accurate because some llvm instruction gets transformed differently  into selection DAG, so we don't have ISD::*. One such example is GEP.

## Priority I
- [x] ret 
- [x] br
- [x] icmp 
- [x] add
- [x] sub
- [X] mul
- [X] udiv
- [X] sdiv
- [X] urem
- [X] srem
- [x] ashr
- [x] shl
- [x] lshr
- [x] and
- [x] or
- [x] load
- [x] store
- [x] xor
- [x] select
- [x] phi
- [x] call

## Priority II

- [ ] switch
- [ ] invoke
- [ ] callbr
- [ ] resume 
- [ ] catchswitch
- [ ] catchret
- [ ] cleanupret
- [ ] unreachable
- [ ] zext
- [ ] sext
- [ ] fsub
- [ ] fadd
- [ ] fmul
- [ ] fdiv
- [ ] frem
- [ ] trunc
- [ ] fptrunc
- [ ] fpext
- [ ] ptoui
- [ ] fptosi
- [ ] uitofp
- [ ] sitofp
- [ ] ptrtoint
- [ ] inttoptr
- [ ] bitcast
- [ ] addrspacecast
- [ ] fcmp
- [ ] freeze
- [ ] va_arg
- [ ] landingpad
- [ ] catchpad
- [ ] cleanuppad
- [ ] fneg

## General
- [ ] Implement PC Relative 16 Jump as PSEUDO Instruction. The only way to do this as of current is for the linker to encode the absolute address!
- [ ] Implement peeophole for MachineBasicBlock and the following: ```B some_label some_lable:...```

## Calling Conventions Custom (C ABI)

Here we are going to define the calling convention:

**R7** is link register, **R6** is stack pointer, and **R5** is frame pointer. 

Argument register are **R0**, and **R1**. 

Return happens at R0.

Additional arguments are passed on the stack in reverse order. 

## Example

```
int add(int a, int b, int c, int d){
    return a + b + c;
}
```


At the beginning of the add, it is the caller's responsibility to lower the stack frame into the following:  

```
R0  :       int a
R1  :       int b
FP-2:       int d
FP-1:       int c
FP  :       R7 (Link Register)
```
## Example code/Example Output

This is the code
```c
int add_sum(int n) {
  int result = 0;
  for (int i = 0; i < n; ++i) {
    result += i;
  }

  return result;
}
```

This is the code after

```
        .file   "main.ll"
        .text
        .globl  add_sum                         ; -- Begin function add_sum
        .type   add_sum,@function
add_sum:                                ; @add_sum
        .cfi_startproc
; %bb.0:
        STR     R0, [R6,2]
        MOV     R0, #0
        STR     R0, [R6,1]
        STR     R0, [R6,0]
        MOV     R0, #1
.LBB0_1:                                ; =>This Inner Loop Header: Depth=1
        LDR     R1, [R6,0]
        LDR     R2, [R6,2]
        CMP     R2, R1
        BLE     .LBB0_4
        B       .LBB0_2
.LBB0_2:                                ;   in Loop: Header=BB0_1 Depth=1
        LDR     R1, [R6,1]
        LDR     R2, [R6,0]
        ADD     R1, R1 , R2
        STR     R1, [R6,1]
; %bb.3:                                ;   in Loop: Header=BB0_1 Depth=1
        LDR     R1, [R6,0]
        ADD     R1, R1 , R0
        STR     R1, [R6,0]
        B       .LBB0_1
.LBB0_4:
        LDR     R0, [R6,1]
        BX      R7
.Lfunc_end0:
        .size   add_sum, .Lfunc_end0-add_sum
        .cfi_endproc
                                        ; -- End function
        .section        ".note.GNU-stack","",@progbits
```

Recursive FIB: 

```
int fib(int a){
    if(a == 1 || a == 2){
        return a;
    }
    return fib(a - 1) + fib(a -2);
}
```

```
        .file   "main.c"
        .text
        .globl  fib                             ; -- Begin function fib
        .type   fib,@function
fib:                                    ; @fib
; %bb.0:                                ; %entry
        MOV     R4, #-5
        ADD     R6, R4, R6
        STR     R7, [R6, #3]                    ; 2-byte Folded Spill
        STR     R3, [R6, #4]                    ; 2-byte Folded Spill
        STR     R0, [R6, #1]
        LDR     R0, [R6, #1]
        MOV     R4, #1
        CMP     R0, R4
        BEQ     .LBB0_2
        B       .LBB0_1
.LBB0_1:                                ; %lor.lhs.false
        LDR     R0, [R6, #1]
        MOV     R4, #2
        CMP     R0, R4
        BNE     .LBB0_3
        B       .LBB0_2
.LBB0_2:                                ; %if.then
        LDR     R0, [R6, #1]
        STR     R0, [R6, #2]
        B       .LBB0_4
.LBB0_3:                                ; %if.end
        LDR     R0, [R6, #1]
        MOV     R4, #-1
        ADD     R0, R4, R0
        BL      #fib
        STR     R0, [R6, #0]                    ; 2-byte Folded Spill
        LDR     R0, [R6, #1]
        MOV     R4, #-2
        ADD     R0, R4, R0
        BL      #fib
        LDR     R1, [R6, #0]                    ; 2-byte Folded Reload
        ADD     R0, R1, R0
        STR     R0, [R6, #2]
        B       .LBB0_4
.LBB0_4:                                ; %return
        LDR     R0, [R6, #2]
        LDR     R7, [R6, #3]                    ; 2-byte Folded Reload
        LDR     R3, [R6, #4]                    ; 2-byte Folded Reload
        MOV     R4, #10
        ADD     R6, R4, R6
        BX      R7
.Lfunc_end0:
        .size   fib, .Lfunc_end0-fib
                                        ; -- End function
        .ident  "clang version 20.0.0git (git@github.com:Mr-Anyone/llvm-project.git df29327afaebbbf1f726d506b20a845f1163088a)"
        .section        ".note.GNU-stack","",@progbits
```
