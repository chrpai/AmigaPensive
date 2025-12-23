/* $VER: Increment.rexx 8.3 (9.11.94) */

NUMERIC DIGITS 14
ARG prg
x=STATEF(prg)
IF x='' THEN EXIT
num=WORD(x,8)
IF ~DATATYPE(num,'W') THEN num=0
ADDRESS COMMAND 'filenote' prg '22'x||num+1'22'x

/* Increment.rexx */
