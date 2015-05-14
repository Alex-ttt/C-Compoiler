.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%d", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_one: 
	push ebp
	mov ebp, esp
	mov dword ptr [ebp + 8], 1
	mov esp, ebp
	pop ebp
	ret 0
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 4
	call f_one
	invoke crt_printf, addr str0
	add esp, 4
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start