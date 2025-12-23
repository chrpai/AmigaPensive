;
;	$VER: AMMS Delete Batch 2.00 [Deutsch] / 12.02.1995
;
;
;



;
;	Fehlermeldung : Keine Zusatzbatches erlaubt !
;
T_NoBatch="Keine Zusatzbatches erlaubt !"
;
;	Fehlermeldung : Unbekannte Zusatzbatch
; batch = Vorderer Teil des Batchnamens
; maxbatches = Hoechste Batchnummer
;
T_UnknownBatch="Nur Namen von 'batch'1 bis 'batch''maxbatches' erlaubt !"
;
; 	Fehlermeldung : User existiert nicht
; p2 = Username
;
T_UnknownUser="User ""'p2'"" existiert nicht !"
;
;	Menu : Batch wirklich loeschen
; p1 = Batchname
;
T_Question="Batch ""'p1'"" wirklich loeschen ? N#LEFT#"
;
;	Menu : Taste fuer Nein
;
KEY_NO="N^M"
;
;	Menu : Taste fuer Ja
;
KEY_YES="JY"
