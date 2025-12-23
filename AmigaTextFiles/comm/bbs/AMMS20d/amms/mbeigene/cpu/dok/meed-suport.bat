;	$VER: MEED-Suport.bat V1.11 für den MegaED V1.12 (28.06.95)
;	(c)'95 by André (CPU) Seidelt
;
_echo "#reset##cls##home#"
_echo "MEED-Suport.bat V1.11 by CPU@SETUP"
_echo "1 = Registrierung verschicken"
_echo "2 = Anregungen verschicken"
_echo "3 = Bugreport verschicken"
_menu/nopm "#invers#[123]:#reset#" -
 "1" REG -
 "2" VOR -
 "3" BUG

;--------------------------------------

REG:
AP_FILE="T:MEED.reg"
THEME="MEED Registration........"

_echo "Generiere Brief in 'AP_FILE' !!!"

AP_DATA="Hallo CPU !!!*n"
_gosub APPEND

AP_DATA="Ich ('{NAME}') bin der Sysop von der Box '{SYSNAME}' und moechte mich fuer"
_gosub APPEND
AP_DATA="Deinen MegaED registrieren lassen.*n"
_gosub APPEND

AP_DATA="Ich werde in den naechsten Tagen die Summe von 20,- DM auf dein Konto ueber-"
_gosub APPEND
AP_DATA="weisen.*n"
_gosub APPEND

AP_DATA="Damit ich mir den Editor dann in der SETUP abholen kann, richtest du mir"
_gosub APPEND
AP_DATA="bitte einen Account mit folgenden Daten ein:"
_gosub APPEND
AP_DATA="Name         : '{NAME}'"
_gosub APPEND
AP_DATA="Vorname      : '{FIRSTNAME}'"
_gosub APPEND
AP_DATA="Nachname     : '{LASTNAME}'"
_gosub APPEND
AP_DATA="Strasse      : '{STREET}'"
_gosub APPEND
AP_DATA="Stadt        : '{CITY}'"
_gosub APPEND
AP_DATA="Telefonnummer: '{TEL}'"
_gosub APPEND

AP_DATA="*nAls Passwort nimmst du bitte : HIER DAS PW EINSETZEN !!!"
_gosub APPEND

AP_DATA="*n*nGruss : '{NAME}' !!!"
_gosub APPEND

_disk edit 'AP_FILE'

_menu/nopm "Brief abschicken ? (J/N):J#left#" -
 "^M" SEND -
 "J"  SEND -
 "N"  EXIT

;--------------------------------------
VOR:
AP_FILE="T:MEED.vor"
THEME="MEED Vorschlag........"

_echo "Generiere Brief in 'AP_FILE' !!!"

AP_DATA="Hallo CPU !!!*n"
_gosub APPEND

AP_DATA="Ich habe da einen Vorschlag, was du noch in deinen MegaED einbauen kannst:*n*n*n"
_gosub APPEND

AP_DATA="Gruss : '{NAME}' !!!"
_gosub APPEND

_disk edit 'AP_FILE'

_menu/nopm "Brief abschicken ? (J/N):J#left#" -
 "^M" SEND -
 "J"  SEND -
 "N"  EXIT

;--------------------------------------
BUG:
AP_FILE="T:MEED.bug"
THEME="MEED Bugreport........"

_echo "Generiere Brief in 'AP_FILE' !!!"

AP_DATA="Hallo CPU !!!*n"
_gosub APPEND

AP_DATA="*n------>>>>>>>>>>>>>>>> Zusatzfragen weiter unten im Text <<<<<<<<<<<<<<<<------*n"
_gosub APPEND

AP_DATA="Ich ('{NAME}'@'{SYSNAME}') habe einen Bug in deinem MegaED gefunden,"
_gosub APPEND
AP_DATA="hier meine Konfiguration:"
_gosub APPEND

_menu/nopm "1 = AMIGA  500^J2 = AMIGA  600^J3 = AMIGA  1000^J4 = AMIGA  1200^J5 = AMIGA  2000^J6 = AMIGA  3000(T)^J7 = AMIGA  4000(T)^JBitte Computertyp angeben:" -
 "1"  500 -
 "2"  600 -
 "3" 1000 -
 "4" 1200 -
 "5" 2000 -
 "6" 3000 -
 "7" 4000

500:
INS_DATA="AMIGA 500"
_goto AMIGA

600:
INS_DATA="AMIGA 600"
_goto AMIGA

1000:
INS_DATA="AMIGA 1000"
_goto AMIGA

1200:
INS_DATA="AMIGA 1200"
_goto AMIGA

2000:
INS_DATA="AMIGA 2000"
_goto AMIGA

3000:
INS_DATA="AMIGA 3000"
_goto AMIGA

4000:
INS_DATA="AMIGA 4000"

AMIGA:
AP_DATA="*n#invers#Computer:#reset# 'INS_DATA'"
_gosub APPEND

_echo "H€nge Prozessortyp an........"
AP_DATA="*n#invers#Prozessor:#reset#"
_gosub APPEND
_extern CPU >>'AP_FILE'

_echo "H€nge Speicherinfo an........"
AP_DATA="*n#invers#Speicher:#reset#"
_gosub APPEND
_extern AVAIL >>'AP_FILE' FLUSH

_echo "H€nge Harddiskinfos an........"
_extern echo >>'AP_FILE' NOLINE "*n#invers#Laufwerke:#reset#"
_extern INFO >>'AP_FILE'

_echo "H€nge Kickstartversion an........"
AP_DATA="*n#invers#Versionen:#reset#"
_gosub APPEND
_extern VERSION >>'AP_FILE' FULL

_echo "H€nge MEED-Version an........"
AP_DATA="*n#invers#MEED-Version:#reset#"
_gosub APPEND
_extern VERSION >>'AP_FILE' MBCom:MEED FULL

_input/num "INS_DATA" "Wieviele Nodes hat deine Box ?(inclusive Konsolen, etc.):"
_echo "H€nge Nodezahl an........"
AP_DATA="*n#invers#Nodeanzahl:#reset# 'INS_DATA'"
_gosub APPEND

_input/num "INS_DATA" "Auf wievielen Nodes lief der MegaED gleichzeitig ?:"
_echo "H€nge Prozesszahl an........"
AP_DATA="*n#invers#Anzahl MEEDs:#reset# 'INS_DATA'"
_gosub APPEND

_menu/nopm "War der MegaED resident geladen ? (J/N):J#left#" -
 "^M" RES_JA -
 "J" RES_JA -
 "N" RES_NE

RES_JA:
INS_DATA="JA"
_goto RESINF

RES_NE:
INS_DATA="NEIN"

RESINF:
_echo "H€nge Residentinfo an........"
AP_DATA="*n#invers#MEED resident:#reset# 'INS_DATA'"
_gosub APPEND

_menu/nopm "Trat der Fehler beim internen (MegaED) oder externen (MEED) Editor auf (I/E):" -
 "E" INT_JA -
 "I" INT_NE

INT_JA:
INS_DATA="INTERN"
_goto RESINF

INT_NE:
INS_DATA="EXTERN"

INTINF:
_echo "H€nge Editorinfo an........"
AP_DATA="*n#invers#Editorart:#reset# 'INS_DATA'"
_gosub APPEND

_echo "H€nge Zusatzfragen an........"
AP_DATA="*n-----------========>> Diese Fragen bitte noch beantworten <<========-----------*n"
_gosub APPEND
AP_DATA="*n#invers#Wenn der Fehler durch den internen Editor erzeugt wurde, bei was ?#reset#"
_gosub APPEND
AP_DATA="*n#invers#(Filesystem, Messagesystem, etc)?:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Wie wurde MEED gestartet (mit/ohne Dateinamen, Optionen, etc.)?:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Wie gross war die zu ladende Textdatei (Byte und Zeilenzahl)?:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Hast du ein Enforces-Protokoll (wenn ja, bitte mitschicken)?:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Brachte der MegaED das System zum Absturz ?:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Wenn ja->welche Gurunummer wurde nach dem Absturz angezeigt ?:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Beschreibe bitte den Fehler des MegaED:#reset#*n"
_gosub APPEND
AP_DATA="*n#invers#Welche Funktionen wurden vorher benutzt ?:*n"
_gosub APPEND
AP_DATA="*n#invers#Kannst Du den Fehler reproduzieren oder tritt er nur manchmal auf?:#reset#*n"
_gosub APPEND
AP_DATA="*n------------------============>> !!! DANKE !!! <<============------------------*n"
_gosub APPEND

AP_DATA="*n*nGruss '{NAME}' !!!"
_gosub APPEND

_echo "^J^J#vfarbe 1#Bitte die Fragen unten im Text auch noch beantworten !!!#reset#^J#invers#[RETURN]#reset#"
_waitkey

_disk edit 'AP_FILE'

_menu/nopm "Brief abschicken ? (J/N):J#left#" -
 "^M" SEND -
 "J"  SEND -
 "N"  EXIT

;--------------------------------------
SEND:
_echo "Verschicke Brief an CPU@SETUP unter dem Thema 'THEME'"
_brief/Nocopy/file='AP_FILE' "CPU@SETUP" "#'THEME'"
EXIT:
_echo "Lsche TMP-File........"
_extern delete 'AP_FILE'
_echo "Bye dann........"
_eod

;--------------------------------------

APPEND:
_extern echo >>'AP_FILE' "'AP_DATA'"
_return

