.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 40
	push dword ptr -3
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	pop ebx
	mov eax, eax
	mov dword ptr [eax + 0], ebx
	push dword ptr 15
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 36
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 36
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov eax, eax
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