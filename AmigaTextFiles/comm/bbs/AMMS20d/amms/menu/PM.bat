;	$VER: 1.01 / 12.02.1994
; PM-Menue
; --------
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
MENU="PM"				; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="PM-Menu"			; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/pm.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "B" CLI -				; Befehleebene
 "D" DOWNLOAD -				; Batch-Download
 "F" FILES -				; File-Menue
 "M" MSGS -				; Msg-Menue
 "P" FMAIL -				; Privatfiles anschauen
 "Q" MAIN -				; Main-Menue
 "R" MAIL -				; PMs lesen
 "S" SEND -				; PM senden
 "U" FSEND 				; Privatfile schicken

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
; **** File-Menue
;
FILES:
COM="FILES"				; Menupunkt setzen
JUMP=="FILES"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Message-Menue
;
MSGS:
COM="MSGS"				; Menupunkt setzen
JUMP=="MSGS"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Privatfiles anschauen
;
FMAIL:
COM="FMAIL"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_fmail					; Privatfiles anschauen
_goto MENU				; -> Menue

;
; **** Main-Menue
;
MAIN:
COM="MAIN"				; Menupunkt setzen
_goto END				; Ende der Batchdatei

;
; **** Alle PMs lesen
;
MAIL:
COM="MAIL"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_mail					; PMs lesen
_goto MENU				; -> Menue

;
; **** Message schreiben
;
SEND:
COM="SEND"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_send					; PM senden
_goto MENU				; -> Menue

;
; **** Privatfile senden
;
FSEND:
COM="FSEND"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if {LEVEL} <= GUESTLEVEL then _goto MENU ; Wenn Gast -> Menue
_fsend					; Privatfile senden
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
