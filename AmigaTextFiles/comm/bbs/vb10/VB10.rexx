/*
 * Voting Booth v1.0 for TransAmiga by Roger Clark [December 23, 1991]
 *
 * This version is for TechnoBBS. Nothing major has been changed and the
 * original documentation (VB10.doc) is included. Only the second step of
 * the installation is different for TechnoBBS: Instead of editing cmds
 * files, add following command to one of your menus and compile it.
 * 
 * Command "VOTE"
 * {
 *    dos("rx >NIL: Doors:Poll/VB10.rexx \(node())");
 * }
 *
 * --
 * Janne Siren (jts@krk.fi, BBS: +358-0-505-4201) [April 19, 1994]
 *
 */

ln = Arg(1)

LineName = Con_LineActive(ln)

If LineName = "" Then Exit 10

Address Command
Address Value LineName

SetStatus "Voting Booth"

Name = GetUserName(ln)

BBSName		= "Epsilon Indi BBS"

VBData		= "Doors:Poll/VBData"
VBResults	= "Doors:Poll/VBResults"
VBUser		= "Doors:Poll/_"||Name

ESC	= '1B'X
BRIGHT	= ESC||'[1m'
NOANSI	= ESC||'[0m'

f0 = ESC||'[30m' ;b0 = ESC||'[40m'  /* Black  */
f1 = ESC||'[31m' ;b1 = ESC||'[41m'  /* Red    */
f2 = ESC||'[32m' ;b2 = ESC||'[42m'  /* Green  */
f3 = ESC||'[33m' ;b3 = ESC||'[43m'  /* Yellow */
f4 = ESC||'[34m' ;b4 = ESC||'[44m'  /* Blue   */
f5 = ESC||'[35m' ;b5 = ESC||'[45m'  /* Purple */
f6 = ESC||'[36m' ;b6 = ESC||'[46m'  /* Cyan   */
f7 = ESC||'[37m' ;b7 = ESC||'[47m'  /* White  */

CR	= D2C(13)
LF	= D2C(10)
CLS	= D2C(12)
CRLF	= CR||LF
CLS	= "[H[J"

LockCarrier

If ~exists(VBData) then call NoVote
Call Open('Data',VBData,'R')
  QNum = Readln('Data')
  Do A = 1 to QNum
    QText.A.0 = Readln('Data')
    QAnsNum.A = Readln('Data')
    Do B = 1 to QAnsNum.A
      QText.A.B = Readln('Data')
    End
  End
Call Close('Data')

/* If the Result file doesn't exist, make a blank one */

If ~Exists(VBResults) then do
  Call Open('Res',VBResults,'W')
    Call Writeln('Res',0)
  Call Close('Res')
  ResNum = 0
  Call PadRes
End

/* Open and read in past voting results */

Call Open('Res',VBResults,'R')
  ResNum = Readln('Res')
  If Resnum > 0 then do
    Do A = 1 to ResNum
      Pick.A.0 = Readln('Res')
      Do B = 1 to Pick.A.0
        Pick.A.B = Readln('Res')
      End
    End
  End
Call Close('Res')

PadRes:
If ResNum < QNum Then do      /* More questions than results */
  ResNum = ResNum + 1         /* So we pad the result file.  */
  Do A = ResNum  to QNum
    Pick.A.0 = QAnsNum.A
    Do B = 1 to QAnsNum.A
      Pick.A.B = 0
    End
  End
End

/* Check for userfile and read in the data, or create one if needed */

UNum = 0
If exists(VBUser) then do
  Call Open('User',VBUser,'R')
  UNum = Readln('User')
  Do A = 1 to UNum
    UPick.A = Readln('User')
  End
  Call Close('User')
End

If Unum < QNum then do
  UNum = UNum + 1
  Do A = Unum to QNum
    UPick.A = 0
  End
End

/* Present a choice of questions to vote on */

Main:
SendModem CLS||F7||'-----=( '||F2||BBSName||F3||' Voting Booth v1.0'||F7||' )=-----'||CRLF||CRLF

Do A = 1 to QNum
  If UPick.A = 0 then VF = "* "
  If UPick.A > 0 then VF = "  "
  SendModem F1||VF||F3||A||F6||") "||F3||QText.A.0||CRLF
End

Opt = AskInput(ln, CRLF||F7||'Question '||F6||'('||F7||' 1'||F6||'-'||F7||Qnum||F6||','||F7||' Q'||F6||'='||F7||'Quit '||F6||')'||F3||': '||F7, "", 10, "CAPITAL")

If ~CheckCarrier(ln) then call DropCarrier

If Upper(Opt) = 'Q' then Call Done
If Opt < 1 | Opt > QNum then call Main

/* Vote on a particular question */

VoteLoop:
SendModem CLS||F2||QText.Opt.0||CRLF||CRLF

TV = 0
Do D = 1 to QAnsNum.Opt
  TV = TV + Pick.Opt.D
End
Do C = 1 to QAnsNum.Opt
  VF = "  "
  If UPick.Opt = C Then VF = "* "
  X = Length(QText.Opt.C) ; S = 45 - X ;
  SPC = " "
  Do T = 1 to S
    SPC = SPC||"."
  End
  If TV = 0 then PCNT = 0
  If TV > 0 Then PCNT = (100 * (Pick.Opt.C / TV)) % 1
  SendModem F1||VF||F3||C||F6||") "||F3||QText.Opt.C||F2||SPC||F3||PCNT||"% "||F1||"["||F3||Pick.Opt.C||F1||"]"||CRLF
End
SendModem CRLF
If UPick.Opt > 0 then do
  CV = GetYesNo(ln, 'Would you like to change your vote?'||F7, 0, 1)

  If ~CheckCarrier(ln) then call DropCarrier

  If CV = 1 then do
    X = UPick.Opt
    UPick.Opt = 0
    Pick.Opt.X = Pick.Opt.X - 1
    Call VoteLoop
  End

  Call Main
End
Ans = AskInput(ln, F7||'Answer '||F6||'('||F7||' 1'||F6||'-'||F7||QAnsNum.Opt||F6||','||F7||' Q'||F6||'='||F7||'Quit '||F6||')'||F3||': '||F7, "", 10, "CAPITAL")

If ~CheckCarrier(ln) then call DropCarrier

If Upper(Ans) = 'Q' then Call Main
If Ans <1 | Ans > QAnsNum.Opt then call VoteLoop
UPick.Opt = Ans
Pick.Opt.Ans = Pick.Opt.Ans + 1
Call Main

NoVote:
SendModem CLS||'-----=( No Voting Topics )=-----'||CRLF||CRLF
UnLockCarrier
Exit 0

DropCarrier:
UnLockCarrier
Exit 0

Done:
SendModem CRLF
Address Command 'Delete '||VBResults
Call Open('Res',VBResults,'W')
  Call Writeln('Res',QNum)
  Do A = 1 to QNum
    Call Writeln('Res',Pick.A.0)
    Do B = 1 to Pick.A.0
      Call Writeln('Res',Pick.A.B)
    End
  End
Call Close('Res')
If Exists(VBUser) then do
  Address Command 'Delete "'||VBUser||'"'
End
Call Open('User',VBUser,'W')
  Call Writeln('User',QNum)
  Do A = 1 to QNum
    Call Writeln('User',UPick.A)
  End
Call Close('User')

UnLockCarrier
Exit 0
