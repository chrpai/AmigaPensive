#include <exec/types.h>
#include <exec/memory.h>
#include <exec/semaphores.h>
#include <exec/nodes.h>
#include <exec/lists.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <dos/dos.h>
#include <dos/rdargs.h>
#include <dos/dosextens.h>
#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <proto/pipeline.h>
#include <proto/pipeutil.h>
#include <nodes.h>

#include "/configs.h"
#include "/configs_protos.h"

/* #define DEBUG 1 */

#define	NUMARGS		1

struct	RDArgs	*Args;
long	 	 args[NUMARGS];

struct FL_Config 	*cfg;

static UBYTE   *VersTag  = "\0$VER: Lightning Filelister Key Generator 1.0 "__AMIGADATE__;
UBYTE          *template = "CONFIGFILE";

VOID GenKey( struct KeyFile *key );
void Usage ( void );

void Usage ( void )
{
	Printf ( "Lightning FileLister v1.0. Copyright (c)1996 Rod Schnell.\n\n" ) ;
	Printf ( "Usage: Lightning [CONFIGFILE]\n\n", NULL ) ;
	Printf ( "       CONFIGFILE - optional argument, the full path and name to config file.\n" ) ;
	Printf ( "                    Default is zeus:config/file/lightning.cfg\n" ) ;
}

void
main ( int argc, char **argv )
{
	int		i, serialnum;
  char  ProgPath[108];
	char 	configpath[108];
	struct KeyFile *key;
	FILE *out;
	BPTR 	lock;

	for ( i = 0; i < NUMARGS; i ++ )
		args[i] = NULL;

	if ( Args = ReadArgs ( template, args, NULL ) )
	{
		if(args[0]) strncpy(configpath, (UBYTE *)args[0], 108);
		else (strncpy(configpath, "Zeus:config/File/Lightning.cfg", 108));

		if( key = (struct KeyFile *) AllocMem ( sizeof ( struct KeyFile ), MEMF_CLEAR ) )
		{
			GenKey( key );

			lock=GetProgramDir(); /*** DO NOT UNLOCK THIS LOCK ***/
			NameFromLock(lock, ProgPath,108);
			AddPart(ProgPath, "Lightning.key", 108);

  		if(out=fopen(ProgPath,"wb"))
  		{
				fwrite(key, sizeof( struct KeyFile ), 1, out);
				fclose(out);
  		}
			FreeMem ( key, sizeof ( struct KeyFile ) ) ;
			printf("\nAll Done!\n", NULL);
		}
		FreeArgs ( Args );
	}
}


VOID GenKey( struct KeyFile *key )
{
/* Key ULONGS (where keyfile info is hidden) are key->a[19], key->a[97] */
/* and key->a[132]                                                          */

	char Buffer[1024], *c;
	ULONG i, t, k, j;

	printf("Enter Junk String: ", NULL);
	gets(Buffer);
	j = strlen(Buffer);
	for(i=0, t=0, c=&Buffer[0];i < 240;i++)
	{
		key->a[i] = ((ULONG) *c) + (i + 100) * 3000;
		t++;
		c++;
		if(t == j)
		{
			c = &Buffer[0];
			t = 0;
		}
	}

	printf("\n Enter Sysop Name [39 chars max]: ", NULL);
	gets(Buffer);
	strcpy(key->Sysop, Buffer);
	j = strlen(Buffer);
	for(c = &Buffer[0], i = 0, k = 0;i < j;c++, i++)
		k += ((ULONG) *c) + i*2 + 100;
	key->a[19] = k;

	printf("         Enter BBS Serial Number: ", NULL);
	gets(Buffer);
	key->a[97] = atoi(Buffer);

	printf("   Enter Lightning Serial Number: ", NULL);
	gets(Buffer);
	key->a[132] = atoi(Buffer);

}


