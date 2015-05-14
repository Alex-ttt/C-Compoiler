.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "1", 0
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	invoke crt_printf, addr str0
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start