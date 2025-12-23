;	$VER: 1.01 / 12.02.1994
; Statistik-Menue
; ---------------
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
; **** Statistic-Menue
;
MENU="Statistic"			; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Statistic-Menu"		; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/statistic.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "A" ANY_PORTS -			; Portauslastung anderer Ports
 "B" CLI -				; Befehleebene
 "G" ALL_PORTS -			; Gesamtport-Statistik
 "I" INFO_NODES -			; Infotexte der Nodes anschauen
 "L" LIST_USER -			; Userliste anschauen
 "N" SHOW_NODES -			; Nodeliste anschauen
 "P" PROTOCOL -				; Protokoll anschauen
 "Q" MAIN -				; Main-Menue
 "S" ONE_PORT -				; Userdaten anschauen
 "T" TOP_TEN -				; Top Ten anschauen
 "U" SHOW_USER -			; Userdaten anschauen
 "V" SHOW_NET				; Netzstruktur anschauen

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

;
; **** Statistik mehrerer Ports anschauen
;
ANY_PORTS:
COM="STATISTIC PORT/ANY"		; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show ports/all				; Alle Ports anzeigen
_disk text "menu:'{ALANGUAGE}'/any_ports.txt" ; Infotext anzeigen
_input/maxlength=20 NAME ""		; Prompt steht im Info / Name eingeben
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_on error/ignore			; Fehler ignorieren
_statistic port "'NAME'"		; Protokoll anzeigen
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Rueckkehr in die Befehlsebene
;
CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; Ende der Batchdatei

;
; **** Gesamtauslastung des Systems anschauen
;
ALL_PORTS:
COM="STATISTIC PORT/ALL"		; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_statistic port *			; Gesamtauslastung anzeigen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Nodeliste anschauen
;
INFO_NODES:
COM="SHOW NODES/FULL"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{ALANGUAGE}'/info_nodes.txt" ; Infotext anzeigen
_input/maxlength=16 NAME ""		; Prompt steht im Info / Name eingeben
_echo					; Leerzeile ausgeben
_show nodes/full "'NAME'"		; Nodeliste mit Netzstruktur anschauen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Userliste anschauen
;
LIST_USER:
COM="LIST USER"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{ALANGUAGE}'/list_user.txt" ; Infotext anzeigen
_input/maxlength=20 NAME ""		; Prompt steht im Info / Name eingeben
_echo					; Leerzeile ausgeben
_list user "'NAME'"			; User listen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Nodeliste anschauen
;
SHOW_NODES:
COM="SHOW NODES"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show nodes				; Nodeliste anschauen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Protokoll anschauen
;
PROTOCOL:
COM="PROTOCOL"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_disk text "menu:'{ALANGUAGE}'/protocol.txt" ; Infotext anzeigen
_input/maxlength=20 NAME ""		; Prompt steht im Info / Name eingeben
_echo					; Leerzeile ausgeben
_protocol "'NAME'"			; Protokoll anzeigen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Main-Menue
;
MAIN:
COM="MAIN"				; Menupunkt setzen
_goto END				; Ende der Batchdatei

;
; **** Statistik eines Ports anschauen
;
ONE_PORT:
COM="STATISTIC PORT"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_statistic port 			; Auslastung des akt. Ports anzeigen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Top Ten anschauen
;
TOP_TEN:
COM="STATISTIC TOPTEN"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_statistic topten			; Topten anzeigen
_goto MENU				; -> Menue

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
; Netzstruktur anschauen
;
SHOW_NET:
COM="SHOW NODES/NET"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_show nodes/net				; Netzstruktur anschauen
_gosub WAIT				; Auf Taste warten
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
