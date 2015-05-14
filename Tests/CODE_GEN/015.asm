.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	str4 db "%d", 0dh, 0ah, 0
	str5 db "%d", 0dh, 0ah, 0
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	mov var_b, 3
	mov eax, offset var_a
	mov ebx, dword ptr [var_b + 0]
	add dword ptr [eax + 0], ebx
	mov ecx, eax
	push dword ptr [var_a + 0]
	invoke crt_printf, addr str0
	add esp, 4
	mov eax, 3
	mov ebx, dword ptr [var_b + 0]
	imul eax, ebx
	mov ebx, eax
	mov eax, offset var_a
	sub dword ptr [eax + 0], ebx
	mov ecx, eax
	push dword ptr [var_a + 0]
	invoke crt_printf, addr str1
	add esp, 4
	mov var_a, 1
	mov eax, offset var_a
	mov ebx, 15
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [var_a + 0]
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, offset var_a
	mov ebx, 3
	mov ecx, eax
	mov eax, dword ptr [ecx + 0]
	cdq
	idiv ebx
	mov dword ptr [ecx + 0], eax
	push dword ptr [var_a + 0]
	invoke crt_printf, addr str3
	add esp, 4
	mov eax, dword ptr [var_b + 0]
	mov ebx, 3
	imul eax, ebx
	mov ebx, 3
	imul eax, ebx
	mov ebx, eax
	mov eax, offset var_a
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [var_a + 0]
	invoke crt_printf, addr str4
	add esp, 4
	mov eax, offset var_a
	mov ebx, 2
	mov ecx, eax
	mov eax, dword ptr [ecx + 0]
	cdq
	idiv ebx
	mov dword ptr [ecx + 0], edx
	push dword ptr [var_a + 0]
	invoke crt_printf, addr str5
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start