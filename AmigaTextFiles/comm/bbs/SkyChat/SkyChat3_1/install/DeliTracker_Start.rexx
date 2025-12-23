/*
** Startet DeliTracker und spielt das Modul ab. (by M.Kaulartz)
**
** Geben Sie bei <pfad> Ihr DeliTracker-Verzechnis an.
**
** Die bei <config> angegebene Datei stellt eine optionale Konfigdatei dar,
** die Sie einladen können. Ansonsten schneiden Sie dies einfach weg.
*/

parse arg module
module=strip(module,b,'"')

pfad = "HD1:Tools/DeliTracker_II/"

if ~show(ports,"DELITRACKER") then do
  address command "run <>nil: "pfad"DeliTracker2 config=SkyTools:SkyChat/DeliTracker.cfg cx_popup=no"
  end

address "DELITRACKER"
quick yes
songend no
playmod module
volume 64
