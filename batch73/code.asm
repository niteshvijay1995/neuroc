global  main
extern  printf
extern scanf
SECTION .data
formatin: db "%d", 0
formatout: db "%d", 10, 0
b3c2_chemistry: times 4 db 0
b3c2_physics: times 4 db 0
b3c2_maths: times 4 db 0
d5cb34567: times 4 db 0
b5: times 4 db 0
d4_chemistry: times 4 db 0
d4_physics: times 4 db 0
d4_maths: times 4 db 0
SECTION .text
main:
push ebx
push ecx
mov ebx,1
mov [b5],ebx
push d5cb34567
push formatin
call scanf
add esp,8
push b5
push formatin
call scanf
add esp,8
mov ebx,0
mov [d4_maths],ebx
mov ebx,0
mov [d4_physics],ebx
mov ebx,0
mov [d4_chemistry],ebx
label_20:
mov ebx,[b5]
push ebx
mov ebx,[d5cb34567]
pop ecx
cmp ecx,ebx
jle true_20_9383
jmp false_20_9383
true_20_9383:
mov ebx, 1
jmp resume_20_9383
false_20_9383:
mov ebx, 0
resume_20_9383:
cmp ebx,1
jne label_out_20
push b3c2_maths
push formatin
call scanf
add esp,8
push b3c2_physics
push formatin
call scanf
add esp,8
push b3c2_chemistry
push formatin
call scanf
add esp,8
mov ebx,[d4_chemistry]
PUSH ebx
mov ebx,[b3c2_chemistry]
POP ecx
add ebx,ecx
mov [d4_chemistry],ebx
mov ebx,[d4_physics]
PUSH ebx
mov ebx,[b3c2_physics]
POP ecx
add ebx,ecx
mov [d4_physics],ebx
mov ebx,[d4_maths]
PUSH ebx
mov ebx,[b3c2_maths]
POP ecx
add ebx,ecx
mov [d4_maths],ebx
mov ebx,1
PUSH ebx
mov ebx,[b5]
POP ecx
add ebx,ecx
mov [b5],ebx
jmp label_20
label_out_20:
mov ebx,[d4]
push ebx
push formatout
call printf
add esp,8
mov ebx,[d3]
push ebx
push formatout
call printf
add esp,8
pop ecx
pop ebx
mov eax, 0
ret
