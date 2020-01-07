; E64 65ce02 kernel

	.cpu	"65ce02"

	.include "E64_kernel_constants.asm"

	* = $e000

cold_start
			; no need to manually clear decimal flag, done by 65ce02 reset procedure
			; no need to manually set i flag, done by reset procedure
			; no need to manually set e flag, done by reset procedure
	ldx #$ff	; prepare large stack at $dfff
	ldy #$df
	txs		; move x into sl
	tys		; move y into sh
	cle		; clear extended mode flag to enable large stack

	lda #$00		; explicitly turn off all timer interrupts
	sta TIMER_BASE

	; set up timer0 interrupt
	lda #$0b			; load value 3000 ($0bb8 = 3000bpm = 50Hz) into high and low bytes (big endian)
	sta TIMER_BASE+2
	lda #$b8
	sta TIMER_BASE+3
	lda #%00000001		; turn on interrupt generation by clock0
	tsb TIMER_BASE+1
	; set up timer0 interrupt vector
	lda #<timer0_irq_handler_continued
	sta TIMER0_VECTOR
	lda #>timer0_irq_handler_continued
	sta TIMER0_VECTOR+1

	; set up timer1 interrupt
	lda #$01
	sta TIMER_BASE+2
	lda #$85
	sta TIMER_BASE+3
	lda #%00000010
	tsb TIMER_BASE+1
	; set up timer1 interrupt vector
	lda #<timer1_irq_handler_continued
	sta TIMER1_VECTOR
	lda #>timer1_irq_handler_continued
	sta TIMER1_VECTOR+1

	; set up timer2 interrupt
	lda #$00
	sta TIMER_BASE+2
	lda #$3c
	sta TIMER_BASE+3
	lda #%00000100
	tsb TIMER_BASE+1
	; set up timer2 interrupt vector
	lda #<timer2_irq_handler_continued
	sta TIMER2_VECTOR
	lda #>timer2_irq_handler_continued
	sta TIMER2_VECTOR+1

	; set up timer3 interrupt
	lda #$01
	sta TIMER_BASE+2
	lda #$42
	sta TIMER_BASE+3
	lda #%00001000
	tsb TIMER_BASE+1
	; set up timer2 interrupt vector
	lda #<timer3_irq_handler_continued
	sta TIMER3_VECTOR
	lda #>timer3_irq_handler_continued
	sta TIMER3_VECTOR+1

	; setup cia interrupt
	lda #%00000001	; set bit 0 in accumulator
	tsb CIA_BASE+1	; turn on keyboard interrupt generation by CIA
	; install the vector for the cia irq routine
	lda #<cia_irq_handler_continued
	sta CIA_VECTOR
	lda #>cia_irq_handler_continued
	sta CIA_VECTOR+1

	cli		; clear irq disable flag (enable all irqs)

				; set colors:
	lda #$06	; blue for background
	sta VICV_BG
	lda #$00	; black for border
	sta VICV_BO
	lda #$0e	; light blue for text color
	sta CURR_TEXT_COLOR

	lda #$00			; store $0000c000 in TXT pointer vicv, and $0000c800 in COL
	sta VICV_TXT
	sta VICV_TXT+1
	sta VICV_TXT+3
	sta VICV_COL
	sta VICV_COL+1
	sta VICV_COL+3
	lda #$c0
	sta VICV_TXT+2
	lda #$c8
	sta VICV_COL+2

	jsr clear_screen

	phw #welc1		; first welcome message
	jsr put_string
	phw #welc2		; second part of message
	jsr put_string

	; play welcome sound
	lda #$82
	sta SID0_BASE+$01	; frequency of voice 1 (high byte)
	lda #$09
	sta SID0_BASE+$05	; attack and decay of voice 1
	lda #$1e
	sta SID0_BASE+$0f	; freq of voice 3
	lda #$0f
	sta SID0_BASE+$18	; max volume
	lda #$ff
	sta SID0_LEFT		; left channel mix
	lda #$10
	sta SID0_RGHT		; right channel mix
	lda #$15
	sta SID0_BASE+$04	; play bell by opening gate on bit0 register #4
                                ; bit 4 is for a triangle wave form
                                ; bit 2 is for a ring modulation connected to voice 3

-	inc $c200	; increase a char in screen memory

	ldx #$00	; init counter to 0
-	lda $0380,x	; load key state
	sta $c400,x	; store it somewhere on screen
	inc x
	cpx #$49	; last key?
	bne -

	bra --

exception_handler
	pha		; save processor state
	phx
	phy
	phz
			; retrieve the pushed status byte from stack
	tsx		; load sl into x
	tsy		; load sh into y
	stx IP0L	; store sp into pointer register IP0
	sty IP0H
	ldy #$05	; index is $5
	lda (IP0L),y	; load the pushed status byte into accumulator
	and #%00010000	; was break "flag" present on stack?
	beq irq_handler	; no, branch to irq handler

	; START OF BRK handler
	; blabla
brk_handler
	nop		; blabla
	nop		; blabla
	bra exception_cleanup

	; START OF IRQ handler
	; all devices that can cause interrupts, will be checked
	; best order to check:
	; (1) VICV
	; (2) timer
	; (3) CIA
irq_handler
	; VICV portion
	;
	; blabla

	; timer portion
timer_irq_handler
	lda TIMER_BASE			; load ISR status
	bpl cia_irq_handler		; did timer cause an interrupt? No: (bit 7 = 0), skip to cia irq handler
timer0_irq_handler
	sta IP4L						; there is an interrupt, store ISR status in basepage
	bbr 0,IP4L,timer1_irq_handler	; If not caused by timer 0, skip to timer 1
	lda #%00000001
	sta TIMER_BASE			; acknowledge
	jmp (TIMER0_VECTOR)
timer0_irq_handler_continued
	lda $c800
	inc a
	and #%00001111
	sta $c800
	jmp exception_cleanup
timer1_irq_handler
	bbr 1,IP4L,timer2_irq_handler
	lda #%00000010
	sta TIMER_BASE		; acknowledge
	jmp (TIMER1_VECTOR)
timer1_irq_handler_continued
	lda $c801
	inc a
	and #%00001111
	sta $c801
	jmp exception_cleanup
timer2_irq_handler
	bbr 2,IP4L,timer3_irq_handler
	lda #%00000100
	sta TIMER_BASE		; acknowledge
	jmp (TIMER2_VECTOR)
timer2_irq_handler_continued
	lda $c802
	inc a
	and #%00001111
	sta $c802
	jmp exception_cleanup
timer3_irq_handler
	;bbr 3,IP4L,exception_cleanup	; it must be timer3!
	lda #%00001000
	sta TIMER_BASE		; acknowledge
	jmp (TIMER3_VECTOR)
timer3_irq_handler_continued
	lda $c804
	inc a
	and #%00001111
	sta $c804
	jmp exception_cleanup

	; CIA portion
cia_irq_handler
	lda CIA_BASE		; load ISR status
	bpl exception_cleanup	; did CIA cause the interrupt? No (bit 7 = 0), skip to cleanup

	lda #%00000001	; acknowledge the interrupt
	sta CIA_BASE

	jmp (CIA_VECTOR)	; jmp
cia_irq_handler_continued
	lda CIA_BASE		; if bit 0 is on, a keyboard event is waiting
	and #%00000001
	beq exception_cleanup	; no, skip the next part
	lda CIA_BASE+2		; read a scancode
	bmi cia_irq_handler_continued	; if bit 7 is set, check for a next event
	tax			; move scancode into x register
	lda scancode_to_ascii,x	; lookup corresponding ascii value in table
	jsr put_char
	bra cia_irq_handler_continued
	jmp exception_cleanup

exception_cleanup
	plz		; cleanup stuff of the exception handler
	ply		; restore processor state
	plx
	pla
	rti		; return from interrupt

nmi_handler
	pha
	phx
	phy
	phz
	; bla
	; bla

	plz
	ply
	plx
	pla
	rti

clear_screen
	lda #$00
	sta P0
	sta P1
	lda #$c0
	sta P0+1
	lda #$c8
	sta P1+1
	ldy #$00
-	lda #ASCII_SPACE
	sta (P0),y
	lda CURR_TEXT_COLOR	; load current text color from ram
	sta (P1),y
	inc y
	bne -
	inc P0+1
	inc P1+1
	ldx P0+1
	cpx #$c8
	bne -
	rts

; put_char expects an ascii value in accumulator
; it will print the character at the current cursor position and increase the cursor position
put_char
	phx
	phy
	cmp #ASCII_LF
	beq +
	tax
	lda ascii_to_screencode,x
	ldy CURSOR_POS
	sta $c000,y
	lda CURR_TEXT_COLOR
	sta $c800,y
	inc y
	sty CURSOR_POS
	ply
	plx
	rts
+	lda CURSOR_POS		; print LF
	clc
	adc #$40		; add 64 chars to current position
	and #%11000000		; move to the first char of the line
	sta CURSOR_POS
	ply
	plx
	rts

; put_string
; this routine expects an address of the string to be pushed onto the stack
; at the end the rtn # instruction is used to clean up the stack
put_string
	ldy #$00
-	lda (#$03,s),y
	beq +
	jsr put_char
	inc y
	bra -
+	rtn #$02

; strings
welc1	.text "E64 (C)2019",ASCII_NULL
welc2	.text " kernel version 0.1.20191205",ASCII_NULL

	* = $ff00
	.include "E64_kernel_rom_tables.asm"

; end of 8k rom, three vectors:
	* = $fffa
nmi_vector
	.word nmi_handler
reset_vector
	.word cold_start
brk_vector
	.word exception_handler
