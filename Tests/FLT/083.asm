.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f", 0
	float0 dd 3.600000
	float1 dd 2.000000
	float2 dd 2.100000
	float3 dd 0.300000
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 0
	push float3
	fld float2
	fld float3
	fdivp
	fstp tmp4
	push tmp4
	push float1
	fld float0
	fld float1
	fmulp
	fstp tmp4
	push tmp4
	pop eax
	mov tmp4, eax
	fld tmp4
	push float3
	fld float2
	fld float3
	fdivp
	fstp tmp4
	push tmp4
	pop eax
	mov tmp4, eax
	fld tmp4
	faddp
	fstp tmp4
	push tmp4
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 8
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start