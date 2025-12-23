

          -=- FAME-SuspendResume is for FAME BBS Systems only. -=-


Contends of FAMESuRe.lha:
-------------------------

  FAME-SuspendResume (dir)

    FAME (dir)
      Modules (dir)
        CheckForNode         - CLI tool to check if a specific Node exists.

    Rexx (dir)
      FAMEOFF.rexx           - Arexx script to suspend Node 0.
      FAMEON.rexx            - Arexx script to resume Node 0.

    FAME-TCP-Stack.bat       - Batch/script to start a TCP Stack (Miami).
    FAME-TERM0.bat           - Batch/script to start a terminal (TERM).
    ReadMe.txt               - The text you are currently reading.


Description:
------------

FAMESuRe.lha contains two batch files (scripts) which will allow you if you
run a FAME BBS to start a TCP-stack or a terminal program while a Node is
online and this without quiting the FAME Node by hand.

Both scripts are using the Arexx-scripts FAMEOFF.rexx and FAMEON.rexx.

Those Arexx-scripts will be needed to suspend a Node and to resume it later.

Both Arexx-scripts are written for Node 0 only, but if you need them for
other FAME Nodes you have to edit them and to change the Node number from
0 to the number you want to use.

Edit the part string: 'FAMERexxPort.0' to FAMERexxPort.<MyNewNumber>.

FAMEOFF.rexx and FAMEON.rexx have to be copied into 'REXX:'.

The both CLI scripts FAME-TCP-Stack.bat and FAME-TERM0.bat are calling
a CLI tool named 'CheckForNode' normally located in

                     'FAME:Modules/CheckForNode'

which is the default path for 'CheckForNode'.
Please copy this tool to the path above.

You may also have to change the names and locations for ther used TCP-stack
and terminal program in both batches.

FAME-TCP-Stack.bat is originally written to start Miami at the following
location: 'Work:Internet/MIAMI/Miami'.

FAME-TERM0.bat is originally written to start TERM at the following
location: 'TERM:TERM'.

Be sure that FAME-TCP-Stack.bat and FAME-TERM0.bat have the script bit set.


That's all.


Copyright notes:
----------------

 - term is copyrighted © 1990 - 1996 by Olaf Barthel.
   All Rights Reserved.

 - Miami is copyrighted © 1996 by Holger Kruse.
   All Rights Reserved.

 - FAME and all it's contents is copyrighted © 1993 - 1996 by David Wettig.
   All Rights Reserved.


Contact:
--------

               Feel free to call the main FAME support bbs:



       pUNISHMENT iNC. (tRSi/X/F-iNNOVATiON) GERMAN HQ (SYSOP SieGeL)

            Node 1. +49-30-694 8470      Node 2. +49-30-694 8570

                   << FAME support and develop BBS >>



               You can reach me in this board under Strider.



                                I am:

                        David "Strider" Wettig

                  If you want to contact me via e-mail:

                           strider@trsi.de



             You can also reach the following FAME developers
                via e-mail or in the pUNISHMENT iNC. BBS:



       Oliver "BloodRock" Lange        bloodrock@trsi.de
       Ulrich Arne "Beatnik" Jansen    beatnik@trsi.de
       Sascha "SieGeL" Pfalz           siegel@trsi.de

--------------------------------- ______  ____ :___  _________ -----
David 'Strider/tRSi' Wettig       \  __/_/    \|   \/    /  _/___
E-Mail: strider@trsi.de           |    _//     \   \/   /  _|   /
E-Mail: strider@deadline.snafu.de |____|________\__/   /|_______\tGø
URL: http://www.trsi.de/inno/strider/index.html   /____\ Developer
--------------------------------------------------------------------
<EOF>
