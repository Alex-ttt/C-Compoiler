.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "do-while", 0dh, 0ah, 0
	str1 db "while-do", 0dh, 0ah, 0
	str2 db "for", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	pop eax
for0_condition: 
	cmp eax, 0
	je for0_end
while0_condition: 
	mov eax, 1
	cmp eax, 0
	je while0_end
dowhile0_start: 
	invoke crt_printf, addr str0
	jmp dowhile0_end
	mov eax, 1
	cmp eax, 0
	jne dowhile0_start
dowhile0_end: 
	invoke crt_printf, addr str1
	jmp while0_end
	jmp while0_condition
while0_end: 
	invoke crt_printf, addr str2
	jmp for0_end
	jmp for0_condition
for0_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start