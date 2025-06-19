; RUN: llc < %s | FileCheck %s

target triple = "cpen211"

define dso_local void @void_function() #0 {
entry:
  ret void
}
; CHECK: void_function:

define dso_local i16 @fib(i16 noundef %a) #0 {
entry:
  %retval = alloca i16, align 2
  %a.addr = alloca i16, align 2
  store i16 %a, ptr %a.addr, align 2
  %0 = load i16, ptr %a.addr, align 2
  %cmp = icmp eq i16 %0, 1
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %entry
  %1 = load i16, ptr %a.addr, align 2
  %cmp1 = icmp eq i16 %1, 0
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %lor.lhs.false, %entry
  %2 = load i16, ptr %a.addr, align 2
  store i16 %2, ptr %retval, align 2
  br label %return

if.end:                                           ; preds = %lor.lhs.false
  %3 = load i16, ptr %a.addr, align 2
  %sub = sub nsw i16 %3, 1
  %call = call i16 @fib(i16 noundef %sub)
  %4 = load i16, ptr %a.addr, align 2
  %sub2 = sub nsw i16 %4, 2
  %call3 = call i16 @fib(i16 noundef %sub2)
  %add = add nsw i16 %call, %call3
  store i16 %add, ptr %retval, align 2
  br label %return

return:                                           ; preds = %if.end, %if.then
  %5 = load i16, ptr %retval, align 2
  ret i16 %5
}

; CHECK: fib:
define dso_local i16 @fib_two(i16 noundef %a) local_unnamed_addr #0 {
entry:
  %or.cond8 = icmp ult i16 %a, 2
  br i1 %or.cond8, label %return, label %if.end

if.end:                                           ; preds = %entry, %if.end
  %a.tr10 = phi i16 [ %sub2, %if.end ], [ %a, %entry ]
  %accumulator.tr9 = phi i16 [ %add, %if.end ], [ 0, %entry ]
  %sub = add nsw i16 %a.tr10, -1
  %call = tail call i16 @fib(i16 noundef %sub)
  %sub2 = add nsw i16 %a.tr10, -2
  %add = add nsw i16 %call, %accumulator.tr9
  %or.cond = icmp ult i16 %sub2, 2
  br i1 %or.cond, label %return, label %if.end

return:                                           ; preds = %if.end, %entry
  %accumulator.tr.lcssa = phi i16 [ 0, %entry ], [ %add, %if.end ]
  %a.tr.lcssa = phi i16 [ %a, %entry ], [ %sub2, %if.end ]
  %accumulator.ret.tr = add nsw i16 %a.tr.lcssa, %accumulator.tr.lcssa
  ret i16 %accumulator.ret.tr
}
; CHECK: fib_two:

define dso_local i16 @logical_shift_left_one(i16 %0){
main:
    %2  = shl i16 %0, 1
    ret i16 %2
}
; CHECK: logical_shift_left_one:

define dso_local i16 @logical_shift_right_one(i16 %0){
main:
    %2  = lshr i16 %0, 1
    ret i16 %2
}
; CHECK: logical_shift_right_one:

define dso_local i16 @arithmetic_shift_right_one(i16 %0){
main:
    %2  = ashr i16 %0, 1
    ret i16 %2
}
; CHECK: arithmetic_shift_right_one:
