.key textdatei/A,empfaenger/A,brettverwalter
.bra {
.ket }
;
;UploadBonusPro-infomail-BATCH
;
;Bitte hier das mailtool eintragen, das die Gutschrifteninformationen
;dem Uploader als PMail zusendet.
;
;wenn Du keine Information an den Uploader senden lassen moechtest, dann
;loesche oder rename diese batchdatei einfach; infomails werden durch
;den UploadBonusPro-Server verschickt; Aber nur, wenn dieses batch als
;prometheus:batches/UBPinfomail_gh.bat vorhanden ist.
;--------------------------------------------------------------------


;defaulteinstellung:
EBLcopy {textdatei} {empfaenger}
;Standartaufruf, eblcopy wird als Absender Sysop eintragen

;Aufrufbeispiel:
;EBLcopy {textdatei} {empfaenger} POSTMASTER {brettverwalter}
;eblcopy wird als Absender Postmaster uebergeben, sowie
;als ANTWORT-AN einen von UploadBonusPro ermittelten
;Brettverwalter des Brettes, aus dem sein Upload gezogen wurde.

;weiteres Beispiel:
;EBLcopy {textdatei} /BRETT/NAME {empfaenger} /ZERBMUELL
;eblcopy wird als Ziel ein Brett uebergeben, sowie der Empfaenger
;als Absender.  Wenn man ein Brett fuer Verwalter/Sysop einrichtet,
;in dem man die Gutschriften nachlesen moechte, dann kann man das
;ueber einen solchen Aufruf leicht realisieren.
;Das Brett /zerbmuell wird als ANTWORT-AN uebergeben, sodass ein
;unbedachter reply den Uploader, der ja als "Absender" der infomail
;eingetragen wird , nicht wundern muss.

;Das eblcopy ist freeware copyright bei EBL@STARDATE.westfalen.de und
;kann in jeder guten Prometheus-Box, sowie direkt in der Stardate
;downgeloadet werden.
;Stardate: 0251 263561 / 262560 / 263506
;--------------------------------------------------------------------



;Beispielaufruf fuer den maildaemon von Karma Design

;Maildaemon {textdatei} {empfaenger}

;Der maildaemon ist shareware copyright bei Karlchen@karma.rhein-ruhr.de
;und kann in der Karma Design bezogen werden.
;Karma Design: 0208 670722 / 677334
;--------------------------------------------------------------------


;Beispielaufruf fuer unbekannte mailtools bitte selbst editieren;
;es ist nur wichtig, dass der fuer das mailtool gueltige Aufrufsyntax
;eingehalten wird.

;mailtool {empfaenger} {textdatei} SYSOP COSYS
;Beispiel mit SYSOP als Absenderangabe und COSYS als ANTWORT-AN
;--------------------------------------------------------------------

;weiterer Hinweis: es sind untereinander natuerlich auch mehrere
;Aufrufe moeglich;  die uebergebene Textdatei wird erst geloescht, wenn
;das batch beendet ist.

;Kommentarzeilen koennen nach Installation geloescht werden!
