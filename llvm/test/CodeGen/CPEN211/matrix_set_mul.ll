; RUN: llc < %s | FileCheck %s

source_filename = "main.c"
target datalayout = "e-p:16:16-i16:16"
target triple = "cpen211"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write)
define dso_local void @set_matrix(ptr nocapture noundef writeonly %a, i16 noundef %val) local_unnamed_addr #0 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.cond.cleanup3
  %i.016 = phi i16 [ 0, %entry ], [ %inc7, %for.cond.cleanup3 ]
  br label %for.body4

for.cond.cleanup:                                 ; preds = %for.cond.cleanup3
  ret void

for.cond.cleanup3:                                ; preds = %for.body4
  %inc7 = add nuw nsw i16 %i.016, 1
  %cmp = icmp samesign ult i16 %i.016, 9
  br i1 %cmp, label %for.cond1.preheader, label %for.cond.cleanup, !llvm.loop !3

for.body4:                                        ; preds = %for.cond1.preheader, %for.body4
  %j.015 = phi i16 [ 0, %for.cond1.preheader ], [ %inc, %for.body4 ]
  %add = add nuw nsw i16 %j.015, %i.016
  %arrayidx5 = getelementptr inbounds nuw [10 x i16], ptr %a, i16 %i.016, i16 %j.015
  store i16 %add, ptr %arrayidx5, align 2, !tbaa !6
  %inc = add nuw nsw i16 %j.015, 1
  %cmp2 = icmp samesign ult i16 %j.015, 9
  br i1 %cmp2, label %for.body4, label %for.cond.cleanup3, !llvm.loop !10
}
; CHECK-LABEL: set_matrix:

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite)
define dso_local void @matrix_2d_add(ptr nocapture noundef writeonly %a, ptr nocapture noundef readonly %b, ptr nocapture noundef readonly %c) local_unnamed_addr #1 {
entry:
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.cond.cleanup3
  %i.022 = phi i16 [ 0, %entry ], [ %inc11, %for.cond.cleanup3 ]
  br label %for.body4

for.cond.cleanup:                                 ; preds = %for.cond.cleanup3
  ret void

for.cond.cleanup3:                                ; preds = %for.body4
  %inc11 = add nuw nsw i16 %i.022, 1
  %cmp = icmp samesign ult i16 %i.022, 9
  br i1 %cmp, label %for.cond1.preheader, label %for.cond.cleanup, !llvm.loop !11

for.body4:                                        ; preds = %for.cond1.preheader, %for.body4
  %j.021 = phi i16 [ 0, %for.cond1.preheader ], [ %inc, %for.body4 ]
  %arrayidx5 = getelementptr inbounds nuw [10 x i16], ptr %b, i16 %i.022, i16 %j.021
  %0 = load i16, ptr %arrayidx5, align 2, !tbaa !6
  %arrayidx7 = getelementptr inbounds nuw [10 x i16], ptr %c, i16 %i.022, i16 %j.021
  %1 = load i16, ptr %arrayidx7, align 2, !tbaa !6
  %add = add nsw i16 %1, %0
  %arrayidx9 = getelementptr inbounds nuw [10 x i16], ptr %a, i16 %i.022, i16 %j.021
  store i16 %add, ptr %arrayidx9, align 2, !tbaa !6
  %inc = add nuw nsw i16 %j.021, 1
  %cmp2 = icmp samesign ult i16 %j.021, 9
  br i1 %cmp2, label %for.body4, label %for.cond.cleanup3, !llvm.loop !12
}

; CHECK-LABEL: matrix_2d_add:

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local noundef i16 @main() local_unnamed_addr #2 {
entry:
  ret i16 0
}

; CHECK-LABEL: main

attributes #0 = { nofree norecurse nosync nounwind memory(argmem: write) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { nofree norecurse nosync nounwind memory(argmem: readwrite) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 20.0.0git (git@github.com:Mr-Anyone/llvm-project.git 939e19a25a0138c9cff9be8ef77caf0a50e4a71e)"}
!3 = distinct !{!3, !4, !5}
!4 = !{!"llvm.loop.mustprogress"}
!5 = !{!"llvm.loop.unroll.disable"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !4, !5}
!11 = distinct !{!11, !4, !5}
!12 = distinct !{!12, !4, !5}

