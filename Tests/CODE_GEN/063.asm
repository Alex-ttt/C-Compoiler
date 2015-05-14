.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "(%d, %d)", 0dh, 0ah, 0
	var_a dd 2 dup(0)
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
f_swap: 
	push ebp
	mov ebp, esp
	sub esp, 8
	mov ebx, dword ptr [ebp + 8]
	push dword ptr [ebx + 4]
	push dword ptr [ebx + 0]
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	mov ebx, dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	push dword ptr [ebx + 4]
	mov ebx, dword ptr [ebx + 0]
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	push dword ptr [ebp + -4]
	mov ebx, dword ptr [ebp + -8]
	mov eax, dword ptr [ebp + 12]
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	mov eax, offset var_a
	mov ebx, 4
	add eax, ebx
	mov ebx, 0
	mov dword ptr [eax + 0], ebx
	mov eax, offset var_a
	push ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
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
	push offset var_a
	call f_print
	add esp, 4
	push offset var_b
	call f_print
	add esp, 4
	push offset var_b
	push offset var_a
	call f_swap
	add esp, 8
	push offset var_a
	call f_print
	add esp, 4
	push offset var_b
	call f_print
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start