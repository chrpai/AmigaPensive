/* $VER: ToggleNoise.rexx 6.1 © 1993 Richard Lee Stockton (24.7.93) */

x=GETCLIP('BBS_NOISE')
IF x='OFF' THEN
  DO
    CALL SETCLIP('BBS_NOISE')
    temp='BBBBS sounds and voice are ON.'
  END
ELSE
  DO
    CALL SETCLIP('BBS_NOISE','OFF')
    temp='BBBBS sounds and voice are OFF.'
  END
IF ADDRESS()='BAUD' THEN MSG temp
ELSE SAY temp


/* ToggleNoise.rexx */
