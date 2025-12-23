;
; FileList V2.20  by cANNoN/iDP!
; $VER: FileList V2.20 / 23.06.97
;
; *** LOCALE-TEXTE ***
;


; temporaeres Verzeichnis

 TEMPPATH     = "T:"

; Datenverzeichnis

 DATAPATH     = "T:"

; String der zu listenden Files

 LISTSTRING   = "~(BOARDINFO)"

; beim Start ausgewaehlter Packer (Packernummer in der Config)

 PACKMODE     = 1

; beim Start angewaehlter Scan-Modus (1=alle Bretter, 2=Brettliste des Users)

 SCANMODE     = 1

; Beginn des Requesters

 HEAD         = 12

; Hoehe des Requesters

 LENGTH       = 5

; Steuertasten fuers Menue

 KEY0_ON      = "#P 10,52##FC 5##B#"
 KEY0_OFF     = "#P 10,52##FC 4##B#"
 KEY1_ON      = "#P 10,23##FC 5##B#"
 KEY1_OFF     = "#P 10,23##FC 4##B#"
 KEY2_ON      = "#P 19,7##FC 5##B#M#RS##FC 5#ake #B#F#RS##FC 5#ilelist#RS#"
 KEY2_OFF     = "#P 19,7##FC 2##B#M#RS##FC 2#ake #B#F#RS##FC 2#ilelist#RS#"
 KEY3_ON      = "#P 19,25##FC 5##B#S#RS##FC 5#how #B#F#RS##FC 5#ilelist#RS#"
 KEY3_OFF     = "#P 19,25##FC 2##B#S#RS##FC 2#how #B#F#RS##FC 2#ilelist#RS#"
 KEY4_ON      = "#P 19,43##FC 5##B#D#RS##FC 5#own #B#F#RS##FC 5#ilelist#RS#"
 KEY4_OFF     = "#P 19,43##FC 2##B#D#RS##FC 2#own #B#F#RS##FC 2#ilelist#RS#"
 KEY5_ON      = "#P 19,65##FC 5##B#A#RS##FC 5#bort#RS#"
 KEY5_OFF     = "#P 19,65##FC 2##B#A#RS##FC 2#bort#RS#"
 KEY_SEL_U    = "^1"
 KEY_SEL_D    = "^2"
 KEY_SEL_L    = "^3"
 KEY_SEL_R    = "^4"
 KEY_SEL_0    = "0s"
 KEY_SEL_1    = "1p"
 KEY_SEL_2    = "2m"
 KEY_SEL_3    = "3v"
 KEY_SEL_4    = "4d"
 KEY_SEL_5    = "5aqx"
 KEY_OK       = "^M "
 KEY_HELP     = "?h"

; Zonenscrollbereiche einstellen

 T_ZONEON     = "#SR 12,17##LI 17#^J"
 T_ZONEOFF    = "#SROFF##LI 23#"

; Status und Arbeitsanzeige beim scannen der Fileliste

 T_SCAN       = "#P 8,23#{LIMSPACEON}{TAB 53}#P 8,23##FC 2##B#S#RS##FC 2#canning #B#F#RS##FC 2#ilelist #FC 3##B#...#RS#"
 T_SCANLINE_1 = "     #FC 2##B#S#RS##FC 2#canning #FC 6##B#"
 T_SCANLINE_2 = "#RS##FC 6#"
 T_SCANLINE_3 = " #FC 3##B#...#RS#"
 T_NEXTLINE   = "^J"
 T_READY      = "     #FC 1##B#R#RS##FC 1#eady#B#!#RS#"

; Status und Loeschzeile fuer die Packer

 T_PACK       = "#P 8,23#{LIMSPACEON}{TAB 53}#P 8,23##FC 2##B#P#RS##FC 2#acking #B#F#RS##FC 2#ilelist #FC 3##B#...#RS#"
 T_CLEAR_PACK = "#P 10,23#{LIMSPACEON}{TAB 16}"

; Scan-Modi, Loeschzeile fuer den Scan-Modus

 T_SCANMODE_1 = "all fileboards   #R 3#all"
 T_SCANMODE_2 = "marked fileboards#R 3#all"
 T_SCANMODE_3 = "all fileboards   #R 3#new"
 T_SCANMODE_4 = "marked fileboards#R 3#new"
 T_CLEAR_SCAN = ""

; Status und Ausgabe fuer Download

 T_DOWN       = "#P 8,23#{LIMSPACEON}{TAB 53}#P 8,23##FC 2##B#D#RS##FC 2#ownloading #B#F#RS##FC 2#ilelist #FC 3##B#...#RS#"
 T_DSR_ON     = "#SR 12,17##LI 17##FC 0#^J"
 T_DSR_OFF    = "     #FC 1##B#D#RS##FC 1#ownload #B#o#RS##FC 1#k#B#!#RS##SROFF##LI 23#"
 T_DSR_BREAK  = "     #FC 1##B#D#RS##FC 1#ownload #B#a#RS##FC 1#borted#B#!#RS##SROFF##LI 23#"

; Status beim waehlen der Menuepunkte

 T_SELECT     = "#P 8,23#{LIMSPACEON}{TAB 53}#P 8,23##FC 2##B#W#RS##FC 2#aiting #FC 3##B#...#RS#"

; Ausgabe, wenn Fileliste nicht existiert

 T_NOFILELIST = "^J#R 30##FC 1##B#NO FILELIST EXISTS!#RS#^J#R 19##FC 5#-=#B#> #FC 1#T#RS##FC 1#ry #B#t#RS##FC 1#o #B#m#RS##FC 1#ake #B#a F#RS##FC 1#ilelist #B#a#RS##FC 1#t #B#f#RS##FC 1#irst#B#!! #FC 5#<#RS##FC 5#=-#RS#"

; Ausgabe beim anzeigen am Ende der Fileliste

 T_WAITKEY    = "#R 23##FC 5#-=#B#> #FC 1#P#RS##FC 1#ress #B#a#RS##FC 1#ny #B#k#RS##FC 1#ey #B#t#RS##FC 1#o #B#c#RS##FC 1#ontinue #FC 5##B#<#RS##FC 5#=-#RS#"

; FEHLER: Ausgabe fuer FileList ist schon gestartet und dann Ende

 T_EXIST      = "^J#R 16##FC 5#-=#B#> #FC 1#F#RS##FC 1#ile#B#L#RS##FC 1#ist #B#c#RS##FC 1#an #B#o#RS##FC 1#nly #B#b#RS##FC 1#e #B#r#RS##FC 1#unning #B#o#RS##FC 1#ne #B#t#RS##FC 1#imes#B#. #FC 5#<#RS##FC 5#=-#RS#^J"
 T_EXIST_END  = "#R 25##FC 5#-=#B#> #FC 1#P#RS##FC 1#lease #B#w#RS##FC 1#ait #B#a m#RS##FC 1#oment#B#. #FC 5#<#RS##FC 5#=-#RS#^J^J"

; FEHLER: Ausgabe fuer 'FileList.dirs' existiert nicht und dann Ende

 T_NODIRS     = "#SROFF##LI 23#^J#R 16##FC 5#-=#B#> #FC 1#T#RS##FC 1#he #B#f#RS##FC 1#ile #FC 6##B#''#RS##FC 6#FileList.dirs#B#'' #FC 1#d#RS##FC 1#oes #B#n#RS##FC 1#ot #B#e#RS##FC 1#xists #FC 5##B#<#RS##FC 5#=-#RS#^J"
 T_NODIRS_END = "#R 23##FC 5#-=#B#> #FC 1#P#RS##FC 1#lease #B#c#RS##FC 1#heck #B#t#RS##FC 1#his #B#a#RS##FC 1#t #B#f#RS##FC 1#irst #FC 5##B#<#RS##FC 5#=-#RS#^J^J"

; FEHLER: Ausgabe fuer 'FileList.cfg' existiert nicht und dann Ende

 T_NOCFG      = "^J#R 16##FC 5#-=#B#> #FC 1#T#RS##FC 1#he #B#f#RS##FC 1#ile #FC 6##B#''#RS##FC 6#FileList.cfg#B#'' #FC 1#d#RS##FC 1#oes #B#n#RS##FC 1#ot #B#e#RS##FC 1#xists#B#! #FC 5#<#RS##FC 5#=-#RS#^J"
 T_NOCFG_END  = "#R 23##FC 5#-=#B#> #FC 1#P#RS##FC 1#lease #B#c#RS##FC 1#heck #B#t#RS##FC 1#his #B#a#RS##FC 1#t #B#f#RS##FC 1#irst #FC 5##B#<#RS##FC 5#=-#RS#^J^J"

; Ausgabe beim beenden

 T_END        = "#SROFF##LI 24##R 17##FC 2##B#T#RS##FC 2#hanx #B#f#RS##FC 2#or #B#u#RS##FC 2#sing #FC 1##B#F#RS##FC 1#ile#B#L#RS##FC 1#ist #FC 5#V#B#2#RS##FC 5#.#B#20  #FC 2#b#RS##FC 2#y #FC 6##B#c#RS##FC 6#ANN#B#o#RS##FC 6#N#FC 4#/#FC 6##B#i#RS##FC 6#DP#B#!#RS#^J^J"

; Text vor Ausgabe der Hilfe

 T_HELPON     = "#SR 12,17##LI 17#^J"

; Text nach Ausgabe der Hilfe

 T_HELPOFF    = "#SROFF##LI 23#"
