.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0
	var_a dd 2 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	mov var_a, 3
	mov eax, offset var_a
	mov ebx, 4
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov ebx, 3
	imul eax, ebx
	mov ebx, 1
	sub eax, ebx
	push eax
	mov eax, offset var_a
	mov ebx, dword ptr [eax + 0]
	mov eax, offset var_a
	mov eax, dword ptr [eax + 0]
	cdq
	idiv ebx
	pop ebx
	imul eax, ebx
	push eax
	mov eax, offset var_a
	mov ebx, dword ptr [eax + 0]
	mov eax, offset var_a
	mov eax, dword ptr [eax + 0]
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	mov eax, offset var_a
	mov ebx, 4
	add eax, ebx
	pop ebx
	sub dword ptr [eax + 0], ebx
	mov ecx, eax
	mov eax, offset var_a
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, offset var_a
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start