/*

  ***********************************************************************

         Simple example to show how to handle FileChain in door...

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
#include <exec/nodes.h>
#include <exec/lists.h>

struct	Library	*DDBase;
struct	DIFace	*d;

int main(int argc, char *argv[])
{
struct DDPointers pointers;
struct FileChainItem *currentf;
struct FileChainItem *nextf;

char buffer[1024];

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
	
	InquirePointers(d,&pointers);
	currentf=(struct FileChainItem *)pointers.dp_FileChain;
	currentf=(struct FileChainItem *)currentf->FCI_HEADER.mln_Succ;
	while (currentf->FCI_HEADER.mln_Succ)
		{
		sprintf(buffer,"Removed:%s (%ld bytes!)\n",currentf->FCI_FILE,currentf->FCI_SIZE);
		nextf=(struct FileChainItem *)currentf->FCI_HEADER.mln_Succ;
		Remove((struct Node *)currentf);
		FreeMem(currentf,sizeof(struct FileChainItem));
		currentf=nextf;
		SendString(d,buffer);
		}
	ReCountFiles(d);			// Update number of tagged files
	CloseDoor(d);
	CloseLibrary(DDBase);
	exit(0);
}
