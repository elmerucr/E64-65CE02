; c256 definitions - 2019-11-21 elmerucr

; ascii definitions
NULL	equ	$00
LF		equ	$0a

; cia
CIA_BASE	equ	$ffff0300

; vicv
VICV_BASE	equ	$ffff0400 	; vicv base register
VICV_TXT	equ VICV_BASE+$02
VICV_COL	equ VICV_BASE+$06

; sound
SOUND_BASE	equ $ffff0500	; sound base register
SID0_BASE	equ	SOUND_BASE
SID1_BASE	equ	SOUND_BASE+$20
; sound indices
SID0_LEFT	equ $80
SID0_RGHT	equ $81

CURR_TEXT_COLOR	equ $00008000
