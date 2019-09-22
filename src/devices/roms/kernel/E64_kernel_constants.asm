	; zero page
	cursor_pos = $00

	P0 = $f0	; pointer for general use
	P1 = $f2	; pointer for general use
	P2 = $f4	; pointer for general use
	P3 = $f6	; pointer for general use
	P4 = $f8	; pointer for general use
	P5 = $fa	; pointer for general use
	P6 = $fc	; pointer for general use
	P7 = $fe	; pointer for general use

	; kernel ram area
	TIMER0_VECTOR = $0800
	TIMER1_VECTOR = $0802
	TIMER2_VECTOR = $0804
	TIMER3_VECTOR = $0806
	CIA_VECTOR    = $0808



	CIA_BASE = $0300

	VICV_BASE = $0400
	VICV_BO = VICV_BASE + $00
	VICV_BG = VICV_BASE + $01
	VICV_TSL = VICV_BASE + $02
	VICV_TSH = VICV_BASE + $03
	VICV_CSL = VICV_BASE + $04
	VICV_CSH = VICV_BASE + $05

	SID0_BASE = $0500
	SID0_LEFT = $0580
	SID0_RGHT = $0581
	SID1_BASE = $0520
	SID1_LEFT = $0582
	SID1_RGHT = $0583

	ASCII_NULL	= $00
	ASCII_BKSPC	= $08
	ASCII_LF	= $0a
	ASCII_CR	= $0d
	ASCII_SPACE	= $20
	ASCII_DEL	= $7f