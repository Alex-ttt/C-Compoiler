.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d %d %d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_fo: 
	push ebp
	mov ebp, esp
	sub esp, 16
	push dword ptr [ebp + 8]
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
	push dword ptr 0
	push dword ptr [ebp + -4]
	pop eax
	pop ebx
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	pop eax
	cmp eax, 0
	jne if0_true
	je if0_false
if0_true: 
	sub esp, 0
	push dword ptr [ebp + -4]
	pop eax
	mov dword ptr [ebp + 12], eax
	jmp f_fo_end
BLOCK_LABEL0: 
	jmp if0_end
if0_false: 
	sub esp, 0
	push dword ptr [ebp + -4]
	pop eax
	neg eax
	push eax
	pop eax
	mov dword ptr [ebp + 12], eax
	jmp f_fo_end
BLOCK_LABEL1: 
if0_end: 
BLOCK_LABEL2: 
f_fo_end: 
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 12
	sub esp, 16
	push dword ptr 5
	pop eax
	neg eax
	push eax
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr [ebp + 8]
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
	push dword ptr 0
	push dword ptr [ebp + -4]
	pop eax
	pop ebx
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	pop eax
	cmp eax, 0
	jne if1_true
	je if1_false
if1_true: 
	sub esp, 0
	push dword ptr [ebp + -4]
	mov eax, ebp
	mov ebx, -4
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	jmp BLOCK_LABEL6
	pop eax
BLOCK_LABEL5: 
	jmp if1_end
if1_false: 
	sub esp, 0
	push dword ptr [ebp + -4]
	pop eax
	neg eax
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
	jmp BLOCK_LABEL6
	pop eax
BLOCK_LABEL4: 
if1_end: 
BLOCK_LABEL6: 
	sub esp, 16
	push dword ptr 6
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr [ebp + 8]
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
	push dword ptr 0
	push dword ptr [ebp + -4]
	pop eax
	pop ebx
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	pop eax
	cmp eax, 0
	jne if2_true
	je if2_false
if2_true: 
	sub esp, 0
	push dword ptr [ebp + -4]
	mov eax, ebp
	mov ebx, -8
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	jmp BLOCK_LABEL9
	pop eax
BLOCK_LABEL8: 
	jmp if2_end
if2_false: 
	sub esp, 0
	push dword ptr [ebp + -4]
	pop eax
	neg eax
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
	jmp BLOCK_LABEL9
	pop eax
BLOCK_LABEL7: 
if2_end: 
BLOCK_LABEL9: 
	sub esp, 16
	push dword ptr 0
	mov eax, ebp
	mov ebx, 8
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr [ebp + 8]
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
	push dword ptr 0
	push dword ptr [ebp + -4]
	pop eax
	pop ebx
	cmp eax, ebx
	mov eax, 0
	setg al
	push eax
	pop eax
	cmp eax, 0
	jne if3_true
	je if3_false
if3_true: 
	sub esp, 0
	push dword ptr [ebp + -4]
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	jmp BLOCK_LABEL12
	pop eax
BLOCK_LABEL11: 
	jmp if3_end
if3_false: 
	sub esp, 0
	push dword ptr [ebp + -4]
	pop eax
	neg eax
	push eax
	mov eax, ebp
	mov ebx, -12
	add eax, ebx
	push eax
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	jmp BLOCK_LABEL12
	pop eax
BLOCK_LABEL10: 
if3_end: 
BLOCK_LABEL12: 
	push dword ptr [ebp + -12]
	push dword ptr [ebp + -8]
	push dword ptr [ebp + -4]
	invoke crt_printf, addr str0
	add esp, 12
BLOCK_LABEL3: 
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start