KEY_MARKDIR="M"
KEY_UNMARKDIR="U"
KEY_ABORT="A"

MARKMENU="-
#setline 'STATLINE'##cl#   -
#vfarbe 3#'KEY_MARKDIR'#reset# : #vfarbe 1#mark all boards     -
#vfarbe 3#'KEY_UNMARKDIR'#reset# : #vfarbe 1#unmark all boards     -
#vfarbe 3#'KEY_ABORT'#reset# : #vfarbe 1#abort"

KEY_MARK="M"
KEY_QUIT="Q"
KEY_NEXT="N"
KEY_LAST="L"

HELPTEXT="-
#setline 'STATLINE'##cl#  -
#vfarbe 3#CR#reset# : #vfarbe 1#choose     -
#vfarbe 3#'KEY_MARK'#reset# : #vfarbe 1#mark/unmark     -
#vfarbe 3#'KEY_NEXT'/'KEY_LAST'#reset# : #vfarbe 1#next/last page     -
#vfarbe 3#'KEY_QUIT'#reset# : #vfarbe 1#quit"

PATHTEXT="-
#eoff##home##cls#-
#setline 'PATHLINE'##vfarbe 5#Path#reset# : #vfarbe 6#{cdpath}"

_eod
