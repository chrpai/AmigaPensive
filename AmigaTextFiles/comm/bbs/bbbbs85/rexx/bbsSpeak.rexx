/* bbsSpeak.rexx 6.6 (11.11.93)
Handles speach for BBBBS - Edit this file to your preferences.
*/

IF GETCLIP('BBS_NOISE')='OFF' THEN EXIT

PARSE ARG com name bbspath saypath .

DO i=1 TO 10 WHILE GETCLIP('BBS_Sounding')~=''
  CALL DELAY(100)
END
CALL SETCLIP('BBS_Sounding',com)

IF com='LOGON'        THEN CALL logon()
IF com='CHAT'         THEN CALL chat()
IF com='FILE_REQUEST' THEN CALL file_request()

CALL SETCLIP('BBS_Sounding')
EXIT


logon:
sp1='Yo sissop.'
sp2=TRANSLATE(name,' ','_')'.'
sp3='has logged awon.'
IF SHOWLIST('H','SPEAK') THEN
  DO
    IF writeopen('SPEAK:')~=0 THEN
      DO
        CALL WRITELN(f,sp1)
        DO i=1 TO WORDS(sp2)
          CALL DELAY(14)
          CALL WRITELN(f,WORD(sp2,i))
        END
        CALL DELAY(14)
        CALL WRITELN(f,sp3)
        CALL CLOSE(f)
      END
  END
ELSE IF EXISTS(saypath) THEN
  DO
    ADDRESS COMMAND saypath sp1
    DO i=1 TO WORDS(sp2)
      ADDRESS COMMAND saypath WORD(sp2,i)
    END
    ADDRESS COMMAND saypath sp3
  END
RETURN


chat:
CALL DELAY(100)
sp1='Yo sissop.'
sp2=TRANSLATE(name,' ','_')'.'
sp3='wants to chat with you.'
IF SHOWLIST('H','SPEAK') THEN  /* check on SPEAK: device */
  DO
    IF EXISTS(bbspath'BBS_TEXT/YELL') THEN /* we have yell file */
      ADDRESS COMMAND 'C:Run C:Type >SPEAK:' bbspath'BBS_TEXT/YELL'
    ELSE IF writeopen('SPEAK:')~=0 THEN
      DO
        CALL WRITELN(f,sp1)
        DO i=1 TO WORDS(sp2)
          CALL DELAY(14)
          CALL WRITELN(f,WORD(sp2,i))
        END
        CALL DELAY(14)
        CALL WRITELN(f,sp3)
        CALL WRITELN(f,sp1)
        CALL CLOSE(f)
      END
  END
ELSE IF EXISTS(saypath) THEN          /* default to SAY */
  DO
    IF EXISTS(bbspath'BBS_TEXT/YELL') THEN /* we have yell file */
      ADDRESS COMMAND 'C:Run' saypath '-x' bbspath'BBS_TEXT/YELL'
    ELSE
      DO
        ADDRESS COMMAND saypath sp1
        DO i=1 TO WORDS(sp2)
          ADDRESS COMMAND saypath WORD(sp2,i)
        END
        ADDRESS COMMAND saypath sp3
        ADDRESS COMMAND saypath sp1
      END
  END
RETURN


file_request:
sp1='Yo sissop.'
sp2='a file is being re kwested.'
IF SHOWLIST('H','SPEAK') THEN
  DO
    IF writeopen('SPEAK:')~=0 THEN
      DO
        CALL WRITELN(f,sp1)
        CALL WRITELN(f,sp2)
        CALL CLOSE(f)
      END
  END
ELSE IF EXISTS(saypath) THEN
  DO
    ADDRESS COMMAND saypath sp1
    ADDRESS COMMAND saypath sp2
  END
RETURN


writeopen:
PARSE ARG fname
CALL CLOSE(f)
ok=OPEN(f,fname,'W')
IF ok=0 THEN RETURN 0
RETURN 1

/* bbsSpeak.rexx */
