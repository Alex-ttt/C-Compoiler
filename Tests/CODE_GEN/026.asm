.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0
	var_a dd 100 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 5
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	mov ebx, -15
	mov dword ptr [eax + 0], ebx
	push dword ptr 1
	push dword ptr 1
	push dword ptr 3
	push dword ptr 5
	push dword ptr [ebp + -4]
	mov ebx, dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	imul eax, ebx
	pop ebx
	imul eax, ebx
	pop ebx
	cdq
	idiv ebx
	pop ebx
	cdq
	idiv ebx
	pop ebx
	mov eax, edx
	cdq
	idiv ebx
	pop ebx
	sub eax, ebx
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	mov eax, 396
	mov ebx, offset var_a
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	mov ebx, offset var_a
	add eax, ebx
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