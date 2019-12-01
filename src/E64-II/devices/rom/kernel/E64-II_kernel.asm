; elmerucr - 6/10/2019
; compiles with vasmm68k_mot

	include 'E64-II_kernel_definitions.asm'

	org		$00000000
	dc.l	$00d00000				; vector 0 - supervisor stackpointer
	dc.l	kernel_main				; vector 1 - reset vector
	org		$00000010
	dc.l	exception_handler		; vector 4 - illegal instruction
	org		$00000028
	dc.l	exception_handler		; vector 10 - unimpl instruction
	dc.l	exception_handler		; vector 11 - unimpl instruction
	org		$0000007c
	dc.l	interrupt_autovector	; vector 31 - level 7 interrupt autovector

; fake exception handler
	org		$00000400
exception_handler
	move.l #$deadbeef,d0
	rte

; level 7 interrupt autovector
	org $00000500
interrupt_autovector
	rte

; start of main kernel code
	org	$7800
kernel_main
	; set screen colors
	move.b	#$0c,VICV_BASE
	move.b	#$06,VICV_BASE+1
	; set text color
	move.b	#$0c,CURR_TEXT_COLOR

	; set txt pointer
	move.l	#$00f00000,VICV_TXT
	move.l	#$00f00800,VICV_COL

	; clear screen
	jsr		clear_screen

	; play a welcome sound on SID0
	lea		SID0_BASE,a0
	; frequency of voice 1 (high byte)
	move.b	#$82,($01,a0)
	; attack and decay of voice 1
	move.b	#$09,($05,a0)
	; freq of voice 3
	move.b	#$1e,($0f,a0)
	; max volume
	move.b	#$0f,($18,a0)
	; left channel mix
	move.b	#$ff,(SID0_LEFT,a0)
	; right channel mix
	move.b	#$10,(SID0_RGHT,a0)
	; play bell by opening gate on bit0 register #4
	; bit 4 is for a triangle wave form
    ; bit 2 is for a ring modulation connected to voice 3
	move.b	#%10000101,($04,a0)

mainloop
	; put something in the usp
	movea.l	#$00d00000,a0
	move	a0,usp

	; copy keyboard state in to screen
	moveq	#$0,d0
	movea.l	VICV_TXT,a0
	lea		$200(a0),a0
.1	move.l	(CIA_BASE+$80,d0),(a0,d0)
	addq	#$1,d0
	cmp.b	#$20,d0
	bne		.1

	addq.b	#$1,$00f00000
	bra.s	mainloop

clear_screen
	movem.l	d0-d1/a0-a2,-(a7)
	movea.l	(VICV_TXT),a0
	movea.l	(VICV_COL),a1
	movea.l	a0,a2
	lea		$800(a2),a2
	move.l	#$20202020,d0
	moveq	#$00,d1
	move.b	CURR_TEXT_COLOR,d1
	lsl.l	#$8,d1
	move.b	CURR_TEXT_COLOR,d1
	lsl.l	#$8,d1
	move.b	CURR_TEXT_COLOR,d1
	lsl.l	#$8,d1
	move.b	CURR_TEXT_COLOR,d1
.1	move.l	d0,(a0)+
	move.l	d1,(a1)+
	cmp.l	a0,a2
	bne		.1
	movem.l	(a7)+,d0-d1/a0-a2
	rts



	align 1
	include "E64-II_kernel_tables.asm"

	org		$00007ffc
	dc.l	$deadbeef