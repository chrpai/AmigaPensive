/* $VER: bbsArcSTOP.rexx 6.1 © 1993 Richard Lee Stockton (24.7.93)
Lists archive processes by username and process, and allows abort.
    copyright 1993 Richard Lee Stockton FREELY DISTRIBUTABLE
*/

scrn=''
IF ADDRESS()='BAUD' THEN scrn='BAUD'
numlost=''
DO loop=1
  num=make_req(numlist)
  IF num=0 | p.num='' THEN ITERATE loop
  temp='BBS_STOP_'p.num
  temp2='ABORT'
  x=Request(,,'DELETE any partially processed files?',,' DELETE ',' LEAVE ''EM ',scrn)
  IF x='OKAY' THEN temp2='DELETE'
  CALL SETCLIP(temp,temp2)
  numlist=numlist num
END
EXIT


make_req:
ARG oldnums 
text='Enter number of process to STOP\   (or enter BLANK to exit)\'
c=0
a=0
p.=''
DO i=1 TO 9
  t=GETCLIP('BBS_QUICKIN'i)
  IF t='' THEN ITERATE i
  c=c+1
  text=text'\'c'. QUICKIN Process  '
  IF GETCLIP('BBS_STOP_QUICKIN'i)='' THEN text=text t
  ELSE text=text 'ABORTED'
  p.c='QUICKIN'i
END
DO i=1 TO 9
  t=GETCLIP('BBS_QUICKOUT'i)
  IF t='' THEN ITERATE i
  c=c+1
  text=text'\'c'. QUICKOUT Process '
  IF GETCLIP('BBS_STOP_QUICKOUT'i)='' THEN text=text t
  ELSE text=text 'ABORTED'
  p.c='QUICKOUT'i
END
DO i=1 TO 9
  t=GETCLIP('BBS_ALPHA'i)
  IF t='' THEN ITERATE i
  c=c+1
  text=text'\'c'. Alpha FileList   '
  IF GETCLIP('BBS_STOP_ALPHA'i)='' THEN text=text t
  ELSE text=text 'ABORTED'
  p.c='ALPHA'i
END
DO i=1 TO 9
  t=GETCLIP('BBS_BROWSE'i)
  IF t='' THEN ITERATE i
  c=c+1
  text=text'\'c'. Full Descriptions'
  IF GETCLIP('BBS_STOP_BROWSE'i)='' THEN text=text t
  ELSE text=text 'ABORTED'
  p.c='BROWSE'i
END
DO i=1 TO 9
  t=GETCLIP('BBS_MSGS'i)
  IF t='' THEN ITERATE i
  c=c+1
  text=text'\'c'. Messages         '
  IF GETCLIP('BBS_STOP_MSGS'i)='' THEN text=text t
  ELSE text=text 'ABORTED'
  p.c='MSGS'i
END
IF c>12 THEN c=12
IF c=0 THEN
  DO
    text='- No archives are being processed -'
    CALL Request(,,text,,,'EXIT',scrn)
    EXIT
  END
ELSE x=Request(,,text,'',,'EXIT',scrn)
IF x='' THEN EXIT
IF x<1 | x>c | ~DATATYPE(x,'W') THEN RETURN 0
RETURN x

/* bbsArcSTOP.rexx */

