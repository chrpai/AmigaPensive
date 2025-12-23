/* $VER: bbsRestart.rexx 8.3 (11.11.94) */

bbspath=GETCLIP('BBS_path')
IF OPEN(f,bbspath'logs/log.'DATE('S'),'A')~=0 THEN
  DO
    CALL WRITELN(f,'')
    CALL WRITELN(f,'RESTART!  BBBBSdemon.baud called bbsRestart.rexx at' TIME('C'))
    CALL WRITELN(f,'')
    CALL CLOSE(f)
  END
ADDRESS BAUD Send '\mBBBBS.baud'

/* bbsRestart.rexx */
