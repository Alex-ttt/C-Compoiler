.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 0
	push dword ptr 5
	push dword ptr 5
	pop eax
	pop ebx
	sub eax, ebx
	push eax
	invoke crt_printf, addr str0
	add esp, 4
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start