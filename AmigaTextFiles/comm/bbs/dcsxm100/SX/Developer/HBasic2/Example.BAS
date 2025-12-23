'
' SYSTEM-X Door Written in HBasic2!!
'

REM $NOWINDOW

LIBRARY DECLARE "AEDoor.library"
DECLARE FUNCTION CreateComm& LIBRARY
DECLARE SUB DeleteComm& LIBRARY
DECLARE SUB WriteStr& LIBRARY
DECLARE FUNCTION HotKey& LIBRARY
DECLARE FUNCTION Prompt& LIBRARY
DECLARE FUNCTION GetString& LIBRARY
DECLARE FUNCTION GetData& LIBRARY
DECLARE SUB SendStrCmd& LIBRARY
DECLARE SUB SendStrDataCmd& LIBRARY
LIBRARY OPEN "AEDoor.library",2

cr$=CHR$(13)+CHR$(10)
DIM SHARED dif&,SX
ON ERROR GOTO exitt

IF COMMAND$="" OR VAL(COMMAND$)=0 THEN skip
dif& = CreateComm&(0)
skip:IF dif& = 0 THEN ?"This program requires AmiExpress BBS Software.":END
strf& = GetString(dif&)
IF PEEK$(strf&) = "SX" THEN SX=1 ELSE SX=0

'#######################################################################
'-- START --------------------------------------------------------------
'#######################################################################

IF SX
	PS cr$+"This door is running Under SYSTEM-X!"+cr$+cr$
ELSE
	PS cr$+"This door is running Under AmiExpress!"+cr$+cr$
END IF

PS "Testing testing!!.."+cr$+cr$

PS "Enter some text: "
InputMsg st$,25

PS "You entered: "+st$+cr$+cr$


'#######################################################################
'-- END ----------------------------------------------------------------
'#######################################################################

exitt:
DeleteComm& dif&
SYSTEM

SUB PS (st$)
	IF SX
		SendStrDataCmd& dif&, 1500, 0, SADD(st$+CHR$(0))
	ELSE
		WriteStr& dif&,SADD(st$+CHR$(0)),2
	END IF
	END SUB

SUB Hot(s&)
	s& = HotKey&(dif&,SADD(CHR$(0)))
	IF s& = -1 THEN exitt
	END SUB

SUB InputMsg (st$,l%)
	x& = Prompt&(dif&,l%,SADD(CHR$(0)))
	IF x& = 0 THEN exitt
	st$ = PEEK$(x&)
	END SUB
