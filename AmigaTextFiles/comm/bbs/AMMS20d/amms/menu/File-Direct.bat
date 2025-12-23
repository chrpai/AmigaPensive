;	$VER: 1.11 / 12.04.1995
; File-Direktanwahl
; -----------------
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
; **** File-Direktanwahl
;
MENU="File-Direct"			; Name des Menues
COMNAME=MENU				; WHO-Befehlsname setzen
COM="Menu"				; Menupunkt in Variable COM speichern
_on error SAVE_ERROR			; Beim Fehler -> Fehlerbehandlungsrou.

; Variablen definieren

F1="#FARBE 1,7#"			; Farbe fuer nicht angewaehlt
F2="#FARBE 7,1#"			; Farbe fuer angewaehlt
POS=0					; Zuerst 1. Menuepunkt anwaehlen
YS=2					; Ab Zeile 2 Menuepunkte ausgeben

BF1="#FARBE 5,7#"			; Farbe fuer nicht angewaehlt
BF2="#FARBE 7,5#"			; Farbe fuer angewaehlt
BPOS=0					; Zuerst 1. Board anwaehlen

; Alle Directories als Menuepunkte und weitere Menuepunkte einlesen
; und ausgeben

_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_gosub GETDIRS				; Dirs als Menuepunkte einlesen und ausgeben

I=MC					; Alte Menuepunktanzahl retten
_disk execute/keep "menu:'{alanguage}'/file-direct.bat" ; Zusatz-Menuepunkte holen
_gosub PSCREEN				; Rest-Menuepunkte ausgeben

_disk text "menu:'{alanguage}'/file-direct.txt" ; Menuetext ausgeben

F=F2					; Aktiv-Farbe in F
INDEX=POS				; Aktuellen Menuepunkt in INDEX
_gosub PMENU				; Aktiv-Menuepunkt ausgeben
_goto CMENU				; -> Waehlen im Menue

; Menuetext ausgeben und Tasten abfragen

MENU:
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
I=0					; Alle Menuepunkt ausgeben
_gosub PSCREEN				; Alle Menuepunkte ausgeben
_disk text "menu:'{alanguage}'/file-direct.txt" ; Menuetext ausgeben
;_goto CMENU				; -> Waehlen im Menue


; ***********
; *
; * Funktion: Ein Menuepunkt im Menue auswaehlen.
; *	      - Wechsel der Menuepunkte mit den Cursortasten
; *	      - Anwahl eines Menuepunktes mit der Taste RETURN
; *	      - Ende mit der Taste Q
; *	      - Befehlsebene mit der Taste B
; *	      - Das Labelfeld Lx dient zum Ansprung der einzelnen
; *		Unterroutinen bei der Anwahl eines Menuepunktes.
; *		Bei den Brettern steht ein B fuer den Sprung zum
; *		Label CMENU_B. Die anderen Menuepunkte springen
; *		zu den Labeln CMENU_'Lx' ('Lx' = Inhalt des Labels).
; *
; * Name  : CMENU
; *
; * Input : Mx = Feld mit Menuepunkten (M0, M1, M2 usw.)
; *	    MC = Anzahl der Menuepunkte
; *	    POS = Akt. Menuepunkt des Users (0 bis MC-1).
; *	    F1 = Farbstring : Menuepunkt nicht aktuell
; *	    F2 = Farbstring : Menuepunkt aktuell
; *	    YS = Zeilennummer, wo Menue beginnt
; *
; * Output: keine
; *
; * Zusatzvariablen : X,Y,N
; *
CMENU:
COM="Menu"				; Menupunkt in Variable COM speichern
_menu/noecho "" -			; Tasten abfragen
 "^1" CMENU_UP -
 "^2" CMENU_DOWN -
 "^3" CMENU_LEFT -
 "^4" CMENU_RIGHT -
 "^M" CMENU_GET -			; Menuepunkt zum Lesen anwaehlen
 "^R" MENU -				; Bildschirm refreshen
 "B" CMENU_CLI -				; Befehlsebene
 "Q" CMENU_EXIT				; Ende des Menues

;
; ** Ende
;
CMENU_EXIT:
_goto END				; -> Ende

;
; **** Rueckkehr in die Befehlsebene
;
CMENU_CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; -> Ende der Batchdatei

;
; ** Cursor HOCH
;
CMENU_UP:
COM="CMENU_UP"				; Menupunkt setzen
INDEX=POS				; Alte Position merken
_if/i POS < 3 then POS=POS-(((POS+(MC-1)/3*3))/MC)*3+(MC+2)/3*3
					; Bei Unterlauf -> Letzte Zeile
POS=POS-3				; Neue Position errechnen
_gosub CMENU_PRINT			; Menuepunkt optisch wechseln
_goto CMENU				; -> Neue Taste abfragen

;
; ** Cursor RUNTER
;
CMENU_DOWN:
COM="CMENU_DOWN"			; Menupunkt setzen
INDEX=POS				; Alte Position merken
POS=POS+3				; Neue Position errechnen
_if/i POS >= MC then POS=POS%3		; Bei Unterlauf -> Erste Zeile
_gosub CMENU_PRINT			; Menuepunkt optisch wechseln
_goto CMENU				; -> Neue Taste abfragen

;
; ** Cursor LINKS
;
CMENU_LEFT:
COM="CMENU_LEFT"			; Menupunkt setzen
INDEX=POS				; Alte Position merken
_if/i (POS+2)/3*3 != (POS+3)/3*3 then POS=POS+3-(((POS+3)/MC)*(2-(MC-1)%3))
					; Bei Unterlauf -> Letzte Spalte
POS=POS-1				; Neue Position errechnen
_gosub CMENU_PRINT			; Menuepunkt optisch wechseln
_goto CMENU				; -> Neue Taste abfragen

;
; ** Cursor RECHTS
;
CMENU_RIGHT:
COM="CMENU_RIGHT"			; Menupunkt setzen
INDEX=POS				; Alte Position merken
_if/i (POS+1)/3*3+(POS+1)/MC > POS/3*3 then POS=POS/3*3-1
					; Bei Spaltenueberlauf -> Erste Spalte
POS=POS+1				; Neue Position errechnen
_gosub CMENU_PRINT			; Menuepunkt optisch wechseln
_goto CMENU				; -> Neue Taste abfragen

;
; ***** UNTERROUTINE : Wechsel eines Menuepunktes
; Input: INDEX = Alte Position
;	 POS = Neue Position
CMENU_PRINT:
F=F1					; Loesch-Farbe setzen
_gosub PMENU				; Alten Menuepunkt inaktiv ausgeben
INDEX=POS
F=F2					; Setz-Farbe setzen
_gosub PMENU				; Neuen Menuepunkt aktiv ausgeben
_return					; Ende

;
; ** Menuepunkt anwaehlen
;
CMENU_GET:
COM="CMENU_GET"				; Menupunkt setzen
N=L'POS'				; Sprunglabel in N
_goto CMENU_'N'				; Sprung in Unterroutine

;
; ** Ein Directory listen / Board auswaehlen
;
CMENU_B:
COM="CMENU_B"				; Menupunkt setzen

N=M'POS'				; Directoryname holen
_cd :					; Ins Root-Directory
_cd "'N'"				; In Directory wechseln

; Alle Bretter aus dem Directory auslesen
BC=-1					; Anzahl der Boards initialisieren
N=""					; Ab erstem Boards suchen
CMENU_B_LOOP:
BC=BC+1					; Boardanzahl erhoehen
_get dir/fboards "'N'" N		; Naechstes Board suchen
BM'BC'=N				; Boardname speichern
_if N != "" then _goto CMENU_B_LOOP	; Board vorhanden ? Ja -> weiter suchen

_disk execute/keep "menu:'{alanguage}'/file-direct-b.bat" ; Zusatz-Menuepunkte holen

BY=YS+MC/3+4				; Oberste Menuezeile errechnen
I=0					; Alle Boards ausgeben
BPOS=0					; Cursor auf ersten Menuepunkt
_echo/nolf "#RESET##SETLINE 'BY'##CLB#"	; Unteren Bildschirm loeschen
_gosub PALLBOARDS			; Menue aufbauen
_disk text "menu:'{alanguage}'/file-direct-b.txt" ; Menuetext ausgeben
_goto CBOARD				; -> Boards auswaehlen



; ***********
; *
; * Funktion: Ein Menuepunkt im Menue ausgeben.
; *
; * Name  : PMENU
; *
; * Input : Mx = Feld mit Menuepunkten (M0, M1, M2 usw.)
; *	    INDEX = Akt. Menuepunkt des Users (0 bis MC-1).
; *	    F = Farbstring
; *	    YS = Zeilennummer, wo Menue beginnt
; *
; * Output: keine
; *
; * Zusatzvariablen : X,Y,N
; *
PMENU:
X=(INDEX-INDEX/3*3)*26+1		; Spalte X errechnen
Y=INDEX/3+YS				; Zeile Y errechnen
N=M'INDEX'				; Menuepunkt holen
_echo/nolf "#SETPOS 'Y','X'#'F'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#LEFT 26#'N'"
;_echo/nolf "#SETPOS 'Y','X'#'F''N'"	; Menuepunkt ausgeben
_return					; Ende


; ***********
; *
; * Funktion: Mehere Menuepunkte auf dem Bildschirm ausgeben.
; *
; * Name  : PSCREEN
; *
; * Input : Mx = Feld mit Menuepunkten (M0, M1, M2 usw.)
; *	    POS = Akt. Menuepunkt des Users (0 bis MC-1).
; *	    MC = Anzahl der Menuepunkte
; *	    F1 = Farbstring : Menuepunkt nicht aktuell
; *	    F2 = Farbstring : Menuepunkt aktuell
; *	    YS = Zeilennummer, wo Menue beginnt
; *	    I = Menuepunktnummer, ab der die Ausgabe beginnen soll.
; *		Wenn I > 0, dann sollten zuvor die anderen Menuepunkte
; *		zuvor ausgegeben sein.
; *
; * Output: I = MC
; *
; * Zusatzvariablen : F,X,Y,N
; *
PSCREEN:
X=(I-I/3*3)*26+1			; Spalte X errechnen
Y=I/3+YS				; Zeile Y errechnen
_if/i I == MC then _goto PSCREEN_END	; Ende erreicht ? JA -> Ende
N=M'I'					; Menuepunkt holen
F=F1					; Farbe setzen
_if/i I == POS then F=F2		; Akt. Menuepunkt ? Ja, Farbe aendern
_echo/nolf "#SETPOS 'Y','X'#'F'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#LEFT 26#'N'"
;_echo/nolf "#SETPOS 'Y','X'#'F''N'"	; Menuepunkt ausgeben
I=I+1					; Feldnummer auf naechsten Menuepunkt
_goto PSCREEN				; -> Naechsten Menuepunkt ausgeben

PSCREEN_END:
_if/i X == 27 then -			; Zwei Spalten noch ausgeben ?
   _echo/nolf "#SETPOS 'Y','X'#'F1'{LIMSPACEON}{TAB 52}{LIMSPACEOFF}"
_if/i X == 53 then -			; Eine Spalte noch ausgeben ?
   _echo/nolf "#SETPOS 'Y','X'#'F1'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}"
_return					; Ende


; ***********
; *
; * Funktion: Alle Directories, die Boards enthalten, als Menuepunkte
; *	      einlesen. Die Variablen DIRx, Lx und MC sind GLOBAL !!
; *	      Die Directories werden ausgegeben !
; *
; * Name  : GETDIRS
; *
; * Input : keine
; *
; * Output: Mx = Feld mit Menuepunkten (M0, M1, M2 usw.)
; *	    Lx = Feld mit Labeln (L0, L1, L2 usw.)
; *	    MC = Anzahl der gefundenen Directories
; *	    YS = Zeilennummer, wo Menue beginnt
; *
; * Zusatzvariablen : LE,N,Nx,X,Y
; *
GETDIRS:
MC=0					; Anzahl der Dirs initilisieren
LE=0					; Rekursions-Level initialisieren
_cd :					; Alle Directories ab dem Root-Dir

; Pruefe, ob Root-Directory Boards enthaelt

_get dir/fboard "" N			; Existiert Board in Directory ?
_if N == "" then _goto GETDIRS_RE	; Nein -> Unterdirectories suchen

; Root-Directory enthaelt Boards : Menuepunkt ausgeben

M'MC'=":"				; Root-Directoryname speichern
L'MC'="B"				; Label B wie Board zum Menuepunkt setzen
X=(MC-MC/3*3)*26+1			; Spalte X errechnen
Y=MC/3+YS				; Zeile Y errechnen
_echo/nolf "#SETPOS 'Y','X'#'F1'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#LEFT 26#:"
;_echo/nolf "#SETPOS 'Y','X'#'F''N'"	; Menuepunkt ausgeben
MC=MC+1					; Anzahl der Menuepunkte erhoehen

; Rekursionsroutine starten

GETDIRS_RE:
N'LE'=""				; Leerstring fuer ersten Eintrag

GETDIRS_LOOP:
_get dir/dir/fboard N'LE' N		; Dirname einlesen
_if N == "" then _return		; kein weitere Name -> Ende

N'LE'=N					; Dirname wegen Rekursion speichern
M'MC'="'{PATH}''N'"			; Dirname in Menuepunkt speichern
L'MC'="B"				; Label B wie Board zum Menuepunkt setzen

; Pruefe, ob Directory Boards enthaelt

_cd "'N'"				; Wechsel ins Unterdirectory
_get dir/fboard "" N			; Existiert Board in Directory ?
_if N == "" then _goto GETDIRS_NEXT	; Nein -> Unterdirectories suchen

; Directory enthaelt Boards : Menuepunkt ausgeben

X=(MC-MC/3*3)*26+1			; Spalte X errechnen
Y=MC/3+YS				; Zeile Y errechnen
N=M'MC'					; Menuepunkt holen
_echo/nolf "#SETPOS 'Y','X'#'F1'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#LEFT 26#'N'"
;_echo/nolf "#SETPOS 'Y','X'#'F''N'"	; Menuepunkt ausgeben
MC=MC+1					; Anzahl der Menuepunkte erhoehen

; Unterdirectories durchsuchen

GETDIRS_NEXT:
LE=LE+1					; Rekursionslevel erhoehen
_gosub GETDIRS_RE			; Unterdirectories durchsuchen
LE=LE-1					; Rekursionslevel erniedrigen
_cd /					; Ein Directory zurueck

_goto GETDIRS_LOOP			; -> Naechstes Directory suchen



; ***********
; *
; * Funktion: Ein Board im Boardmenue auswaehlen.
; *	      - Wechsel der Menuepunkte mit den Cursortasten
; *	      - Anwahl eines Menuepunktes mit der Taste RETURN
; *	      - Ende mit der Taste Q
; *	      - Befehlsebene mit der Taste B
; *
; * Name  : CBOARD
; *
; * Input : BMx = Feld mit Menuepunkten (BM0, NM1, BM2 usw.)
; *	    BPOS = Akt. Menuepunkt des Users (0 bis BC-1).
; *	    BF1 = Farbstring : Menuepunkt nicht aktuell
; *	    BF2 = Farbstring : Menuepunkt aktuell
; *	    BY = Zeilennummer, wo Menue beginnt
; *
; * Output: keine
; *
; * Zusatzvariablen : X,Y,N
; *
CBOARD:
T=0					; Bildschirm nicht geloescht signal.
CBOARD1:
COM="Choose Board"			; Menupunkt in Variable COM speichern
_menu/noecho "" -			; Tasten abfragen
 "^1" CBOARD_UP -
 "^2" CBOARD_DOWN -
 "^3" CBOARD_LEFT -
 "^4" CBOARD_RIGHT -
 "^M" CBOARD_FILES -			; Menuepunkt zum Files anwaehlen
 "^R" CBOARD_REFRESH -			; Bildschirm refreshen
 "B" CBOARD_CLI -			; Befehlsebene
 "M" CBOARD_MARK -			; Board in Liste ein-/austragen
 "Q" CBOARD_EXIT -			; Ende des Menues
 "F" CBOARD_FILES -			; Menuepunkt zum Files anwaehlen
 "U" CBOARD_UPLOAD			; Menuepunkt zum Upload anwaehlen


;
; ** Ende
;
CBOARD_EXIT:
Y=(MC-1)/3+YS				; Letzte Zeile des Dir-Menues errechnen
_echo/nolf "#RESET##SETLINE 'BY'##CLB##SETLINE 'Y'#" ; Board-Menu loeschen
_disk text "menu:'{alanguage}'/file-direct.txt" ; Menuetext ausgeben
_goto CMENU				; -> Menue

;
; ** Bildschirm neu aufbauen
;
CBOARD_REFRESH:
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
I=0					; Alle Menuepunkt ausgeben
_gosub PSCREEN				; Alle Menuepunkte ausgeben
_disk text "menu:'{alanguage}'/file-direct.txt" ; Menuetext ausgeben
I=0					; Alle Boards ausgeben
_gosub PALLBOARDS			; Menue aufbauen
_disk text "menu:'{alanguage}'/file-direct-b.txt" ; Menuetext ausgeben
_goto CBOARD				; -> Neue Taste abfragen

;
; **** Board in Boardliste ein-/austragen
;
CBOARD_MARK:
COM="CBOARD_MARK"			; Menupunkt setzen
_if/i {LEVEL} <= GUESTLEVEL then _goto CBOARD1 ; Wenn Gast -> Taste ignorieren
_if/i BPOS >= BC-2 then _goto CBOARD1	; Exit-Menuepunkt ? Ja -> Taste ignorieren

N=BM'BPOS'				; Boardname holen
_check fboard/boardlist "'N'" CBOARD_T	; Board in Boardliste ? Nein -> verzw.
_files/action=106 "'N'"			; Board aus Boardliste austragen
INDEX=BPOS
F=BF2					; Setz-Farbe setzen
_gosub PBOARD				; Menuepunkt neu ausgeben
_goto CBOARD1				; -> Neue Taste abfragen

CBOARD_T:
_files/action=105 "'N'"			; Board in Boardliste aufnehmen
INDEX=BPOS
F=BF2					; Setz-Farbe setzen
_gosub PBOARD				; Menuepunkt neu ausgeben
_goto CBOARD1				; -> Neue Taste abfragen

;
; **** Rueckkehr in die Befehlsebene
;
CBOARD_CLI:
COM="CLI"				; Menupunkt setzen
JUMP=="CLI"				; Einsprunglabel im Main-Menue setzen
_goto END				; -> Ende der Batchdatei

;
; ** Board lesen
;
CBOARD_FILES:
COM="CBOARD_FILES"			; Menupunkt setzen
_if/i BPOS == BC-2 then _goto CBOARD_EXIT ; Exit-Menuepunkt ? Ja -> Ende
_if/i BPOS == BC-1 then _goto CBOARD_CLI ; CLI-Menuepunkt ? Ja -> CLI

N=BM'BPOS'				; Boardname holen
_check fboard/acc=16 "'N'" CBOARD1	; Falls kein Board-Zugriff -> Taste ignorieren
T=1					; Bildschirm geloescht signalisieren
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_files "'N'"				; Files anschauen
_goto CBOARD_REFRESH			; -> Bildschirm refreshen

;
; ** Batch-Upload
;
CBOARD_UPLOAD:
COM="CBOARD_UPLOAD"			; Menupunkt setzen
_if/i BPOS >= BC-2 then _goto CBOARD1	; Exit-Menuepunkt ? Ja -> Taste ignorieren

N=BM'BPOS'				; Boardname holen
_check fboard/acc=2 "'N'" CBOARD1	; Falls kein Upload-Zugriff -> Taste ignorieren
T=1					; Bildschirm geloescht signalisieren
_echo/nolf "#RESET##HOME##CLS#"		; Bildschirm loeschen
_batch upload "'N'"			; Batchupload
_goto CBOARD_REFRESH			; -> Bildschirm refreshen

;
; ** Cursor HOCH
;
CBOARD_UP:
COM="CBOARD_UP"				; Menupunkt setzen
INDEX=BPOS				; Alte Position merken
_if/i BPOS < 3 then BPOS=BPOS-(((BPOS+(BC-1)/3*3))/BC)*3+(BC+2)/3*3
					; Bei Unterlauf -> Letzte Zeile
BPOS=BPOS-3				; Neue Position errechnen
_gosub CBOARD_PRINT			; Menuepunkt optisch wechseln
_goto CBOARD1				; -> Neue Taste abfragen

;
; ** Cursor RUNTER
;
CBOARD_DOWN:
COM="CBOARD_DOWN"			; Menupunkt setzen
INDEX=BPOS				; Alte Position merken
BPOS=BPOS+3				; Neue Position errechnen
_if/i BPOS >= BC then BPOS=BPOS%3	; Bei Unterlauf -> Erste Zeile
_gosub CBOARD_PRINT			; Menuepunkt optisch wechseln
_goto CBOARD1				; -> Neue Taste abfragen

;
; ** Cursor LINKS
;
CBOARD_LEFT:
COM="CBOARD_LEFT"			; Menupunkt setzen
INDEX=BPOS				; Alte Position merken
_if/i (BPOS+2)/3*3 != (BPOS+3)/3*3 then BPOS=BPOS+3-(((BPOS+3)/BC)*(2-(BC-1)%3))
					; Bei Unterlauf -> Letzte Spalte
BPOS=BPOS-1				; Neue Position errechnen
_gosub CBOARD_PRINT			; Menuepunkt optisch wechseln
_goto CBOARD1				; -> Neue Taste abfragen

;
; ** Cursor RECHTS
;
CBOARD_RIGHT:
COM="CBOARD_RIGHT"			; Menupunkt setzen
INDEX=BPOS				; Alte Position merken
_if/i (BPOS+1)/3*3+(BPOS+1)/BC > BPOS/3*3 then BPOS=BPOS/3*3-1
					; Bei Spaltenueberlauf -> Erste Spalte
BPOS=BPOS+1				; Neue Position errechnen
_gosub CBOARD_PRINT			; Menuepunkt optisch wechseln
_goto CBOARD1				; -> Neue Taste abfragen


;
; ***** UNTERROUTINE : Wechsel eines Menuepunktes
; Input: INDEX = Alte Position
;	 BPOS = Neue Position
CBOARD_PRINT:
F=BF1					; Loesch-Farbe setzen
_gosub PBOARD				; Alten Menuepunkt inaktiv ausgeben
INDEX=BPOS
F=BF2					; Setz-Farbe setzen
_gosub PBOARD				; Neuen Menuepunkt aktiv ausgeben
_return					; Ende


; ***********
; *
; * Funktion: Ein Menuepunkt im Menue ausgeben.
; *
; * Name  : PBOARD
; *
; * Input : BMx = Feld mit Menuepunkten (BM0, BM1, NM2 usw.)
; *	    INDEX = Akt. Menuepunkt des Users (0 bis BC-1).
; *	    F = Farbstring
; *	    BY = Zeilennummer, wo Menue beginnt
; *
; * Output: keine
; *
; * Zusatzvariablen : X,Y,N
; *
PBOARD:
X=(INDEX-INDEX/3*3)*26+1		; Spalte X errechnen
Y=INDEX/3+BY				; Zeile Y errechnen
N=BM'INDEX'				; Menuepunkt holen
M=" "					; Board nicht in Boardliste
_check fboard/boardlist "'N'" PBOARD_PRINT
M="*"					; Board in Boardliste
PBOARD_PRINT:
_echo/nolf "#SETPOS 'Y','X'#'F'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#LEFT 26#'M' 'N'"
_return					; Ende


; ***********
; *
; * Funktion: Mehere Menuepunkte auf dem Bildschirm ausgeben.
; *
; * Name  : PALLBOARDS
; *
; * Input : BMx = Feld mit Menuepunkten (M0, M1, M2 usw.)
; *	    BPOS = Akt. Menuepunkt des Users (0 bis BC-1).
; *	    BC = Anzahl der Menuepunkte
; *	    BF1 = Farbstring : Menuepunkt nicht aktuell
; *	    BF2 = Farbstring : Menuepunkt aktuell
; *	    BY = Zeilennummer, wo Menue beginnt
; *	    I = Menuepunktnummer, ab der die Ausgabe beginnen soll.
; *		Wenn I > 0, dann sollten zuvor die anderen Menuepunkte
; *		zuvor ausgegeben sein.
; *
; * Output: I = BC
; *
; * Zusatzvariablen : F,X,Y,N
; *
PALLBOARDS:
X=(I-I/3*3)*26+1			; Spalte X errechnen
Y=I/3+BY				; Zeile Y errechnen
_if/i I == BC then _goto PALLBOARDS_END	; Ende erreicht ? JA -> Ende
N=BM'I'					; Menuepunkt holen
F=BF1					; Farbe setzen
_if/i I == BPOS then F=BF2		; Akt. Menuepunkt ? Ja, Farbe aendern
M=" "					; Board nicht in Boardliste
_check fboard/boardlist "'N'" PALLBOARDS_PRINT
M="*"					; Board in Boardliste
PALLBOARDS_PRINT:
_echo/nolf "#SETPOS 'Y','X'#'F'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}#LEFT 26#'M' 'N'"
I=I+1					; Feldnummer auf naechsten Menuepunkt
_goto PALLBOARDS			; -> Naechsten Menuepunkt ausgeben

PALLBOARDS_END:
_if/i X == 27 then -			; Zwei Spalten noch ausgeben ?
   _echo/nolf "#SETPOS 'Y','X'#'BF1'{LIMSPACEON}{TAB 52}{LIMSPACEOFF}"
_if/i X == 53 then -			; Eine Spalte noch ausgeben ?
   _echo/nolf "#SETPOS 'Y','X'#'BF1'{LIMSPACEON}{TAB 26}{LIMSPACEOFF}"
_return					; Ende



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
; **** Endroutine
; **** Diese Routine muss fuer das Beenden des Menues aufgerufen werden.
;
END:
_eod					; Ende

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
