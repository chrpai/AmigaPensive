/**************************************************************************
 * CNet Amiga Programs and examples © 1988-1996 ZenMetal Software         *
 * These examples may be freely distributed to any developer wishing      *
 * to write doors for CNet Amiga.  ZenMetal retains copyright on all code *
 * within these examples                                                  *
 *                                                                        *
 * DOSDOOR.C                                                              *
 * A simple DOS DOOR to illustrate how to get input/output from users     *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <clib/dos_protos.h>

void main(int argc, char *argv[])
{
	BPTR inputfh=NULL;
	BPTR outputfh=NULL;
	char OutLine[100];
	char Buffer[5];
	BOOL done=FALSE;

	inputfh=Input();		/* get DOS input/output filehandles */
	outputfh=Output();

	SetMode(inputfh, 1);	/* set stdio to RAW: mode */
	SetMode(outputfh, 1);

	Write(outputfh, "Enter text: ", 6);
	for( ; !done ; )
		{
		if(WaitForChar( inputfh, 5000000))	/* wait at least 5 seconds for input */
			{
			if(Read(inputfh, Buffer, 1))	/* WaitForChar returned non-zero - get one character from stdio */
				{
				Buffer[1] = 0;
				switch(Buffer[0])
					{
					case 27:	/* ESCape pressed - exit! */
						done=TRUE;
					default:	/* just general baloney to verify keys pressed */
						sprintf(OutLine, "\nUser Entered '%c'\n", Buffer[0]);
						Write(outputfh, OutLine, strlen(OutLine));
					}
				}
			}
		else
			{
			Write(outputfh, "\n**timeout\n", 11);
			}
		}

	SetMode(inputfh, 0);	/* restore state of stdio to CON: mode */
	SetMode(outputfh, 0);
	exit(0);
}
