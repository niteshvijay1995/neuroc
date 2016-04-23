global  main
extern  printf
extern scanf
SECTION .data
formatin: db "%d", 0
formatout: db "%d", 10, 0
b5b567: times 4 db 0
c3bd: times 4 db 0
d3: times 4 db 0
SECTION .text
main:
push ebx
push ecx
push eax
mov ebx,1
mov [b5b567],ebx
mov ebx,0
mov [d3],ebx
label_8:
mov ebx,[b5b567]
push ebx
mov ebx,7
pop ecx
cmp ecx,ebx
jle yes_8_383
jmp no_8_383
yes_8_383:
mov ebx, 1
jmp resume_8_383
no_8_383:
mov ebx, 0
resume_8_383:
cmp ebx,1
jne label_out_8
push c3bd
push formatin
call scanf
add esp,8
mov ebx,[c3bd]
PUSH ebx
mov ebx,[d3]
POP ecx
add ebx,ecx
mov [d3],ebx
mov ebx,1
PUSH ebx
mov ebx,[b5b567]
POP ecx
add ebx,ecx
mov [b5b567],ebx
jmp label_8
label_out_8:
mov ebx,[d3]
push ebx
push formatout
call printf
add esp,8
pop ecx
pop ebx
pop eax
mov eax, 0
ret
