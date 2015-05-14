.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d is even", 0dh, 0ah, 0
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
	mov eax, ebx
for0_condition: 
	push eax
	mov eax, dword ptr [ebp + -4]
	cmp eax, 10
	mov eax, 0
	setle al
	cmp eax, 0
	je for0_end
	mov eax, dword ptr [ebp + -4]
	mov ebx, 1
	and eax, ebx
	cmp eax, 0
	jne if0_true
	je if0_false
if0_true: 
	jmp for0_incr
	jmp if0_end
if0_false: 
if0_end: 
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str0
	add esp, 4
for0_incr: 
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
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start