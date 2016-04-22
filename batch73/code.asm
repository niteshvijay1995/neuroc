global  main
extern  printf
extern scanf
SECTION .data
formatin: db "%d", 0
formatout: db "%d", 10, 0
b7: times 4 db 0
b2: times 4 db 0
d5cb34567: times 4 db 0
b5: times 4 db 0
SECTION .text
main:
push ebx
push ecx
pop ecx
pop ebx
mov eax, 0
ret
