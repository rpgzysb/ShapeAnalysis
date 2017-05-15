; ModuleID = './Testcase/insert.ll'
source_filename = "./Testcase/insert.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.node = type { %struct.node*, i32 }

; Function Attrs: noinline nounwind uwtable
define void @insert(%struct.node* %arg, i32 %arg1) #0 {
bb:
  br label %bb2

bb2:                                              ; preds = %bb5, %bb
  %.0 = phi %struct.node* [ %arg, %bb ], [ %tmp7, %bb5 ]
  %tmp = getelementptr inbounds %struct.node, %struct.node* %.0, i32 0, i32 0
  %tmp3 = load %struct.node*, %struct.node** %tmp, align 8
  %tmp4 = icmp ne %struct.node* %tmp3, null
  br i1 %tmp4, label %bb5, label %bb8

bb5:                                              ; preds = %bb2
  %tmp6 = getelementptr inbounds %struct.node, %struct.node* %.0, i32 0, i32 0
  %tmp7 = load %struct.node*, %struct.node** %tmp6, align 8
  br label %bb2

bb8:                                              ; preds = %bb2
  %tmp9 = call noalias i8* @malloc(i64 8) #2
  %tmp10 = bitcast i8* %tmp9 to %struct.node*
  %tmp11 = getelementptr inbounds %struct.node, %struct.node* %tmp10, i32 0, i32 1
  store i32 %arg1, i32* %tmp11, align 8
  %tmp12 = getelementptr inbounds %struct.node, %struct.node* %.0, i32 0, i32 0
  %tmp13 = load %struct.node*, %struct.node** %tmp12, align 8
  %tmp14 = getelementptr inbounds %struct.node, %struct.node* %tmp10, i32 0, i32 0
  store %struct.node* %tmp13, %struct.node** %tmp14, align 8
  %tmp15 = getelementptr inbounds %struct.node, %struct.node* %.0, i32 0, i32 0
  store %struct.node* %tmp10, %struct.node** %tmp15, align 8
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 5.0.0 (trunk 299827)"}
