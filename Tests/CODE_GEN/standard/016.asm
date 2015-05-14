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
	push dword ptr 1
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr 3
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str0
	add esp, 12
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start