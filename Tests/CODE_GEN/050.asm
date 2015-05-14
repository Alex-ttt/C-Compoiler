.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 24
	push dword ptr 0
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr -30
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -16]
	push dword ptr [ebp + -20]
	push dword ptr [ebp + -24]
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
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start