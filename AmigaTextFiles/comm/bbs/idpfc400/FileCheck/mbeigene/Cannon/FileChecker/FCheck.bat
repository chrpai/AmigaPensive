;
; FileCheck - AMMS-Batch V1.00  by cANNoN/iDP!
;
; $VER: 01.00 / 05.05.1998
;


    path = "mbeigene:Cannon/FileChecker/Languages/"
 pattern = "FileCheck.#?"
  output = "'{userdir}'/FileCheck.cfg"
 prelang = "FileCheck"
 preansi = "Ansi"


_if o1 == "T" then _goto KILL
_if o2 == "T" then _goto LIST
_if o3 == "T" then _goto SET
_if o4 == "T" then _goto SHOW
_eod


KILL:
_extern/quiet Delete "'output'"
_echo "Voreingestellter System-Style wurde wiederhergestellt."
_eod

LIST:
_echo "Verfuegbare Styles:"
_echo "-------------------------------------------------------------------------------"
_extern List "'path'" PAT="'pattern'" LFORMAT="  %e"
_echo "-------------------------------------------------------------------------------"
_eod

SET:
_parse "FILECHECK:" P0 N/Style language
_check file "'path''prelang'.'language'" ERROR_LANGUAGE
_fd open file "'output'" W
_fd write file "'language'"
_fd close file
_echo "Der Style wurde auf ""'language'"" geaendert."
_eod

SHOW:
_parse "FILECHECK:" P0 N/Style language
_check file "'path''prelang'.'language'" ERROR_LANGUAGE
_check file "'path''preansi'.'language'" ERROR_ANSI
_disk text/nomore "'path''preansi'.'language'"
_eod


ERROR_LANGUAGE:
_echo "Der Style ""'language'"" existiert nicht!"
_eod

ERROR_ANSI:
_echo "Zum Style ""'language'"" existiert keine Uebersicht!"
_eod
