#include <exec/types.h>
#include <exec/exec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_NAME ((STRPTR) argarray[0])
#define ARG_STRING ((STRPTR) argarray[1])

void main()
	{
		BYTE *template="NAME/A,STRING/F";
		LONG argarray[]={NULL,NULL};
		struct RDArgs *rdargs;
		if(rdargs=ReadArgs(template,argarray,NULL))
			{
				if(ARG_STRING)
					SetVar(ARG_NAME,ARG_STRING,-1,GVF_LOCAL_ONLY);
				else
					DeleteVar(ARG_NAME,GVF_LOCAL_ONLY);
				FreeArgs(rdargs);
			}
		exit(0);
	}
