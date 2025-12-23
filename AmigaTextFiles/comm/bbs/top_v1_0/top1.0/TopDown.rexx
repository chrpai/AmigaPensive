/* XTOP! Downloaders Module! */
/* L'ordine e` : 1- Nome, 2 - Bytes,3 - Number of Files */

Options Results
Signal On Syntax

patt = 'BBS:Text/New_TopDown'

If ~Exists(Patt) Then Exit

K = Open(File,Patt,'R')

Do I = 1 To 10
	Do P = 1 To 4
		ULIST.I.P = Readln(File)
	End
End
K = Close(File)

GetUser 1
USER.1 = Result
GetUser 36
USER.2 = Result
GetUser 37
USER.3 = Result
GetUser 4
USER.4 = Left(Result,12,' ')||'\C6 ณ\C7 '
GetUser 44
CPS = Result
BAUD
LPL = Result
If CPS = 0 Then DO
	CPS = LPL % 10
	CPS = CPS + CPS % 7
END
USER.4 = USER.4||Right(CPS,4)||'\C6 ณ \C7'
BAUD
BPS = Result
USER.4 = USER.4||Right(BPS,5)' \C6'

DO I = 1 To 10
	If ULIST.I.1 = USER.1 Then DO
		POS = I
		DO L = POS TO 9
			C = L + 1
			DO O = 1 To 4
				ULIST.L.O = ULIST.C.O
			END
		END
		I = 10
		POS = 10
		Do P = 1 To 4
			ULIST.POS.P = USER.P
		End
	END
END

DO I = 1 TO 10
	If ULIST.I.2 <= USER.2 Then DO
		POS = I
		I = 10
	END
END

DO Z = POS To 9
	K = 9 - (Z - POS)
	C = K + 1
	DO O = 1 To 4
		ULIST.C.O = ULIST.K.O
	END
END

Do P = 1 To 4
	ULIST.POS.P = USER.P
End

K = OPEN(File,Patt,'W')
Do I = 1 To 10
	Do P = 1 To 4
		G = Writeln(file,ULIST.I.P)
	End
End
K = Close(File)

TRANSMIT '\C2-*- XTop! Best Ten Downloaders -- Programmed By Massimiliano Pala ''95 -*-'
TRANSMIT '\C6ษอออออออออออออออออออออัออออออออออออออออออออัออออออออออออออัออออออัอออออออป'
TRANSMIT '\C6บ \C5Name                \C6ณ\C5 Bytes       Files  \C6ณ\C5 From         \C6ณ\C5 Cps  \C6ณ\C5 Baud  \C6บ'
TRANSMIT '\C6ฬอออออออออออออออออออออุออออออออออออออออออออุออออออออออออออุออออออุอออออออน'
DO I = 1 To 10
	linea = '\C6บ\C7 'Left(ULIST.I.1,20)'\C6ณ\C7 'Right(ULIST.I.2,10)' \C6(\C1'Right(ULIST.I.3,5)'\C6) ณ\C7 '
	Linea = Linea||ULIST.I.4||'บ'
	TRANSMIT Linea
END
TRANSMIT '\C6ฬอออออออออออออออออออออฯออออออออออออออออออออฯออออออออออออออฯออออออฯอออออออน'
TRANSMIT '\C6บ \C2You Have Reached the \C3'Right(POS,2)'\C2 Position in this Classification for DownLoads! \C6บ'
TRANSMIT '\C6ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ'

