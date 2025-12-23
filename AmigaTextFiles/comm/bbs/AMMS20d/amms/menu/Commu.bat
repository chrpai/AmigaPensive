;	$VER: 1.01 / 12.02.1994
; Kommunikations-Menue
; --------------------
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
; **** Kommunikations-Menue
;
MENU="Communication"			; Name des Menues
COM="MENU"				; Menupunkt in Variable COM speichern
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Einige Texte in Variablen holen
; In dieser Batchdatei werden globale Variablen erzeugt, deren Inhalt
; fuer einige Promptausgaben des Menues genutzt werden.
; Diese globalen Variablen werden in der End-Routine END wieder geloescht.

_disk execute/keep "menu:'{ALANGUAGE}'/commu.bat"

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Communication-Menu"		; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/commu.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "1" PMSGON -				; PMSGs AN
 "2" PMSGOFF -				; PMSGs AUS
 "3" MSGON -				; Messages AN
 "4" MSGOFF -				; Messages AUS
 "B" CLI -				; Befehleebene
 "C" WCHAT -				; WCHAT
 "F" FCHAT -				; FCHAT
 "M" MESSAGE -				; nachricht an Port
 "P" PMSG -				; Privat-Nachricht
 "Q" MAIN -				; Main-Menue
 "T" TURBO -				; Turbo-Messages
 "W" WHO -				; Wer ist Online
 "X" XCHAT				; XCHAT

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

;
; **** Privat-Nachrichten-Empfang anstellen
;
PMSGON:
COM="PMSG ON"				; Menupunkt setzen
_pmsg/on				; Privatnachrichten-Empfang an
_goto MENU				; -> Menue

;
; **** Privat-Nachrichten-Empfang ausstellen
;
PMSGOFF:
COM="PMSG OFF"				; Menupunkt setzen
_pmsg/off				; Privatnachrichten-Empfang aus
_goto MENU				; -> Menue

;
; **** Message-Empfang anstellen
;
MSGON:
COM="MESSAGE ON"			; Menupunkt setzen
_message/on				; Message-Empfang an
_goto MENU				; -> Menue

;
; **** Message-Empfang ausstellen
;
MSGOFF:
COM="MESSAGE OFF"			; Menupunkt setzen
_message/off				; Message-Empfang aus
_goto MENU				; -> Menue

;
; **** Rueckkehr in die Befehlsebene
;
CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** WCHAT starten
;
WCHAT:
COM="WCHAT"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_wchat					; WCHAT starten
_goto MENU				; -> Menue

;
; **** FCHAT starten
;
FCHAT:
COM="FCHAT"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_fchat					; FCHAT starten
_goto MENU				; -> Menue

;
; **** Message an einen anderen Port verschicken
;
MESSAGE:
COM="MESSAGE"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show ports				; Portliste anschauen
_input/command/maxlength=12 PORT PR_PORT ; Portname eingeben
_if PORT == "" then  _goto MENU		; Kein Portname ? Ja -> Menue
_input/command/maxlength=30 MSG PR_MSG	; Message eingeben
_if MSG == "" then  _goto MENU		; Keine message ? Ja -> Menue
_on error/ignore			; Fehler ignorieren
_message "'PORT'" MSG			; Message an einen Port senden
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.
_gosub WAIT				; Aus Taste warten
_goto MENU				; -> Menue

;
; **** Message an einen anderen Port verschicken
;
PMSG:
COM="PMSG"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_input/command/maxlength=20 USER PR_USER ; Username eingeben
_if USER == "" then  _goto MENU		; Kein Portname ? Ja -> Menue
_echo					; Eine Leerzeile ausgeben
_pmsg "'USER'"				; PMsg verschicken
_gosub WAIT				; Aus Taste warten
_goto MENU				; -> Menue

;
; **** Main-Menue
;
MAIN:
COM="MAIN"				; Menupunkt setzen
_goto END				; Ende der Batchdatei

;
; **** Turbo-Message
;
TURBO:
COM="TM"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_TM					; Turbo-Message starten
_goto MENU				; -> Menue

;
; **** Portliste mit Usern anschauen
;
WHO:
COM="WHO"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show ports/all				; Portliste anschauen
_gosub WAIT				; Aus Taste warten
_goto MENU				; -> Menue

;
; **** XCHAT starten
;
XCHAT:
COM="XCHAT"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_xchat					; WCHAT starten
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
