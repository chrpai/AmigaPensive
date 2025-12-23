/*
** $VER: Z_MultiAction 0.50b (29.06.96)
**
** Written by Paolo Carotenuto
**
*/
OPTIONS RESULTS
DROP MM.
MM.PathCfg='Zeus:Config/'
MM.PrgName='Z_MultiAction v0.50b'
IF ~SHOW('l',"rexxsupport.library") THEN;DO;IF ~ADDLIB("rexxsupport.library",0,-30,0) THEN;DO;SAY "Could not open RexxSupport.library";EXIT 10;END;END
CALL WriteLog(MM.PrgName '- Started')
IF ~OPEN('in',MM.PathCfg||'Z_MultiAction.Cfg','R') THEN;DO
CALL WriteLog('*** Error: Can not open config "'MM.PathCfg'Z_MultiAction.cfg"');EXIT 10
END
MM.Day=0;MM.Command.Count=0
DO WHILE ~EOF('in')
Line=STRIP(READLN('in'))
IF (LEFT(Line,1) ~=';')&Line ~='' THEN;DO
IF UPPER(WORD(Line,1))='DAY' THEN;DO
Week=UPPER(DATE('W'))
SELECT
WHEN Week='SUNDAY' THEN MM.Day=0
WHEN Week='MONDAY' THEN MM.Day=1
WHEN Week='TUESDAY' THEN MM.Day=2
WHEN Week='WEDNESDAY' THEN MM.Day=3
WHEN Week='THURSDAY' THEN MM.Day=4
WHEN Week='FRIDAY' THEN MM.Day=5
WHEN Week='SATURDAY' THEN MM.Day=6
OTHERWISE CALL WriteLog('Wrong DAY to Process this sequences!!')
END
IF POS(MM.Day,SUBWORD(Line,2))=0 THEN MM.Day=FALSE
ITERATE
END
IF UPPER(WORD(Line,1))='FOUND' THEN;DO
MM.Found=WORD(Line,2)
CALL WriteLog('Trying to find ['UPPER(MM.Found)']')
ITERATE
END
IF UPPER(Line) ~='END' THEN CALL AddLine(Command,Line)
IF UPPER(WORD(Line,1))='END' THEN CALL Process()
END
END
CALL WriteLog(MM.PrgName '- Ended')
EXIT
Process:PROCEDURE EXPOSE MM.
IF MM.Day ~=FALSE THEN;DO
IF Found(MM.Found) ~=FALSE THEN;DO
DO l=0 FOR MM.Files.Count
CALL WriteLog('  Actually found ['MM.Files.l']')
MM.Nome=MM.Files.l
MM.Patt=MM.Path||MM.Files.l
MM.Size=WORD(STATEF(MM.Patt),2)
DO i=0 FOR MM.Command.Count
SELECT
WHEN UPPER(WORD(MM.Command.i,1))='IFSIZE' THEN;DO
IF IfSize(MM.Files.l,SUBWORD(MM.Command.i,2))=FALSE THEN LEAVE
END
WHEN UPPER(WORD(MM.Command.i,1))='CHECKDATE' THEN;DO
IF CheckDate(MM.Files.l,SUBWORD(MM.Command.i,2))=FALSE THEN LEAVE
END
WHEN UPPER(WORD(MM.Command.i,1))='COMMENT' THEN CALL Comment(MM.Files.l,SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='DELETE' THEN CALL Delete(MM.Files.l)
WHEN UPPER(WORD(MM.Command.i,1))='RENAME' THEN CALL Rename(MM.Files.l,SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='COPY' THEN CALL Copy(MM.Files.l,SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='MOVE' THEN CALL Move(MM.Files.l,SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='NOTIFY' THEN CALL Notify(SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='HATCH' THEN CALL Hatch(MM.Files.l,SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='EVENTLOG' THEN CALL EventLog(SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='SCRIPT' THEN CALL Script(SUBWORD(MM.Command.i,2))
WHEN UPPER(WORD(MM.Command.i,1))='CLICMD' THEN CALL CliCmd(SUBWORD(MM.Command.i,2))
OTHERWISE CALL WriteLog('  I don`t understand the command ['UPPER(WORD(MM.Command.i,1))']')
END
END
END
END
END
ELSE CALL WriteLog('  Wrong day to process this...')
CALL WriteLog('Ending...')
MM.Command.Count=0
RETURN
Found:PROCEDURE EXPOSE MM.
PARSE ARG Command
Pos1=LastPos('/',command);Pos2=LastPos(':',command)
IF pos1=0 THEN;DO;Progs=RIGHT(command,length(command)-pos2);MM.Path=LEFT(Command,pos2);END
ELSE;DO;Progs=RIGHT(command,length(command)-pos1);MM.Path=LEFT(Command,pos1);END
ADDRESS COMMAND 'C:List 'Command '>t:ScanFiles QUICK NOHEAD FILES LFORMAT "%n"'
IF WORD(STATEF('T:ScanFiles'),2) ~=0 THEN;DO
x=0;OPEN('sf','T:ScanFiles','R');DO WHILE ~EOF('sf');Linea=READLN('sf');IF Linea ~='' THEN;DO;MM.Files.x=Linea;x=x+1;END;END;CLOSE('sf')
MM.Files.Count=x;MM.Date=LEFT(LogData(),14);MM.Time=LEFT(time(),5);MM.DtTm=LogData();RETURN TRUE;END;CALL WriteLog('  Nothing!');RETURN FALSE
IfSize:PROCEDURE EXPOSE MM.
PARSE ARG Progs,Size
IF WORD(STATEF(MM.Path||Progs),2)>Size THEN;DO
CALL WriteLog('  Size of file ['MM.Path||Progs'] is acceptable for Processing')
RETURN TRUE
END
CALL WriteLog('  Size of file ['MM.Path||Progs'] is less than threshold - Aborting')
RETURN FALSE
Comment:PROCEDURE EXPOSE MM.
PARSE ARG Progs,Testo
CALL WriteLog('  Setting comment on ['MM.Path||Progs']')
Testo='"'Trasforma(Testo)'"'
ADDRESS COMMAND 'C:FileNote 'MM.Path||Progs Testo
RETURN
CheckDate:PROCEDURE EXPOSE MM.
PARSE ARG Progs, Day .
DayProg=( DATE('I')-WORD(STATEF(MM.Path||Progs),5))
IF DayProg> Day THEN;DO
CALL WriteLog('  Excepted day limit of file ['MM.Path||Progs'] Processing...')
RETURN TRUE
END
RETURN FALSE
Delete:PROCEDURE EXPOSE MM.
PARSE ARG Progs
CALL WriteLog('  Deleting file ['MM.Path||Progs']')
ADDRESS COMMAND 'C:Delete >NIL: 'MM.Path||Progs
RETURN
Rename:PROCEDURE EXPOSE MM.
PARSE ARG Progs, Dest
IF INDEX(Dest,':')=0|INDEX(Dest,':')=0 THEN NewDest=MM.Path||STRIP(Dest)
ELSE NewDest= Dest
IF RIGHT(Dest,1)=':'|RIGHT(Dest,1)='/' THEN NewDest= Dest||Progs
CALL WriteLog('  Renaming ['MM.Path||Progs'] to ['STRIP(NewDest)']')
NewDest=CheckFile(NewDest)
ADDRESS COMMAND 'C:Rename >NIL: 'MM.Path||Progs NewDest
RETURN
Copy:PROCEDURE EXPOSE MM.
PARSE ARG Progs, Dest
NewDest= Dest
IF RIGHT(Dest,1)=':'|RIGHT(Dest,1)='/' THEN NewDest= Dest||Progs
CALL WriteLog('  Copying ['MM.Path||Progs'] to ['STRIP(NewDest)']')
NewDest=CheckFile(NewDest)
ADDRESS COMMAND 'C:Copy >NIL: 'MM.Path||Progs NewDest
RETURN
Move:PROCEDURE EXPOSE MM.
PARSE ARG Progs, Dest
NewDest= Dest
IF RIGHT(Dest,1)=':'|RIGHT(Dest,1)='/' THEN NewDest= Dest||Progs
CALL WriteLog('  Moving ['MM.Path||Progs'] to ['STRIP(NewDest)']')
NewDest=CheckFile(NewDest)
ADDRESS COMMAND 'C:Copy >NIL: 'MM.Path||Progs NewDest
ADDRESS COMMAND 'C:Delete >NIL: 'MM.Path||Progs
RETURN
Hatch:PROCEDURE EXPOSE MM.
PARSE ARG Progs,TagName Altro
PARSE VAR Altro '"' Hatch.Desc '"' Hatch.Repl .;Hatch.File=MM.Path||Progs
CALL WriteLog('  Hatching file ['MM.Path||Progs'] into area ['TagName']')
IF Hatch.Repl='' THEN ADDRESS COMMAND 'Zeus:Bin/ZMP >NIL: HATCH AREA' TagName 'FILE' Hatch.File 'DESC "'Hatch.Desc'"'
ELSE ADDRESS COMMAND 'Zeus:Bin/ZMP >NIL: HATCH AREA' TagName 'FILE' Hatch.File 'DESC "'Hatch.Desc'" REPLACE' Hatch.Repl
RETURN
Notify:PROCEDURE EXPOSE MM.
PARSE ARG AreaNum User Message
Mess.To=TRANSLATE(User,' ','_');Mess.Subj='Zeus Robotic Notification';Mess.File='T:MM.Trans'
MM.Body.Count=0;CALL AddLine(Body,Trasforma(STRIP(TRANSLATE(Message,' ','"'))));CALL AddLine(Body,'')
OPEN('fout',Mess.File,'W');DO h=0 FOR MM.Body.Count;WRITELN('fout',MM.Body.h);END;CLOSE('fout')
CALL WriteLog('  Notifying to ['User'] into area ['AreaNum']')
ADDRESS COMMAND 'Zeus:Bin/PostMsg >NIL: FROM="'MM.PrgName'" TO="'Mess.To'" SUBJECT="'Mess.Subj'" TEXTFILE="'Mess.File'" AREA="'AreaNum'" PRIVATE'
RETURN
EventLog:PROCEDURE EXPOSE MM.
PARSE ARG Name Message
CALL WriteLog('  Write to EVENT.Log of ['TRANSLATE(Name,' ','_')']')
ADDRESS COMMAND 'Echo '||MM.DtTm':' Trasforma(Message)||' >>User:'||Name'/'||Event.log
RETURN
Script:PROCEDURE EXPOSE MM.
PARSE ARG Script
CALL WriteLog('  Executing ['Trasforma(Script)'] script')
ADDRESS COMMAND 'Execute >NIL: 'Trasforma(Script)
RETURN
CliCmd:PROCEDURE EXPOSE MM.
PARSE ARG Cmd
CALL WriteLog('  Executing ['Trasforma(Cmd)'] Cli Command')
ADDRESS COMMAND Trasforma(Cmd)' >NIL:'
RETURN
Replace:PROCEDURE;PARSE ARG String,New,Old;DO WHILE INDEX(String,Old) ~=0;INTERPRET "PARSE VAR String l '"Old"' r";String=l||New||r;END;RETURN String
Trasforma:PROCEDURE EXPOSE MM.;PARSE ARG String;IF INDEX(String,'%')=0 THEN RETURN String
String=Replace(String,MM.Nome,'%NAME');String=Replace(String,MM.Patt,'%PATH')
String=Replace(String,MM.Size,'%SIZE');String=Replace(String,MM.Date,'%DATE')
String=Replace(String,MM.Time,'%TIME');String=Replace(String,MM.DtTm,'%DTTM')
RETURN String
LogData:PROCEDURE EXPOSE MM.;Tempo= DATE();Week=LEFT(date('W'),3);PARSE VAR Tempo Giorno Mese Anno;RETURN Week giorno mese RIGHT(anno,2) LEFT(time(),5)
AddLine:PROCEDURE EXPOSE MM.;PARSE ARG Stem,Line;INTERPRET "Cont = MM."Stem".Count";INTERPRET "MM."Stem".Cont = Line";INTERPRET "MM."Stem".Count = Cont +1";RETURN
WriteLog:PROCEDURE EXPOSE MM.;PARSE ARG Testo;SAY Testo;RETURN
CheckFile:PROCEDURE EXPOSE MM.;PARSE ARG Name;IF EXISTS(Name) THEN;DO;z=0;DO FOREVER;z=z+1;IF ~EXISTS(Name','z) THEN;DO;Name=Name','z;LEAVE;END;END;END;RETURN Name