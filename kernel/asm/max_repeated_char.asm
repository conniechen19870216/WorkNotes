;
;
;
;	1. Description: output the maximum length of the consecutive characters
;	2. Usage of data segment:
;		a) HINT
;		b) A/A0/A1 
;		c) B/B0/B1 
;	3. Usage of registers
;          BL is used to store the second number
;
;
	DATA_SEG SEGMENT
	HINT	DB	'***** welcome you !*****', 0DH, 0AH
		DB	'input data:', '$'
         A	DB      ?
        A0	DB      ?
        A1	DB      ?
         B	DB      ?
        B0	DB      ?
        B1	DB      ?
	DATA_SEG ENDS

	STACK_SEG SEGMENT STACK
		  DB	200  DUP(0)
	STACK_SEG ENDS

	CODE_SEG SEGMENT
		ASSUME	CS: CODE_SEG, DS: DATA_SEG, SS: STACK_SEG
	START:	MOV	AX, DATA_SEG	; output the hint
		MOV	DS, AX
		LEA	DX, [HINT]
		MOV	AH, 9
		INT	21H
		MOV	DL, 13		; '\r\n'
		MOV	AH, 2
		INT	21H
		MOV	DL, 10
		INT	21H
		MOV	AH, 1		; read the 1st charactor
		INT	21H
		CMP	AL, 13
		JZ	EXIT
		MOV	[A], AL         ; store the infomation of the 1st charactor
		MOV	[A0], AL
		MOV	[A1], 1
	X1:	MOV	AH, 1           ; read the 2nd charactor and loop if the same as the 1st one
		INT	21H
		CMP	AL, 13
		JZ	STOP
		MOV	[B], AL
		MOV	BL, [B]
		CMP	BL, [A]
		JNZ	X2
		ADD	[A1], 1
		JMP	X1
	X2:	MOV	[B0], BL
		MOV	[B1], 1
	X3:	MOV	AH, 1
		INT	21H
		MOV	[A], AL
		CMP	BL, [A]
		JNZ	X4
		ADD	[B1], 1
		JMP	X3
	X4:	MOV	CL, [B1]
		CMP	CL, [A1]
		JB	Y1
		MOV	CL, [B0]
		MOV	[A0], CL
		MOV	CL, [B1]
		MOV	[A1], CL
	Y1:	CMP	AL, 13
		JZ	STOP
		MOV	[B0], AL
		MOV	[B1], 1
		MOV	[B], AL
		MOV	BL, [B]
		JMP	X3 
	STOP:	MOV	DL, 13
		MOV	AH, 2
		INT	21H
		MOV	DL, 10
		INT	21H
		CMP	[A1], 10
		JB	Z1 
		MOV	AH, 0
		MOV	AL, [A1]
		MOV	BL, 10
		DIV	BL
		ADD	AX, 3030H
		MOV	BX, AX
		MOV	AH, 2
		MOV	DL, BL
		INT	21H
		MOV	DL, BH
		INT	21H
		JMP	Z2
	Z1:	ADD	[A1], 30H
		MOV	DL, [A1]
		INT	21H
		SUB	[A1], 30H
	Z2:	MOV	AH, 2
		MOV	DL, 13
		INT	21H
		MOV	DL, 10
		INT	21H
		MOV	CX, 0
		MOV	CL, [A1]
		MOV	AH, 2
	M:	MOV	DL, [A0]
		INT	21H
		LOOP	M
	EXIT:   MOV	AH, 4CH
		INT	21H
	CODE_SEG ENDS
		 END	START
