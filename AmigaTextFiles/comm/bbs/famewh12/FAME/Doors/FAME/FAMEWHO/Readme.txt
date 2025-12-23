
                                FAMEWHO.FIM v1.2
                               ~~~~~~~~~~~~~~~~~~


FAMEWHO.FIM is coded by: David 'Strider/tRSi' Wettig
                         e-mail: strider@deadline.snafu.de

Design and idea by: sCANDIC/dCN.


FAMEWHO.FIM is a FAME door to replace the standard WHO door.

This door can be called from a BBSCMD called "WHO" or CONF<x>CMD
called "WHO". <x> is the Conference number.

This version is now able to hide and real hide users from being displayed
in the WHO. Also "Force WHO" mode will be checked.

Update is not implemented yet and won't be implemented in the near feature.
If there is in the arguments the "U" for update FAMEWHO.FIM will start the
internal FAME WHO with the same arguments.
Later if the ToolPort exists i will code a WHO door which uses the
notification possibilities of the ToolPort - means updates will be depending
on events and not on time periods.

FAMEWHO.FIM also starts the internal WHO if the version check fails.
FAMEWHO.FIM needs at least MainPart v1.13 to run.

Source code included to show how to write your own WHO doors with the usage
of the FAMESemaphore, to show how to code FAME doors and how to use
the FAMESemaphore general.

Note: Source needs compiled object code "FAMEDoorStartUp.o" !
      Please change the path to it in the included "SCOPTIONS" file.

      "FAMEDoorStartUp.o" and the sources for "FAMEDoorStartUp.o" are a
      part of the FAME release package and are including all needed door
      communication and startup procedures.

      Please refer FAME.guide where to find the developer files, mabye you
      must install them first if you havn't done this on FAME installation.

This package for FAMEWHO.FIM including all files, sources, text files
and executables is a part of the whole FAME release package and for this
they are a subject of the global rules and disclaimers defined in the
original FAME.guide.


------------------------------------------------------------------------------


To reach the highest performance load this door resident:


------------ Cut here and put into: FAME:Settings/FAMEStart.bat --------------

Resident FAME:Doors/FAME/FAMEWHO/FAMEWHO.FIM PURE

------------ Cut here and put into: FAME:Settings/FAMEStart.bat --------------

FAMEWHO.FIM can be loaded resident!!!

That's not a hack you may think because of the PURE argument.
FAMEWHO.FIM is pure! The PURE argument is only to load it force
resident, because of modem transfering the protection bits will be cleared.


------------------------------------------------------------------------------

FAMEWHO.FIM v1.2 programmed by:

--------------------------------- ______  ____ :___  _________ -----
David 'Strider/tRSi' Wettig       \  __/_/    \|   \/    /  _/___
E-Mail: strider@trsi.de           |    _//     \   \/   /  _|   /
E-Mail: strider@deadline.snafu.de |____|________\__/   /|_______\tGø
URL: http://www.trsi.de/inno/strider/index.html   /____\ Developer
--------------------------------------------------------------------
<EOF>
