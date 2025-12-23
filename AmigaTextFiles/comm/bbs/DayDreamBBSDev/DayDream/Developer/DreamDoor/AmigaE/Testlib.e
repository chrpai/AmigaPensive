/*****************************************************************************

           Demonstration how to use DreamDoor.library and Amiga-E!

                    (C) Copyright 199x Antti Häyrynen 

*****************************************************************************/

MODULE 'DayDream'
MODULE 'DDDoor_Pragmas'

DEF dif

PROC main()
  IF ddbase:=OpenLibrary('dreamdoor.library',4)

	IF dif:=InitDoor(arg)			/* Establish Link   */

			  SendString(dif,'Hello world!\n')	

    CloseDoor(dif)				/* Close Link w/ DD  */
	ELSE
	  WriteF('Requires Microsoft Windows V3.1+!\n')
	ENDIF
    CloseLibrary(ddbase)
  ELSE
    WriteF('Could not open dreamdoor.library V4+!\n')
  ENDIF
ENDPROC
