;
;	Command : Download V1.02
;
;	Language : english
;
;	Known variables :
;	-----------------
;	TIMEOUT : timeout of auto logout (sec.)
;

;
;	Choose : Autologout/Download/Abort
;
T1="^J#fett#D#reset#ownload / Download+#fett#L#reset#ogout / #fett#A#reset#bort (D/L/A) ? D#left#"
;
;	Choose : Autologout
;
T2="^JLogout in 'TIMEOUT' seconds, press A for abort ...^J"
;
;	Menu : Key for Download
;
DOWN="D^M"
;
;	Menu : Key for Autologout
;
AUTO="L"
;
;	Menu : Key for Abort
;
ABORT="A"
