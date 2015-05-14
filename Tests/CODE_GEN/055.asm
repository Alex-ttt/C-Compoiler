.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
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
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 24
	push dword ptr 1
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 2
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 3
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	sub esp, 12
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -4]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -12]
	call f_add
	add esp, 24
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	pop ebx
	mov dword ptr [eax + 4], ebx
	pop ebx
	mov dword ptr [eax + 8], ebx
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	mov ebx, 8
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -24
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -24
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