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
	sub esp, 48
	push dword ptr -1
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	pop ebx
	mov eax, eax
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -44
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -44
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -48
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov eax, eax
	mov ebx, dword ptr [ebp + -44]
	push dword ptr [eax + 0]
	push dword ptr [ebx + 0]
	mov ebx, dword ptr [ebp + -48]
	mov ebx, dword ptr [ebx + 0]
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