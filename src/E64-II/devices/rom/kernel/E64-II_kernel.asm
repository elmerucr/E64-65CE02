; elmerucr - 6/10/2019
; compiles with vasmm68k_mot

	include 'E64-II_kernel_definitions.asm'

	org $00000000
	dc.l	$00d00000				; vector 0 - supervisor stackpointer
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
	; set screen colors
	move.b	#$0c,VICV_BASE
	move.b	#$06,VICV_BASE+1

	; play a welcome sound
	;
	; frequency of voice 1 (high byte)
	move.b	#$82,SID0_BASE+$01
	; attack and decay of voice 1
	move.b	#$09,SID0_BASE+$05
	; freq of voice 3
	move.b	#$1e,SID0_BASE+$0f
	; max volume
	move.b	#$0f,SID0_BASE+$18
	; left channel mix
	move.b	#$ff,SID0_LEFT
	; right channel mix
	move.b	#$10,SID0_RGHT
	; play bell by opening gate on bit0 register #4
	; bit 4 is for a triangle wave form
    ; bit 2 is for a ring modulation connected to voice 3
	move.b	#%10000101,SID0_BASE+$04


	;lea		$00c00000,a0
	;movec	a0,msp
	;ori.w	#$1000,sr			; activate msp
start_of_loop
	movea.l	#$00d00000,a0
	move	a0,usp
	move.l	#$ff50449c,d0		; load blue color value into register d0
	move.b	d0,d1
	lea		$00008000,a0		; load address of background color register into a0
	move.l	d0,(a0)
	move.l	d0,-(a7)			; put on stack
	move.b	aap(pc),d1
	bra.s	start_of_loop
	move.w	sr,d0
	andi.w	#%1111100011111111,d0
	ori.w	#%0000001100000000,d0
	move.w	d0,sr
	move.w	$3000.l,$30b0.l
	addq	#$01,d3
	jmp		kernel_main

	org $00007ffc
	dc.l	$deadbeef