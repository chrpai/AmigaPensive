;                           VAK - Main-Programm
; $VER: 3.10
;
; (c) by Demon Driver `93 - `xx
;
; AMMS v1.00 (c) by Adrock & Fastman `89 - `93
;

Var:
VERSION="3.10"
VAKAN=0
POS=1
LAS=1
QuitT="T"

ReadPrefs:
_gosub Check_VK
NEWLI=0
_disk execute/keep VAK:Sets/AllVK.set
_disk execute/keep VAK:Sets/'{language}'/MainMenu.set
_gosub Check_Opt
_goto MainMask

Check_Opt:
_echo "#reset#"
_parse VAK p0 N USER
_if "'O1'" == "T" then _goto Own_Show
_if "'O2'" == "T" then _goto Own_Edit
_if "'O3'" == "T" then _goto Own_Delete
_if "'O4'" == "T" then _goto All_List_View
_if "'O5'" == "T" then _goto Help
_if "'O6'" == "T" then _goto FastNew
_if "'USER'" == "" then _return
_gosub Cut_Name
_goto Get_Username

Cut_Name:
_extern VAK:VAK.exe -cs "'USER'"
_disk execute/keep T:Username.tmp
_gosub RAW_Show_User
QuitT="F"
_goto Quit

MainMask:
_if/integer NEWLI == 1 then _goto ReadPrefs
Mode="Main Menu"
_gosub UpLines
_disk text VAK:Mask/'{language}'/Main.mas
_gosub Select
_goto MainMenu

UpLines:
_echo/nolf "#reset##cls##home#"
_echo "#farbe 0,7#{LIMSPACEON}{TAB 59}{LIMSPACEOFF}#SETPOS 2,0##farbe 0,4#{LIMSPACEON}{TAB 59}{LIMSPACEOFF}#reset#"
_echo/nolf "#SETPOS 1,0##farbe 0,7#User : {name 20} Timeleft : {onlineleft 3}   Level : {level 4}#reset#"
_echo/nolf "#SETPOS 2,0##farbe 3,4#Mode : 'MODE' #SETPOS 2,33# VKs : 'VAKAN'#SETPOS 2,48# VK : 'VAKTR'#reset#"
_echo/nolf "#SETPOS 1,60#      VAK v'VERSION'#SETPOS 2,60#    (c) by DD `94#SETPOS 4,0#"
_return

; ------------------------------------------------------------------------

VorMain:
_if "'O1'" == "T" then _goto Quit
_if "'O2'" == "T" then _goto Quit
_if "'O3'" == "T" then _goto Quit
_if "'O4'" == "T" then _goto Quit
_if "'O5'" == "T" then _goto Quit
_gosub DeSelect
_gosub Select
_goto MainMenu

MainMenu:
DUMMY=CY+3
_echo/nolf "#reset##SETPOS 'DUMMY','CX'#"
_menu/nopmsgs/noecho "" -
 "Q" Quit -
"^M" SelectPoint -
"^R" MainMask -
"^1" UP -
"^2" DOWN -
"^3" LEFT -
"^4" RIGHT -
 "S" Own_Show -
 "E" Own_Edit -
 "D" Own_Delete -
 "L" All_List -
 "V" All_View -
 "B" All_List_View -
 "A" About -
 "H" Help -
 "Z" SysOp

; ----------------------------

Quit:
_if "'QuitT'" == "T" then _gosub Quit1
_goto end

Quit1:
Mode="Quit"
_gosub UpLines
_echo "^J^J^J                       #vfarbe 4#............................."
_echo "                       : #reset#Thanx for using VAK v'VERSION' #vfarbe 4#:"
_echo "                       :       #reset#(c) by DD `93       #vfarbe 4#:"
_echo "                       :...........................:#reset#^J^J^J"
_wait 1
_return

About:
LAS=POS
POS=8
Mode="About"
_gosub UpLines
_disk text VAK:Text/'{language}'/About.txt
_gosub Key_Wait
_goto MainMask

Help:
LAS=POS
POS=7
PAGE=1
_gosub Help_Read
_if "'O5'" == "T" then _goto Quit
_goto MainMask

; ---------------------------

Own_Show:
LAS=POS
POS=2
_if VAKTR == "No " then _goto VorMain
_gosub Punkt_OwnShow
_if "'O1'" == "T" then _goto Quit
_goto MainMask

Own_Edit:
LAS=POS
POS=1
_gosub Punkt_OwnEdit
_if "'O2'" == "T" then _goto Quit
_goto MainMask

Own_Delete:
LAS=POS
POS=3
_if VAKTR == "No " then _goto VorMain
_gosub Punkt_OwnDelete
USER=
USERNAME=
_if "'O3'" == "T" then _goto Quit
_goto MainMask

All_List:
LAS=POS
POS=4
Mode="List all VKs"
_gosub UpLines
_disk text VAK:Text/VK-List.txt
_gosub Key_Wait
_goto MainMask

; ---------------------------

All_View:
LAS=POS
POS=5
Mode="Show VK of a User"
_gosub UpLines
_echo/nolf " Username : "
_input/maxlength=20 Username
_on error All_View_NoVAK
_gosub RAW_Show_User
_gosub Key_Wait
_goto MainMask

All_View_NoVAK:
_gosub UpLines
_if {language} == "English" then _echo "                           This user have no VK"
_if {language} == "Deutsch" then _echo "                         Dieser User hat keine VK"
_gosub Key_Wait
_goto MainMask

; ----------------------------

All_List_View:
LAS=POS
POS=6
AKT=1
LET=1
AX=0
AY=4
LX=0
LY=4
PAGE=1
_gosub Punkt_All_List_View
_if "'O4'" == "T" then _goto Quit
_goto MainMask

; ----------------------------

SelectPoint:
_if/integer POS == 1 then _goto Own_Edit
_if/integer POS == 2 then _goto Own_Show
_if/integer POS == 3 then _goto Own_Delete
_if/integer POS == 4 then _goto All_List
_if/integer POS == 5 then _goto All_View
_if/integer POS == 6 then _goto All_List_View
_if/integer POS == 7 then _goto Help
_if/integer POS == 8 then _goto About
_if/integer POS == 9 then _goto Quit
_goto MainMask

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

; ------------------------------------------------------------------------

Punkt_OwnShow:
Mode="Show VK"
_gosub UpLines
_gosub RAW_show
_gosub Key_Wait
_return

; ---------------------------

Punkt_OwnEdit:
Mode="Edit VK"
_gosub UpLines
_if VAKTR == "Yes" then _gosub RAW_Show
_echo/nolf "^J^J"
_if {language} == "English" then _echo/nolf "  Really edit your VK : Y#left 1#"
_if {language} == "Deutsch" then _echo/nolf "  Wirklich Deine VK editieren : J#left 1#"
_menu/nopmsgs "" -
"^M" Punkt_OwnEdit_Edit -
 "J" Punkt_OwnEdit_Edit -
 "Y" Punkt_OwnEdit_Edit -
 "N" Punkt_OwnEdit_End

Punkt_OwnEdit_End:
_return

Punkt_OwnEdit_Edit:
_if VAKTR == "No " then _extern copy VAK:Sets/VK "mbudir:'{name}'/"
_disk edit/lines=6 "mbudir:'{name}'/vk"
TEST=VAKTR
_gosub Check_VK
_if VAKTR != TEST then _gosub Make_List
_return

; ---------------------------

Punkt_OwnDelete:
Mode="Delete VK"
_gosub UpLines
_gosub RAW_show
_echo "^J^J"
_if {language} == "English" then _echo/nolf "   Really delete your VK : N#left 1#"
_if {language} == "Deutsch" then _echo/nolf "   Wirklich Deine VK loeschen : N#left 1#"
_menu/nopmsgs "" -
"^M" Punkt_OwnDelete_end -
 "N" Punkt_OwnDelete_end -
 "Y" Punkt_OwnDelete_sure -
 "J" Punkt_OwnDelete_sure

Punkt_OwnDelete_end:
_return

Punkt_OwnDelete_sure:
_extern delete >nil: "mbudir:'{name}'/VK"
_gosub Check_VK
_gosub Make_List
_goto Punkt_OwnDelete_end

; ------------------------------------------------------------------------

RAW_show:
_if {language} == "English" then _echo "                                 Your VK :"
_if {language} == "Deutsch" then _echo "                                Deine VK :"
_echo "=============================================================================="
_extern VAK:VAK.exe -sv "'{name}'"
_echo "^J=============================================================================="
_return

RAW_show_User:
Mode="Show VK of 'Username'"
_gosub UpLines
_echo "=============================================================================="
_extern VAK:VAK.exe -sv "'Username'"
_echo "^J=============================================================================="
_return

; ------------------------------------------------------------------------
; ---------------------------- All-List-View -----------------------------

Punkt_All_List_View:
_disk execute/keep VAK:sets/AllPage.set
_disk execute/keep VAK:sets/Page'PAGE'.set
_goto Punkt_All_List_View1

Punkt_All_List_View1:
Mode="List/View  Page 'PAGE'/'PAGES'"
_gosub UpLines
DX=0
DY=4
DN=0
_if/integer PAGE == PAGES then DO=PAGENR
_if/integer PAGE < PAGES then DO=45
_echo/nolf "#farbe 7,4#"
_gosub ListView_Mask
_gosub LV_Select
_echo/nolf "#reset##SETPOS 'DY',0##down 1#"
_disk text VAK:Mask/'{language}'/ListView.mas
_goto ListView_Menu

ListView_Mask:
DN=DN+1
DUMMY=POS'DN'
_echo/nolf "#SETPOS 'DY','DX'#{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#left 26#'DUMMY'"
DX=DX+26
_if/integer DX == 78 then DY=DY+1
_if/integer DX == 78 then DX=0
_if/integer DN == DO then _gosub ListView_MaskRest
_if/integer DN == DO then _return
_goto ListView_Mask

ListView_MaskRest:
_if/integer DX == 0 then _return
_echo/nolf "#SETPOS 'DY','DX'#{LIMSPACEON}{TAB 26}{LIMSPACEOFF}"
DX=DX+26
_if/integer DX == 78 then DX=0
_goto ListView_MaskRest

ListView_Menu:
_menu/nopmsgs/noecho "" -
 "Q" LV_Quit -
 "X" LV_Quit -
"^R" Punkt_All_List_View -
"^M" LV_Show_Select -
 "H" LV_Help -
 "T" LV_Top_Page -
 "B" LV_End_Page -
 "N" LV_Next_Page -
 "L" LV_Prev_Page -
 "F" LV_First_Page -
 "E" LV_Last_Page -
"^1" LV_UP -
"^2" LV_DOWN -
"^3" LV_LEFT -
"^4" LV_RIGHT

; ------------------------------------------

LV_Help:
TPAGE=PAGE
PAGE=9
_gosub Help_Read
PAGE=TPAGE
_goto Punkt_All_List_View

LV_Show_Select:
Username=POS'AKT'
_gosub RAW_Show_User
_gosub Key_Wait
_goto Punkt_All_List_View

; ------------------------------------------

LV_UP:
_if/integer AKT-3 < 1 then _goto LV_Prev_Page
LAT=AKT
AKT=AKT-3
LX=AX
LY=AY
AY=AY-1
_gosub LV_DeSelect
_gosub LV_Select
_goto ListView_Menu

LV_DOWN:
_if/integer AKT+3 > DO then _goto LV_Next_Page
LAT=AKT
AKT=AKT+3
LX=AX
LY=AY
AY=AY+1
_gosub LV_DeSelect
_gosub LV_Select
_goto ListView_Menu

LV_LEFT:
_if/integer AKT-1 < 1 then _goto ListView_Menu
LX=AX
LY=AY
_if/integer AX == 0 then AY=AY-1
_if/integer AX == 0 then AX=78
AX=AX-26
LAT=AKT
AKT=AKT-1
_gosub LV_DeSelect
_gosub LV_Select
_goto ListView_Menu

LV_RIGHT:
_if/integer AKT+1 > DO then _goto ListView_Menu
LX=AX
LY=AY
_if/integer AX == 52 then AY=AY+1
_if/integer AX == 52 then AX=-26
AX=AX+26
LAT=AKT
AKT=AKT+1
_gosub LV_DeSelect
_gosub LV_Select
_goto ListView_Menu 

; ------------

LV_Quit:
_return

; ---------------------------

LV_Top_Page:
LAT=AKT
AKT=1
LX=AX
LY=AY
AX=0
AY=4
_gosub LV_DeSelect
_gosub LV_Select
_goto ListView_Menu

LV_End_Page:
LAT=AKT
AKT=DO
LX=AX
LY=AY
AY=4
TEST=0
DUM=AKT
_gosub LV_EP_YSel
TEST=AKT-1
_gosub LV_EP_XSel
_gosub LV_DeSelect
_gosub LV_Select
_goto ListView_Menu

LV_EP_YSel:
TEST=TEST+3
_if/integer TEST >= DUM then _return
AY=AY+1
_goto LV_EP_YSel

LV_EP_XSel:
_if/integer TEST < 3 then AX=26*TEST
_if/integer TEST < 3 then _return
TEST=TEST-3
_goto LV_EP_XSel

LV_Next_Page:
_if/integer PAGE == PAGES then _goto ListView_Menu
PAGE=PAGE+1
AX=0
AY=4
AKT=1
_goto Punkt_All_List_View

LV_Prev_Page:
_if/integer PAGE == 1 then _goto ListView_Menu
PAGE=PAGE-1
AX=0
AY=4
AKT=1
_goto Punkt_All_List_View

LV_First_Page:
PAGE=1
AX=0
AY=4
AKT=1
_goto Punkt_All_List_View

LV_Last_Page:
PAGE=PAGES
AX=0
AY=4
AKT=1
_goto Punkt_All_List_View

; -------------------------------------

LV_Select:
DUMMY=POS'AKT'
_echo/nolf "#SETPOS 'AY','AX'##farbe 4,6#'DUMMY'"
_return

LV_DeSelect:
DUMMY=POS'LAT'
_echo/nolf "#SETPOS 'LY','LX'##farbe 7,4#'DUMMY'
_return

; ------------------------------------------------------------------------
; ------------------------------------------------------------------------

Check_VK:
_extern VAK:VAK.exe -cu "'{name}'"
_disk execute/keep t:Check.tmp
_if/integer CHECK == 1 then VAKTR="No "
_if/integer CHECK == 2 then VAKTR="Yes"
_return

; ------------------------------------------------------------------------

Help_Read:
PAGES=12
_goto Help_Read_Menu

Help_Read_Menu:
MODE="Help  Page 'PAGE'/'PAGES'"
_gosub UpLines
_disk text VAK:Help/'{language}'/Page'PAGE'.txt
_menu/noecho/nopmsgs "" -
 "Q" HelpQuit -
 "G" GoPage -
 "I" Index -
"^R" Help_Read_Menu -
"^1" FirstPage - 
"^2" LastPage - 
"^3" PageBack - 
"^4" PageForw -
 "E" EditText

HelpQuit:
_if "'O5'" == "T" then _goto Quit
_return

GoPage:
Mode="Help  Go to a page"
_gosub UpLines
_echo/nolf "Type the pagenumber (max. 'PAGES') : "
_input/maxlength=3/numeric NUMBER
_if "'NUMBER'" == "" then _goto Help_Read_Menu
_if/integer NUMBER > PAGES then _goto Help_Read_Menu
_if/integer NUMBER < 1 then _goto Help_Read_Menu
PAGE=NUMBER
_goto Help_Read_Menu

Index:
PAGE=2
_goto Help_Read_Menu

FirstPage:
PAGE=1
_goto Help_Read_Menu

LastPage:
PAGE=PAGES
_goto Help_Read_Menu

PageBack:
_if/integer PAGE == 1 then _goto Help_Read_Menu
PAGE=PAGE-1
_goto Help_Read_Menu

PageForw:
_if/integer PAGE == PAGES then _goto Help_Read_Menu
PAGE=PAGE+1
_goto Help_Read_Menu

EditText:
_if "'{name}'" != "Demon Driver" then _goto Help_Read_Menu
_disk ed VAK:Help/English/Page'PAGE'.txt
_gosub Key_Wait
_goto Help_Read_Menu

; ------------------------------------------------------------------------

Make_List:
Mode="Make new VK-List"
_gosub UpLines
_extern VAK:VAK.exe
NEWLI=1
_gosub Key_Wait
_return

; ----------------------------------

Key_Wait:
_echo
_disk text VAK:Mask/'{language}'/Wait.mas
_waitkey
_return

; ----------------------------------

FastNew:
_extern copy VAK:sets/VK "mbudir:'{name}'/vk"
_disk edit/lines=6 "mbudir:'{name}'/vk"
_gosub Make_List
_echo/nolf "#reset##cls##home#"
_goto end

; ----------------------------------

SysOp:
_if "'OS'" == "F" then _goto MainMask
Mode="SysOp - Menu"
_gosub uplines
_disk text VAK:Mask/'{language}'/SysOp.mas
_menu/nopmsgs "" -
"^M" SysOp -
 "Q" MainMask -
 "F" Force_NewList -
 "E" EditUserVK
_goto MainMask

Force_NewList:
Mode="Force NEW VK-List"
_gosub uplines
_extern VAK:VAK.exe
_disk text vak:Mask/'{language}'/Wait.mas
_waitkey "^M"
_goto SysOp

EditUserVK:
Mode="Edit User VK"
_gosub Uplines
_echo/nolf "#setpos 4,1#Username : "
_input/maxlength=20 USERNAME
_extern VAK:VAK.exe -cu "'USERNAME'"
_disk execute/keep t:Check.tmp
_echo "'CHECK'"
_if/integer 'CHECK' == 0 then _goto EditUserVK_NoUser
_if/integer 'CHECK' == 1 then _goto EditUserVK_NoVK
_disk edit "mbudir:'USERNAME'/VK"
_disk text vak:Mask/'{language}'/Wait.mas
_waitkey "^M"
_goto SysOp

EditUserVK_NoUser:
_gosub Uplines
_echo "^j"
_echo "                             User don`t exsits"
_disk text vak:Mask/'{language}'/Wait.mas
_waitkey
_goto SysOp

EditUserVK_NoVK:
Mode="Edit User VK (No VK)"
_gosub Uplines
_echo "                            This User has no VK^J"
_echo/nolf "Make new : N#left 1#"
_menu "" -
"^M" SysOp -
 "N" SysOp -
 "J" EditUserVK_New -
 "Y" EditUserVK_New

EditUserVK_New:
_extern copy VAK:Sets/VK "mbudir:'USERNAME'/"
_disk edit "mbudir:'USERNAME'/VK"
_goto SysOp


; ----------------------------------

End:
_eod
