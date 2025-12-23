#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <libraries/wwbbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/wwbbs.h>

#include "forcelogoff_rev.h"

char *ver=VERSTAG;

struct Library *WorldWideBase;

void main(int argc,char **argv)
	{
		if(argc!=2)
			exit(RETURN_FAIL);
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			exit(RETURN_FAIL);
		{
			if(GetStatusTags(STTAG_Name,argv[1],STTAG_Exists,TRUE,TAG_END))
				{
					while(!NodeCommandTags(argv[1],NDCMD_Panic,TAG_END))
						;
				}
			else
				{
					struct EasyStruct es={sizeof(struct EasyStruct),0,"Force Log Off Request","Node `%s' is currently idle.","Okay"};
					EasyRequest(NULL,&es,NULL,argv[1]);
				}
		}
		CloseLibrary(WorldWideBase);
		exit(RETURN_OK);
	}
