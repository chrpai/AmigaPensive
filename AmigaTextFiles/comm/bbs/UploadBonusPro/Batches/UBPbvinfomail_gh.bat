.key textdatei/A,brettverwalter/A,uploader/A
.bra {
.ket }
;
;UploadBonusPro-BVinfomail-BATCH
;
;wie die infomail.bat, jedoch soll der uebergebene Text an den Brettverwalter
;gesendet werden.

;defaulteinstellung:
EBLcopy {textdatei} {brettverwalter} {uploader}
;Standartaufruf; eblcopy wird als Absender den Uploader eintragen, der
;die Bonusgutschrift bekam.

;Aufrufbeispiel:
;EBLCOPY {textdatei} /BRETT/NAME
;eblcopy wird als Ziel ein Brett uebergeben;
;man kann also ein Brett fuer Verwalter/Sysop einrichten, in das man die
;BVinformationen jederzeit nachlesen kann, auch online oder sogar ueber
;Point, wenn man zulaesst, das dieses Brett bestellbar ist.

;weiterer Hinweis: es sind untereinander natuerlich auch mehrere
;Aufrufe moeglich.  Die uebergebene Textdatei wird erst geloescht, wenn
;das batch beendet ist.

;Kommentarzeilen koennen nach Installation geloescht werden!
