.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d %d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 48
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
	mov ebx, -40
	add eax, ebx
	mov ebx, 36
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -48
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
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
	mov ebx, dword ptr [ebp + -48]
	push dword ptr [ebx + 0]
	mov ebx, dword ptr [ebp + -44]
	pop ebx
	mov eax, dword ptr [ebx + 0]
	add eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebp + -44]
	mov dword ptr [eax + 0], ebx
	mov ebx, dword ptr [ebp + -44]
	push dword ptr [ebx + 0]
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	pop ebx
	mov eax, eax
	mov eax, dword ptr [eax + 0]
	neg eax
	imul eax, ebx
	mov ebx, eax
	mov eax, dword ptr [ebp + -48]
	mov dword ptr [eax + 0], ebx
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov ebx, 36
	add eax, ebx
	mov ebx, dword ptr [ebp + -48]
	push dword ptr [eax + 0]
	push dword ptr [ebx + 0]
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	mov eax, eax
	mov ebx, dword ptr [ebp + -44]
	push dword ptr [eax + 0]
	push dword ptr [ebx + 0]
	invoke crt_printf, addr str0
	add esp, 16
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start