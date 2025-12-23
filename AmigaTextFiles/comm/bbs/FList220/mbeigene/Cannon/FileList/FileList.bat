;
; FileList V2.12  by cANNoN/iDP!
; $VER: FileList V2.12 / 23.06.97
;


 comname   = "FILELIST V2.12"
 path      = "mbeigene:Cannon/FileList/"
 cdpath    = "'{cdpath}'"
 posi      = 0
 count     = 0
 point     = 2


_on break/ignore
_on error/ignore
_on hangup HANGUP

_gosub READ_CONFIG
_gosub LOAD_TEXT

_if o1 == "T" then _goto SCAN

_goto MAKE_MENU


; =<scan filelist>========================================================

SCAN:
_load line "'path'FileList.dirs" posi areaname posi
_if/i posi == -1 then _goto NO_DIRS
_if/i posi ==  0 then _goto EXIT
_cd "'areaname'"
 fbname = ""
_gosub GET_BOARD
_goto SCAN

GET_BOARD:
_get dir/fboards "'fbname'" fbname
_if fbname == "" then _return
 xx   = 0
_gosub CHECK_NOBOARD
_if/i xx == 1 then _goto GET_BOARD
_get dir/fboards/path "'fbname'" diname
_filelist/action=116/file="'diname'BOARDINFO" "'LISTSTRING'" "'fbname'"
_goto GET_BOARD

; ------------------------------------------------------------------------

CHECK_NOBOARD:
_fd open noboards "'path'FileList.noboards" R

CHECK_NOBOARD2:
_fd read/eoflabel=CHECK_NOBOARD_END noboards string
_if/p fbname == string then xx = 1
_if/p fbname == string then _goto CHECK_NOBOARD_END
_goto CHECK_NOBOARD2

CHECK_NOBOARD_END:
_fd close noboards
_return

; =<scanning for download>================================================

DOWN_SCAN:
_load line "'path'FileList.dirs" posi areaname posi
_if/i posi == -1 then _goto NO_DIRS
_if/i posi ==  0 then _return
_cd "'areaname'"
 fbname = ""
_gosub DOWN_BOARD
_goto DOWN_SCAN

DOWN_BOARD:
_get dir/fboards "'fbname'" fbname
_if fbname == "" then _return
 xx   = 0
_gosub CHECK_NOBOARD
_if/i xx == 1 then _goto DOWN_BOARD
_check fboard'fbpara' "'fbname'" DOWN_BOARD
_get dir/fboards/path "'fbname'" diname
_check file "'diname'BOARDINFO" DOWN_BOARD
_echo/nolf T_SCANLINE_1
_echo/nolf "'areaname'"
_echo/nolf T_SCANLINE_2
_echo/nolf "'fbname'"
_echo/nolf T_SCANLINE_3
 diskfile = "'diname'BOARDINFO"
_extern/quiet QJoin "'diskfile'" "'DATAPATH'FileList.txt" 
_echo/nolf T_NEXTLINE
_goto DOWN_BOARD

; ------------------------------------------------------------------------

; =<read cruncher-config>=================================================

READ_CONFIG:
_load line "'path'FileList.cfg" posi packer'count' posi
_if/i posi == -1 then _goto NO_CONFIG
_if/i posi ==  0 then _return
_load line/empty "'path'FileList.cfg" posi ppara'count' posi
_load line/empty "'path'FileList.cfg" posi suffix'count' posi
 count = count+1
_goto READ_CONFIG

; ------------------------------------------------------------------------

; =<load locale-texts>====================================================

LOAD_TEXT:
 lang={DEFLANGUAGE}
_check file "'path''{ALANGUAGE}'/FileList.bat" GET_TEXT
 lang={ALANGUAGE}
GET_TEXT:
_disk execute/keep "'path''lang'/FileList.bat"
_gosub CHECK_PORT
_if o1 == "T" then _return
_if/i PACKMODE >= count then PACKMODE = count-1
_disk text "'path''lang'/FileList.txt"
_echo/nolf KEY2_OFF
_echo/nolf KEY3_OFF
_echo/nolf KEY4_OFF
_echo/nolf KEY5_OFF
_gosub SHOW_PACK_OFF
_gosub SHOW_SCAN_OFF
_check file "'path''lang'/FileList.start" SHOW_NOLOGO
_echo/nolf T_ZONEON
_disk text "'path''lang'/FileList.start"
_echo/nolf T_ZONEOFF
SHOW_NOLOGO:
_return

; ------------------------------------------------------------------------

; =<check ports>==========================================================

CHECK_PORT:
_check port/command="FILELIST*" ~('{PORT}') NOTHING
_echo/nolf T_EXIST
_echo/nolf T_EXIST_END
_eod
NOTHING:
_return

; ------------------------------------------------------------------------

; =<no filelist exists>===================================================

NO_FILELIST:
_echo/nolf T_ZONEON
_echo/nolf T_NOFILELIST
_echo/nolf T_ZONEOFF
_return

; ------------------------------------------------------------------------

; =<download filelist>====================================================

DOWN_FILELIST:
_check file "'DATAPATH'FileList.txt" NO_FILELIST
_echo/nolf T_PACK
_gosub CRUNCH
_echo/nolf T_DOWN
_echo/nolf T_DSR_ON
 suffix = suffix'PACKMODE'
_on error DOWN_BREAK
_disk down "'DATAPATH'FileList.'suffix'"
_echo/nolf T_DSR_OFF
_goto MAKE_MENU

DOWN_BREAK:
_on error/ignore
_echo/nolf T_DSR_BREAK
_goto MAKE_MENU

; ------------------------------------------------------------------------

; =<show filelist>========================================================

SHOW_FILELIST:
_check file "'DATAPATH'FileList.txt" NO_FILELIST
_cls
_disk text "'DATAPATH'FileList.txt"
_echo/nolf T_WAITKEY
_waitkey
_gosub LOAD_TEXT
_goto MAKE_MENU

; ------------------------------------------------------------------------

; =<crunch filelist>======================================================

CRUNCH:
 cruncher = ppara'PACKMODE'
_if cruncher == "" then _return
 suffix = suffix'PACKMODE'
_extern/quiet Delete "'DATAPATH'FileList.'suffix'"
_extern 'cruncher' "'DATAPATH'FileList" "'DATAPATH'FileList.txt"
_return

; ------------------------------------------------------------------------

; =<add front- and endtexts>==============================================

ADD_FRONT:
_extern/quiet Delete "'DATAPATH'FileList.txt"
_check file "'path'FileList.frontadd" NOEXISTS_1
_export userdat "'path'FileList.frontadd" "'TEMPPATH'FileList.TMP" "'{NAME}'"
_extern/quiet QJoin "'TEMPPATH'FileList.TMP" "'DATAPATH'FileList.txt"
_extern/quiet Delete "'TEMPPATH'FileList.TMP"
NOEXISTS_1:
_return

ADD_END:
_check file "'path'FileList.endadd" NOEXISTS_2:
_export userdat "'path'FileList.endadd" "'TEMPPATH'FileList.TMP" "'{NAME}'"
_extern/quiet QJoin "'TEMPPATH'FileList.TMP" "'DATAPATH'FileList.txt"
_extern/quiet Delete "'TEMPPATH'FileList.TMP"
NOEXISTS_2:
_return

; ------------------------------------------------------------------------

; =<select a packer>======================================================

SHOW_PACK_ON:
_echo/nolf T_CLEAR_PACK
_echo/nolf KEY1_ON
 cruncher = packer'PACKMODE'
_echo/nolf "'cruncher'"
_return

SHOW_PACK_OFF:
_echo/nolf T_CLEAR_PACK
_echo/nolf KEY1_OFF
 cruncher = packer'PACKMODE'
_echo/nolf "'cruncher'"
_return

; ------------------------------------------------------------------------

; =<select a scanmode>====================================================

SHOW_SCAN_ON:
_echo/nolf T_CLEAR_SCAN
_echo/nolf KEY0_ON
_echo/nolf T_SCANMODE_'SCANMODE'
_return

SHOW_SCAN_OFF:
_echo/nolf T_CLEAR_SCAN
_echo/nolf KEY0_OFF
_echo/nolf T_SCANMODE_'SCANMODE'
_return

; ------------------------------------------------------------------------

; =<menu-routines>========================================================

MAKE_MENU:
_echo/nolf T_SELECT
_menu/nopmsgs/noecho KEY'point'_ON KEY_SEL_L LEFT KEY_SEL_R RIGHT KEY_OK ENTER KEY_SEL_0 POINT0_2 KEY_SEL_1 POINT1_2 KEY_SEL_2 POINT2 KEY_SEL_3 POINT3 KEY_SEL_4 POINT4 KEY_SEL_5 POINT5 KEY_HELP HELP

LEFT:
_if/i point == 0 then _goto MAKE_MENU
_if/i point == 1 then _gosub SHOW_PACK_OFF
_echo/nolf KEY'point'_OFF
 point = point-1
_if/i point == 0 then _gosub SHOW_SCAN_ON
_if/i point == 1 then _gosub SHOW_PACK_ON
_goto MAKE_MENU

RIGHT:
_if/i point == 5 then _goto MAKE_MENU
_if/i point == 0 then _gosub SHOW_SCAN_OFF
_if/i point == 1 then _gosub SHOW_PACK_OFF
_echo/nolf KEY'point'_OFF
 point = point+1
_if/i point == 1 then _gosub SHOW_PACK_ON
_goto MAKE_MENU

ENTER:
_goto POINT'point'

POINT0:
 SCANMODE = SCANMODE+1
_if/i SCANMODE >= 5 then SCANMODE = 1
_gosub SHOW_SCAN_ON
_goto MAKE_MENU

POINT0_2:
 SCANMODE = SCANMODE+1
_if/i SCANMODE >= 5 then SCANMODE = 1
_if/i point != 0 then _gosub SHOW_SCAN_OFF
_if/i point == 0 then _gosub SHOW_SCAN_ON
_goto MAKE_MENU

POINT1:
 PACKMODE = PACKMODE+1
_if/i PACKMODE >= count then PACKMODE = 0
_gosub SHOW_PACK_ON
_goto MAKE_MENU

POINT1_2:
 PACKMODE = PACKMODE+1
_if/i PACKMODE >= count then PACKMODE = 0
_if/i point != 1 then _gosub SHOW_PACK_OFF
_if/i point == 1 then _gosub SHOW_PACK_ON
_goto MAKE_MENU

POINT2:
_echo/nolf KEY'point'_OFF
_gosub SHOW_PACK_OFF
_gosub SHOW_SCAN_OFF
_echo/nolf KEY2_ON
_echo/nolf T_SCAN
_echo/nolf T_ZONEON
_gosub ADD_FRONT
 fbpara = "/acc=1"
_if/i SCANMODE == 2 then fbpara = fbpara+"/boardlist"
_if/i SCANMODE == 3 then fbpara = fbpara+"/new"
_if/i SCANMODE == 4 then fbpara = fbpara+"/boardlist/new"
_gosub DOWN_SCAN
_gosub ADD_END
_echo/nolf T_READY
_echo/nolf T_ZONEOFF
_echo/nolf KEY2_OFF
_goto MAKE_MENU

POINT3:
_echo/nolf KEY'point'_OFF
_gosub SHOW_PACK_OFF
_gosub SHOW_SCAN_OFF
_echo/nolf KEY3_ON
_gosub SHOW_FILELIST
_echo/nolf KEY3_OFF
_goto MAKE_MENU

POINT4:
_echo/nolf KEY'point'_OFF
_gosub SHOW_PACK_OFF
_gosub SHOW_SCAN_OFF
_echo/nolf KEY4_ON
_gosub DOWN_FILELIST
_echo/nolf KEY4_OFF
_goto MAKE_MENU

POINT5:
_echo/nolf KEY'point'_OFF
_gosub SHOW_PACK_OFF
_gosub SHOW_SCAN_OFF
_echo/nolf KEY5_ON
_goto END
_eod

HELP:
_check file "'path''lang'/FileList.help'point'" MAKE_MENU
_echo/nolf T_HELPON
_disk text "'path''lang'/FileList.help'point'"
_echo/nolf T_HELPOFF
_goto MAKE_MENU

; ------------------------------------------------------------------------

; =<error handling>=======================================================

NO_DIRS:
_echo/nolf T_NODIRS
_echo/nolf T_NODIRS_END
_eod

NO_CONFIG:
 o1 = "T"
_gosub LOAD_TEXT
_echo/nolf T_NOCFG
_echo/nolf T_NOCFG_END
_eod

; ------------------------------------------------------------------------

; =<exit filelist>========================================================

END:
_echo/nolf T_END

EXIT:
_cd ":"
_cd "'cdpath'"

HANGUP:
_extern/quiet Delete "'DATAPATH'FileList.txt"
 suffix = suffix'PACKMODE'
_extern/quiet Delete "'DATAPATH'FileList.'suffix'"
_fd close -1
_eod

; ------------------------------------------------------------------------
