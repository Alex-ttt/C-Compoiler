.686
.model flat, stdcall
include c:\masm32\include\msvcrt.inc
includelib c:\masm32\lib\msvcrt.lib
.data
	str0 db "%f %f", 0
	float0 dd 0.000000
	float1 dd 0.000000
	float2 dd 1.000000
	float3 dd 3.033400
	float4 dd 0.000000
	var_a dd 1 dup(0)
	var_b dd 1 dup(0)
	tmp4 real4 0.000000
	tmp8 real8 0.000000
.code
f_main: 
	push ebp
	mov ebp, esp
	sub esp, 0
	push float2
	push offset var_a
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push float4
	push float3
	pop eax
	push eax
	pop eax
	mov tmp4, eax
	fld tmp4
	fchs
	fstp tmp4
	push tmp4
	pop eax
	mov tmp4, eax
	fld tmp4
	fld float4
	faddp
	fstp tmp4
	push tmp4
	push offset var_b
	pop eax
	pop ebx
	mov dword ptr [eax + 0], ebx
	mov eax, ebx
	push eax
	pop eax
	push dword ptr [var_b + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	push dword ptr [var_a + 0]
	pop tmp4
	fld tmp4
	fstp tmp8
	mov eax, offset tmp8
	push dword ptr [eax + 4]
	push dword ptr [eax + 0]
	invoke crt_printf, addr str0
	add esp, 16
f_main_end: 
	mov esp, ebp
	pop ebp
	ret 0
start: 
	call f_main
	ret 0
end start