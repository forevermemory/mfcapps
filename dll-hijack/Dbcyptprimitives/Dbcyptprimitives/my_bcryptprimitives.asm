.data

.code 
align 16

extern g_func_addr:dq 

DirectInput8Create  proc
	lea rax,g_func_addr
	mov rax,[rax+1*8]
	jmp rax
DirectInput8Create  endp

DllCanUnloadNow  proc
	lea rax,g_func_addr
	mov rax,[rax+2*8]
	jmp rax
DllCanUnloadNow  endp

DllGetClassObject  proc
	lea rax,g_func_addr
	mov rax,[rax+3*8]
	jmp rax
DllGetClassObject  endp

DllRegisterServer  proc
	lea rax,g_func_addr
	mov rax,[rax+4*8]
	jmp rax
DllRegisterServer  endp

DllUnregisterServer  proc
	lea rax,g_func_addr
	mov rax,[rax+5*8]
	jmp rax
DllUnregisterServer  endp

GetdfDIJoystick  proc
	lea rax,g_func_addr
	mov rax,[rax+6*8]
	jmp rax
GetdfDIJoystick  endp

end
