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

	org $00007000
aap
	dc.b	$fe

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

start_of_loop
	movea.l	#$00d00000,a0
	move	a0,usp
	move.l	d0,(a0)
	move.b	aap(pc),d1
	addq.b	#$1,$00f00000
	bra.s	start_of_loop

clear_screen
	movem.l	d0-d2/a0-a1,-(a7)
	movea.l	VICV_TXT,a0
	movea.l	VICV_COL,a1
	move.l	#$0,d0
	move.b	ascii_to_screencode+' ',d1
	move.b	CURR_TEXT_COLOR,d2
.1	move.b	d1,(a0,d0)
	move.b	d2,(a1,d0)
	addq	#$1,d0
	cmp.w	#$800,d0
	bne		.1
	movem.l	(a7)+,d0-d2/a0-a1
	rts

	align 1
	include "E64-II_kernel_tables.asm"

	org		$00007ffc
	dc.l	$deadbeef