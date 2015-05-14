.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0dh, 0ah, 0
	str1 db "%d", 0dh, 0ah, 0
	str2 db "%d", 0dh, 0ah, 0
	str3 db "%d", 0dh, 0ah, 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 4
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	sub esp, 4
	push dword ptr 1
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	sub esp, 4
	push dword ptr 2
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	sub esp, 4
	push dword ptr 3
	mov eax, ebp
	mov ebx, -16
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -16]
	invoke crt_printf, addr str0
	add esp, 4
	push dword ptr [ebp + -12]
	invoke crt_printf, addr str1
	add esp, 4
	push dword ptr [ebp + -8]
	invoke crt_printf, addr str2
	add esp, 4
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str3
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start