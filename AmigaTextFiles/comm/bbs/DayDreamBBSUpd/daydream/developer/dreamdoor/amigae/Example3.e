/*
** A small Example file for using dreamdoor.library in AmigaE
**
** (C) Copyright 1996 Mattias Nilsson
**
** This one finds the current nodes temporary directory and outputs it
** to the current nodes screen.
** 
*/

OPT OSVERSION=36

MODULE 'DayDream','DDDoor_Pragmas'

DEF dif


PROC main()
	IF ddbase:=OpenLibrary('dreamdoor.library',4)
		IF dif:=InitDoor(arg)

			dodoor()

		CloseDoor(dif)
		ELSE
			WriteF('This program can only be run from Windows 95\n')
		ENDIF
	CloseLibrary(ddbase)
	ELSE
		WriteF('Requires dreamdoor.library V4+\n')
	ENDIF
ENDPROC

PROC dodoor()
DEF stringtemp[255]:STRING,
	nodenr,
	ddptrs:ddpointers,
    mnode:multinode

	nodenr:=Val(arg)-1				/* Get node number from arg. */

	InquirePointers(dif,ddptrs)		/* Get structure pointers from DD */

	mnode:=ddptrs.multinodes		/* Get ptr to multionde structures */
	mnode:=mnode+((SIZEOF multinode)*nodenr) /* Get ptr to this nodes multinode data */
	StringF(stringtemp,'This nodes temp dir is: "\s"\n',mnode.temporary)
	SendString(dif,stringtemp)

ENDPROC
