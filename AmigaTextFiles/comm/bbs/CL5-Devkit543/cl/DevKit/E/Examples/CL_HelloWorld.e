/*
 * Beispiel für Ausgabe eines Textes auf den Screen des Ports
 *  und für eine Eingabeaufforderung (Stringeingabe)
 */

MODULE 'cl/clonline'            -> Die Funktionen der CLOnline.library einbinden

ENUM ER_OL_CLONLINE, ER_CLONL_OPEN
                                -> mögliche Exceptions festlegen!

CONST   COLOR_Black     = 0,            -> ANSI-Farbe 0 festlegen
        COLOR_Red       = 1,            -> ANSI-Farbe 1 festlegen
        COLOR_Green     = 2,            -> ANSI-Farbe 2 festlegen
        COLOR_Yellow    = 3,            -> ANSI-Farbe 3 festlegen
        COLOR_Blue      = 4,            -> ANSI-Farbe 4 festlegen
        COLOR_Pink      = 5,            -> ANSI-Farbe 5 festlegen
        COLOR_Cyan      = 6,            -> ANSI-Farbe 6 festlegen
        COLOR_White     = 7             -> ANSI-Farbe 7 festlegen
CONST   STYLE_NORMAL    = 0,            -> ANSI-Style NORMAL festlegen
        STYLE_BOLD      = 1,            -> ANSI-Style BOLD festlegen
        STYLE_KURSIV    = 2,            -> ANSI-Style KURSIV festlegen
        STYLE_UNDERLINED= 3,            -> ANSI-Style UNDERLINED festlegen
        STYLE_INVERSE   = 4             -> ANSI-Style INVERSE festlegen
CONST   TYPE_ASCII      = 0,            -> TYPE für Blueread ect. festlegen
        TYPE_NUMBER     = 1,            -> TYPE für Blueread ect. festlegen
        TYPE_BOOL       = 2,            -> TYPE für Blueread ect. festlegen
        TYPE_SECRET     = 3             -> TYPE für Blueread ect. festlegen


PROC main() HANDLE              -> Programmanfang!
DEF status, str[70]:STRING, def[70]:STRING, len, type, maske, strptr
IF (clonlinebase:=OpenLibrary('clonline.library',0))=NIL THEN Raise(ER_OL_CLONLINE)
                                -> ^^^^ clonline.library öffnen!
 IF (status:=ClONL_Open('helloworld.clcat'))=NIL THEN Raise(ER_CLONL_OPEN)
                                -> ^^^^ Catalogfile öffnen!
  ClONL_FlushOutput()           -> Ausgabebuffer leeren
   ClONL_ClearScreen()          -> Bildschirm leeren (löschen)
    ClONL_Color(COLOR_White, COLOR_Blue, STYLE_NORMAL)
                                -> ^^^^ Vordergrundfarbe auf Rot, Hintergrund-
                                -> farbe auf Blau, Style auf Normal
     ClONL_DrawTitle()          -> Titel zeichnen...
      ClONL_Normal()            -> Textstyles und ANSI-Modes wieder auf normal
       len:=70                  -> Maximale Stringlenge für BlueRead angeben!
        type:=TYPE_ASCII        -> Eingabetyp festlegen
         maske:=0               -> Maske auf 0 (nicht benötigt)
          StrCopy(def,' So, bitte was eingeben... ',28)
                                -> ^^^^ Defaultstring festlegen
           strptr:=ClONL_BlueRead(str,def,len,type,maske)
                                -> ^^^^ String einlesen, def dabei als DEFAULTstring
            ClONL_VPrintf('\n\nEingabe:',10)
                                -> ^^^^ Text ausgeben!
             ClONL_VPrintf(str,70)
                                -> ^^^^ oben eingegebener String ausgeben!
              ClONL_VPrintf('\n',1)
                                -> ^^^^ Zeilenende ausgeben!...
 ClONL_Close()                  -> Portaktivitäten beenden
CloseLibrary(clonlinebase)      -> clonline.library schließen
EXCEPT                          -> exceptions verwalten!
SELECT exception                -> exceptionart festlegen
        CASE ER_OL_CLONLINE     -> Wenn exception=ER_CLONLINE, dann...
                WriteF(' Konnte clonline.library nicht öffnen!\n')
        CASE ER_CLONL_OPEN      -> Wenn exception=ER_CLONL_OPEN, dann...
                WriteF(' Can`t Init ANSI-System...\n')
ENDSELECT                       -> Exceptionhandling beenden!
ENDPROC                         -> Programmende


