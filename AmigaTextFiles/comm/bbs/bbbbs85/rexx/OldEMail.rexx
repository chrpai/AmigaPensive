/*   $VERS: 5.1 OldEmail.rexx 29 Jun 1992 (29.6.92)
copyright 1992 Richard Lee Stockton FREELY DISTRIBUTABLE
         Reports on old email and emailfiles.
*/

SAY 'Email may be deleted at any time without bad effects.'
SAY 'Remember to delete the attached emailfile, too.'
SAY

ARG days deleteflag .
IF days<1 THEN
  DO
    SAY 'USAGE: rx OldEmail <days> [DELETE]'
    SAY 'ie, ''rx OldEmail 90''  will report all email'
    SAY 'and emailfiles that are more than 90 days old.'
    SAY 'The DELETE keyword causes the old email to be displayed, then,'
    SAY 'your approval is required for EACH deletion of email or files.'
    EXIT
  END

CALL PRAGMA('P',-2)   /* lower priority so no slowdown of BBS */

SAY 'Today is' DATE('W') DATE()
SAY 'Reporting Email and EmailFiles more than' days 'days old...'
testdate=DATE('I')-days

bbspath=GETCLIP('BBS_path')
maildirs=SHOWDIR(bbspath'Email','D')
filedirs=SHOWDIR(bbspath'EmailFiles','D')

DO i=1 TO WORDS(maildirs)
  thisdir=WORD(maildirs,i)
  mail=SHOWDIR(bbspath'Email/'thisdir,'F')
  DO j=1 TO WORDS(mail)
    thismail=WORD(mail,j)
    thisfile=bbspath'Email/'thisdir'/'thismail
    thisdate=WORD(STATEF(thisfile),5)
    IF thisdate<testdate THEN
      DO
        SAY DATE(,thisdate,'I') thisfile
        IF deleteflag='DELETE' THEN
          DO
            x=OPEN(f,thisfile,'R')
            IF x=0 THEN ITERATE j
            DO loop=1 WHILE ~EOF(f)
              SAY READLN(f)
              IF loop//18=0 THEN
                DO
                  OPTIONS PROMPT 'Q=Quit  [RETURN]=Continue '
                  PULL junk
                  IF junk='Q' THEN LEAVE loop
                END
            END
            CALL CLOSE(f)
            OPTIONS PROMPT 'Delete this email? (nY) > '
            PULL temp
            IF LEFT(temp,1)~='N' THEN CALL DELETE(thisfile)
          END
      END
  END
END

DO i=1 TO WORDS(filedirs)
  thisdir=WORD(filedirs,i)
  filz=SHOWDIR(bbspath'EmailFiles/'thisdir,'F')
  DO j=1 TO WORDS(filz)
    thisfile=bbspath'EmailFiles/'thisdir'/'WORD(filz,j)
    thisdate=WORD(STATEF(thisfile),5)
    IF thisdate<testdate THEN
      DO
        SAY DATE(,thisdate,'I') thisfile
        IF deleteflag='DELETE' THEN
          DO
            OPTIONS PROMPT 'Delete this file? (nY) > '
            PULL temp
            IF LEFT(temp,1)~='N' THEN CALL DELETE(thisfile)
          END
      END
  END
END

SAY 'Done!'

/* OldEmail.rexx */
