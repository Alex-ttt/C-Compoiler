.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	var_a dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	mov var_a, 1
	mov eax, dword ptr [var_a + 0]
	mov ebx, 2
	cdq
	idiv ebx
	push edx
	invoke crt_printf, addr str0
	add esp, 4
	mov ebx, dword ptr [var_a + 0]
	mov eax, dword ptr [var_a + 0]
	cdq
	idiv ebx
	push edx
	invoke crt_printf, addr str1
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start