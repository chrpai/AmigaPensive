;
;       Script zum Erstellen der Fastnet-Netzbretter
;	Es werden keine lokalen Bretter erstellt !
;
;       V 1.34 / 16-Mai-95
;
;  Neue Directories 
;
;      keine 
;
;  Neue MessieBretter 
;
;     ":Fastnet/Pins/Frauen"
;
;  Neue FileBretter
;
;     keine
;
; ---------------------------------------------------------------
;
;  Sollte ein Brett schon existieren wird es nicht mehr in die
;  Brettlisten der einzelnen Netzuser eingetragen !!!
;  Da es wohl einige Probleme mit anderen Netzusern gab, die
;  keine Fastnet-Netuser sind !!!
;
; ---------------------------------------------------------------

_on error FAIL				; Bei Fehler -> FAIL

NETUSER="NET@*"				; Kennung der Netzuser

_check file mbdat:BRETT-CREATE.dat set
_disk execute/keeplevel mbdat:BRETT-CREATE.dat
_goto start

set:
_echo
_echo "Setup (Wird in mbdat:BRETT-CREATE.dat gespeichert)"
_echo 
_input SYSOP "Name des Verwalters fuer die Netzbretter : "
_if SYSOP == "" then _eod
_input/N MAXMSGS "Maximale Messageanzahl pro Messagebrett : "
_if MAXMSGS == "" then _eod
_input/N MAXFILES "Maximale Fileanzahl pro Filebrett : "
_if MAXFILES == "" then _eod
_ex echo >mbdat:BRETT-CREATE.dat "SYSOP=*"'SYSOP'*""
_ex echo >>mbdat:BRETT-CREATE.dat "MAXMSGS='MAXMSGS'"
_ex echo >>mbdat:BRETT-CREATE.dat "MAXFILES='MAXFILES'"


start:


_echo
_echo "Verwalter: 'SYSOP'"
_echo "Max. Msganzahl pro Brett: 'MAXMSGS'"
_echo "Max. Fileanzahl pro Brett 'MAXFILES'"
_menu "Etwas aendern (j/N) ? " "j" set "y" set "n" weiter "^M" weiter

weiter:

fbocounter = 0
mbocounter = 0
dircounter = 0

; Verwalter eingeben

; Alle Bretter einrichten

_cd :					; Ins Root-Directory


Create-Bretties:

DIRNAME="Fastnet"			; Directory erzeugen
_gosub MAKEDIR


DIRNAME="Fastnet/Computer"		; Directory erzeugen
_gosub MAKEDIR


DIRNAME="Fastnet/Computer/Allgemein"	; Directory erzeugen
_gosub MAKEDIR

TEXT=1					; Text 1 anzeigen
COPYBOARD=""				; Zugriffe editieren
BOARDNAME="'DIRNAME'/Spiele"
_gosub MAKEBOARD
COPYBOARD=BOARDNAME			; Zugriffe kopieren
BOARDNAME="'DIRNAME'/Midi-Talk"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Pin"
_gosub MAKEBOARD



DIRNAME="Fastnet/Computer/Amiga"	; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Allgemein"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Software"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Hardware"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Sprachen"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Amiga_E"
_gosub MAKEBOARD


DIRNAME="Fastnet/Computer/PC"		; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Allgemein"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Software"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Hardware"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Sprachen"
_gosub MAKEBOARD


DIRNAME="Fastnet/Computer/Rest"		; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Allgemein"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Software"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Hardware"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Sprachen"
_gosub MAKEBOARD


DIRNAME="Fastnet/Hobby"			; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Sammeln"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Rezepte"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/RolePlaying"
_gosub MAKEBOARD



DIRNAME="Fastnet/Pins"			; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Allgemein"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Arbeit&Beruf"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Berlin"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Diskussion"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Flohmarkt"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Frauen"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Gay-Talk"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Politik"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Rechtschreibung"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Sport"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Streit"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Tiere"
_gosub MAKEBOARD


DIRNAME="Fastnet/Technik"		; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Auto"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Fahrrad"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/HiFi"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Mobilfunk"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Telekom"
_gosub MAKEBOARD


DIRNAME="Fastnet/Unterhaltung"		; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Buehne"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Fernsehen"
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/Kino&Video"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Musik"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Raver"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/ScienceFiction"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Witze"
_gosub MAKEBOARD


DIRNAME="Fastnet/Medien"		; Directory erzeugen
_gosub MAKEDIR

TEXT=2					; Text 2 anzeigen
COPYBOARD=""				; Zugriffe editieren
BOARDNAME="'DIRNAME'/Allgemein"
_gosub MAKEBOARD
COPYBOARD=BOARDNAME			; Zugriffe kopieren


DIRNAME="Fastnet/System"		; Directory erzeugen
_gosub MAKEDIR
BOARDNAME="'DIRNAME'/AMMS-Info"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Fastnet-Info"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Anregung&Kritik"
_gosub MAKEBOARD

TEXT=3					; Text 3 anzeigen
COPYBOARD=""				; Zugriffe editieren
BOARDNAME="'DIRNAME'/AMMS-Talk"
_gosub MAKEBOARD
COPYBOARD=BOARDNAME			; Zugriffe kopieren
BOARDNAME="'DIRNAME'/BugReport"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Fastnet-Talk"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Sysop-Talk"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Vote-Talk"
_gosub MAKEBOARD
BOARDNAME="'DIRNAME'/Vote-Info"
_gosub MAKEBOARD



TEXT=4					; Text 4 anzeigen
COPYBOARD=""				; Zugriffe editieren
BOARDNAME="'DIRNAME'/Updates"
_gosub MAKEFBOARD


DIRNAME="Fastnet/Files"			; Directory erzeugen
_gosub MAKEDIR

TEXT=5					; Text 5 anzeigen
COPYBOARD=""				; Zugriffe editieren
BOARDNAME="'DIRNAME'/Allgemein"
_gosub MAKEFBOARD
COPYBOARD=BOARDNAME			; Zugriffe kopieren
BOARDNAME="'DIRNAME'/Amiga"
_gosub MAKEFBOARD
BOARDNAME="'DIRNAME'/PC"
_gosub MAKEFBOARD
BOARDNAME="'DIRNAME'/AMMS-Tools"
_gosub MAKEFBOARD

; ENDE ...
_echo "^J 'dircounter' DIR(s) erzeugt !!"
_echo " 'mbocounter' Messiebrett(er) erzeugt !!"
_echo " 'fbocounter' Filebrett(er) erzeugt !!"
_echo "^JFertig !^J"
_eod

FAIL:
_echo "^JIrgendetwas ist schiefgelaufen !^J"
_eod


; ************************
; *
; *     Name : MAKEFBOARD
; *
; * Funktion : Unterroutine zum Erzeugen eines Boards
; *
; *    Input : BOARDNAME=Name des Boards
; *	       COPYBOARD=Name des Boards, dessen Zugriffe kopiert werden
; *			 Leer=Zugriffe editieren
; *
; *   Output : keine
; *
; ************************
MAKEFBOARD:
_check fboard "'BOARDNAME'" MAKEFBOARD_CREATE
_echo "#VFARBE 1#- Filebrett 'BOARDNAME' existiert schon ...#RESET#"
_goto MAKEFBOARD_END2

MAKEFBOARD_CREATE:
_echo "+ Erzeuge Netzfilebrett 'BOARDNAME' fuer max. 'MAXFILES' Files ..."
_make fboard "'BOARDNAME'" "'SYSOP'" "Netzbrett" MAXFILES 999999
 fbocounter = fbocounter+1

MAKEFBOARD_COPY:
_if COPYBOARD == "" then _goto MAKEFBOARD_EDIT
_edit fboard/copy "'COPYBOARD'" "'BOARDNAME'"
_goto MAKEFBOARD_END

MAKEFBOARD_EDIT:
_gosub PRINTTEXT
_edit fboard "'BOARDNAME'"

MAKEFBOARD_END:
_files/action=9/user="'NETUSER'" "'BOARDNAME'"
MAKEFBOARD_END2:
 TEXT=0
_return


; ************************
; *
; *     Name : MAKEBOARD
; *
; * Funktion : Unterroutine zum Erzeugen eines Boards
; *
; *    Input : BOARDNAME=Name des Boards
; *	       COPYBOARD=Name des Boards, dessen Zugriffe kopiert werden
; *			 Leer=Zugriffe editieren
; *
; *   Output : keine
; *
; ************************
MAKEBOARD:
_check board "'BOARDNAME'" MAKEBOARD_CREATE
_echo "#VFARBE 1#- Brett 'BOARDNAME' existiert schon ...#RESET#"
_goto MAKEBOARD_END2

MAKEBOARD_CREATE:
_echo "+ Erzeuge Netzbrett 'BOARDNAME' fuer 'MAXMSGS' Messages ..."
_make board "'BOARDNAME'" "'SYSOP'" "Netzbrett" MAXMSGS 200
 mbocounter = mbocounter+1

MAKEBOARD_COPY:
_if COPYBOARD == "" then _goto MAKEBOARD_EDIT
_edit board/copy "'COPYBOARD'" "'BOARDNAME'"
_goto MAKEBOARD_END

MAKEBOARD_EDIT:
_gosub PRINTTEXT
_edit board "'BOARDNAME'"

MAKEBOARD_END:
_read/action=9/user="'NETUSER'" "'BOARDNAME'"
MAKEBOARD_END2:
 TEXT=0
_return


; ************************
; *
; *     Name : MAKEDIR
; *
; * Funktion : Unterroutine zum Erzeugen eines Directory's
; *
; *    Input : DIRNAME=Name des Directory's
; *
; *   Output : keine
; *
; ************************
MAKEDIR:
_check dir "'DIRNAME'" MAKEDIR_CREATE
_echo "#VFARBE 1#Netzdir 'DIRNAME' existiert schon ...#RESET#"
_return
MAKEDIR_CREATE:
_echo "Erzeuge Netzdir 'DIRNAME' ..."
_make dir "'DIRNAME'"
 dircounter = dircounter+1
_return


; ************************
; *
; *     Name : PRINTTEXT
; *
; * Funktion : Unterroutine zum Anzeigen eines Hilfsstextes
; *
; *    Input : TEXT=Nummer des Hilfstextes
; *
; *   Output : keine
; *
; ************************
PRINTTEXT:
_if/i TEXT == 1 then _goto PRINTTEXT_1
_if/i TEXT == 2 then _goto PRINTTEXT_2
_if/i TEXT == 3 then _goto PRINTTEXT_3
_if/i TEXT == 4 then _goto PRINTTEXT_4
_if/i TEXT == 5 then _goto PRINTTEXT_5
_return

PRINTTEXT_1:
_echo "Bitte jetzt die Zugriffe fuer die oeffentlichen Netzbretter setzen."
_echo "Die ueberprueften User und die Netzuser bekommen die Zugriffe -RWD."
_echo "Die restlichen User duerfen hoechstens Lese-Zugriff -R-- bekommen !"
_echo "Beende den EDIT BOARD mit der Taste x."
_echo "<TASTE FUER EDIT BOARD>"
_waitkey
_return

PRINTTEXT_2:
_echo "Bitte jetzt die Zugriffe fuer die oeffentlichen Lese-Netzbretter setzen."
_echo "Die Netzuser bekommen die Zugriffe -RWD !"
_echo "Die restlichen User duerfen hoechstens Lese-Zugriff -R-- bekommen !"
_echo "Beende den EDIT BOARD mit der Taste x."
_echo "<TASTE FUER EDIT BOARD>"
_waitkey
_return

PRINTTEXT_3:
_echo "Bitte jetzt die Zugriffe fuer die Sysop-Netzbretter setzen."
_echo "Die Netzuser und die Sysops bekommen die Zugriffe -RWD !"
_echo "Die restlichen User bekommen KEIN Zugriff ---- !"
_echo "Beende den EDIT BOARD mit der Taste x."
_echo "<TASTE FUER EDIT BOARD>"
_waitkey
_return

PRINTTEXT_4:
_echo "Bitte jetzt die Zugriffe fuer das Update-Filebrett setzen."
_echo "Die Netzuser und die Sysops bekommen die Zugriffe -UDE !"
_echo "Die restlichen User bekommen KEIN Zugriff ---- !"
_echo "Beende den EDIT FBOARD mit der Taste x."
_echo "<TASTE FUER EDIT FBOARD>"
_waitkey
_return

PRINTTEXT_5:
_echo "Bitte jetzt die Zugriffe fuer die oeffentlichen Filebrett setzen."
_echo "Die Netzuser und die Sysops bekommen die Zugriffe -UDE !"
_echo "Die restlichen User bekommen hoechstens Download-Zugriff --D- !"
_echo "Beende den EDIT FBOARD mit der Taste x."
_echo "<TASTE FUER EDIT FBOARD>"
_waitkey
_return
