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
	org		$00000068
	dc.l	interrupt_2_autovector
	org		$00000078
	dc.l	interrupt_6_autovector	; vector 30 - level 6 interrupt autovector
	dc.l	interrupt_7_autovector	; vector 31 - level 7 interrupt autovector

; fake exception handler
	org		$00000400
exception_handler
	move.l #$deadbeef,d0
	rte

; start of main kernel code
	org	$7800
kernel_main
	; set up timer0 interrupt
	move.w	#$0bb8,TIMER_BASE+2		; load value 3000 ($0bb8 = 3000bpm = 50Hz) into high and low bytes
	ori.b	#%00000001,TIMER_BASE+1	; turn on interrupt generation by clock0
	;lda #<timer0_irq_handler_continued
	;sta TIMER0_VECTOR
	;lda #>timer0_irq_handler_continued
	;sta TIMER0_VECTOR+1

	; set screen colors
	move.b	#$00,VICV_BASE			; c64 black
	move.b	#$06,VICV_BASE+1		; c64 blue
	; set text color
	move.b	#$0e,CURR_TEXT_COLOR	; c64 light blue

	; set txt pointer
	move.l	#$00f00000,VICV_TXT
	move.l	#$00f00800,VICV_COL

	; reset cursor position
	move.w	#$0,CURSOR_POS

	; clear screen
	bsr		clear_screen

	move.b	#'E',d0
	bsr		put_char

	; set ipl to level 1
	move.w	sr,d0
	andi.w	#%1111100011111111,d0
	ori.w	#%0000000100000000,d0
	move.w	d0,sr

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
	lea		$400(a0),a0
	lea		CIA_BASE,a1
	lea		$80(a1),a1
.1	move.b	(a1,d0),(a0,d0)
	addq	#$1,d0
	cmp.b	#$49,d0
	bne		.1

	addq.b	#$1,$00f00001
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


; put_char expects an ascii value (byte in d0)
put_char
	movem.l	d1-d2/a0-a2,-(a7)
	move.w	CURSOR_POS,d1
	move.b	CURR_TEXT_COLOR,d2
	movea.l	VICV_TXT,a0
	movea.l	VICV_COL,a1
	lea		ascii_to_screencode,a2
	move.b	(a2,d0),d0
	move.b	d0,(a0,d1)
	move.b	d2,(a1,d1)
	movem.l	(a7)+,d1-d2/a0-a2
	rts


; level 2 interrupt autovector (timer)
interrupt_2_autovector
	; acknowledge intterupt
	ori.b	#%00000001,TIMER_BASE
	move.l	a0,-(a7)
	movea.l	VICV_COL,a0
	addq.b	#$1,(a0)
	andi.b	#%00001111,(a0)
	movea.l	(a7)+,a0
	rte

; level 6 interrupt autovector
interrupt_6_autovector
	rte

; level 7 interrupt autovector
interrupt_7_autovector
	rte

	align 1
	include "E64-II_kernel_tables.asm"

	org		$00007ffc
	dc.l	$deadbeef