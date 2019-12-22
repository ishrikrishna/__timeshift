; __timeshit 
; First Stage (Real Mode) && Second Stage (Protected mode) Bootloader
; 
; No license and Warranty - Use at your own risk after studying the code below.
;
; Have a great day,
; Krishna

BITS 16
ORG 0x7c00 ; Our code will be loaded here

jmp _start

;*******************************************
; Global Descriptor Table (GDT)
;*******************************************

GDT: 
	; null descriptor
	dd 0                
	dd 0 
	
	; code descriptor
	dw 0xFFFF           ; limit low
	dw 0                ; base low
	db 0                ; base middle
	db 10011010b        ; access
	db 11001111b        ; granularity
	db 0                ; base high
	
	; data descriptor
	dw 0xFFFF           ; limit low (Same as code)10:56 AM 7/8/2007
	dw 0                ; base low
	db 0                ; base middle
	db 10010010b        ; access
	db 11001111b        ; granularity
	db 0                ; base high
	
	; code descriptor
	dw 0xFFFF           ; limit low
	dw 0x0                ; base low
	db 0x0                ; base middle
	db 11111010b        ; access
	db 11001111b        ; granularity
	db 0                ; base high
	
	; data descriptor
	dw 0xFFFF           ; limit low (Same as code)10:56 AM 7/8/2007
	dw 0x0                ; base low
	db 0x0               ; base middle
	db 11110010b        ; access
	db 11001111b        ; granularity
	db 0                ; base high
END_GDT:

; GDT Pointer
GDT_PTR: 
	dw END_GDT - GDT - 1 	; limit (Size of GDT)
	dd GDT 			; base of GDT
	
; Location of entries in GDT
CODE_SEG equ 0x08
DATA_SEG equ 0x10

; Calling this enables Protected mode
; Shall be called only after setting up and loading GDT
; Otherwise, Processor will throw exception
EnablePM:					; clear interrupts
	mov	eax, cr0			; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax
	ret

Print16:
	lodsb
	or al, al
	jz .done
	mov ah, 0xe
	int 0x10
	jmp Print16
.done:
	mov al, 0x0D
	mov ah, 0xe
	int 0x10
	mov al, 0x0A
	mov ah, 0xe
	int 0x10
	ret

; Entry point in 16 bit Real Mode
; See the jmp instruction at the top of this file
_start:
	;-------------------------------;
	;   Setup segments and stack	;
	;-------------------------------;
	
	xor	ax, ax				; null segments
	mov	ds, ax
	mov	es, ax
	mov	ax, 0x9000			; stack begins at 0x9000-0xffff
	mov	ss, ax
	mov	esp, 0xFFFFFFFF
	
	; Welcome Message
	mov si, wel
	call Print16
	
	; Loading Protected Mode code
	mov ah, 0x02
	mov al, 64
	mov cx, 0x0002
	mov dh, 0
	mov dl, 0
	mov bx, 0x7e00
	int 0x13
	
	;jmp $
	lgdt [GDT_PTR] ; Install GDT
	
	; Jumping to Protected Mode
	; Notifying the user here
	mov si, pms
	call Print16
	
	; Enabling PM Mode
	call EnablePM
	
	; Jump to protected mode routine
	; Disabling the interupts because we didn't setup any interuppt handlers
	; Interuppts shall be enabled only after setting up and loading IDT table properly
	cli
	jmp CODE_SEG:PmodeEntry ; CS will be Auto-Updated to CODE_SEG
	
	jmp $
	wel db "Welcome!", 0
	pms db "Going into protected mode.", 0
	
	; Fill remaining bytes with zeros
	; and add boot signature
	times 510-($-$$) db 0
	dw 0xaa55
	
BITS 32
wel32Msg db "Entered Second Stage in Protected Mode.", 0

IDT_DESC:
.offset_low: dw 0
.selector: dw CODE_SEG
.reserved: db 0
.type: db 0
.offset_high: dw 0

IDT equ 0x500
IDT_PTR:
	dw 2048
	dd IDT
	
; ENTRY Point/subroutine for Protected mode
PmodeEntry:
	; Reload stack and data segment registers with GDT entry
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0xffffff
	mov esp, ebp
	
	;Enable A20 Gate to allow 32 bit addressing
	call EnableA20Gate
	
	cli
	call SetupIDT
	call RemapPIC
	
	mov eax, 33
	mov ebx, KBDISR
	call SetISR
	
	call refreshClock
	mov eax, 40
	mov ebx, ClockISR
	call SetISR
	call initRTC
	
	sti
	
	mov si, wel32Msg 	; Setting PM welcome message to be printed
	mov eax, 0
	mov ebx, 0
	call Print32		; Printing PM Welcome message
	;int 33
	
	; Kernel code is already loaded in memory during real mode from disk
	; via MBR (First Stage)
	;
	; Fetching kernel entry point
	; Kernel is an ELF file
	; 4 bytes start from offset 0x18 in ELF Header contains the entry point
	mov ebx, [0x8200 + 0x18]
	; Calculating effective addres to jump to
	lea ebx, [0x8200 + ebx]
	; Jumping to kernel
	jmp ebx
	
	; Halting the system
	jmp hang

hang:
	jmp hang
	
SetupIDT:
	cli
	mov eax, DefaultISR
	mov word [IDT_DESC.offset_low], ax
	shr eax, 16
	mov word [IDT_DESC.offset_high], ax
	mov byte [IDT_DESC.type], 10001110b
	mov ebx, 0
	mov edi, IDT
.repeat:
	mov eax, dword [IDT_DESC]
	mov dword [edi + ebx], eax
	mov eax, dword [IDT_DESC + 4]
	mov dword [edi + ebx + 4], eax
	add ebx, 8
	cmp ebx, 2048
	jge .done 
	jmp .repeat
.done:
	lidt [IDT_PTR]
	ret

SetISR:
	shl eax, 3
	mov edi, IDT
	add edi, eax
	mov word [edi], bx
	shr ebx, 16
	mov word [edi + 6], bx
	ret

DefaultISR:
	;.IntMsg db "I", 0
	;.IntMsgLen equ $ - .IntMsg - 1
	;cli
	;pusha
	;mov si, .IntMsg
	;mov eax, -.IntMsgLen
	;mov ebx, 25
	;call Print32
    mov al, 0x20
    out 0x20, al
    ;popa
	iret

clock:
	.hour dw '00'
	db ':'
	.minute dw '00'
	db ':'
	.second dw '00'
	db ' '
	.day dw '00'
	db '/'
	.month dw '00'
	db '/'
	.cent dw '00'
	.year dw '00'
	db 0
clockLen equ $-clock-1
	
updateClock:
    out 0x70, al
    in al, 0x71
    shl ax, 4
    shr al, 4
    add al, '0'
    mov byte [ebx + 1], al
    shr ax, 4
    shr al, 4
    add al, '0'
    mov byte [ebx], al
    ret
refreshClock:
    mov esi, clock
	mov eax, -clockLen
	mov ebx, 1
	call Print32
	ret

ClockISR:
	cli
	pusha
	
	mov ebx, clock.second
    mov al, 0x00
    call updateClock
	mov ebx, clock.minute
    mov al, 0x02
    call updateClock
	mov ebx, clock.hour
    mov al, 0x04
    call updateClock
    
	mov ebx, clock.day
    mov al, 0x07
    call updateClock
	mov ebx, clock.month
    mov al, 0x08
    call updateClock
	mov ebx, clock.cent
    mov al, 0x32
    call updateClock
    mov ebx, clock.year
    mov al, 0x09
    call updateClock
    call refreshClock
    
    ; ACK RTC
    mov al, 0x0c
    out 0x70, al
    in al, 0x71
    
    mov al, 0xa0
    out 0xa0, al
    mov al, 0x20
    out 0x20, al
    popa
	iret
	
kIntMsg db "Keyboard!", 0
kIntMsgLen equ $ - kIntMsg - 1
KBDISR:
	cli
	pusha
	
	mov esi, kIntMsg
	mov eax, kIntMsgLen
	mov ebx, 20
	call Print32
    
    mov al, 0x20
    out 0x20, al
    popa
	iret
	
; Sub routine to print message in 32 bit protected mode
Print32:
	;Print to screen
	mov ecx, 80
	xchg eax, ebx
	mul ecx
	add eax, ebx
	shl eax, 1
	mov edi, 0xb8000
	add edi, eax
	mov ah, 0x1b
.repeat:
	lodsb
	or al, al
	jz .done
	mov word [edi], ax
	add edi, 2
	jmp .repeat
.done:
	ret
	
RemapPIC:
	;cli
	mov al, 0x11
	out 0x20, al     ;restart PIC1
	out 0xA0, al     ;restart PIC2
	
	mov al, 0x20
	out 0x21, al     ;PIC1 now starts at 32
	mov al, 0x28
	out 0xA1, al     ;PIC2 now starts at 40
	
	mov al, 0x04
	out 0x21, al     ;setup cascading
	mov al, 0x02
	out 0xA1, al
	
	mov al, 0x01
	out 0x21, al
	out 0xA1, al     ;done!
	ret
	
initRTC:
	mov al, 0x0b
	out 0x70, al
	in al, 0x71
	or al, 0x40
	out 0x71, al
	ret
	
SetTimer:
	; COUNT = input hz / frequency
 
	mov	dx, 1193180 / 100	; 100hz, or 10 milliseconds
 
	; FIRST send the command word to the PIT. Sets binary counting,
	; Mode 3, Read or Load LSB first then MSB, Channel 0
 
	mov	al, 110110b
	out	0x43, al
 
	; Now we can write to channel 0. 
	;Because we set the "Load LSB first then MSB" bit, that is
	; the way we send it
 
	mov	ax, dx
	out	0x40, al	;LSB
	xchg	ah, al
	out	0x40, al	;MSB
	ret
	
EnableA20Gate:
; Check A20 line
; Returns to caller if A20 gate is cleared.
; Continues to A20_on if A20 line is set.
; Written by Elad Ashkcenazi    
 
	pushad
	mov edi,0x112345  ;odd megabyte address.
	mov esi,0x012345  ;even megabyte address.
	mov [esi],esi     ;making sure that both addresses contain diffrent values.
	mov [edi],edi     ;(if A20 line is cleared the two pointers would point to 
					  ;the address 0x012345 that would contain 0x112345 (edi)) 
	cmpsd             ;compare addresses to see if the're equivalent.
	popad
	je A20_off        ;if not equivalent , A20 line is set.
	ret               ;if equivalent , the A20 line is cleared.
 
A20_off:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

times 1534-($-$$) db 0	
dw 0xbaad
