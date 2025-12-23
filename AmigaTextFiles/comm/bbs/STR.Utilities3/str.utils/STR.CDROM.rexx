/*

    ShowCDROM.trans - Shows the name of current CD-ROM in FBase.

*/

options results

call pragma('w','n')

getuser '28';  ansi  = result
getuser '15'; access = result

if ansi = 1 then exit

cdrom = 'CD0:'  /* Change this to fit your system */
                   /* Don't forget to change this in the FileScrollerCD.rexx */

if ~exists(cdrom) then exit

cdrom.            = 32   /* These are the access groups.       */
cdrom.GROLIER_02  = 32   /* Sorry, there aren't any ranges     */
cdrom.FFCD_700    = 1    /* available.  Actually, a user will  */
cdrom.NASA        = 1    /* have access if the AG is equal to  */
cdrom.MPC_001     = 3    /* or larger than this number.  Here, */
cdrom.AMINET_0693 = 1    /* I use 1 as normal, 3 as Adult, and */
cdrom.ULTIMOD     = 1    /* 32 as sysop.                       */
cdrom.GIFS_GALORE = 1
cdrom.PRIVATES    = 3
cdrom.HOT_PIX_V3  = 3
cdrom.AMIGACD_ONE = 1
cdrom.Amiga       = 1
cdrom.PICTURE_CD  = 3
cdrom.SUPER_CD    = 3
cdrom.SCIFI       = 1    /* See note at bottom of script       */

address command 'ls >T:CDName -dF "%n\n"' cdrom

if open(io,'T:CDName','r') then cdname = readln(io)
call close(io)

if cdname = '' | cdname = 'unnamed' then cdname = 'SCIFI'  /* See Note below */

if cdrom.cdname > access then exit

print
print '\q1The CD-ROM "\z4\c7'cdname'\q1" is currently in the CD-Drive.  Use "\z4\c7READCD\q1" at any\n1BBS Menu prompt for access.'

exit

/*

Note:

The CD-ROM 'SCI-FI and Fantasy' has no name on the CD itself.  It's like
an unlabled MS-DOS floppy!!  So, I had to put this kludge in.  I don't
know how many other CD-ROMs don't have names, but if you find one that
doesn't have a name, let me know.

*/
