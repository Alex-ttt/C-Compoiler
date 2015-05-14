.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "(%d, %d)", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	var_a dd 200 dup(0)
	var_b dd 2 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_print: 
	push ebp
	mov ebp, esp
	mov eax, dword ptr [ebp + 8]
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, dword ptr [ebp + 8]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 8
	mov eax, 400
	mov ebx, offset var_a
	add eax, ebx
	push eax
	call f_print
	add esp, 4
	mov eax, offset var_b
	mov ebx, 4
	add eax, ebx
	mov ebx, 5
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_b
	push ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, 400
	mov ebx, offset var_a
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	mov ebx, dword ptr [var_b + 0]
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [var_b + 4]
	mov dword ptr [eax + 4], ebx
	push dword ptr [ebp + -4]
	call f_print
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 8
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	push eax
	call f_print
	add esp, 4
	mov eax, dword ptr [ebp + -4]
	mov ebx, 8
	sub eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	push eax
	call f_print
	add esp, 4
	push dword ptr [ebp + -4]
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [ebp + -8]
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str1
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, 4
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	mov ebx, dword ptr [ebp + -8]
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str2
	add esp, 4
	mov eax, dword ptr [ebp + -8]
	mov ebx, 4
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	mov ebx, dword ptr [ebp + -8]
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str3
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start