;	$VER: 1.02 / 22.05.1995
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
MENU="Sysop"				; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Sysop-Menu"			; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/sysop.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "A" SHOW_APPLICATIONS -		; Userantrage anschauen
 "B" CLI -				; Befehleebene
 "E" SHOW_EVENTLOGS -			; Eventlogs anschauen
 "F" SHOW_ERRORTXT -			; Datei mbdat:errors.txt anschauen
 "M" SHOW_MENUERR -			; Menuefehler anschauen
 "P" PASSWORD -				; Passwort aendern
 "Q" MAIN - 				; Main-Menue
 "S" SHOW_USER -			; Userdaten anschauen
 "U" EDIT_USER				; Usereditor

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

;
; **** Userantraege anschauen
;
SHOW_APPLICATIONS:
COM="EDIT APPLICATIONS"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_ex echo >>mbdat:antraege.txt ""	; File erzeugen, falls nicht vorhanden
_disk edit "mbdat:antraege.txt"		; Userantraege aendern
_goto MENU				; -> Menue

;
; **** Rueckkehr in die Befehlsebene
;
CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Eventlogs anschauen
;
SHOW_EVENTLOGS:
COM="SHOW EVENTLOGS"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_ex echo >>mbdat:event.log ""		; File erzeugen, falls nicht vorhanden
_disk edit "mbdat:event.log"		; Eventlogs aendern
_goto MENU				; -> Menue

;
; **** Datei mbdat:errors.txt anschauen
;
SHOW_ERRORTXT:
COM="SHOW ERRORTXT"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_ex echo >>mbdat:errors.txt NOLINE	; File erzeugen, falls nicht vorhanden
_disk edit "mbdat:errors.txt"		; Errors.txt aendern
_goto MENU				; -> Menue

;
; **** Menufehler anschauen
;
SHOW_MENUERR:
COM="SHOW MENUERR"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_ex echo >>mbdat:menu.err ""		; File erzeugen, falls nicht vorhanden
_disk edit "mbdat:menu.err"		; Menuefehler aendern
_goto MENU				; -> Menue

;
; **** Userdaten anschauen
;
PASSWORD:
COM="SET PW"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{ALANGUAGE}'/password.txt" ; Infotext anzeigen
_input/maxlength=20 NAME ""		; Prompt steht im Info / Name eingeben
_if NAME == "" then _goto MENU		; Bei Leereingabe -> Menue
_check user "'NAME'" MENU		; Falls User nicht existiert -> Menue
_echo					; Leerzeile ausgeben
_set pw "'NAME'"			; User-Passwort aendern
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
_disk text "menu:'{ALANGUAGE}'/show_user.txt" ; Infotext anzeigen
_input/maxlength=20 NAME ""		; Prompt steht im Info / Name eingeben
_echo					; Leerzeile ausgeben
_show user "'NAME'"			; User listen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Userdaten aendern
;
EDIT_USER:
COM="EDIT USER"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{ALANGUAGE}'/useredit.txt" ; Infotext anzeigen
_input/maxlength=20 NAME ""		; Prompt steht im Info / Name eingeben
_if NAME == "" then _goto EDIT_USER_ED	; Bei Leereingabe EDIT USER
_check user "'NAME'" MENU		; Falls User nicht existiert -> Menue
EDIT_USER_ED:
_edit user "'NAME'"			; Userdaten aendern
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
