/*
**
** A small Example file for using dreamdoor.library in AmigaE
**
** (C) Copyright 1996 Mattias Nilsson
**
** This example outputs the current users handle and location to the
** current nodes screen.
*/

OPT OSVERSION=36

MODULE 'DayDream','DDDoor_Pragmas'

DEF dif

PROC main()
	IF ddbase:=OpenLibrary('dreamdoor.library',4)
		IF dif:=InitDoor(arg)		/* Establish link woth DayDream node */

			dodoor()				/* Do the door */

		CloseDoor(dif)				/* Close DayDream node link */
		ELSE
			WriteF('This program can only be run from Windows 95\n')
		ENDIF
	CloseLibrary(ddbase)
	ELSE
		WriteF('Requires dreamdoor.library V4+\n')
	ENDIF
ENDPROC

PROC dodoor()
DEF ddptrs:ddpointers,
	usr:user,
	tstr[255]:STRING

	InquirePointers(dif,ddptrs)		/* Get pointers to DayDream's structures */
	usr:=ddptrs.curruser

	StringF(tstr,'\s / \s started this door.\n',usr.handle,usr.zipcity)
	SendString(dif,tstr)
ENDPROC
