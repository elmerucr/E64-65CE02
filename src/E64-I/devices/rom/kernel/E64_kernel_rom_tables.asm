; rom tables at end of rom

ascii_to_screencode
	.byte	$80		; 0x00 - NUL	non printable character
	.byte	$80		; 0x01 - SOH	non printable character
	.byte	$80		; 0x02 - STX	non printable character
	.byte	$80		; 0x03 - ETX	non printable character
	.byte	$80		; 0x04 - EOT	non printable character
	.byte	$80		; 0x05 - ENQ	non printable character
	.byte	$80		; 0x06 - ACK	non printable character
	.byte	$80		; 0x07 - BEL	non printable character
	.byte	$80		; 0x08 - BS	non printable character
	.byte	$80		; 0x09 - TAB	non printable character
	.byte	$80		; 0x0a - LF	non printable character
	.byte	$80		; 0x0b - VT	non printable character
	.byte	$80		; 0x0c - FF	non printable character
	.byte	$80		; 0x0d - CR	non printable character
	.byte	$80		; 0x0e - SO	non printable character
	.byte	$80		; 0x0f - SI	non printable character
	.byte	$80		; 0x10 - DLE	non printable character
	.byte	$80		; 0x11 - DC1	non printable character
	.byte	$80		; 0x12 - DC2	non printable character
	.byte	$80		; 0x13 - DC3	non printable character
	.byte	$80		; 0x14 - DC4	non printable character
	.byte	$80		; 0x15 - NAC	non printable character
	.byte	$80		; 0x16 - SYN	non printable character
	.byte	$80		; 0x17 - ETB	non printable character
	.byte	$80		; 0x18 - CAN	non printable character
	.byte	$80		; 0x19 - EM	non printable character
	.byte	$80		; 0x1a - SUB	non printable character
	.byte	$80		; 0x1b - ESC	non printable character
	.byte	$80		; 0x1c - FS	non printable character
	.byte	$80		; 0x1d - GS	non printable character
	.byte	$80		; 0x1e - RS	non printable character
	.byte	$80		; 0x1f - US	non printable character
	.byte	$20		; 0x20 - (space)
	.byte	$21		; 0x21 - !
	.byte	$22		; 0x22 - "
	.byte	$23		; 0x23 - #
	.byte	$24		; 0x24 - $
	.byte	$25		; 0x25 - %
	.byte	$26		; 0x26 - &
	.byte	$27		; 0x27 - '
	.byte	$28		; 0x28 - (
	.byte	$29		; 0x29 - )
	.byte	$2a		; 0x2a - *
	.byte	$2b		; 0x2b - +
	.byte	$2c		; 0x2c - ,
	.byte	$2d		; 0x2d - -
	.byte	$2e		; 0x2e - .
	.byte	$2f		; 0x2f - /
	.byte	$30		; 0x30 - 0
	.byte	$31		; 0x31 - 1
	.byte	$32		; 0x32 - 2
	.byte	$33		; 0x33 - 3
	.byte	$34		; 0x34 - 4
	.byte	$35		; 0x35 - 5
	.byte	$36		; 0x36 - 6
	.byte	$37		; 0x37 - 7
	.byte	$38		; 0x38 - 8
	.byte	$39		; 0x39 - 9
	.byte	$3a		; 0x3a - :
	.byte	$3b		; 0x3b - ;
	.byte	$3c		; 0x3c - <
	.byte	$3d		; 0x3d - =
	.byte	$3e		; 0x3e - >
	.byte	$3f		; 0x3f - ?
	.byte	$00		; 0x40 - @
	.byte	$41		; 0x41 - A
	.byte	$42		; 0x42 - B
	.byte	$43		; 0x43 - C
	.byte	$44		; 0x44 - D
	.byte	$45		; 0x45 - E
	.byte	$46		; 0x46 - F
	.byte	$47		; 0x47 - G
	.byte	$48		; 0x48 - H
	.byte	$49		; 0x49 - I
	.byte	$4a		; 0x4a - J
	.byte	$4b		; 0x4b - K
	.byte	$4c		; 0x4c - L
	.byte	$4d		; 0x4d - M
	.byte	$4e		; 0x4e - N
	.byte	$4f		; 0x4f - O
	.byte	$50		; 0x50 - P
	.byte	$51		; 0x51 - Q
	.byte	$52		; 0x52 - R
	.byte	$53		; 0x53 - S
	.byte	$54		; 0x54 - T
	.byte	$55		; 0x55 - U
	.byte	$56		; 0x56 - V
	.byte	$57		; 0x57 - W
	.byte	$58		; 0x58 - X
	.byte	$59		; 0x59 - Y
	.byte	$5a		; 0x5a - Z
	.byte	$1b		; 0x5b - [
	.byte	$5c		; 0x5c - \	patched
	.byte	$1d		; 0x5d - ]
	.byte	$5e		; 0x5e - ^	patched
	.byte	$5f		; 0x5f - _	patched
	.byte	$63		; 0x60 - `	patched
	.byte	$01		; 0x61 - a
	.byte	$02		; 0x62 - b
	.byte	$03		; 0x63 - c
	.byte	$04		; 0x64 - d
	.byte	$05		; 0x65 - e
	.byte	$06		; 0x66 - f
	.byte	$07		; 0x67 - g
	.byte	$08		; 0x68 - h
	.byte	$09		; 0x69 - i
	.byte	$0a		; 0x6a - j
	.byte	$0b		; 0x6b - k
	.byte	$0c		; 0x6c - l
	.byte	$0d		; 0x6d - m
	.byte	$0e		; 0x6e - n
	.byte	$0f		; 0x6f - o
	.byte	$10		; 0x70 - p
	.byte	$11		; 0x71 - q
	.byte	$12		; 0x72 - r
	.byte	$13		; 0x73 - s
	.byte	$14		; 0x74 - t
	.byte	$15		; 0x75 - u
	.byte	$16		; 0x76 - v
	.byte	$17		; 0x77 - w
	.byte	$18		; 0x78 - x
	.byte	$19		; 0x79 - y
	.byte	$1a		; 0x7a - z
	.byte	$64		; 0x7b - {	patched
	.byte	$65		; 0x7c - |	patched
	.byte	$66		; 0x7d - }	patched
	.byte	$67		; 0x7e - ~	patched
	.byte	$80		; 0x7f - DEL	non printable character

scancode_to_ascii:		; temporary hack!!!!!!!
	.byte	$80		; C256_SCANCODE_EMPTY
	.byte	$80		; C256_SCANCODE_ESCAPE
    	.byte	$80		; C256_SCANCODE_F1,
	.byte	$80		; C256_SCANCODE_F2,
	.byte	$80		; C256_SCANCODE_F3,
	.byte	$80		; C256_SCANCODE_F4,
	.byte	$80		; C256_SCANCODE_F5,
	.byte	$80		; C256_SCANCODE_F6,
	.byte	$80		; C256_SCANCODE_F7,
	.byte	$80		; C256_SCANCODE_F8,
	.byte	$60		; C256_SCANCODE_GRAVE,
	.byte	$31		; C256_SCANCODE_1,
	.byte	$32		; C256_SCANCODE_2,
	.byte	$33		; C256_SCANCODE_3,
	.byte	$34		; C256_SCANCODE_4,
	.byte	$35		; C256_SCANCODE_5,
	.byte	$36		; C256_SCANCODE_6,
	.byte	$37		; C256_SCANCODE_7,
	.byte	$38		; C256_SCANCODE_8,
	.byte	$39		; C256_SCANCODE_9,
	.byte	$30		; C256_SCANCODE_0,
	.byte	$2d		; C256_SCANCODE_MINUS,
	.byte	$3d		; C256_SCANCODE_EQUALS,
	.byte	$80		; C256_SCANCODE_BACKSPACE,
	.byte	$80		; C256_SCANCODE_TAB,
	.byte	$71		; C256_SCANCODE_Q,
	.byte	$77		; C256_SCANCODE_W,
	.byte	$65		; C256_SCANCODE_E,
	.byte	$72		; C256_SCANCODE_R,
	.byte	$74		; C256_SCANCODE_T,
	.byte	$79		; C256_SCANCODE_Y,
	.byte	$75		; C256_SCANCODE_U,
	.byte	$69		; C256_SCANCODE_I,
	.byte	$6f		; C256_SCANCODE_O,
	.byte	$70		; C256_SCANCODE_P,
	.byte	$5b		; C256_SCANCODE_[,
	.byte	$5d		; C256_SCANCODE_],
	.byte	$0a		; C256_SCANCODE_LF,
	.byte	$61		; C256_SCANCODE_A,
	.byte	$73		; C256_SCANCODE_S,
	.byte	$64		; C256_SCANCODE_D,
	.byte	$66		; C256_SCANCODE_F,
	.byte	$67		; C256_SCANCODE_G,
	.byte	$68		; C256_SCANCODE_H,
	.byte	$6a		; C256_SCANCODE_J,
	.byte	$6b		; C256_SCANCODE_K,
	.byte	$6c		; C256_SCANCODE_L,
	.byte	$3b		; C256_SCANCODE_;,

hex_table
	.text	"0123456789abcdef"
