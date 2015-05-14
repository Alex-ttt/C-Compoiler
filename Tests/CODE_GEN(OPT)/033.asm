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
	sub esp, 12
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push ebx
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 15
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -12]
	mov ebx, dword ptr [ebp + -8]
	push dword ptr [ebx + 0]
	mov ebx, dword ptr [ebp + -4]
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str0
	add esp, 12
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start