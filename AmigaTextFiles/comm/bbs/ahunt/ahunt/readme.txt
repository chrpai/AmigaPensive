                                   hunt
  Copyright (c) 1985 Conrad C. Huang, Gregory S. Couch, Kenneth C.R.C. Arnold
                         San Francisco, California

        Copyright (c) 1985 Regents of the University of California.
       All rights reserved.  The Berkeley software License Agreement
          specifies the terms and conditions for redistribution.

                              21 August 1986

       AHUNT - A hunt port for the amiga - 1997 by Markus Marquardt
                   EMail: adrock@h-raiser.tmb.sb.sub.de
                              Berlin, Germany

                                 June 1997


Introduction

    Ahunt is a port of the multiplayer unix game hunt. The original version
    uses a network environment for communication between the players. Ahunt
    doesn't support any networks. So Ahunt is useful for single amigas running
    a bbs program with several serial ports.

    As far as possible all strings were localized. The english language ist
    the default and built in Ahunt. The only additional catalog provided is
    at this time german ("deutsch").


Installation

    Ahunt can be used with every bbs software which provides a standart
    i/o-handler for external programs (afaik this is i.e. the DLG bbs
    software). The handler must support the RAW mode to run properly with
    ahunt.

    As emulation ANSI/VT100 with eight colors is used.

    To install ahunt just copy all files from the Ahunt directory to your
    prefered directory on your harddisk. There are no assigns necessary.

    There are two executable programs to use with Ahunt:

    ahuntd

        This is the server program. It has to be once, i.e. on startup of
        your bbs software. WARNING! ahuntd needs at least 20000 BYTES OF
        STACK! So if your run ahuntd from within a batch you should increase
        the stack wich the stack command from amigados.

        Example for starting ahuntd within a batch:

            stack 20000
            run ahuntd

    ahunt

        This executable has to be run from every player to start the game.
        As parameters it expects a portname and a playername. To try ahunt
        you can start ahunt even from an amigados shell (NOT with the run
        command!).

        Example for starting ahunt from a shell:

            ahunt shell sysop


Misc. Stuff

    This Software are FREEWARE. Only a small fee for media is permitted.
    Commercial use of this package is FORBIDDEN. The author accepts no
    liability for damage caused as a result of this software.

    Any questions, suggestions or bug reports are welcome on the email
    address above.
