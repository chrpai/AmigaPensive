;	$VER: 1.03 / 18.08.1995
; Message-Menue
; -------------
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
; **** Message-Menue
;
MENU="Message"				; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Message-Menu"			; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/msg.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "B" CLI -				; Befehleebene
 "D" DOWNLOAD -				; ZConnect-Download
 "E" CHANGE_BOARDLIST -			; Brettliste aendern
 "F" FILES -				; File-Menue
 "L" SHOW_BOARDLIST -			; Brettliste anschauen
 "N" NEW_READ -				; Neue Messages anschauen
 "P" PMS -				; PM-Menue
 "Q" MAIN -				; Main-Menue
 "R" DATE -				; Msgs ab Datum als ungelesen markieren
 "S" SELECT -				; Select (Brett-Direktwahl)
 "U" UPLOAD -				; ZConnect-Upload
 "V" PACKER -				; ZConnect-Packer
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
; **** ZConnect-Download
;
DOWNLOAD:
COM="ZCONNECT DOWNLOAD"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo "#RESET##HOME##CLS#"		; Bildschirm loeschen
_zconnect download			; ZConnect-Download starten
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Brettliste aendern
;
CHANGE_BOARDLIST:
COM="CHANGE BOARDLIST"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo "#RESET##HOME##CLS#"		; Bildschirm loeschen
_read/action=132 :			; Brettliste aendern
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** File-Menue
;
FILES:
COM="FILES"				; Menupunkt setzen
JUMP=="FILES"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Brettliste anschauen
;
SHOW_BOARDLIST:
COM="SHOW BOARDLIST"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show boardlist				; Brettliste anschauen
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Alle neue Messages lesen
;
NEW_READ:
COM="RN"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo "#RESET##HOME##CLS#"		; Bildschirm loeschen
_rn					; Neue Messages lesen
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
; **** Messages ab Datum als ungelesen markieren
;
DATE:
COM="READ/DATE"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{alanguage}'/read_date.txt" ; Text anzeigen
_input/maxlength=20 DATE		; Datum eingeben
_if DATE != "" then _read/date='DATE'	; Message ab Datum als ungelesen markieren
_goto MENU				; -> Menue

;
; **** ZConnect-Packer
;
PACKER:
COM="ZCONNECT PACKER"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo "#RESET##HOME##CLS#"		; Bildschirm loeschen
_zconnect packer			; ZConnect-Packer starten
_gosub WAIT				; Warte auf Taste
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
; **** ZConnect-Upload
;
UPLOAD:
COM="ZCONNECT UPLOAD"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo "#RESET##HOME##CLS#"		; Bildschirm loeschen
_zconnect upload			; ZConnect-Upload starten
_gosub WAIT				; Warte auf Taste
_goto MENU				; -> Menue

;
; **** Brett-Direktwahl
;
DIRECT:
COM="DIRECT"				; Menupunkt setzen
_disk execute "menu:msg-direct.bat"	; Direktwahl starten
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
_if COM == "Menu" then _goto END	; Wenn Fehler im Menue -> Main-Menue
_goto MENU				; -> Menue
