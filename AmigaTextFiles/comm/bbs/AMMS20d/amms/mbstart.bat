assign >nil: amms: exists
if warn
assign amms:     ""                 ; Hauptverzeichnis
assign mbtmp:    t:                 ; Tmp-Dir
assign mbcom:    mbcom              ; Assignment fuer Mailboxbefehele
assign mbres:    mbres              ; Assignment fuer residente Mailboxbefehle
assign mbdevs:   mbdevs             ; Assignment fuer Ports
assign mbbatch:  mbbatch            ; Assignment fuer Batchdateien
assign mbmsg:    mbmsg              ; Assignment fuer Nachrichten
assign mbnet:    mbnet              ; Assignment fuer Netzfiles
assign mbtext:   mbtext             ; Assignment fuer Texte
assign mbuser:   mbuser             ; Assignment fuer Userdaten
assign mbudir:   mbudir             ; Assignment fuer Userdirs
assign mbdat:    mbdat              ; Assignment fuer sonstige Daten
assign files:    mbfiles            ; Assignment fuer Files
assign mbeigene: mbeigene           ; Assignment fuer Baghi's Menu
assign doors:    doors              ; Fuer Onlinegames etc.
assign menu:     menu               ; Fuers Menu
assign vak:	 mbeigene:demon/vak ; Fuers VAK

assign libs:    amms:libs  add      ; AMMS-Libs auch aus diesem Verzeichnis
assign c:       amms:c     add      ; Befehle auch aus diesem Verzeichnis
assign fonts:   amms:fonts add      ; Fonts auch von hier
endif

run mb                              ; Hauptprogramm starten
wait 4

; Autostart aller Ports, die im Directory amms:startup stehen !

list >t:start1 amms:startup lformat "execute %s%s *`wait 1*`"
sort t:start1 t:start2
execute t:start2
delete t:start1 t:start2 QUIET
