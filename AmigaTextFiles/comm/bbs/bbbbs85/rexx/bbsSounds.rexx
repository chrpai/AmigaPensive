/* bbsSounds.rexx 6.5 (26.10.93)
   Handles sounds for BBBBS - Edit this file to your preferences.

     Valid sounds are (see BBBBS.DOC for more info);

        LOGON  LOGOFF  UPLOAD  DOWNLOAD  JUMP  CBV
        INFO  TIMEOUT  YELL  LOST  TFAIL  MESSAGE
        NEW_USER  NEW_FILE  FEEDBACK  ATZ_FAIL

          ...and the "default" sound,  ALERT

    First, the "IF" statement will be executed looking for specific
    instructions. You can use any effects available in c:sound. You can
    use any soundfile located anywhere, just specify its full path.

    If there is no valid IF instruction, AND the soundname does not
    exist in the Sounds directory, the last ELSE loop will be executed.

    If the soundname is referred to in the IF loop, the default ALERT
    sample will be sounded. If ALERT does not exist, or no instruction
    for the specific soundname is found, then no sound will be heard.
    Otherwise, if the sound exists in Sounds directory, it will be played.

    You may also turn off ALL sounds using a switch in CONFIG.BBS, or by
    using the Shift-F4 Function Key to toggle voice & sounds ON/OFF.

    I have set the sounds at a lower volume (v32) to balance better with
    the Amiga voice, should you be using both. Feel free to alter at will.
*/

IF GETCLIP('BBS_NOISE')='OFF' THEN EXIT
ARG soundpath sound 
sound=STRIP(sound)
soundpath=STRIP(soundpath)
soundspec=soundpath||sound

IF sound='STUPID_USER' THEN soundspec='DH0:Stuff/Sounds/Ugly FADE'

/* insert your external soundfiles or special args here. Syntax: */
/*  ELSE IF sound='VALID_SOUND' THEN soundspec='new_soundfile_path' */

ELSE IF soundspec~='' & EXISTS(soundspec) THEN soundspec=soundspec 'v24'
ELSE
  DO
    IF EXISTS(soundpath'ALERT') THEN
      DO
        IF sound='TIMEOUT'  THEN soundspec=soundpath'ALERT v32'
        IF sound='LOST'     THEN soundspec=soundpath'ALERT 3 v32 e4' /* vol 32=>4 */
        IF sound='ATZ_FAIL' THEN soundspec=soundpath'ALERT 5 v4 e32' /* vol 4=>32 */
      END
    IF ~EXISTS(WORD(soundspec,1)) THEN EXIT 1
  END
DO i=1 TO 10 WHILE GETCLIP('BBS_Sounding')~=''
  CALL DELAY(100)
END
CALL SETCLIP('BBS_Sounding',sound)
ADDRESS COMMAND 'c:sound' soundspec 'q'  /* q = no window */
CALL SETCLIP('BBS_Sounding')
EXIT 0

/* bbsSounds.rexx */
