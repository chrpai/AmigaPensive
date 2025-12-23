;	$VER: 1.01 / 12.02.1994
; Main-Menue
; ----------
; Im Menu ist Command-Bit 6 gesetzt. Deshalb kann der User definiert
; auf Befehle zugreifen, auf die er normalerweise keinen Zugriff hat.
; Die Variable COM wird zum Erkennen von Fehlern benutzt. Tritt ein
; Fehler auf, so wird in die Routine SAVE_ERROR verzweigt, die den Fehler
; im File mbdat:menu.err protokolliert.
; Die Variable JUMP wird zum eventuellen Sprung von einem Untermenue zum
; anderen benutzt, ohne das Hauptmenue dazwischen aufzurufen. In dieser
; Variable muss dann das entsprechende Label des Untermenues stehen.
;

; Batch-Break sperren

_break off/z/y				; CTRL-Z und CTRL-Y sperren
_break on/x/c				; CTRL-C und CTRL-X zulassen

; Konstanten fuer das gesamte Menu definieren

GUESTLEVEL==1000			; Level fuer Gast
USERLEVEL==2000				; Mindest Level fuer User
SYSOPLEVEL==9000			; Mindest Level fuer Sysop
JUMP_MAIN=="MAIN_MENU"			; Einsprunglabel ins Main-Menues

; Beim Start : Einsprung ins MAIN-Menu

JUMP==JUMP_MAIN				; Main-Menu ist Anfang

;
; **** Main-Menu
;
MAIN:
MENU="MAIN"				; Name des Menues
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Main-Menu"			; WHO-Befehlsname setzen
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Automatischer Sprung in ein Menu

_goto 'JUMP'				; Sprung zu einem Menu

; Menutext ausgeben und Tasten abfragen

MAIN_MENU:
_menu/file "menu:'{ALANGUAGE}'/main.txt" -
 "^R" MAIN_MENU -			; Bildschirm erneuern
 "A" APPLICATION -			; Antrag
 "B" CLI -				; Befehleebene
 "C" CALL -				; Sysop rufen
 "F" FILES -				; File-System
 "G" GAMES -				; Spiele
 "K" COMMU -				; Kommunikationssystem
 "M" MSGS -				; Message-System
 "O" SYSOP -				; Sysop-Menu
 "P" PMS -				; PM-System
 "R" RECALL -				; Andere User einloggen
 "S" SETTINGS -				; Einstellungen
 "T" STATISTIC -			; Statistiken
 "X" LOGOUT				; System verlassen

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
GUESTLEVEL==				; Variablen loeschen
USERLEVEL==
SYSOPLEVEL==
JUMP_MAIN==
JUMP==
_eod					; Ende

;
; **** Antrag
;
APPLICATION:
COM="APPLICATION"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} == GUESTLEVEL then _goto APPLICATION_START ; Wenn Gast -> Antrag
_if/i {LEVEL} < SYSOPLEVEL then _goto MAIN ; Wenn User -> Menu
APPLICATION_START:
_application				; Antrag ausfuehren
_gosub WAIT				; Warte auf Taste
_goto MAIN				; -> Menue

;
; **** Rueckkehr in die Befehlsebene
;
CLI:
_on error/ignore			; Ignoriere Fehler in der Batch
_disk text "menu:'{ALANGUAGE}'/CLI.txt" ; Gibt Informationstext aus
_goto END				; Verlasse Menu

;
; **** Sysop rufen
;
CALL:
COM="CALL"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_call					; Rufe Sysop
_gosub WAIT				; Warte auf Taste
_goto MAIN				; -> Menue

;
; **** File-Menue aufrufen
;
FILES:
COM="FILES"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:file.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** Spiele-Menue aufrufen
;
GAMES:
COM="GAMES"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:game.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** Kommunikations-Menue aufrufen
;
COMMU:
COM="COMMUNICATION"			; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:commu.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** Msgs-Menue aufrufen
;
MSGS:
COM="MSGS"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:msg.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** Sysop-Menue aufrufen
;
SYSOP:
COM="SYSOP"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_if/i {LEVEL} >= SYSOPLEVEL then -	; Wenn Sysop -> Sysop-Menue
   _disk execute menu:sysop.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** PM-Menue aufrufen
;
PMS:
COM="PMs"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:pm.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** unter anderem user einloggen
;
RECALL:
COM="RECALL"				; Menupunkt setzen
_recall					; Unter anderem User einloggen
_gosub WAIT				; Warte auf Taste
_goto MAIN				; -> Menue

;
; **** Einstellungen aendern
;
SETTINGS:
COM="SETTINGS"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:settings.bat		; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** Statistik-Menue aufrufen
;
STATISTIC:
COM="STATISTIC"				; Menupunkt setzen
JUMP==JUMP_MAIN				; Default: Ruecksprung in Main-Menu
_disk execute menu:statistic.bat	; Untermenu ausfuehren
_goto MAIN				; -> Menue

;
; **** Verlasse Box
;
LOGOUT:
com="LOGOUT"				; Menupunkt LOGOUT setzen
_logout					; Verlasse Box
_goto MAIN				; -> Menue

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
_if COM == "Menu" then _goto CLI	; Wenn Fehler im Main-Menu -> CLI
_goto MAIN				; -> Main-Menu
