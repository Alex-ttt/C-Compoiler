.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_sigma: 
	push ebp
	mov ebp, esp
	mov ebx, dword ptr [ebp + 12]
	mov eax, dword ptr [ebp + 8]
	add eax, ebx
	mov dword ptr [ebp + 16], eax
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 1
	push dword ptr -1
	call f_sigma
	add esp, 8
	invoke crt_printf, addr str0
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start