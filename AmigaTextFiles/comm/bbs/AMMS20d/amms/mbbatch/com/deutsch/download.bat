;
;	Befehl : Download V1.02
;
;	Sprache: deutsch
;
;	Bekannte Variablen :
;	--------------------
;	TIMEOUT : Timeoutzeit in Sekunden fuer Autologout
;

;
;	Auswahl : Autologout/Download/Abbruch
;
T1="^J#fett#D#reset#ownload / Download+#fett#L#reset#ogout / #fett#A#reset#bbruch (D/L/A) ? D#left#"
;
;	Meldung : Autologout in x Sekunden
;
T2="^JLogout in 'TIMEOUT' Sekunden (A=Abbruch) ...^J"
;
;	Menu : Taste fuer Download
;
DOWN="D^M"
;
;	Menu : Taste fuer Autologout
;
AUTO="L"
;
;	Menu : Taste fuer Abbruch
;
ABORT="A"
