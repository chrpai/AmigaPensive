#include <proto/dos.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "configs.h"
#include "configs_protos.h"

/* #define DEBUG 1 */

int GetCfgLine(FILE *fptr,char *str)
{
	char *strptr;
	unsigned char count;
	unsigned char len;
	
	do
	{
		if(!fgets(str,255,fptr))
			return 0;
		if(str[strlen(str)-1]=='\n')
			str[strlen(str)-1]=0;
		strptr=stpblk(str);
		len=strlen(strptr);
		for(count=0;count<len;count++)
		{
			if(strptr[count]==';')
			{
				strptr[count]=0;
				break;
			}
		}
		len=strlen(strptr);
		for(count=len-1;((count>=0)&&(isspace(strptr[count])));count--)
			strptr[count]=0;
	} while(!strlen(strptr));
	return 1;
}

VOID ConfigErr(char *buffer)
{
	Printf("Improper syntax in config file:\n\"%s\"\n", buffer);
	Printf("Skipping entry.\n");
}

int ReadCfg(char *cfgname, struct FL_Config *cfg)
{
	char buffer[256];
	char *temp, *temp2;
	FILE *fptr;

	/* Initialize some variables to default values */
	
	cfg->filebuffers = 64;
	cfg->wrapdesc    = 39;
	cfg->sortalpha   = TRUE;
	cfg->sortfwd     = TRUE;
	time(&(cfg->newtime));

  /* Parse the config file */

	if(fptr=fopen(cfgname,"r"))
	{
		Printf("Loading configfile: %s\n", cfgname);
		while(GetCfgLine(fptr,buffer))
		{
#ifdef DEBUG
Printf("DEBUG***: %s\n", buffer);
#endif
			if(!strnicmp(buffer,"NODENUMBER",10))
			{
				temp=stpblk(&buffer[10]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->nodenum, temp, 107);
				temp2 = temp;
				while ( *temp2 != '\0' )
				{
					if ( *temp2 == ':' ) *temp2 = '.';
					if ( *temp2 == '/' ) *temp2 = '.';
					temp2++;
				}
				strncpy(cfg->freqfile, temp, 107);
				
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->nodenum);
				Printf("DEBUG***: %s\n", cfg->freqfile);
#endif
				continue;
			}

			if(!strnicmp(buffer,"INSERTDIR",9))
			{
				temp=stpblk(&buffer[9]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->insdir, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->insdir);
#endif
				continue;
			}

			if(!strnicmp(buffer,"DEFDESC", 7))
			{
				temp=stpblk(&buffer[7]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->defdesc, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->defdesc);
#endif
				continue;
			}

			if(!strnicmp(buffer,"UPDATE",6))
			{
				cfg->update = TRUE;
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", (LONG)cfg->update);
#endif
				continue;
			}

			if(!strnicmp(buffer,"SKIPUNVAL",9))
			{
				cfg->skipunval = TRUE;
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", (LONG)cfg->skipunval);
#endif
				continue;
			}

			if(!strnicmp(buffer,"LONGDESC",8))
			{
				cfg->longdesc = TRUE;
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", (LONG)cfg->longdesc);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NOREGNAME",9))
			{
				cfg->noregname = TRUE;
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", (LONG)cfg->noregname);
#endif
				continue;
			}

			if(!strnicmp(buffer,"MARKUNVAL", 9))
			{
				temp=stpblk(&buffer[9]);
				if(strlen(temp) != 1)
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->markunval, temp, 1);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->markunval);
#endif
				continue;
			}

			if(!strnicmp(buffer,"SORT",4))
			{
				temp=stpblk(&buffer[4]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				if(strstr( temp, "ALPHA")) cfg->sortalpha = TRUE;
				else cfg->sortalpha = FALSE;
				if(strstr( temp, "REVERSE")) cfg->sortfwd = FALSE;
				else cfg->sortfwd = TRUE;
				if(strstr( temp, "SIZE")) cfg->sortsize = TRUE;
				else cfg->sortsize = FALSE;
				continue;
			}

			if(!strnicmp(buffer,"ABOUT", 5))
			{
				temp=stpblk(&buffer[5]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->about, temp, 38);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->about);
#endif
				continue;
			}

			if(!strnicmp(buffer,"FILEBUFFERS", 11))
			{
				temp=stpblk(&buffer[11]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				cfg->filebuffers = atoi( temp );
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", cfg->filebuffers );
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWDAYS", 7))
			{
				temp=stpblk(&buffer[7]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				cfg->newtime -= (60 * 60 * 24 * atoi( temp ));
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", cfg->newtime);
#endif
				continue;
			}

			if(!strnicmp(buffer,"WRAPDESC", 8))
			{
				temp=stpblk(&buffer[8]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				cfg->wrapdesc = atoi( temp );
#ifdef DEBUG
				Printf("DEBUG***: %ld\n", cfg->wrapdesc );
#endif
				continue;
			}

			if(!strnicmp(buffer,"TXTFILE", 7))
			{
				temp=stpblk(&buffer[7]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->alltxt, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->alltxt);
#endif
				continue;
			}

			if(!strnicmp(buffer,"ARCTXT", 6))
			{
				temp=stpblk(&buffer[6]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->arcalltxt, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->arcalltxt);
#endif
				continue;
			}

			if(!strnicmp(buffer,"TXTCOMMENT", 10))
			{
				temp=stpblk(&buffer[10]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->cmmalltxt, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->cmmalltxt);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWTXTFILE", 10))
			{
				temp=stpblk(&buffer[10]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->newtxt, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->newtxt);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWARCTXT", 9))
			{
				temp=stpblk(&buffer[9]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->arcnewtxt, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->arcnewtxt);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWTXTCOMMENT", 13))
			{
				temp=stpblk(&buffer[13]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->cmmnewtxt, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->cmmnewtxt);
#endif
				continue;
			}

			if(!strnicmp(buffer,"IBMFILE", 7))
			{
				temp=stpblk(&buffer[7]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->allibm, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->allibm);
#endif
				continue;
			}

			if(!strnicmp(buffer,"ARCIBM", 6))
			{
				temp=stpblk(&buffer[6]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->arcallibm, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->arcallibm);
#endif
				continue;
			}

			if(!strnicmp(buffer,"IBMCOMMENT", 10))
			{
				temp=stpblk(&buffer[10]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->cmmallibm, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->cmmallibm);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWIBMFILE", 10))
			{
				temp=stpblk(&buffer[10]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->newibm, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->newibm);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWARCIBM", 9))
			{
				temp=stpblk(&buffer[9]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->arcnewibm, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->arcnewibm);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWIBMCOMMENT", 13))
			{
				temp=stpblk(&buffer[13]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->cmmnewibm, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->cmmnewibm);
#endif
				continue;
			}

			if(!strnicmp(buffer,"GUIDEFILE", 9))
			{
				temp=stpblk(&buffer[9]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->allgde, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->allgde);
#endif
				continue;
			}

			if(!strnicmp(buffer,"ARCGUIDE", 8))
			{
				temp=stpblk(&buffer[8]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->arcallgde, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->arcallgde);
#endif
				continue;
			}

			if(!strnicmp(buffer,"GUIDECOMMENT", 12))
			{
				temp=stpblk(&buffer[12]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->cmmallgde, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->cmmallgde);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWGUIDEFILE", 12))
			{
				temp=stpblk(&buffer[12]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->newgde, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->newgde);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWARCGUIDE", 11))
			{
				temp=stpblk(&buffer[11]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->arcnewgde, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->arcnewgde);
#endif
				continue;
			}

			if(!strnicmp(buffer,"NEWGUIDECOMMENT", 15))
			{
				temp=stpblk(&buffer[15]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->cmmnewgde, temp, 107);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->cmmnewgde);
#endif
				continue;
			}

			if(!strnicmp(buffer,"AREAS", 5))
			{
				temp=stpblk(&buffer[5]);
				if(!strlen(temp))
				{
					ConfigErr(buffer);
					continue;
				}
				strncpy(cfg->areasstring, temp, 255);
#ifdef DEBUG
				Printf("DEBUG***: %s\n", cfg->areasstring);
#endif
				continue;
			}

		}
		fclose(fptr);
	}
	else
	{
		Printf("Error reading %s\n",cfgname);
		return(10);
	}
}
