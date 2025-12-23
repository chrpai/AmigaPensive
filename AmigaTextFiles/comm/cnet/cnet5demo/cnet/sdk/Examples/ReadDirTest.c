// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

#include "cnet:sdk/include/version.h"

struct Library        *CNetBase=NULL;
struct CNetFileEntry  *filelist;


void DoDisplay(struct CNetFileEntry *list, UBYTE depth)
{
	char buff[20];
	struct CNetFileEntry *fptr=list;
	UBYTE i;

	depth++;
	while(fptr=NextDirEntry(HeaderEntry(list), fptr))
		{
		if(fptr->size)
			sprintf(buff, "(%ld bytes)", fptr->size);

		for(i=0;i<depth;i++)
			Printf("\t");

		Printf("%3ld. %s%s %s\n", EntryOrdinal(fptr, HeaderEntry(list)), fptr->ftype ? "[DIR]":"", fptr->filename, fptr->size ? buff:"");
		if(fptr->ftype)
			{
			DoDisplay(fptr->child, depth);
			}
		}
}

void main( int argc, char *argv[])
{
	if(argc < 2)
		{
		Printf("\nSyntax: %s <path> [TRUE:FALSE]\n\n", argv[0]);
		exit(RETURN_WARN);
		}

	if(CNetBase=OpenLibrary(CNETLIBNAME, CNETLIBVERSION))
		{
		if(filelist=CNetReadDir(argv[1], argc > 2)) // presence of ANY 2nd argument signifies recurse..
			{
			struct CNetFileEntry *fptr;
			if(fptr=HeaderEntry(filelist)) // the header entry contains info
			                               // about the directory of files
                                        // attached to the linked list
				{
				Printf("Directory of: %s\n", fptr->filename);
				DoDisplay(fptr, 0);
				}
			CNetDisposeDir(&filelist);
			}
		CloseLibrary(CNetBase);
		}
	exit(RETURN_OK);
}
