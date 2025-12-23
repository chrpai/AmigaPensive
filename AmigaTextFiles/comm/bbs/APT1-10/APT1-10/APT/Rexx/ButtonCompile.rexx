/* buttoncompile.rexx
**
** $VER: buttoncompile 0.0.1 (19.8.93)
**
** ARexx program for controlling ApT-BBS by ApT-Design.
**
** Copyright © 1993 ApT-Design All Rights Reserved
**
** Compiles/Creates Button data files for use with ApT-BBS! System.
**
** This file should contain all of your related menu information for
** use with the button system. It should then be run via the
**
** 'rx ButtonCompile' command to which it will save out the files in
** the specified directories.
**
** NOTES: Requires the 'tplates:button' directory to be created. This
** will be used to save the various '.bank' files to.
**
*/

CR = 'A'x
parse arg lineno

options results
portname = 'APTMANAGER'
address value portname

say CR CR "Button Compiler initiated..." CR

/*
** Some repeated 'defines', saves having larger than life
** Argument lines..
**
**                 UnHighlighted  Highlighted
** ------------------------------------------
** Button Background = BLACK        WHITE
** Button Text       = WHITE        BLUE
**/

COLOUR0 = 'BCOL_DEF=0 STYLE_DEF="[30m " BCOL_HI=0 STYLE_HI="[30m>"'
COLOUR1 = 'BCOL_DEF=0 STYLE_DEF="[31m " BCOL_HI=7 STYLE_HI="[34m>"'
COLOUR2 = 'BCOL_DEF=0 STYLE_DEF="[32m " BCOL_HI=7 STYLE_HI="[34m>"'
COLOUR3 = 'BCOL_DEF=0 STYLE_DEF="[33m " BCOL_HI=7 STYLE_HI="[34m>"'
COLOUR4 = 'BCOL_DEF=0 STYLE_DEF="[34m " BCOL_HI=7 STYLE_HI="[34m>"'
COLOUR5 = 'BCOL_DEF=0 STYLE_DEF="[35m " BCOL_HI=7 STYLE_HI="[34m>"'
COLOUR6 = 'BCOL_DEF=0 STYLE_DEF="[36m " BCOL_HI=7 STYLE_HI="[34m>"'
COLOUR7 = 'BCOL_DEF=0 STYLE_DEF="[37m " BCOL_HI=7 STYLE_HI="[34m>"'

/*
** Terminate xy menu
**
**/

B0 = " N   -  NO"
B1 = " Y   -  YES"
TX=63 ; TY=8

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+3'" UP=1 DOWN=1 LEFT=0 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+4'" UP=0 DOWN=0 LEFT=1 RIGHT=1'
'BUTTONDATA' 'SAVE="tplates:button/terminate'TX''TY'.bank"'
'BUTTONDATA' 'CLEAR'

/*
** Terminate xy menu
**
**/

B0 = " N   -  NO"
B1 = " Y   -  YES"
TX=2 ; TY=2

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+3'" UP=1 DOWN=1 LEFT=0 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+4'" UP=0 DOWN=0 LEFT=1 RIGHT=1'
'BUTTONDATA' 'SAVE="tplates:button/terminate'TX''TY'.bank"'
'BUTTONDATA' 'CLEAR'

/*
** Terminate xy menu
**
**/

B0 = " N   -  NO"
B1 = " Y   -  YES"
TX=30 ; TY=14

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+3'" UP=1 DOWN=1 LEFT=0 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+4'" UP=0 DOWN=0 LEFT=1 RIGHT=1'
'BUTTONDATA' 'SAVE="tplates:button/terminate'TX''TY'.bank"'
'BUTTONDATA' 'CLEAR'

/*
** Terminate xy menu
**
**/

B0 = " N   -  NO"
B1 = " Y   -  YES"
TX=32 ; TY=15

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+3'" UP=1 DOWN=1 LEFT=0 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=13 XPOS="'TX+1'" YPOS="'TY+4'" UP=0 DOWN=0 LEFT=1 RIGHT=1'
'BUTTONDATA' 'SAVE="tplates:button/terminate'TX''TY'.bank"'
'BUTTONDATA' 'CLEAR'

/*
** The main menu
**
** Setup the actual contents of the buttons text. All
** within the correct order.. working from button 0,..1,..5 etc.
**/

B0 = " F   -   File Conferences"
B1 = " M   -   Message Conferences"
B2 = " C   -   Call For System Operator"
B3 = " L   -   Leave Feedback to Operator"
B4 = " P   -   Goto Edit Preferences Menu"
B5 = " D   -   Entertainment Areas"
B6 = " X   -   Aminet CDROM Files"
B7 = " W   -   Waiting Mail?"
B8 = " G   -   Terminate Connection"
B9 = " B   -   ApT-BBS! Software."
B10 = " 1   -   Amiga Files & Mail"
B11 = " 2   -   IBM Files & Mail"
B12 = " 3   -   QWK Off-Line Mail Bundler"
B13 = " 4   -   Global New File Scan"
B14 = " 5   -   ALL Picture Areas"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=5 UP=14 DOWN=1 LEFT=1 RIGHT=14'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=6 UP=0 DOWN=2 LEFT=2 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=7 UP=1 DOWN=3 LEFT=3 RIGHT=1'
'BUTTONDATA' 'BUTTONTEXT="'B3'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=8 UP=2 DOWN=4 LEFT=4 RIGHT=2'
'BUTTONDATA' 'BUTTONTEXT="'B4'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=9 UP=3 DOWN=5 LEFT=5 RIGHT=3'
'BUTTONDATA' 'BUTTONTEXT="'B5'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=10 UP=4 DOWN=6 LEFT=6 RIGHT=4'
'BUTTONDATA' 'BUTTONTEXT="'B6'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=11 UP=5 DOWN=7 LEFT=7 RIGHT=5'
'BUTTONDATA' 'BUTTONTEXT="'B7'"' COLOUR2 'WIDTH=38 XPOS=21 YPOS=12 UP=6 DOWN=8 LEFT=8 RIGHT=6'
'BUTTONDATA' 'BUTTONTEXT="'B8'"' COLOUR1 'WIDTH=38 XPOS=21 YPOS=13 UP=7 DOWN=9 LEFT=9 RIGHT=7'
'BUTTONDATA' 'BUTTONTEXT="'B9'"' COLOUR5 'WIDTH=38 XPOS=21 YPOS=14 UP=8 DOWN=10 LEFT=10 RIGHT=8'
'BUTTONDATA' 'BUTTONTEXT="'B10'"' COLOUR3 'WIDTH=38 XPOS=21 YPOS=15 UP=9 DOWN=11 LEFT=11 RIGHT=9'
'BUTTONDATA' 'BUTTONTEXT="'B11'"' COLOUR3 'WIDTH=38 XPOS=21 YPOS=16 UP=10 DOWN=12 LEFT=12 RIGHT=10'
'BUTTONDATA' 'BUTTONTEXT="'B12'"' COLOUR3 'WIDTH=38 XPOS=21 YPOS=17 UP=11 DOWN=13 LEFT=13 RIGHT=11'
'BUTTONDATA' 'BUTTONTEXT="'B13'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=18 UP=12 DOWN=14 LEFT=14 RIGHT=12'
'BUTTONDATA' 'BUTTONTEXT="'B14'"' COLOUR7 'WIDTH=38 XPOS=21 YPOS=19 UP=13 DOWN=0 LEFT=0 RIGHT=13'
'BUTTONDATA' 'SAVE="tplates:button/main.bank"'
'BUTTONDATA' 'CLEAR'

/*
** Preferences selection menu:
**/


B0 = "  P   -   Edit your personal Information"
B1 = "  S   -   Edit System specific Data"
B2 = "  Q   -   Return to previous menu"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=49 XPOS=16 YPOS=10 UP=2 DOWN=1 LEFT=1 RIGHT=2' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=49 XPOS=16 YPOS=11 UP=0 DOWN=2 LEFT=2 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=49 XPOS=16 YPOS=12 UP=1 DOWN=0 LEFT=0 RIGHT=1' /* 0 */
'BUTTONDATA' 'SAVE="tplates:button/prefs.bank"'
'BUTTONDATA' 'CLEAR'

/*
** Terminate!? menu
**
**/

B0 = " N   -  NO"
B1 = " Y   -  YES"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=13 XPOS=64 YPOS=11 UP=1 DOWN=1 LEFT=0 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=13 XPOS=64 YPOS=12 UP=0 DOWN=0 LEFT=1 RIGHT=1'
'BUTTONDATA' 'SAVE="tplates:button/terminate.bank"'
'BUTTONDATA' 'CLEAR'


/*
** Amiga menu
**
** Setup the actual contents of the buttons text. All
** within the correct order.. working from button 0,..1,..5 etc.
**/

B0 = " F   -   File Conferences"
B1 = " M   -   Message Conferences"
B2 = " C   -   Call For System Operator"
B3 = " L   -   Leave Feedback to Operator"
B4 = " P   -   Goto Edit Preferences Menu"
B5 = " D   -   Entertainment Areas"
B6 = " Q   -   Quit Back to Main Menu"
B7 = " W   -   Waiting Mail?"
B8 = " G   -   Terminate Connection"
B9 = " B   -   QWK Off-Line Mail Bundler"
B10 = " N   -   New File Scan"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=6 UP=10 DOWN=1 LEFT=1 RIGHT=10'  /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=7 UP=0 DOWN=2 LEFT=2 RIGHT=0'    /* 1 */
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=8 UP=1 DOWN=3 LEFT=3 RIGHT=1'    /* 2 */
'BUTTONDATA' 'BUTTONTEXT="'B3'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=9 UP=2 DOWN=4 LEFT=4 RIGHT=2'    /* 3 */
'BUTTONDATA' 'BUTTONTEXT="'B4'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=10 UP=3 DOWN=5 LEFT=5 RIGHT=3'   /* 4 */
'BUTTONDATA' 'BUTTONTEXT="'B5'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=11 UP=4 DOWN=6 LEFT=6 RIGHT=4'   /* 5 */
'BUTTONDATA' 'BUTTONTEXT="'B6'"' COLOUR3 'WIDTH=37 XPOS=21 YPOS=12 UP=5 DOWN=7 LEFT=7 RIGHT=5'   /* 6 */
'BUTTONDATA' 'BUTTONTEXT="'B7'"' COLOUR2 'WIDTH=37 XPOS=21 YPOS=13 UP=6 DOWN=8 LEFT=8 RIGHT=6'   /* 7 */
'BUTTONDATA' 'BUTTONTEXT="'B8'"' COLOUR1 'WIDTH=37 XPOS=21 YPOS=14 UP=7 DOWN=9 LEFT=9 RIGHT=7'   /* 8 */
'BUTTONDATA' 'BUTTONTEXT="'B9'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=15 UP=8 DOWN=10 LEFT=10 RIGHT=8' /* 9 */
'BUTTONDATA' 'BUTTONTEXT="'B10'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=16 UP=9 DOWN=0 LEFT=0 RIGHT=9'  /* 10 */
'BUTTONDATA' 'SAVE="tplates:button/amiga.bank"'
'BUTTONDATA' 'CLEAR'

/*
** ApT menu
**
** Setup the actual contents of the buttons text. All
** within the correct order.. working from button 0,..1,..5 etc.
**/

B0 = " F   -   File Conferences"
B1 = " M   -   Message Conferences"
B2 = " C   -   Call For System Operator"
B3 = " L   -   Leave Feedback to Operator"
B4 = " Q   -   Quit Back to Main Menu"
B5 = " W   -   Waiting Mail?"
B6 = " G   -   Terminate Connection"
B7 = " N   -   New File Scan"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=6 UP=7 DOWN=1 LEFT=1 RIGHT=7' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=7 UP=0 DOWN=2 LEFT=2 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=8 UP=1 DOWN=3 LEFT=3 RIGHT=1' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B3'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=9 UP=2 DOWN=4 LEFT=4 RIGHT=2' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B4'"' COLOUR3 'WIDTH=37 XPOS=21 YPOS=10 UP=3 DOWN=5 LEFT=5 RIGHT=3' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B5'"' COLOUR2 'WIDTH=37 XPOS=21 YPOS=11 UP=4 DOWN=6 LEFT=6 RIGHT=4' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B6'"' COLOUR1 'WIDTH=37 XPOS=21 YPOS=12 UP=5 DOWN=7 LEFT=7 RIGHT=5' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B7'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=13 UP=6 DOWN=0 LEFT=0 RIGHT=6' /* 0 */
'BUTTONDATA' 'SAVE="tplates:button/apt.bank"'
'BUTTONDATA' 'CLEAR'

/*
** IBM menu
**
** Setup the actual contents of the buttons text. All
** within the correct order.. working from button 0,..1,..5 etc.
**/

B0 = " F   -   File Conferences"
B1 = " M   -   Message Conferences"
B2 = " C   -   Call For System Operator"
B3 = " L   -   Leave Feedback to Operator"
B4 = " P   -   Goto Edit Preferences Menu"
B5 = " D   -   Entertainment Areas"
B6 = " Q   -   Quit Back to Main Menu"
B7 = " W   -   Waiting Mail?"
B8 = " G   -   Terminate Connection"
B9 = " B   -   QWK Off-Line Mail Bundler"
B10 = " N   -   New File Scan"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=6 UP=10 DOWN=1 LEFT=1 RIGHT=10' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=7 UP=0 DOWN=2 LEFT=2 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=8 UP=1 DOWN=3 LEFT=3 RIGHT=1' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B3'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=9 UP=2 DOWN=4 LEFT=4 RIGHT=2' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B4'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=10 UP=3 DOWN=5 LEFT=5 RIGHT=3' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B5'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=11 UP=4 DOWN=6 LEFT=6 RIGHT=4' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B6'"' COLOUR3 'WIDTH=37 XPOS=21 YPOS=12 UP=5 DOWN=7 LEFT=7 RIGHT=5' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B7'"' COLOUR2 'WIDTH=37 XPOS=21 YPOS=13 UP=6 DOWN=8 LEFT=8 RIGHT=6' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B8'"' COLOUR1 'WIDTH=37 XPOS=21 YPOS=14 UP=7 DOWN=9 LEFT=9 RIGHT=7' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B9'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=15 UP=8 DOWN=10 LEFT=10 RIGHT=8' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B10'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=16 UP=9 DOWN=0 LEFT=0 RIGHT=9' /* 0 */
'BUTTONDATA' 'SAVE="tplates:button/ibm.bank"'
'BUTTONDATA' 'CLEAR'

/*
** GAME menu
**
** Setup the actual contents of the buttons text. All
** within the correct order.. working from button 0,..1,..5 etc.
**/

B0 = "0  -  Hack&Slash"
B1 = "1  -  Space Emp' 1"
B2 = "2  -  Space Emp' 2"
B3 = "3  -  Blackmar's"
B4 = "4  -  GLOBAL WAR!"
B5 = "5  -  ParaWorld"
B6 = "6  -  Knight-Realm"
B7 = "7  -  Lotto"
B8 = "8  -  AnsiWarriors"
B9 = "9  -  What Animal?"
B10 = "H  -  Chess"
B11 = "M  -  Master Mind"
B12 = "P  -  Solitaire"
B13 = "T  -  TIS"
B14 = "V  -  Knot!"
B15 = "Z  -  Trivia"
B16 = "X  -  DeepSpace!"

B17 = "C  -  Call the Sysop"
B18 = "Q  -  Quit to Last Menu"
B19 = "G  -  GoodBye Log-Off"
B20 = "E  -  Database"
B21 = "F  -  Fish Disk Viewer"
B22 = "N  -  Neverending Story"

B23 = "A  -  Ballyhoo"
B24 = "B  -  Suspect"
B25 = "D  -  LGOP"
B26 = "I  -  Infidel"
B27 = "J  -  Zork1"
B28 = "K  -  Zork2"
B29 = "O  -  Zork3"
B30 = "R  -  Planetfall"
B31 = "S  -  HHGTTG"
B32 = "W  -  Hollywood HJ"
B33 = "Y  -  Wishbringer"
B34 = "L  -  AMFV"
B35 = "U  -  BBS Lister"


'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=2 UP=34 DOWN=1 LEFT=23 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=3 UP=0 DOWN=2 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=4 UP=1 DOWN=3 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B3'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=5 UP=2 DOWN=4 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B4'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=6 UP=3 DOWN=5 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B5'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=7 UP=4 DOWN=6 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B6'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=8 UP=5 DOWN=7 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B7'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=9 UP=6 DOWN=8 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B8'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=10 UP=7 DOWN=9 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B9'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=11 UP=8 DOWN=10 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B10'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=12 UP=9 DOWN=11 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B11'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=13 UP=10 DOWN=12 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B12'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=14 UP=11 DOWN=13 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B13'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=15 UP=12 DOWN=14 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B14'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=16 UP=13 DOWN=15 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B15'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=17 UP=14 DOWN=16 LEFT=22 RIGHT=17' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B16'"' COLOUR7 'WIDTH=18 XPOS=2 YPOS=18 UP=15 DOWN=17 LEFT=22 RIGHT=17' /* 0 */

'BUTTONDATA' 'BUTTONTEXT="'B17'"' COLOUR3 'WIDTH=23 XPOS=28 YPOS=7 UP=16 DOWN=18 LEFT=0 RIGHT=23' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B18'"' COLOUR2 'WIDTH=23 XPOS=28 YPOS=8 UP=17 DOWN=19 LEFT=0 RIGHT=23' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B19'"' COLOUR1 'WIDTH=23 XPOS=28 YPOS=9 UP=18 DOWN=20 LEFT=0 RIGHT=23' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B20'"' COLOUR7 'WIDTH=23 XPOS=28 YPOS=10 UP=19 DOWN=21 LEFT=0 RIGHT=23' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B21'"' COLOUR7 'WIDTH=23 XPOS=28 YPOS=11 UP=20 DOWN=22 LEFT=0 RIGHT=23' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B22'"' COLOUR7 'WIDTH=23 XPOS=28 YPOS=12 UP=21 DOWN=35 LEFT=0 RIGHT=23' /* 0 */

'BUTTONDATA' 'BUTTONTEXT="'B23'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=2 UP=22 DOWN=24 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B24'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=3 UP=23 DOWN=25 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B25'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=4 UP=24 DOWN=26 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B26'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=5 UP=25 DOWN=27 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B27'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=6 UP=26 DOWN=28 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B28'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=7 UP=27 DOWN=29 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B29'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=8 UP=28 DOWN=30 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B30'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=9 UP=29 DOWN=31 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B31'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=10 UP=30 DOWN=32 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B32'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=11 UP=31 DOWN=33 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B33'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=12 UP=32 DOWN=34 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B34'"' COLOUR7 'WIDTH=20 XPOS=58 YPOS=13 UP=33 DOWN=0 LEFT=17 RIGHT=0' /* 0 */
'BUTTONDATA' 'BUTTONTEXT="'B35'"' COLOUR7 'WIDTH=23 XPOS=28 YPOS=13 UP=22 DOWN=23 LEFT=0 RIGHT=23' /* 35 */

'BUTTONDATA' 'SAVE="tplates:button/GAME.bank"'
'BUTTONDATA' 'CLEAR'

/*
** Picture Menu
**
** Setup the actual contents of the buttons text. All
** within the correct order.. working from button 0,..1,..5 etc.
**/

B0 = " F   -   File Conferences"
B1 = " M   -   Message Conferences"
B2 = " C   -   Call For System Operator"
B3 = " L   -   Leave Feedback to Operator"
B4 = " P   -   Goto Edit Preferences Menu"
B5 = " D   -   Entertainment Areas"
B6 = " Q   -   Quit Back to Main Menu"
B7 = " W   -   Waiting Mail?"
B8 = " G   -   Terminate Connection"
B9 = " N   -   New File Scan"

'BUTTONDATA' 'CLEAR'
'BUTTONDATA' 'BUTTONTEXT="'B0'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=6 UP=9 DOWN=1 LEFT=1 RIGHT=9'
'BUTTONDATA' 'BUTTONTEXT="'B1'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=7 UP=0 DOWN=2 LEFT=2 RIGHT=0'
'BUTTONDATA' 'BUTTONTEXT="'B2'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=8 UP=1 DOWN=3 LEFT=3 RIGHT=1'
'BUTTONDATA' 'BUTTONTEXT="'B3'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=9 UP=2 DOWN=4 LEFT=4 RIGHT=2'
'BUTTONDATA' 'BUTTONTEXT="'B4'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=10 UP=3 DOWN=5 LEFT=5 RIGHT=3'
'BUTTONDATA' 'BUTTONTEXT="'B5'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=11 UP=4 DOWN=6 LEFT=6 RIGHT=4'
'BUTTONDATA' 'BUTTONTEXT="'B6'"' COLOUR3 'WIDTH=37 XPOS=21 YPOS=12 UP=5 DOWN=7 LEFT=7 RIGHT=5'
'BUTTONDATA' 'BUTTONTEXT="'B7'"' COLOUR2 'WIDTH=37 XPOS=21 YPOS=13 UP=6 DOWN=8 LEFT=8 RIGHT=6'
'BUTTONDATA' 'BUTTONTEXT="'B8'"' COLOUR1 'WIDTH=37 XPOS=21 YPOS=14 UP=7 DOWN=9 LEFT=9 RIGHT=7'
'BUTTONDATA' 'BUTTONTEXT="'B9'"' COLOUR7 'WIDTH=37 XPOS=21 YPOS=15 UP=8 DOWN=0 LEFT=0 RIGHT=8'
'BUTTONDATA' 'SAVE="tplates:button/PICTURE.bank"'
'BUTTONDATA' 'CLEAR'


say CR "Finished...."

EXIT

/*##########################################################################*/

