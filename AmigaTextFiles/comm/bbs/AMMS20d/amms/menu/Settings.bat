;	$VER: 1.01 / 12.02.1994
; Einstellungs-Menue
; ------------------
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
; **** Einstellungs-Menue
;
MENU="Settings"				; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Settings-Menu"			; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/settings.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "A" EDIT_BATCH -			; Login-Batch aendern
 "B" CLI -				; Befehleebene
 "C" CURSOR -				; Cursortasten setzen
 "E" EMULATION -			; Emulation aendern
 "I" LANGUAGE -				; Sprache aendern
 "P" PASSWORD -				; Passwort aendern
 "Q" MAIN -				; Main-Menue
 "S" SHOW_USER -			; Userdaten anschauen
 "U" EDIT_USER -			; Userdaten aendern
 "V" VAK -				; VAK
 "Z" LINES				; Zeilenanzahl aendern

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

;
; **** Login-Batch aendern
;
EDIT_BATCH:
COM="EDIT BATCH"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} >= USERLEVEL then -	; Wenn User ->
   _edit batch				; Login-Batch aendern
_goto MENU				; -> Menue

;
; **** Rueckkehr in die Befehlsebene
;
CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Cursortasten setzen
;
CURSOR:
COM="SET CURSOR"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_set cursor				; Cursortasten aendern
_goto MENU				; -> Menue

;
; **** Emulation aendern
;
EMULATION:
COM="SET EMULATION"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_set emulation/quiet			; Emulation aendern
_goto MENU				; -> Menue

;
; **** Sprache aendern
;
LANGUAGE:
COM="SET LANGUAGE"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_set language/quiet			; Sprache aendern
_goto MENU				; -> Menue

;
; **** Passwort aendern
;
PASSWORD:
COM="SET PW"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} < USERLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_set pw					; Passwort aendern
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Main-Menue
;
MAIN:
COM="MAIN"				; Menupunkt setzen
_goto END				; Ende der Batchdatei

;
; **** Userdaten anschauen
;
SHOW_USER:
COM="SHOW USER"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show user				; Userdaten zeigen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Userdaten aendern
;
EDIT_USER:
COM="EDIT USER"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} < USERLEVEL then _goto MENU ; Wenn Gast -> Menue
NAME=""					; Username loeschen / Kein Parameter
_if/i {LEVEL} >= SYSOPLEVEL then NAME="""'{NAME}'""" ; Wenn Sysop, Name setzen
_edit user 'NAME'			; Userdaten aendern
_goto MENU				; -> Menue

;
; **** VAK
;
VAK:
COM="VAK"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} < USERLEVEL then _goto MENU ; Wenn Gast -> Menue
_VAK					; VAK aufrufen
_goto MENU				; -> Menue

;
; **** Zeilenanzahl aendern
;
LINES:
COM="SET LINES"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_set lines/quiet			; Zeilenanzahl aendern
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
