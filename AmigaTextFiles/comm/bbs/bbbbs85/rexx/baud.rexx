/* baud.rexx */

ARG screenflag .

IF ~SHOW('P','BAUD') THEN
  DO
    SAY ' *** BaudBandit MUST be running FIRST before you start BBBBS!'
    RETURN(0)
  END

ADDRESS 'BAUD'
IF screenflag='OFF' THEN
  DO
    IF GETCLIP('BBS_fkeyhelp')=1 THEN CALL PostMsg()
    Screen OFF
    CALL SETCLIP('BBS_screen',0)
    CALL SETCLIP('BBS_fkeyhelp')
  END
ELSE
  DO
    Screen ON
    CALL SETCLIP('BBS_screen',1)
  END

/* baud.rexx */
