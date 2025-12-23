
                         FAMEMsgHeader.FIM v1.6
                        ~~~~~~~~~~~~~~~~~~~~~~~~


FAMEMsgHeader.FIM coded by: David 'Strider/tRSi' Wettig

Design and idea by: sCANDIC/dCN.


FAMEMsgHeader.FIM is a FAME door to replace the standard mail header of
the message base in MainPart which apears on reading mails.

This door must be called from a SYSCMD called "MAILHEADER".

Source code included to show how to write your own mail header replacement
door and to show how to code FAME doors.

Note: Source needs compiled object code "FAMEDoorStartUp.o" !
      Please change the path to it in the included "SCOPTIONS" file.

      "FAMEDoorStartUp.o" and the sources for "FAMEDoorStartUp.o" are a
      part of the FAME release package and are including all needed door
      communication and startup procedures.

      Please refer FAME.guide where to find the developer files, mabye you
      must install them first if you havn't done this on FAME installation.

This package for FAMEMsgHeader.FIM including all files, sources, text files
and executables is a part of the whole FAME release package and for this
they are a subject of the global rules and disclaimers defined in the
original FAME.guide.


------------------------------------------------------------------------------


To get the highest performance load this door resident:


------------ Cut here and put into: FAME:Settings/FAMEStart.bat --------------

Resident FAME:Doors/FAME/FAMEMsgHeader/FAMEMsgHeader.FIM PURE

------------ Cut here and put into: FAME:Settings/FAMEStart.bat --------------

FAMEMsgHeader.FIM can be loaded resident!!!

That's not a hack you may think because of the PURE argument.
FAMEMsgHeader.FIM is pure! The PURE argument is only to load it force
resident, because of modem transfering the protection bits will be cleared.


------------------------------------------------------------------------------

FAMEMsgHeader.FIM v1.6 programmed by:

--------------------------------- ______  ____ :___  _________ -----
David 'Strider/tRSi' Wettig       \  __/_/    \|   \/    /  _/___
E-Mail: strider@trsi.de           |    _//     \   \/   /  _|   /
E-Mail: strider@deadline.snafu.de |____|________\__/   /|_______\tGø
URL: http://www.trsi.de/inno/strider/index.html   /____\ Developer
--------------------------------------------------------------------
<EOF>
