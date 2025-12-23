.key zdatum/A,zzeit/A,zeilen/A,gutschriften/A,byte/A
.bra {
.ket }
;
;UploadBonusPro-sysopmail-BATCH
;
;Text erstellen:
echo  >ram:UBPsysopmail " "
echo >>ram:UBPsysopmail "Aktuelles Datum und Uhrzeit:{zdatum} {zzeit}"
echo >>ram:UBPsysopmail " "
echo >>ram:UBPsysopmail "Es wurden {zeilen} Protokolleintraege verarbeitet."
echo >>ram:UBPsysopmail "Es wurden {gutschriften} Gutschriften erfolgreich zugewiesen,"
echo >>ram:UBPsysopmail "insgesamt {byte} Byte gutgeschrieben."
echo >>ram:UBPsysopmail " "
echo >>ram:UBPsysopmail "UploadBonusPro-Server wurde ordnungsgemaess beendet."
echo >>ram:UBPsysopmail " "

;Text ueber eblcopy mit Absender System an Sysop senden
EBLcopy ram:UBPsysopmail sysop system

;weitere Moeglichkeit:
;Text ueber eblcopy mit Absender System und ANTWORT-AN Sysop an ein oeffentliches Brett senden
EBLcopy ram:UBPsysopmail /!SYSOPMELDUNGEN system sysop

;Text nach Versenden wieder loeschen:
delete ram:ubpsysopmail quiet
