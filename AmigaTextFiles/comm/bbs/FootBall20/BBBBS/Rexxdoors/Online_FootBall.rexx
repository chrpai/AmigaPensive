/* OLFB Startup for BBBBS systems ONLY!!, Needed for OLFB version 1.55+
   Copy this file to your Rexxdoors directory
   OLFB copyright 1994 by Matt English 3-11-94 */

LF='0A'x
CR='0D'x
IF ADDRESS()~='BAUD' THEN CR=''

bbspath=GETCLIP('BBS_path')
ARG name purse purse2 colorflag secs .


ADDRESS command''BBSpath'rexxdoors/Data/OLFB/Online_Football 'name' . . 'colorflag' 'secs''

exit
