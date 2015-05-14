.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "(%d, %d)", 0dh, 0ah, 0
	var_p dd 20 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 44
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr 0
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
for0_condition: 
	push eax
	mov eax, dword ptr [ebp + -4]
	cmp eax, 10
	mov eax, 0
	setl al
	cmp eax, 0
	je for0_end
	mov eax, dword ptr [ebp + -4]
	mov ebx, 8
	imul eax, ebx
	mov ebx, offset var_p
	add eax, ebx
	push eax
	mov eax, ebp
	mov ebx, -44
	add eax, ebx
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	push dword ptr [ebp + -4]
	mov eax, dword ptr [ebp + -4]
	mov ebx, 8
	imul eax, ebx
	mov ebx, offset var_p
	add eax, ebx
	mov ebx, 0
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	neg eax
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 8
	imul eax, ebx
	mov ebx, offset var_p
	add eax, ebx
	mov ebx, 4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, dword ptr [ebp + -4]
	inc eax
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	jmp for0_condition
for0_end: 
	push eax
	push dword ptr 9
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
for1_condition: 
	push eax
	mov eax, dword ptr [ebp + -4]
	cmp eax, 0
	mov eax, 0
	setge al
	cmp eax, 0
	je for1_end
	mov eax, ebp
	mov ebx, -44
	add eax, ebx
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	mov ebx, 4
	add eax, ebx
	push dword ptr [eax + 0]
	mov eax, ebp
	mov ebx, -44
	add eax, ebx
	push eax
	mov eax, dword ptr [ebp + -4]
	mov ebx, 4
	imul eax, ebx
	pop ebx
	add eax, ebx
	mov eax, dword ptr [eax + 0]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
	mov eax, dword ptr [ebp + -4]
	dec eax
	push eax
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	mov ebx, eax
	pop eax
	mov dword ptr [ebx + 0], eax
	jmp for1_condition
for1_end: 
	push eax
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start