/* 
 * Einfacher Type-Befehl für Connectline (ONLINE-Texte oder in der Dosebene)
 *
 *
 * Features:
 *
 * - Hmmm...naja, sehr kleiner nicht zu verachtender Type-Befehl...
 *
 *  © 1995 by /\/\athias Grundler
 */

MODULE 'cl/clonline'               -> Die Funktionen der CLOnline.library einbinden
MODULE 'tools/file'                -> Funktionen für Filehandling einbinden
MODULE 'tools/exceptions'          -> Exceptions einbinden

ENUM ER_OL_CLONLINE, ER_CLONL_OPEN, ER_OL_CLTEXT
                                -> mögliche Exceptions festlegen!
PROC main() HANDLE              -> Programmanfang!
DEF status, m, l, n, list,x, y=1
VOID(' $VER: CL_Type Version 0.9+ © 1995 by /\\/\\athias Grundler ')
IF (clonlinebase:=OpenLibrary('clonline.library',0))=NIL THEN Raise(ER_OL_CLONLINE)
                                -> ^^^^ clonline.library öffnen!
  IF (status:=ClONL_Open('cltype.clcat'))=NIL THEN Raise(ER_CLONL_OPEN)
                                -> ^^^^ Catalogfile öffnen!
   ClONL_FlushOutput()          -> Ausgabebuffer leeren
    ClONL_ClearScreen()         -> Bildschirm leeren (löschen)
     ClONL_DrawTitle()          -> Titelzeile auf den Screen...
      m,l:=readfile(arg)        -> ARG (filename) in den Speicher lesen...
       n:=countstrings(m,l)     -> Anzahl der Zeilen im File berechnen lassen...
        list:=stringsinfile(m,l,n)
                                -> ^^^^ Eine Liste generieren...
         ForAll({x},list,`WriteF('\s\n',x))
                                -> ^^^^ Jede Zeile auf dem Screen ausgeben...
          ClONL_VPrintf('\n[44m                                                                          \b[37m                                  > TASTE <\n[0m',137)
                                -> Text ausgeben...
        REPEAT                  -> Schleifenanfang...
                Delay(5)        -> Spart (wertvolle) rechenzeit!!!!
        UNTIL (ClONL_IsKeyPressed())=TRUE OR CtrlC()
                                -> ^^^^ Nach ausgabe des Files auf Tastendruck
                                -> oder Ctrl+C warten!!!
 ClONL_Close()                  -> Portaktivitäten beenden
CloseLibrary(clonlinebase)      -> clonline.library schließen
EXCEPT                          -> exceptions verwalten!
SELECT exception                -> exceptionart festlegen
        CASE ER_OL_CLONLINE     -> Wenn exception=ER_OL_CLONLINE, dann...
                WriteF(' Konnte clonline.library nicht öffnen!\n')
        CASE ER_CLONL_OPEN      -> Wenn exception=ER_CLONL_OPEN, dann...
                WriteF(' Can`t Init ANSI-System...\n')
        CASE ER_OL_CLTEXT       -> Wenn exception=ER_OL_CLTEXT, dann
                WriteF(' Konnte cltext.library nicht öffnen!\n')
        DEFAULT                 -> Wenn Fehler nicht zuzuordnen..., dann
                report_exception()
                                -> ^^^^ Fehlernummer ect. ausgeben...
 ClONL_Close()                  -> Portaktivitäten beenden
CloseLibrary(clonlinebase)      -> clonline.library schließen
ENDSELECT                       -> Exceptionhandling beenden!
ENDPROC                         -> Programmende

-> ClTXT_ViewFile(A0,A1,A3)
