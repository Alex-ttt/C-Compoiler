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
	push dword ptr 15
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	push eax
	push dword ptr 3
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr 17
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	push eax
	push dword ptr 5
	pop eax
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	push eax
	push dword ptr 5
	pop eax
	mov ebx, 4
	imul ebx, eax
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov ebx, eax
	mov eax, dword ptr [ebx + 40]
	push eax
	mov eax, dword ptr [ebx + 36]
	push eax
	mov eax, dword ptr [ebx + 32]
	push eax
	mov eax, dword ptr [ebx + 28]
	push eax
	mov eax, dword ptr [ebx + 24]
	push eax
	mov eax, dword ptr [ebx + 20]
	push eax
	mov eax, dword ptr [ebx + 16]
	push eax
	mov eax, dword ptr [ebx + 12]
	push eax
	mov eax, dword ptr [ebx + 8]
	push eax
	mov eax, dword ptr [ebx + 4]
	push eax
	mov eax, dword ptr [ebx + 0]
	push eax
	mov eax, ebp
	mov ebx, -40
	add eax, ebx
	push eax
	push dword ptr 3
	pop eax
	mov ebx, 4
	imul ebx, eax
	pop eax
	add eax, ebx
	push eax
	pop eax
	mov ebx, eax
	mov eax, dword ptr [ebx + 40]
	push eax
	mov eax, dword ptr [ebx + 36]
	push eax
	mov eax, dword ptr [ebx + 32]
	push eax
	mov eax, dword ptr [ebx + 28]
	push eax
	mov eax, dword ptr [ebx + 24]
	push eax
	mov eax, dword ptr [ebx + 20]
	push eax
	mov eax, dword ptr [ebx + 16]
	push eax
	mov eax, dword ptr [ebx + 12]
	push eax
	mov eax, dword ptr [ebx + 8]
	push eax
	mov eax, dword ptr [ebx + 4]
	push eax
	mov eax, dword ptr [ebx + 0]
	push eax
	invoke crt_printf, addr str0
	add esp, 80
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start