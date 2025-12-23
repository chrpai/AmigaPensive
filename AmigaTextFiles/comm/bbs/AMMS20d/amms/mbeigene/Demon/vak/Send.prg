;                            VAK - Send_Programm
; $VER: 2.01
;
; (c) by Demon Driver `93 - `xx
;
; AMMS v1.00 (c) by Adrock & Fastman `89 - `93
;

Var:
VERSION="2.01"
VAKAN=0
POS=1
LAS=1

ReadPrefs:
_parse SEND p0 N USER S SUBJ
LOAD="Send"
_if "'{SYSNAME}'" == "SDII" then LOAD="SendSDII"
_gosub Check_VK
_disk execute/keep VAK:Sets/AllVK.set
_disk execute/keep VAK:Sets/'{language}'/'LOAD'.set
_if "'USER'" != "" then _goto Direckt_PM
_goto MainMask

; ----------------------------

Direckt_PM:
LAS=POS
POS=1
Mode="Write PM"
_gosub UpLines
_if "'USER'" == "" then _goto end
_gosub Cut_Name
_gosub RAW_Show_User
_gosub Key_Wait
_echo
_if "'SUBJ'" == "" then _brief "'USER'"
_if "'SUBJ'" != "" then _brief "'USER'" "'SUBJ'"
_goto end

; ------------------------------------------------------------------------

UpLines:
_echo/nolf "#reset##cls##home#"
_echo "#farbe 0,7#{LIMSPACEON}{TAB 59}{LIMSPACEOFF}#SETPOS 2,0##farbe 0,4#{LIMSPACEON}{TAB 59}{LIMSPACEOFF}#reset#"
_echo/nolf "#SETPOS 1,0##farbe 0,7#User : {name 20} Timeleft : {onlineleft 3}   Level : {level 4}#reset#"
_echo/nolf "#SETPOS 2,0##farbe 3,4#Mode : 'MODE' #SETPOS 2,33# VKs : 'VAKAN'#SETPOS 2,48# VK : 'VAKTR'#reset#"
_echo/nolf "#SETPOS 1,60#    VAK-Send v'VERSION'#SETPOS 2,60#    (c) by DD `94#SETPOS 4,0#"
_return

MainMask:
Mode="Main Menu"
_gosub UpLines
_if "'{SYSNAME}'" == "SDII" then _gosub SDIIMask
_if "'{SYSNAME}'" != "SDII" then _gosub NormMask
_gosub Select
_goto MainMenu

SDIIMask:
_echo/nolf "#reset##SETPOS 4,0#"
_disk text VAK:Mask/'{language}'/SendSDII.mas
_return

NormMask:
_echo/nolf "#reset##SETPOS 4,0#"
_disk text VAK:Mask/'{language}'/Send.mas
_return

; ------------------------------------------------------------------------

MainMenu:
DUMMY=CY+3
_echo/nolf "#SETPOS 'DUMMY','CX'#"
_menu/nopmsgs "" -
 "Q" End -
 "I" Info -
 "T" SysOpPM -
"^M" Take -
 "S" PM_Write -
"^1" UP -
"^2" DOWN -
"^3" LEFT -
"^4" RIGHT

; ------------------------------------------------------------------------

UP:
_if/integer MPU'POS' == 0 then _goto MainMenu
LAS=POS
POS=MPU'LAS'
_gosub DeSelect
_gosub Select
_goto MainMenu

DOWN:
_if/integer MPD'POS' == 0 then _goto MainMenu
LAS=POS
POS=MPD'LAS'
_gosub DeSelect
_gosub Select
_goto MainMenu

LEFT:_if/integer MPL'POS' == 0 then _goto MainMenu
LAS=POS
POS=MPL'LAS'
_gosub DeSelect
_gosub Select
_goto MainMenu

RIGHT:
_if/integer MPR'POS' == 0 then _goto MainMenu
LAS=POS
POS=MPR'LAS'
_gosub DeSelect
_gosub Select
_goto MainMenu

Take:
_if/integer POS == 1 then _goto PM_Write
_if/integer POS == 2 then _goto Info
_if/integer POS == 3 then _goto End
_if/integer POS == 4 then _goto SysOpPM
_goto MainMenu

; ------------------------------------------------------------------------

Select:
AKTX=MPX'POS'
AKTY=MPY'POS'+3
_echo/nolf "#SETPOS 'AKTY','AKTX'##invers#"
DUMMY=MPN'POS'
_echo/nolf "'DUMMY'"
_return

DeSelect:
AKTX=MPX'LAS'
AKTY=MPY'LAS'+3
_echo/nolf "#SETPOS 'AKTY','AKTX'##reset#"
DUMMY=MPN'LAS'
_echo/nolf "'DUMMY'"
_return

; ------------------------------------

Info:
LAS=POS
POS=2
Mode="FastNet-Info"
_gosub UpLines
_show nodes
_gosub Key_Wait
_goto MainMask

PM_Write:
LAS=POS
POS=1
Mode="Write PM"
_gosub UpLines
_disk text VAK:text/'{language}'/SEND_PMWrite.txt
_input/maxlength=40 USER
_if "'USER'" == "" then _goto MainMask
_gosub Cut_Name
_gosub RAW_Show_User
_gosub Key_Wait
_gosub UpLines
_on error
_echo
_brief "'USER'"
_goto MainMask

SysOpPM:
LAS=POS
POS=4
_if "'{SYSNAME}'" != "SDII" then _goto MainMenu
_disk execute mbmenu:SysOpPost.prg
_goto MainMask

; ------------------------------------------------------------------------

Check_VK:
_extern VAK:VAK.exe -cu "'{name}'"
_disk execute/keep t:Check.tmp
_if/integer CHECK == 1 then VAKTR="No "
_if/integer CHECK == 2 then VAKTR="Yes"
_return

; ------------------------------------------------------------------------

Key_Wait:
_echo "#reset#"
_disk text vak:Mask/'{language}'/Wait.mas
_waitkey
_return

Cut_Name:
_extern VAK:VAK.exe -cs "'USER'"
_disk execute/keep t:Username.tmp
_return

RAW_show_User:
Mode="Show VK of 'Username'"
_gosub UpLines
_echo "=============================================================================="
_extern VAK:VAK.exe -sv "'Username'"
_echo "^J=============================================================================="
_return

; --------------

End:
_echo/nolf "#reset##cls##home#"
_eod
