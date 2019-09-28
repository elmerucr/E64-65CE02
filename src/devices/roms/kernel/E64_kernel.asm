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

	; $189e gaat, $189f complete crash...
	; set up timer interrupt
	lda #$9e		; load value 3000 ($0bb8 = 3000bpm = 50Hz) into low and high bytes
	sta TIMER_BASE+2
	lda #$18
	sta TIMER_BASE+3
	lda TIMER_BASE+1	; turn on interrupt generation by clock0
	;ora #%00000000
	ora #%00000001
	sta TIMER_BASE+1

	lda #<timer_irq_handler_continued
	sta TIMER0_VECTOR
	lda #>timer_irq_handler_continued
	sta TIMER0_VECTOR+1

	; setup cia interrupt
	lda #$01	; set bit 0 in accumulator
	tsb CIA_BASE+1	; turn on keyboard interrupt generation by CIA
	; install the vector for the cia irq routine
	lda #<cia_irq_handler_continued
	sta CIA_VECTOR
	lda #>cia_irq_handler_continued
	sta CIA_VECTOR+1

	cli		; clear irq disable flag (enable irqs)

			; set colors:
	lda #$06	; blue for background
	sta VICV_BG
	lda #$00	; black for border
	sta VICV_BO
	lda #$0c	; grey for text color
	sta CURR_TEXT_COLOR

	lda #$00
	sta VICV_TSH	; point vicv to text and color screen
	sta VICV_CSH
	lda #$18
	sta VICV_TSL
	lda #$19
	sta VICV_CSL

	jsr clear_screen

	phw #welc1	; push the address of the first welcome message onto the stack
	jsr put_string
	phw #welc2
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
	lda #$85
	sta SID0_BASE+$04	; play bell by opening gate on bit0 register #4
                                ; bit 4 is for a triangle wave form
                                ; bit 2 is for a ring modulation connected to voice 3

-	lda $c200	; load a char from screen memory
	inc a		; increase value of accumulator
	sta $c200	; store it back

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
	stx P0		; store sp into pointer register P0
	sty P0+1
	ldy #$05	; index is $5
	lda (P0),y	; load the pushed status byte into accumulator
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
	lda TIMER_BASE		; load ISR status
	and #%10000000		; did TIMER cause the interrupt?
	beq cia_irq_handler	; no, skip to cia irq handler

	lda #%00000001		; acknowledge the interrupt
	sta TIMER_BASE

	jmp (TIMER0_VECTOR)
timer_irq_handler_continued
	inc VICV_BG

	jmp exception_cleanup

	; blabla
	; jmp exception_cleanup


	; CIA portion
cia_irq_handler
	lda CIA_BASE		; load ISR status
	and #%10000000		; did CIA cause the interrupt?
	beq exception_cleanup	; no, skip to cleanup

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
welc1	.text "E64 kernel V20190928",ASCII_NULL
welc2	.text " (C)2019 elmerucr",ASCII_NULL

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
