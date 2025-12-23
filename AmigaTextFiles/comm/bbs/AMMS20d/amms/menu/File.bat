;	$VER: 1.03 / 13.04.1994
; File-Menue
; ----------
; Im Menu ist Command-Bit 6 gesetzt. Deshalb kann der User definiert
; auf Befehle zugreifen, auf die er normalerweise keinen Zugriff hat.
; Die Variable COM wird zum Erkennen von Fehlern benutzt. Tritt ein
; Fehler auf, so wird in die Routine SAVE_ERROR verzweigt, die den Fehler
; im File mbdat:menu.err protokolliert.
; Die Variable JUMP wird zum eventuellen Sprung von einem Untermenue zum
; anderen benutzt, ohne das Hauptmenue dazwischen aufzurufen. In dieser
; Variable muss dann das entsprechende Label des Untermenues stehen.
; Soll kein Schnell-Wechsel stattfinden, so darf die Variable JUMP nicht
; geaendert werden.
;

;
; **** File-Menue
;
MENU="File"				; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="File-Menu"			; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/file.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "B" CLI -				; Befehleebene
 "C" DELETE_DOWNLOADS -			; Downloads loeschen
 "D" DOWNLOAD -				; Download aller markierten Files
 "E" CHANGE_BOARDLIST -			; brettliste aendern
 "F" FILES -				; Alle Files anschauen
 "I" FIND_FILE -			; Suche Files
 "L" SHOW_BOARDLIST -			; Brettliste anschauen
 "M" MSGS -				; Message-Menue
 "N" NEW_FILES -			; Neue Files anschauen
 "P" PMS -				; PM-Menue
 "Q" MAIN -				; Main-Menue
 "R" DATE -				; Files ab Datum als ungelistet markieren
 "S" SELECT -				; Brettliste anschauen
 "Z" DIRECT				; Brett-Direktwahl

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

;
; **** Rueckkehr in die Befehlsebene
;
CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Downloads loeschen
;
DELETE_DOWNLOADS:
COM="DELETE DOWNLOAD"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_delete download			; Downloads listen und loeschen
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Batch-Download
;
DOWNLOAD:
COM="DOWNLOAD"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_download				; Batch-Download starten
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Brettliste aendern
;
CHANGE_BOARDLIST:
COM="CHANGE BOARDLIST"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_files/action=132 :			; Brettliste aendern
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Alle Files anschauen
;
FILES:
COM="FILES"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_files					; Alle Files anschauen
_goto MENU				; -> Menue

;
; **** Files suchen
;
FIND_FILE:
COM="FIND FILE"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{ALANGUAGE}'/find_file.txt" ; Infotext ausgeben
_input/maxlength=40/command NAME	; Suchstring eingeben
_if NAME == "" then _goto MENU		; Bei Leereingabe -> Menue
_find file "'NAME'"			; Files suchen
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Brettliste anschauen
;
SHOW_BOARDLIST:
COM="SHOW FBOARDLIST"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show fboardlist			; Brettliste anschauen
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Message-Menue
;
MSGS:
COM="MSGS"				; Menupunkt setzen
JUMP=="MSGS"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Alle Files anschauen
;
NEW_FILES:
COM="FN"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_fn					; Neue Files anschauen
_goto MENU				; -> Menue

;
; **** PM-Menue
;
PMS:
COM="PMS"				; Menupunkt setzen
JUMP=="PMS"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Main-Menue
;
MAIN:
COM="MAIN"				; Menupunkt setzen
_goto END				; Ende der Batchdatei

;
; **** Files ab Datum als ungelistet markieren
;
DATE:
COM="FILES/DATE"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{alanguage}'/files_date.txt" ; Text anzeigen
_input/maxlength=20 DATE		; Datum eingeben
_if DATE != "" then files/date='DATE'	; Files ab Datum als ungelistet markieren
_goto MENU				; -> Menue

;
; **** Brettliste anschauen
;
SELECT:
COM="SELECT"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_select					; Brett-Direktwahl starten
_goto MENU				; -> Menue

;
; **** Brett-Direktwahl
;
DIRECT:
COM="DIRECT"				; Menupunkt setzen
_disk execute "menu:file-direct.bat"	; Direktwahl starten
_if JUMP != JUMP_MAIN then _goto END	; Wenn direkter Menuewechsel -> Ende
_goto MENU				; -> Menue


;
; **** SUBROUTINE: Gib einen Infotext aus und warte auf Taste
;
WAIT:
COM_OLD=COM				; Alten Menupunkt retten
COM="WAIT"				; Neuen Menupunkt setzen
_echo/nolf "^J[RETURN]"			; Gib Wartemeldung aus
_waitkey " ^M"				; Warte auf Space oder Return
COM=COM_OLD				; Alten Menupunkt setzen
_return					; Ende der Unterroutine

;
; **** Fehlerbehandlungs-Routine
;
; Die Routine erwartet in der Variable COM den Namen des akt. Menupunkts.
; Sie protokolliert im File mbdat:menu.err die Fehler des Menues.
;
SAVE_ERROR:
_on error/ignore			; Alle Fehler ignorieren
_extern echo >>mbdat:menu.err "------------------"
_extern echo >>mbdat:menu.err "Fehler im Menu"
_extern echo >>mbdat:menu.err "Menu : 'MENU'"
_extern echo >>mbdat:menu.err "Punkt: 'COM'"
_extern echo >>mbdat:menu.err "User : '{NAME}'"
_extern echo >>mbdat:menu.err "Datum: '{DATE}'"
_extern echo >>mbdat:menu.err "Zeit : '{TIME}'"
_extern echo >>mbdat:menu.err "------------------"
_disk text "menu:'{ALANGUAGE}'/Error.txt"  ; Fehlertext anzeigen
_waitkey " ^M"
_if COM == "Menu" then _goto END	; Wenn Fehler im File-Menu -> Main-Menu
_goto MENU				; -> Menue
