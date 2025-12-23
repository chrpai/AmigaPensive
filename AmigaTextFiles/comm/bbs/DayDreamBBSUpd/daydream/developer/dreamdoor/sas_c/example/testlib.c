/*

  ***********************************************************************

     Simple example file to show how to use the DDDoor.library from C

  ***********************************************************************

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <proto/dddoor.h>
#include <libraries/dddoor.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <daydream.h>
#include <exec/memory.h>

struct	Library	*DDBase;
struct	DIFace	*d;

int main(int argc, char *argv[])
{
	DDBase=(struct Library *)OpenLibrary("dreamdoor.library",1);
	if(DDBase==NULL)
	{
		printf("Needs DreamDoor.library V1.00+ to run\n");
		exit(10);
	}
	d=InitDoor(argv[1]);		/* Establish link with DD */

	if(d==NULL)
	{
		printf("This Program Requires Microsoft Windows V3.1!\n");
		CloseLibrary(DDBase);
		exit(10);
	}
	
	SendString(d,"Hello World!\n");
	
	CloseDoor(d);
	CloseLibrary(DDBase);
	exit(0);
}
