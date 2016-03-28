#include<_DEFINE.asm>
?PR?TURNING SEGMENT CODE;
PUBLIC TURNING;
RSEG ?PR?TURNING;
TURNING:		;INITALIZATION
	LCALL DELAY500US;		//delay5ms
	LCALL DELAY500US;	
	LCALL DELAY500US;
	LCALL DELAY500US;
	LCALL DELAY500US;
	LCALL DELAY500US;
	LCALL DELAY500US;
	LCALL DELAY500US;
	LCALL DELAY500US;
	LCALL DELAY500US;
	
	MOV SP,#80H;
	MOV AUXR,0X8E;		  
	MOV 93H,#0FFH;	P0M1
	MOV 94H,#0FFH; 	P0M0
	MOV 95H,#0FFH; 	P2M1
	MOV 96H,#0FFH;	P2M0

	MOV A,#00H;

;=========test========
	MOV NCYC01,#1;	 +
	MOV NCYCT01,#1;	 +
	MOV DR01,#45;	 +
	MOV TDR01,#45;	 +
				;	 +
	MOV NCYC02,#1;	 +
	MOV NCYCT02,#1;	 +
	MOV DR02,#45;	 +
	MOV TDR02,#45;	 +
				;	 +
	MOV NCYC03,#1;	 +
	MOV NCYCT03,#1;	 +
	MOV DR03,#45;	 +
	MOV TDR03,#45;	 +
				;	 +
	MOV NCYC04,#1;	 +
	MOV NCYCT04,#1;	 +
	MOV DR04,#45;	 +
	MOV TDR04,#45; 	 +
;--------------------+	
	MOV NCYC05,#1;	 +
	MOV NCYCT05,#1;	 +
	MOV DR05,#30;	 +
	MOV TDR05,#30;	 +
				;	 +
	MOV NCYC06,#1;	 +
	MOV NCYCT06,#1;	 +
	MOV DR06,#30;	 +
	MOV TDR06,#30;	 +
				;	 +
	MOV NCYC07,#1;	 +
	MOV NCYCT07,#1;	 +
	MOV DR07,#30;	 +
	MOV TDR07,#30;	 +
				;	 +
	MOV NCYC08,#1;	 +
	MOV NCYCT08,#1;	 +
	MOV DR08,#30;	 +
	MOV TDR08,#30; 	 +
;--------------------+	
	MOV NCYC09,#1;	 +
	MOV NCYCT09,#1;	 +
	MOV DR09,#30;	 +
	MOV TDR09,#30;	 +
				;	 +
	MOV NCYC10,#1;	 +
	MOV NCYCT10,#1;	 +
	MOV DR10,#30;	 +
	MOV TDR10,#30;	 +
				;	 +
	MOV NCYC11,#1;	 +
	MOV NCYCT11,#1;	 +
	MOV DR11,#30;	 +
	MOV TDR11,#30;	 +
				;	 +
	MOV NCYC12,#1;	 +
	MOV NCYCT12,#1;	 +
	MOV DR12,#30;	 +
	MOV TDR12,#30; 	 +
				   ; +
	MOV CTDRP01,#21; +	
	MOV CTDRP02,#61; +
	MOV CTDRP03,#81; +
	MOV CTDRP04,#41; +
				   ; +
	MOV CTDRP05,#1;	 +	<---CAN'T BE ZERO!!!
	MOV CTDRP06,#41; +
	MOV CTDRP07,#61; +
	MOV CTDRP08,#21; +
	MOV CTDRP09,#1;	 +	<----OR WILL MAKE A BUG!!!
	MOV CTDRP10,#41; +
	MOV CTDRP11,#61; +
	MOV CTDRP12,#21; +
				;	 +
	MOV DRE01,#0;	 +
	MOV DRE02,#1;	 +
	MOV DRE03,#0;	 +
	MOV DRE04,#1;	 +

	MOV DRE05,#0;	 +
	MOV DRE06,#1;	 +
	MOV DRE07,#1;	 +
	MOV DRE08,#0;	 +
	
	MOV DRE09,#0;	 +
	MOV DRE10,#1;	 +
	MOV DRE11,#1;	 +
	MOV DRE12,#0;	 +
;========TEST=========

	
;-----------------------------SER1-----------------------------
SER1:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P0.0;
	INC NCYCT01;
	DJNZ NCYCT01,GOON12;			IF NCYCT IS NOT ZERO
		MOV NCYCT01,NCYC01;			ADD OMEGA PER NCYC PIECES
		MOV A,DR01
		CJNE A,TDR01,GOON1;
			AJMP GOON12;
GOON1:
	CLR C;
	SUBB A,TDR01;
	JC ADD1;
		DEC DR01;
		AJMP GOON12;
	ADD1:
		INC DR01;
GOON12:
	DEC NCYCT01;						
	ACALL DELAY500US;		DELAY 0.5 MS
	MOV DRT01,DR01;
LOOP11:
	MOV A,DRT01;
	JZ LOOP12;
		DEC DRT01;
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	AJMP LOOP11;
LOOP12:
		MOV P0,#00H;				END THE HIGHT SIGNAL
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP12;
	
	MOV CYCT,#90;				RELOAD CYCT
;-----------------------------SER2-----------------------------
SER2:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P0.1;
	INC NCYCT02;
	DJNZ NCYCT02,GOON22;
		MOV NCYCT02,NCYC02;			ADD OMEGA PER NCYC PIECES
		MOV A,DR02;
		CJNE A,TDR02,GOON2;
			AJMP GOON22;
GOON2:
	CLR C;
	SUBB A,TDR02;
	JC ADD2;
		DEC DR02;
		AJMP GOON22;
	ADD2:
		INC DR02;
GOON22:
	DEC NCYCT02;				
	ACALL DELAY500US;			ACALL DELAY500US;		
	MOV DRT02,DR02;
LOOP21:
	MOV A,DRT02;
	JZ LOOP22;
		DEC DRT02;
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	AJMP LOOP21
LOOP22:
	MOV P0,#00H;				END THE HIGHT SIGNAL
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP22;
	
	MOV CYCT,#90;				RELOAD CYCT
;-----------------------------SER3-----------------------------
SER3:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P0.2;
	INC NCYCT03;
	DJNZ NCYCT03,GOON32;
		MOV NCYCT03,NCYC03;			ADD OMEGA PER NCYC PIECES
		MOV A,DR03;
		CJNE A,TDR03,GOON3;
	   		AJMP GOON32;
GOON3:
	CLR C;
	SUBB A,TDR03;
	JC ADD3;
		DEC DR03
		AJMP GOON32
	ADD3:
		INC DR03;
GOON32:
	DEC NCYCT03;
	ACALL DELAY500US;			DELAY 0.5 MS
	MOV DRT03,DR03;
LOOP31:
	MOV A,DRT03;
	JZ LOOP32;
		DEC DRT03;
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	AJMP LOOP31;
LOOP32:
	MOV P0,#00H;				END THE HIGHT SIGNAL
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP32;
	
	MOV CYCT,#90;				RELOAD CYCT
;-----------------------------SER4-----------------------------
SER4:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P0.3;
	INC NCYCT04;
	DJNZ NCYCT04,GOON42;
		MOV NCYCT04,NCYC04;			ADD OMEGA PER NCYC PIECES
		MOV A,DR04;
		CJNE A,TDR04,GOON4;
			AJMP GOON42;
GOON4:
	CLR C;
	SUBB A,TDR04;
	JC ADD4;
		DEC DR04;
		AJMP GOON42;
	ADD4:
		INC DR04;
GOON42:
	DEC NCYCT04;				
	ACALL DELAY500US; 			DELAY 0.5 MS
	MOV DRT04,DR04;
LOOP41:
	MOV A,DRT04;
	JZ LOOP42;
		DEC DRT04;
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	AJMP LOOP41;
LOOP42:
	MOV P0,#00H;				END THE HIGHT SIGNAL
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP42;
	
	MOV CYCT,#90;				RELOAD CYCT
;================================END OF THE MAIN SERVERO=====================

;--------------------------------SER5,6--------------------------------------
SER56:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P2.0;
	SETB P2.1;
	INC NCYCT05;
	DJNZ NCYCT05,GOON52;
		MOV NCYCT05,NCYC05;			ADD OMEGA PER NCYC PIECES
		MOV A,DR05;
		CJNE A,TDR05,GOON5;
	   		AJMP GOON52;
GOON5:
	CLR C;
	SUBB A,TDR05;
	JC ADD5;
		DEC DR05;
		AJMP GOON52;
	ADD5:
		INC DR05;
GOON52:
	DEC NCYCT05;
	INC NCYCT06;	
	DJNZ NCYCT06,GOON62;
		MOV NCYCT06,NCYC06;			ADD OMEGA PER NCYC PIECES
		MOV A,DR06;
		CJNE A,TDR06,GOON6;
			AJMP GOON62;
GOON6:
	CLR C;
	SUBB A,TDR06;
	JC ADD6;
		DEC DR06;
		AJMP GOON62;
	ADD6:
		INC DR06;	
GOON62:
	DEC NCYCT06;
	ACALL DELAY500US;
	MOV DRT05,DR06;
	MOV DRT06,DR06;

LOOP561:
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	
	MOV A,DRT05;				IF DRT5 == 0				
	JZ 	CASE56_5;
	
	MOV A,DRT06; 				IF DRT6 == 0
	JZ 	CASE56_6;

	DEC DRT05;
	DEC DRT06;

	AJMP LOOP561;				LOOP IF BOTH SRE NOT 0

CASE56_5:
		CLR P2.0;
	MOV A,DRT06;
	JZ LOOP562;
		DEC DRT06;
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE56_5;	
CASE56_6:
		CLR P2.1;
	MOV A,DRT05;
	JZ LOOP562;
		DEC DRT05;
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE56_6;
LOOP562:
	MOV P0,#00H;
	MOV P2,#00H;
LOOP563:
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP563;
	
	MOV CYCT,#90;				RELOAD CYCT
;--------------------------------SER7,8--------------------------------------
SER78:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P2.2;
	SETB P2.3;
	INC NCYCT07;
	DJNZ NCYCT07,GOON72;
		MOV NCYCT07,NCYC07;			ADD OMEGA PER NCYC PIECES
		MOV A,DR07;
		CJNE A,TDR07,GOON7;
	   		AJMP GOON72;
GOON7:
	CLR C;
	SUBB A,TDR07;
	JC ADD7;
		DEC DR07;
		AJMP GOON72;
	ADD7:
		INC DR07;
GOON72:
	DEC NCYCT07;
	INC NCYCT08;
	DJNZ NCYCT08,GOON82;
		MOV NCYCT08,NCYC08;			ADD OMEGA PER NCYC PIECES
		MOV A,DR08;
		CJNE A,TDR08,GOON8;
			AJMP GOON82;
GOON8:
	CLR C;
	SUBB A,TDR08;
	JC ADD8;
		DEC DR08;
		AJMP GOON82;
	ADD8:
		INC DR08;	
GOON82:
	DEC NCYCT08;
	ACALL DELAY500US;
	MOV DRT07,DR07;
	MOV DRT08,DR08;

LOOP781:
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	
	MOV A,DRT07;				IF DRT5 == 0				
	JZ 	CASE78_7;
	
	MOV A,DRT08; 				IF DRT6 == 0
	JZ 	CASE78_8;

	DEC DRT07;
	DEC DRT08;

	AJMP LOOP781;				LOOP IF BOTH SRE NOT 0

CASE78_7:		 
		CLR P2.2;
	MOV A,DRT08;
	JZ LOOP782;
		DEC DRT08
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE78_7;	
CASE78_8:
		CLR P2.3;
	MOV A,DRT07;
	JZ LOOP782;
		DEC DRT07;
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE78_8;
LOOP782:
	MOV P0,#00H;
	MOV P2,#00H;
LOOP783:
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP783;
	
	MOV CYCT,#90;				RELOAD CYCT
;--------------------------------SER9,10--------------------------------------
SER910:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P2.4;
	SETB P2.5;
	INC NCYCT09
	DJNZ NCYCT09,GOON92;
		MOV NCYCT09,NCYC09;			ADD OMEGA PER NCYC PIECES
		MOV A,DR09;
		CJNE A,TDR09,GOON9;
	   		AJMP GOON92;
GOON9:
	CLR C;
	SUBB A,TDR09;
	JC ADD9;
		DEC DR09;
		AJMP GOON92;
	ADD9:
		INC DR09;
GOON92:
	DEC NCYCT09;
	INC NCYCT10;
	DJNZ NCYCT10,GOON10;
		MOV NCYCT10,NCYC10;			ADD OMEGA PER NCYC PIECES
		MOV A,DR10;
		CJNE A,TDR10,GOON10;
			AJMP GOON102;
GOON10:
	CLR C;
	SUBB A,TDR10;
	JC ADD10;
		DEC DR10;
		AJMP GOON102;
	ADD10:
		INC DR10;	
GOON102:
	DEC NCYCT10;
	ACALL DELAY500US;
	MOV DRT09,DR09;
	MOV DRT10,DR10;

LOOP9101:
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	
	MOV A,DRT09;				IF DRT5 == 0				
	JZ 	CASE910_9;
	
	MOV A,DRT10; 				IF DRT6 == 0
	JZ 	CASE910_10;

	DEC DRT09;
	DEC DRT10;

	AJMP LOOP9101;				LOOP IF BOTH SRE NOT 0

CASE910_9:
		CLR P2.4;				SER09
	MOV A,DRT10;
	JZ LOOP9102;
		DEC DRT10;
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE910_9;	
CASE910_10:
		CLR P2.5;				SER10
	MOV A,DRT09;
	JZ LOOP9102;
		DEC DRT09;	
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE910_10;
LOOP9102:
	MOV P0,#00H;
	MOV P2,#00H;
LOOP9103:
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP9103;
	
	MOV CYCT,#90;				RELOAD CYCT
;--------------------------------SER11,12--------------------------------------
SER1112:
	MOV P0,#00H;
	MOV P2,#00H;
	SETB P2.6;
	SETB P2.7;

	INC NCYCT11;
	DJNZ NCYCT11,GOON112;
		MOV NCYCT11,NCYC11;			ADD OMEGA PER NCYC PIECES
		MOV A,DR11;
		CJNE A,TDR11,GOON11;
	   		AJMP GOON112;
GOON11:
	CLR C;
	SUBB A,TDR11;
	JC ADD11;
		DEC DR11;
		AJMP GOON112;
	ADD11:
		INC DR11;
GOON112:
	DEC NCYCT11;
	INC NCYCT12;
	DJNZ NCYCT12,GOON122;
		MOV NCYCT12,NCYC12;			ADD OMEGA PER NCYC PIECES
		MOV A,DR12;
		CJNE A,TDR12,GOON12_;
			AJMP GOON122;
GOON12_:
	CLR C;
	SUBB A,TDR12;
	JC ADD12;
		DEC DR12;
		AJMP GOON122;
	ADD12:
		INC DR12;	
GOON122:
	DEC NCYCT12;
	ACALL DELAY500US;
	MOV DRT11,DR11;
	MOV DRT12,DR12;

LOOP11121:
		DEC CYCT;		
		ACALL DELAY_1_PIECES;
	
	MOV A,DRT11;				IF DRT5 == 0				
	JZ 	CASE1112_11;
	
	MOV A,DRT12; 				IF DRT6 == 0
	JZ 	CASE1112_12;

	DEC DRT11;
	DEC DRT12;
	
	AJMP LOOP11121;				LOOP IF BOTH SRE NOT 0

CASE1112_11:
		CLR P2.6;				SER09
	MOV A,DRT12;
	JZ LOOP11122;
		DEC DRT12;
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE1112_11;
CASE1112_12:
		CLR P2.7;				SER10
	MOV A,DRT11;
	JZ LOOP11122
		DEC DRT11;
		DEC CYCT;
		ACALL DELAY_1_PIECES;
	AJMP CASE1112_12;
LOOP11122:
	MOV P0,#00H;
	MOV P2,#00H;
LOOP11123:
		ACALL DELAY_1_PIECES;
	DJNZ CYCT,LOOP11123;
	
	MOV CYCT,#90;				RELOAD CYCT
	
;-----------------CHANGE THE TDR-----------------------
MOV R0,#68H;			CTDRP01
CHANGE_MAIN:	;			THE FOUR MAIN SEVERO
	MOV A,@R0;
	JZ C1;					IF NOT RICHED THE TIME
	 	DEC A;
		MOV @R0,A;
		AJMP CEE;		
	C1:
		MOV @R0,#40;		RESET THE CTDRP

		MOV A,R0; 			MOVE TO DIRECTION BIT
		ADD A,#12;
		MOV R0,A;

		MOV A,@R0;
		JZ	DOWN;
			MOV @R0,#0;
			MOV A,R0;
			SUBB A,#84;		GO UP
			MOV R0,A;

			MOV A,#67;		MAIN_H
			MOV @R0,A;
			AJMP CE;		
		DOWN:		;		GO DOWN
			MOV @R0,#1;
			MOV A,R0;
			SUBB A,#84;
			MOV R0,A;

			MOV A,#45;		MAIN_L
			MOV @R0,A;  
	CE: ;					END
	MOV A,R0;
	ADD A,#72;				RECOVER FROM THE TDR
	MOV R0,A;
	CEE: 
	INC R0;
	CJNE R0,#6CH,CHANGE_MAIN;

CHANGE_SUB:	;			THE FOUR MAIN SEVERO
	MOV A,@R0;
	JZ C2;					IF NOT RICHED THE TIME
	 	DEC A;
		MOV @R0,A;
		AJMP CEE2;		
	C2:
		MOV @R0,#40;			RESET THE CTDRP		

		MOV A,R0;
		ADD A,#12;
		MOV R0,A;

		MOV A,@R0;
		JZ	DOWN2;
			MOV @R0,#0;
			MOV A,R0;
			SUBB A,#84;		GO UP
			MOV R0,A;

			MOV A,#30;		SUB_H
			MOV @R0,A;
			AJMP CE2;		
		DOWN2:		;		GO DOWN
			MOV @R0,#1;
			MOV A,R0;
			SUBB A,#84;
			MOV R0,A;

			MOV A,#14;		SUB_L
			MOV @R0,A;  
	CE2: ;					END
	MOV A,R0;
	ADD A,#72;
	MOV R0,A;
	CEE2: 
	INC R0;
	CJNE R0,#74H,CHANGE_SUB;
	
	
LJMP SER1;
;=========================================CYCLE=============================================


DELAY_1_PIECES:			;REPEAT 66 T
	MOV R2,#162;
LOOPING:
	DJNZ R2,LOOPING;
RET

DELAY500US:
	MOV R2,#30;
LOOPING1:
	MOV R1,#125;
	DJNZ R1,$;
	DJNZ R2,LOOPING1;
RET	
								   
END

	 
	
	
	
	