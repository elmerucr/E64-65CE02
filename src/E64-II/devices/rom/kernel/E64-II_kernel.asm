; elmerucr - 6/10/2019
; compiles with vasmm68k_mot

	include 'E64-II_kernel_definitions.asm'

	org $00000000
	dc.l	$fff0					; vector 0 - supervisor stackpointer
	dc.l	kernel_main				; vector 1 - reset vector
	org $00000010
	dc.l	exception_handler		; vector 4 - illegal instruction
	org $00000028
	dc.l	exception_handler		; vector 10 - unimpl instruction
	dc.l	exception_handler		; vector 11 - unimpl instruction
	org $0000007c
	dc.l	interrupt_autovector	; vector 31 - level 7 interrupt autovector

; fake exception handler
	org $00000400
exception_handler
	move.l #$deadbeef,d0
	rte

; level 7 interrupt autovector
	org $00000500
interrupt_autovector
	rte

	org $00007000
aap
	dc.b	$fe

; start of main kernel code
	org	$7800
kernel_main
	move.b	#$02,VICV_BASE
	lea		$00c00000,a0
	movec	a0,msp
	ori.w	#$1000,sr
	movea.l	#$00d00000,a0
	move	a0,usp
	move.l	#$ff50449c,d0		; load blue color value into register d0
	move.b	d0,d1
	lea		$00008000,a0		; load address of background color register into a0
	move.l	d0,(a0)
	move.l	d0,-(a7)			; put on stack
	move.b	aap(pc),d1
	bra.s	kernel_main
	move.w	sr,d0
	andi.w	#%1111100011111111,d0
	ori.w	#%0000001100000000,d0
	move.w	d0,sr
	move.w	$3000.l,$30b0.l
	addq	#$01,d3
	jmp		kernel_main

	org $00007ffc
	dc.l	$deadbeef