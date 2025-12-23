/* $VER: bbsNewFile.rexx 8.3 (18.11.94)
called by BBBBS.baud for each file uploaded
*/

dev='NO_DIR:'   /* To activate, make this a valid directory */

PARSE ARG name filename .

CALL PRAGMA('W','N')          /* Turn requesters off */
IF ~EXISTS(dev) | ~EXISTS(filename) THEN EXIT

ADDRESS COMMAND 'info >RAM:infout' dev

ok=OPEN(f,'ram:infout','R')
IF ok=0 THEN EXIT 20
line=READLN(f)
line=READLN(f)
line=READLN(f)
line=READLN(f)
CALL CLOSE(f)

devblks=WORD(line,4)-1
filblks=WORD(STATEF(filename),3)

IF filblks>devblks THEN EXIT   /* No room at the inn... */

string='copy' filename dev 'CLONE'
ADDRESS COMMAND string
ADDRESS COMMAND 'protect' filename 'arwed'     /* set Archive bits */

/* bbsNewFile.rexx */
