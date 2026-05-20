.686P
.model flat, c
public computeAverageScore
printf proto c :ptr sbyte, :vararg
includelib  libcmt.lib
includelib  legacy_stdio_definitions.lib 

student  struct
    sname   db   8 dup(0)
    sid     db   11 dup(0)
    align   2    ; 指明对齐方式，汇编语言默认是紧凑存放
    scores  dw   8  dup(0)
    average dw   0
student   ends

.code
; 计算学生平均分
; sptr: 学生数组的首地址
; num:  学生人数

computeAverageScore proc sptr: dword, num: dword
    push ebx
    push esi
    push edi

    mov ebx, sptr     ; ebx 存储学生数组首地址
    mov ecx, num      ; ecx 作为外层循环计数器（学生人数）

outer_loop:
    cmp ecx, 0        ; 检查是否所有学生都处理完毕
    je end_outer_loop ; 学生人数为 0 则结束外层循环

    mov esi, ebx      ; esi 指向当前学生结构体首地址
    ;mov eax, 0      ; eax 清零，用于累加成绩总和 可优化为xor eax,eax
    xor eax,eax
    mov edx, 8        ; edx 作为内层循环计数器（8 门课）

inner_loop:
    cmp edx, 0        ; 检查是否处理完 8 门课
    je end_inner_loop ; 8 门课处理完毕则结束内层循环

    ; 累加当前课程成绩：scores[edx - 1]
    add ax, [esi].student.scores[edx * 2 - 2]
    dec edx           ; 递减内层循环计数器
    jmp inner_loop    ; 继续内层循环

end_inner_loop:
    ; 计算平均分：总和 / 8
    shr eax, 3        ; 右移 3 位等价于除以 8
    mov [esi].student.average, ax ; 将平均分存入 average 字段

    ; 指向下一个学生：每个学生结构体占 37 字节（8+11+16+2=37）
    add ebx, 37
    dec ecx           ; 递减外层循环计数器
    jmp outer_loop    ; 继续外层循环

end_outer_loop:
    pop edi
    pop esi
    pop ebx
    ret
computeAverageScore endp

end