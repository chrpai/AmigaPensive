#include <pragma/noviasys_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <exec/memory.h>
#include <novia/cnet.h>

#define CNET_SUBBOARDS "sysdata:subboards3"
#define _TEXT "data/_Text"
#define _MESSAGE3 "data/_Message3"
#define _HEADERS3 "data/_Headers3"
#define _ITEMS3 "data/_Items3"

struct Library *NoviaSysBase = NULL;

void convert();
PortData *cport;

void ioprintf(const char *string, ...)
{
	char *buffer=AllocVec(10000,MEMF_ANY|MEMF_CLEAR);
	if (buffer)
	{
		vsprintf(buffer, string, unsigned int(&string + 1));
		Writeio(buffer,-1);
		FreeVec(buffer);
	}
}

void main( int argc, char **argv )
{
	if ((NoviaSysBase = OpenLibrary("noviasys.library",0)))
	{
		Task *mytask = (Task *)FindTask(NULL);
		StrToLong(argv[1],(LONG *)&mytask->tc_UserData);
		cport = (PortData *)mytask->tc_UserData;
		convert();
		CloseLibrary(NoviaSysBase);
	}
}

void convert()
{
	struct NewSubboardType *nst;
	char buffer[256];
	ULONG counter = 0;
	ULONG	end	  = 0;
	ULONG	subboards = 0;
	BPTR	mulder;
	char *path = AllocVec(25600,MEMF_ANY|MEMF_CLEAR);
	strcpy(path,"novia:sysdata/root");
	ioprintf("\n");

	if ((mulder = Open(CNET_SUBBOARDS,MODE_OLDFILE)))										// Open subboards3	 (cnet's subboard-table)
	{
		Seek(mulder,0,OFFSET_END);																	// get size of cnet's subboard table
		if ((end = Seek(mulder, 0 ,OFFSET_BEGINNING) / sizeof(NewSubboardType)))	// calculate number subboards
		{
			if ((nst = AllocVec(end * sizeof(NewSubboardType), MEMF_ANY)))				// allocate memory for subboad table
			{
				Read(mulder,nst,end*sizeof(NewSubboardType));								// read subboard table
				for (counter = 0; counter < end; counter++)
				{
					if (nst[counter].Parent == -1)
					{
//						if (nst[counter].Subdirectory)
						{
							ioprintf("Title: %s , Subboard %d\n",
								nst[counter].Title,
								nst[counter].Subdirectory);
							ndos_mkdir(nst[counter].Title,cport);
						}
						subboards++;
					}
				}
				for (counter = 0; counter < end; counter++)
				{
					ULONG counter2;
					for (counter2 = 0; counter2 < end; counter2++)
					{	
						if (nst[counter2].Parent == counter)
						{
//							if (nst[counter2].Subdirectory)
							{
								ioprintf("Title: %s , Subboard %d\n",
									nst[counter2].Title,
									nst[counter2].Subdirectory);
								ndos_mkdir(nst[counter2].Title,cport);
							}
							subboards++;
						}
					}
				}
				FreeVec(nst);
			}
		}
		Close(mulder);
		ioprintf(" subboard: %d end: %d\n",subboards,end);
	}
	else
	{
		ioprintf("Can't open: %s\n",CNET_SUBBOARDS);
		exit(20);
	}
}
