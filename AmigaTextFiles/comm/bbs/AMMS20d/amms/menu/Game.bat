;	$VER: 1.01 / 12.02.1994
; GAME-Menue
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
; **** Message-Menue
;
MENU="GAME"				; Name des Menues
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Menuetext ausgeben und Tasten abfragen

MENU:
COM="MENU"				; Menupunkt in Variable COM speichern
COMNAME="Game-Menu"			; WHO-Befehlsname setzen
_menu/file "menu:'{ALANGUAGE}'/game.txt" -
 "^R" MENU -				; Bildschirm erneuern
 "1" HELP_SNAKE -			; Hilfe zu Snake
 "2" HELP_GREED -			; Hilfe zu Greed
 "A" GW -				; Global War
 "B" CLI -				; Befehlsebene
 "C" BATTLESHIP -			; Battleship spielen
 "G" GREED -				; Greed spielen
 "H" HANGMAN -				; Hangman spielen
 "K" KNIWWEL -				; Kniwwel spielen
 "O" WOLITAIRE -			; Wolitaire spielen
 "P" PACMAN -				; Pacman spielen
 "R" ROBOTS -				; Robots spielen
 "S" SNAKE -				; Snake spielen
 "Q" MAIN -				; Hauptmenue
 "W" WANDERER				; Wanderer spielen

;
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

;
; **** Hilfe zu Greed
;
HELP_GREED:
COM="HELP GREED"			; Menupunkt setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_help game greed			; Hilfe zu Greed anzeigen
_gosub WAIT				; Auf Taste warten
_goto MENU				; -> Menue

;
; **** Hilfe zu Snake
;
HELP_SNAKE:
COM="HELP SNAKE"			; Menupunkt setzen
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_help game snake			; Hile zu Snake anzeigen
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
; **** GW spielen
;
GW:
COM="GAME GW"				; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_if/i {LEVEL} > GUESTLEVEL then -	; Wenn kein Gast -> GW spielen
    _game GW				; GW spielen
_goto MENU				; -> Menue

;
; **** Battleship spielen
;
BATTLESHIP:
COM="GAME BATTLESHIP"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game battleship			; Battleship spielen
_goto MENU				; -> Menue

;
; **** Greed spielen
;
GREED:
COM="GAME GREED"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Greed				; Greed spielen
_goto MENU				; -> Menue

;
; **** Hangman spielen
;
HANGMAN:
COM="GAME HANGMAN"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Hangman				; Hangman spielen
_goto MENU				; -> Menue

;
; **** Kniwwel spielen
;
KNIWWEL:
COM="GAME KNIWWEL"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Kniwwel				; Kniwwel spielen
_goto MENU				; -> Menue

;
; **** Wolitaire spielen
;
WOLITAIRE:
COM="GAME WOLITAIRE"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Wolitaire				; Wolitaire spielen
_goto MENU				; -> Menue

;
; **** Pacman spielen
;
PACMAN:
COM="GAME PACMAN"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Pacman				; Pacman spielen
_goto MENU				; -> Menue

;
; **** Robots spielen
;
ROBOTS:
COM="GAME ROBOTS"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Robots				; Robots spielen
_goto MENU				; -> Menue

;
; **** Snake spielen
;
SNAKE:
COM="GAME SNAKE"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Snake				; Snake spielen
_goto MENU				; -> Menue

;
; **** Wanderer spielen
;
WANDERER:
COM="GAME WANDERER"			; Menupunkt setzen
COMNAME=COM				; WHO-Befehlsname setzen
_game Wanderer				; Wnaderer spielen
_goto MENU				; -> Menue

;
; **** Main-Menue
;
MAIN:
COM="MAIN"				; Menupunkt setzen
_goto END				; Ende der Batchdatei

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
_if COM == "Menu" then _goto END	; Wenn Fehler im Pm-Menu -> Main-Menu
_goto MENU				; -> Menue
