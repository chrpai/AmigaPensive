
;UploadBonusPro IST (C)1996 FRANK HARTUNG
;
;die folgenden batchzeilen loeschen alle durch UploadBonusPro evtl.
;angelegten dateien aus der empfindlichen prometheus-struktur - NICHT MEHR !


echo "Loesche von Programm UploadBonusPro angelegte Dateien:"
echo " "
echo "ACHTUNG: Nur ausfuehren wenn Du UploadBonusPro nicht mehr benutzen"
echo "         moechtest."
echo " "
echo "         Du hast 10 Sekunden Zeit um mit Ctrl+D auszubrechen !"
echo " "
wait 10

echo " |-> Loesche Tempdatei..."
echo " |    " noline
c:delete prometheus:protokolle/download.prt_temp

echo " |-> Loesche gemerkte Protokollzeilen..."
echo " |    " noline
c:delete prometheus:protokolle/download.prt_2nd

echo " |-> Loesche alte Zeitpunktdatei..."
echo " |    " noline
c:delete prometheus:daten/uploadverguetung_gh.dat

echo " |-> Loesche Zeitpunktdatei..."
echo " |    " noline
c:delete prometheus:daten/UploadBonusPro_gh.dat

echo " |-> Loesche alte Startzeilendatei..."
echo " |    " noline
c:delete prometheus:daten/uploadverguetung_gh2.dat

echo " |-> Loesche Startzeilendatei..."
echo " |    " noline
c:delete prometheus:daten/UploadBonusPro_gh2.dat

echo " |-> Loesche alte Uploadverguetungs-Protokolldatei..."
echo " |    " noline
c:delete prometheus:protokolle/uploadverguetung_gh.prt

echo " |-> Loesche Uploadverguetungs-Protokolldatei..."
echo " |    " noline
c:delete prometheus:protokolle/UploadBonusPro_gh.prt

echo " |-> Loesche Freiraumaenderungs-Protokolldatei..."
echo " |    " noline
c:delete prometheus:protokolle/UBPclient-Freiraum_gh.prt

echo " |-> Loesche altes Gutschriftenprotokoll in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°gutproto_gh.prt

echo " |-> Loesche Gutschriftenprotokoll in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°UBPproto_gh.prt

echo " |-> Loesche altes Gutschriftenprotokoll-Backup in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°gutproto_gh.prt_bak

echo " |-> Loesche Gutschriftenprotokoll-Backup in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°UBPproto_gh.prt_bak

echo " |-> Loesche alte Gutschriftbytedatei in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°gutbyte_gh.dat

echo " |-> Loesche Gutschriftbytedatei in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°UBPbyte_gh.dat

echo " |-> Loesche Gutschriftbytebackupdatei in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°UBPbyte_gh.bak

echo " |-> Loesche alten Gutschriftdupecheck(FILE) in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°gutdupeFILE_gh.dat

echo " |-> Loesche Gutschriftdupecheck(FILE) in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°UBPdupeFILE_gh.dat

echo " |-> Loesche alten Gutschriftdupecheck(MSGID) in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°gutdupeMSGID_gh.dat

echo " |-> Loesche Gutschriftdupecheck(MSGID) in Postfaecher..."
echo " |    " noline
c:delete prometheus:user/#?/°UBPdupeMSGID_gh.dat

echo " "
echo "Alle durch UploadBonusPro angelegten Datendateien wurden geloescht."
echo "Das UploadBonusPro-Server- sowie Client-Programm musst Du bitte von"
echo "Hand loeschen."
echo "Denke ebenfalls daran den UploadBonusPro-Server-Aufruf aus Deinem"
echo "Prometheus-Zeitplan zu entfernen sowie den UploadBonusPro-Client"
echo "als Befehl in Prometheus sowie ggf. aus der Login.bat zu entfernen."
echo " "
echo "Programm beendet."
