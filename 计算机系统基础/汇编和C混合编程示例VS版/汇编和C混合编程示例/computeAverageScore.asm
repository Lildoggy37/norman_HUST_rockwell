.686P
.model flat, c
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

.data
   lpfmt  db "%s %s %d %d",0dh,0ah,0
   lpfmt_string  db "%s  ",0
   lpfmt_num  db "%d  ",0
.code
;  显示学生信息
;  sptr 学生数组的首地址
;  num  学生人数
display proc  sptr: dword, num:dword
        mov  ebx, sptr
        invoke printf, offset lpfmt_string, ebx
        invoke printf, offset lpfmt_string, addr  [ebx].student.sid

        invoke printf , offset lpfmt_num,  [ebx].student.scores[0]   ; 第0个分数
        invoke printf , offset lpfmt_num,  [ebx].student.scores[2]   ; 第1个分数

        ret
display endp
end