.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "start: %d %d", 0dh, 0ah, 0
	str1 db "end: %d %d", 0dh, 0ah, 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_print: 
	push ebp
	mov ebp, esp
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	push dword ptr [eax + 0]
	invoke crt_printf, addr str1
	add esp, 8
	mov esp, ebp
	pop ebp
	ret 0
f_scale: 
	push ebp
	mov ebp, esp
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 24]
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	imul ebx, dword ptr [eax + 0]
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + 20]
	push dword ptr [ebp + 16]
	push dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	mov dword ptr [ebp + 28], eax
	pop eax
	mov dword ptr [ebp + 32], eax
	pop eax
	mov dword ptr [ebp + 36], eax
	pop eax
	mov dword ptr [ebp + 40], eax
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 32
	push dword ptr 0
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 1
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 0
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 1
	mov eax, ebp
	mov ebx, -32
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_print
	add esp, 16
	sub esp, 16
	push dword ptr 3
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
	push dword ptr [ebp + -28]
	push dword ptr [ebp + -32]
	call f_scale
	add esp, 20
	call f_print
	add esp, 16
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_print
	add esp, 16
	sub esp, 16
	push dword ptr 5
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -16]
	call f_scale
	add esp, 20
	call f_print
	add esp, 16
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start