.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0dh, 0ah, 0
	str1 db "%d %d %d", 0dh, 0ah, 0
	str2 db "%d %d %d", 0dh, 0ah, 0
	var_a dd 3 dup(0)
	var_b dd 3 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_add: 
	push ebp
	mov ebp, esp
	mov eax, ebp
	mov ebx, 20
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	add dword ptr [eax + 0], ebx
	mov ecx, eax
	mov eax, ebp
	mov ebx, 20
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	add dword ptr [eax + 0], ebx
	mov ecx, eax
	mov eax, ebp
	mov ebx, 20
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	add dword ptr [eax + 0], ebx
	mov ecx, eax
	push dword ptr [ebp + 16]
	push dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	mov dword ptr [ebp + 32], eax
	pop eax
	mov dword ptr [ebp + 36], eax
	pop eax
	mov dword ptr [ebp + 40], eax
	mov esp, ebp
	pop ebp
	ret 0
f_foo: 
	push ebp
	mov ebp, esp
	push dword ptr [ebp + 20]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 20]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov ecx, eax
	mov eax, dword ptr [ecx + 0]
	cdq
	idiv ebx
	mov dword ptr [ecx + 0], eax
	push dword ptr [ebp + 20]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov ecx, eax
	mov eax, dword ptr [ecx + 0]
	cdq
	idiv ebx
	mov dword ptr [ecx + 0], edx
	push dword ptr [ebp + 16]
	push dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	mov dword ptr [ebp + 24], eax
	pop eax
	mov dword ptr [ebp + 28], eax
	pop eax
	mov dword ptr [ebp + 32], eax
	mov esp, ebp
	pop ebp
	ret 0
f_foo1: 
	push ebp
	mov ebp, esp
	sub esp, 12
	push dword ptr [ebp + 20]
	sub esp, 12
	push dword ptr [ebp + 32]
	push dword ptr [ebp + 28]
	push dword ptr [ebp + 24]
	push dword ptr [ebp + 16]
	push dword ptr [ebp + 12]
	push dword ptr [ebp + 8]
	call f_add
	add esp, 24
	call f_foo
	add esp, 16
	pop eax
	sub esp, 12
	push dword ptr [ebp + 32]
	push dword ptr [ebp + 28]
	push dword ptr [ebp + 24]
	push dword ptr [ebp + 16]
	push dword ptr [ebp + 12]
	push dword ptr [ebp + 8]
	call f_add
	add esp, 24
	pop eax
	mov dword ptr [ebp + 36], eax
	pop eax
	mov dword ptr [ebp + 40], eax
	pop eax
	mov dword ptr [ebp + 44], eax
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 12
	mov var_a, 1
	mov eax, offset var_a
	mov ebx, 4
	add eax, ebx
	mov ebx, 2
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_a
	mov ebx, 8
	add eax, ebx
	mov ebx, 3
	mov dword ptr [eax + 0], ebx
	mov var_b, -1
	mov eax, offset var_b
	mov ebx, 4
	add eax, ebx
	mov ebx, -2
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_b
	mov ebx, 8
	add eax, ebx
	mov ebx, -3
	mov dword ptr [eax + 0], ebx
	sub esp, 12
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	call f_add
	add esp, 24
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	pop ebx
	mov dword ptr [eax + 8], ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 12
	sub esp, 12
	push dword ptr 3
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	call f_foo
	add esp, 16
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	pop ebx
	mov dword ptr [eax + 8], ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 12
	sub esp, 12
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr -1
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	call f_foo1
	add esp, 28
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	pop ebx
	mov dword ptr [eax + 8], ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str2
	add esp, 12
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start