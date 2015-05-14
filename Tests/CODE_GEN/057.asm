.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0dh, 0ah, 0
	str1 db "%d %d", 0dh, 0ah, 0
	str2 db "%d %d", 0dh, 0ah, 0
	var_a dd 3 dup(0)
	var_b dd 3 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_distance: 
	push ebp
	mov ebp, esp
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
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
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
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	pop ebx
	imul eax, ebx
	push eax
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
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
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
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	pop ebx
	imul eax, ebx
	push eax
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
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	push eax
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
	mov eax, dword ptr [eax + 0]
	sub eax, ebx
	pop ebx
	imul eax, ebx
	pop ebx
	add eax, ebx
	pop ebx
	add eax, ebx
	mov dword ptr [ebp + 32], eax
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	call f_distance
	add esp, 24
	sub esp, 4
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	call f_distance
	add esp, 24
	invoke crt_printf, addr str0
	add esp, 8
	mov var_a, 1
	sub esp, 4
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	call f_distance
	add esp, 24
	sub esp, 4
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	call f_distance
	add esp, 24
	invoke crt_printf, addr str1
	add esp, 8
	mov var_b, -1
	sub esp, 4
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	call f_distance
	add esp, 24
	sub esp, 4
	push dword ptr [var_b + 8]
	push dword ptr [var_b + 4]
	push dword ptr [var_b + 0]
	push dword ptr [var_a + 8]
	push dword ptr [var_a + 4]
	push dword ptr [var_a + 0]
	call f_distance
	add esp, 24
	invoke crt_printf, addr str2
	add esp, 8
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start