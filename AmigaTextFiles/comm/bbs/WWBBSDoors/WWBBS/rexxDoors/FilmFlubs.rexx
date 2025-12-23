/*  $VER: 1.0  FilmFlubs.rexx  15 October 1991  (15.10.91)
            copyright 1991 Richard Lee Stockton
                  FREELY DISTRIBUTABLE
*/

FF='0C'x  /* FormFeed */
CR='0D'x  /* Carraige Return */
SIGNAL ON BREAK_C
SIGNAL ON BREAK_E

ARG name .

/* Open the support library if it is not already open. */
if ~show('L',"rexxsupport.library") then do
    addlib('rexxsupport.library',0,-30,0)
     end

bbspath=GETCLIP('BBS_path')
filename=bbspath'rexxDoors/Data/FilmFlubs.txt'  /* Formfeed separated text */
size=WORD(STATEF(filename),2)
fragment=size/3000

x=OPEN(f,'RAM:DUMMY','W')                /* write to a dummy file */
IF x=0 THEN EXIT(20);
CALL WRITELN(f,'dummy')
CALL CLOSE(f)
micros=WORD(STATEF('RAM:DUMMY'),7)        /*  0 >= micros < 3000  */
location=fragment*micros
IF fragment>1000 THEN
  location=location+fragment*RIGHT(TIME('S'),2)/100
ELSE IF fragment>100 THEN
  location=location+fragment*RIGHT(TIME('S'),1)/10
location=TRUNC(location)
IF location>size THEN location=micros

x=OPEN(f,filename,'R')
IF x=0 THEN RETURN(10);
CALL SEEK(f,location,'B')            /* point to the random place */
line=''
DO WHILE line~=FF & ~EOF(f)        /* find the start (a formfeed) */
  line=READLN(f)
END
count=0
line=''
IF ~EOF(f) THEN      /* if EOF then must be right at end. missed! */
  DO WHILE line~=FF & ~EOF(f)         /* otherwise, show the page */
    line=READLN(f)
    IF ~EOF(f) & line~=FF THEN
      DO
        SAY line||CR
        count=count+1
      END
  END
BREAK_C:
BREAK_E:
CALL CLOSE(f)
RETURN(count);
EXIT;                           /* a little redundant, so sue me! */
