; elmerucr - 18/12/2019
; compiles with vasmm68k_mot

	include 'E64-II_kernel_definitions.asm'

	org		KERNEL_LOC

	dc.l	$00d00000				; vector 0 - supervisor stackpointer
	dc.l	kernel_main				; vector 1 - reset vector

; fake exception handler
exception_handler
	move.l #$deadbeef,d0
	rte

; start of main kernel code
kernel_main
	lea		exception_handler,a0
	move.l	a0,VEC_04_ILLEGAL_INSTRUCTION
	move.l	a0,VEC_10_UNIMPL_INSTRUCTION
	move.l	a0,VEC_11_UNIMPL_INSTRUCTION
	lea		interrupt_2_autovector,a0
	move.l	a0,VEC_26_LEVEL2_IRQ_AUTOVECT
	lea		interrupt_6_autovector,a0
	move.l	a0,VEC_30_LEVEL6_IRQ_AUTOVECT
	lea		interrupt_7_autovector,a0
	move.l	a0,VEC_31_LEVEL7_IRQ_AUTOVECT

	; set up timer0 interrupt
	;move.w	#$0bb8,TIMER_BASE+2		; load value 3000 ($0bb8 = 3000bpm = 50Hz) into high and low bytes
	move.w	#$003c,TIMER_BASE+2		; load value 60 ($003c = 60bpm = 1Hz) into high and low bytes
	ori.b	#%00000001,TIMER_BASE+1	; turn on interrupt generation by clock0

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
	move.b	#'l',d0
	bsr		put_char

	; set ipl to level 1
	move.w	sr,d0
	andi.w	#%1111100011111111,d0
	ori.w	#%0000000100000000,d0
	move.w	d0,sr

	; play a welcome sound on SID0
	lea		SID0_BASE,a0
	; frequency of voice 1
	move.w	#$099f,(a0)
	; attack and decay of voice 1
	move.b	#%00001001,($05,a0)
	; freq of voice 3
	move.w	#$1e00,($0e,a0)
	; max volume
	move.b	#$0f,($18,a0)
	; left channel mix
	move.b	#$ff,(SID0_LEFT,a0)
	; right channel mix
	move.b	#$10,(SID0_RGHT,a0)
	; play bell by opening gate on bit0 register #4
	; bit 4 is for a triangle wave form
    ; bit 2 is for a ring modulation connected to voice 3
	move.b	#%00100001,($04,a0)

	; play a welcome sound on SID1
	lea		SID1_BASE,a0
	; frequency of voice 1
	move.w	#$0e6b,(a0)
	; attack and decay of voice 1
	move.b	#%00001001,($05,a0)
	; freq of voice 3
	move.w	#$1e00,($0e,a0)
	; max volume
	move.b	#$0f,($18,a0)
	; left channel mix
	move.b	#$10,(SID1_LEFT,a0)
	; right channel mix
	move.b	#$ff,(SID1_RGHT,a0)
	; play bell by opening gate on bit0 register #4
	; bit 4 is for a triangle wave form
    ; bit 2 is for a ring modulation connected to voice 3
	move.b	#%00100001,($04,a0)

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

	addq.b	#$1,$00f00080
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


; put_char expects an ascii value in d0
put_char
	movem.l	d1-d2/a0-a2,-(a7)			; save registers
	move.w	CURSOR_POS,d1				; load current cursor position into d1
	move.b	CURR_TEXT_COLOR,d2			; load current text colour into d2
	movea.l	VICV_TXT,a0
	movea.l	VICV_COL,a1
	lea		ascii_to_screencode,a2		; a2 now points to ascii-screencode table
	move.b	(a2,d0),d0					; change the ascii value to a screencode value
	move.b	d0,(a0,d1)
	move.b	d2,(a1,d1)
	addq	#$1,CURSOR_POS
	andi.w	#$07ff,CURSOR_POS
	movem.l	(a7)+,d1-d2/a0-a2			; restore registers
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

	org		KERNEL_LOC+$fffc
	dc.l	$deadbeef