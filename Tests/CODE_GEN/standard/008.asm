.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	var_c dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 0
	push dword ptr 1
	push offset var_a
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr 2
	push offset var_b
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr 3
	push offset var_c
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr [var_c + 0]
	push dword ptr [var_b + 0]
	push dword ptr [var_a + 0]
	pop eax
	pop ebx
	imul eax, ebx
	push eax
	pop eax
	pop ebx
	imul eax, ebx
	push eax
	invoke crt_printf, addr str0
	add esp, 4
	push dword ptr [var_b + 0]
	push dword ptr [var_c + 0]
	push dword ptr [var_a + 0]
	push dword ptr [var_a + 0]
	pop eax
	pop ebx
	imul eax, ebx
	push eax
	push dword ptr [var_b + 0]
	push dword ptr [var_c + 0]
	pop eax
	pop ebx
	cdq
	idiv ebx
	push eax
	pop eax
	pop ebx
	imul eax, ebx
	push eax
	pop eax
	pop ebx
	imul eax, ebx
	push eax
	pop eax
	pop ebx
	imul eax, ebx
	push eax
	invoke crt_printf, addr str1
	add esp, 4
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start