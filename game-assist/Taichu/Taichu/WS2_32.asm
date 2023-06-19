.data

	msgCaption  db 'Message box text',0

.code 
align 16



extern C_VAR : qword
extern pFound_2 : qword

extern tmpRAX : qword
extern tmpRSI : qword
extern tmpRCX : qword
extern tmpRDX : qword
extern tmpR8 : qword
extern tmpR9 : qword
extern tmpR14 : qword

extern MessageBoxA : proc
extern my_printf : proc
extern memcpy : proc   ; dst src len


hook_recv proc
	pop rax 
	
	mov tmpRAX,rax
	mov tmpRCX,rcx
	mov tmpRDX,rdx
	mov tmpR8,r8
	mov tmpR9,r9
	mov tmpRSI,rsi
	mov tmpR14,r14

	;;;;;;;;;;; print
	sub		rsp, 1000h		; shadow stack only [n]8 size


	; mov rcx, r14
	lea		rcx, [r14]
	mov rdx, rsi
	call my_printf

	mov rax,tmpRAX
	mov rcx,tmpRCX
	mov rdx,tmpRDX
	mov r8,tmpR8
	mov r9,tmpR9
	mov rsi,tmpRSI
	mov r14,tmpR14

	add		rsp, 1000h		; restoring shadow stack
	;;;;;;;;;;; print end 

	db 08Bh, 045h , 0D4h  ;mov eax, dword ptr ss:[rbp-2C]
	db 4Ch,8Dh,9Ch,24h, 80h,00h,00h,00h ;lea r11, qword ptr ss:[rsp+80]
	db 49h, 8Bh,5Bh, 20h   ;mov rbx, qword ptr ds:[r11+20]
	db 49h, 8Bh,73h, 28h   ;mov rsi, qword ptr ds:[r11+28]
	mov rsp,r11
	pop r14
	pop rdi
	pop rbp
	ret 

hook_recv endp

asm_func proc
	; RCX = address for the string for the message box
	sub		rsp, 108h		; shadow stack only [n]8 size
	
	lea		rdx, [msgCaption]
	mov		r8, rcx
	mov     rax,0
	mov		r9, rax
	mov C_VAR,rdx
	xor     rcx, rcx
	
	call MessageBoxA

	add		rsp, 108h		; restoring shadow stack
	ret
asm_func endp

end