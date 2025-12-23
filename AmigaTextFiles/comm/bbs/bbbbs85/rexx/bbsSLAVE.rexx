/* bbsSLAVE.rexx 6.6 (17.12.93) */

ARG rate .
CALL OPENPORT('BBS_SLAVE')
IF DATATYPE(rate,'W') THEN
  DO
    CALL SETCLIP('BBS_SLAVE_RATE',rate)
    ADDRESS BBBBS 'START'
    t=WAITPKT('BBS_SLAVE')
    DO i=1
      p=GETPKT('BBS_SLAVE')
      IF p='0000 0000'x THEN LEAVE i
      t=REPLY(p,0)
    END
  END
EXIT 0

/* bbsSLAVE.rexx */
