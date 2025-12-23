KEY_MARKDIR="M"
KEY_UNMARKDIR="U"
KEY_ABORT="A"

MARKMENU="-
#setline 'STATLINE'##cl##vfarbe 1#Alle (File)Bretter im Dir -> -
#vfarbe 3#'KEY_MARKDIR'#reset# : #vfarbe 1#Markieren - -
#vfarbe 3#'KEY_UNMARKDIR'#reset# : #vfarbe 1#EntMarkieren - -
#vfarbe 3#'KEY_ABORT'#reset# : #vfarbe 1#Abbruch ? "

KEY_MARK="M"
KEY_QUIT="Q"
KEY_NEXT="N"
KEY_LAST="L"

HELPTEXT="-
#setline 'STATLINE'##cl#  -
#vfarbe 3#CR#reset# : #vfarbe 1#Waehlen     -
#vfarbe 3#'KEY_MARK'#reset# : #vfarbe 1#(Ent)Markieren     -
#vfarbe 3#'KEY_NEXT'/'KEY_LAST'#reset# : #vfarbe 1#Seite vor/zurueck     -
#vfarbe 3#'KEY_QUIT'#reset# : #vfarbe 1#Ende"

PATHTEXT="-
#eoff##home##cls#-
#setline 'PATHLINE'##vfarbe 5#Pfad#reset# : #vfarbe 6#{cdpath}"

_eod
