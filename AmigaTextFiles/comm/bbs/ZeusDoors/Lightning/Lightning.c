/* Lightning FileLister (c)1996 by Rod Schnell
 *
 */

/* #define DEBUG 1 */

#include "headers.h"

#define	NUMARGS		1

struct	RDArgs	*Args;
long	 	 args[NUMARGS];

struct Library			*PipelineBase;
struct Library			*PipeUtilBase;
struct SharedData		*Shared;
struct FL_Config 		*cfg;
struct FL_AreaNode 	*FLnode;

static UBYTE   *VersTag = "\0$VER: Lightning Filelister 1.2 "__AMIGADATE__;
char  dashes[]          = "-------------------------------------------------------------------------------";
char  spaces[]          = "                                        ";
/****
int __saveds MatchFunc ( ULONG Number, ULONG SecondArg )
{
	if(Number == SecondArg)
	{
Printf("%lu %lu Match!\n", Number, SecondArg);
		FLnode->Use = TRUE;
		
		/* Returning 0 tells ParseNum() to quit */
		
		return(0);
	}

	return ( 1 ) ;
}
****/

void Usage ( void )
{
	Printf ( "Lightning FileLister v1.0. Copyright (c)1996 Rod Schnell.\n\n" ) ;
	Printf ( "Usage: Lightning [CONFIGFILE]\n\n", NULL ) ;
	Printf ( "       CONFIGFILE - optional argument, the full path and name to config file.\n" ) ;
	Printf ( "                    Default is zeus:Config/Lightning/Lightning.cfg\n" ) ;
}

void
Skip ( UBYTE *tx )
{
	while ( *tx != '\0' )
	{
		if ( *tx == '\n' || *tx == '\r' )
		{
			*tx = ' ';
		}
		tx++;
	}
}

/* If single backslash "\" (amigaguide esc char) output two backslashes "\\" */
void
AGPrintLine ( BPTR fh, UBYTE *tx )
{
	while ( *tx != '\0' )
	{
		FPutC(fh, *tx);
		if ( *tx == '\\' )
		{
			FPutC(fh, '\\');
		}
		tx++;
	}
	FPutC(fh, '\n');
}

VOID PrintReg ( BPTR fh, BOOL crlf, LONG serialnum )
{
int i;

	if( cfg->noregname )
	{
		for ( i = 0; i < ((80 - 29)/2); i ++ )
			FPutC(fh, ' ');
		FPrintf ( fh, "registration serial number %ld", serialnum ) ;
	}
	else
	{
		for ( i = 0; i < ((80 - (strlen(Shared->KeyInfo->Name) + 14))/2); i ++ )
			FPutC(fh, ' ');
		FPrintf ( fh, "registered to %s", Shared->KeyInfo->Name ) ;
	}
	if (crlf) FPrintf ( fh, "\n\r\n\r", NULL ) ;
	else FPrintf ( fh, "\n\n", NULL ) ;
}

struct	MyNode
{
	struct	Node		 node;
	struct	fullFile	 fl;
	ULONG			 desc;
} ;

signed int
__saveds
ForwardSort ( struct MyNode *one, struct MyNode *two )
{
	return ( (int) (one->fl.ff_file->FileSize - two->fl.ff_file->FileSize) ) ;
}

signed int
__saveds
BackwardSort ( struct MyNode *one, struct MyNode *two )
{
	return ( (int) (two->fl.ff_file->FileSize - one->fl.ff_file->FileSize) ) ;
}

/* Insert a file to the given filehandle, returns 1 for success 0 for failure */
/* Does not add any leading or trailing CR/LF                                */
LONG InsertFile(BPTR fh, char *text )
{
  char  *tbuff, tpath[108];
  ULONG len;
	
	strncpy(tpath, cfg->insdir, 107);
	if(AddPart(tpath, text, 108))
	{
		tbuff = ReadWholeFile ( tpath, &len ) ;
		if ( tbuff )
		{
    	FPrintf ( fh, "%s", tbuff ) ;
      FreeMem ( tbuff, len ) ;
    }
    else return(0);
    return(1);
  }
  else return(0);
}

long
CheckNewFiles ( struct	PF_Globals *globals, ULONG areanumber, time_t date_to_search )
{
	int			 i = 0;
	struct	NewMapEntry	*ent;

	if ( globals->NewMap && globals->NewMapEntries )
	{
 		ent = (struct NewMapEntry *) globals->NewMap;

		while ( i < globals->NewMapEntries )
		{
			if ( ent->nme_Area == areanumber )
			{
				if ( ent->nme_LastSave > date_to_search )
				{
					return ( 1 ) ;
					break;
				}
			}
			ent++;
			i++;
		}
	}
	return ( 0 ) ;
}

void
MyOutputln ( struct fullFile *fl, BOOL new )
{
	struct	tm	timer;
	char		Date[20], *tk;

	GMTime ( fl->ff_file->UploadDate, &timer ) ;
	strftime ( Date, 10, "%d%b%y", &timer ) ;

	Skip ( fl->ff_text ) ;
	LineWrap( fl->ff_text, cfg->wrapdesc ) ;

	if(cfg->fhtxt)
		FPrintf ( cfg->fhtxt, "%-20.20s %4ldK %7s %4ld ", fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
	if(cfg->fhibm)
		FPrintf ( cfg->fhibm, "%-20.20s %4ldK %7s %4ld ", fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
	if(cfg->fhgde)
	{
		if( cfg->freqfile[0] )
			FPrintf ( cfg->fhgde, "@{\"%-19.19s\" System \"Echo >>outbound:%s.REQ %s\"} %4ldK %7s %4ld ", fl->ff_file->FileName, cfg->freqfile, fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
		else
			FPrintf ( cfg->fhgde, "%-20.20s %4ldK %7s %4ld ", fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
	}
	if(	new && fl->ff_file->UploadDate > cfg->newtime)
	{
		if(	cfg->fhnewtxt )
			FPrintf ( cfg->fhnewtxt, "%-20.20s %4ldK %7s %4ld ", fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
		if(cfg->fhnewibm)
			FPrintf ( cfg->fhnewibm, "%-20.20s %4ldK %7s %4ld ", fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
		if(cfg->fhnewgde)
		{
			if( cfg->freqfile[0] )
				FPrintf ( cfg->fhnewgde, "@{\"%-19.19s\" System \"Echo >>outbound:%s.REQ %s\"} %4ldK %7s %4ld ", fl->ff_file->FileName, cfg->freqfile, fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
			else
				FPrintf ( cfg->fhnewgde, "%-20.20s %4ldK %7s %4ld ", fl->ff_file->FileName, (fl->ff_file->FileSize / 1000), Date, fl->ff_file->NumberOfDownloads) ;
		}
	}

	if(tk = strtok( fl->ff_text, "\n" ))
	{
		if(cfg->fhtxt)    FPrintf(cfg->fhtxt, "%s\n", tk );
		if(cfg->fhibm)    FPrintf(cfg->fhibm, "%s\n\r", tk );
		if(cfg->fhgde) 		AGPrintLine( cfg->fhgde, tk );
		if(new &&	fl->ff_file->UploadDate > cfg->newtime)
		{
			if(cfg->fhnewtxt) FPrintf(cfg->fhnewtxt, "%s\n", tk );
			if(cfg->fhnewibm) FPrintf(cfg->fhnewibm, "%s\n\r", tk );
			if(cfg->fhnewgde) AGPrintLine( cfg->fhnewgde, tk );
		}

		if(cfg->longdesc)
		{
			tk = strtok( NULL, "\n" );
			while( tk )
			{
				if(cfg->fhtxt)    FPrintf( cfg->fhtxt, "%s%s\n",   spaces, tk );
				if(cfg->fhibm)    FPrintf( cfg->fhibm, "%s%s\n\r", spaces, tk );
				if(cfg->fhgde)
				{
					FPrintf( cfg->fhgde, "%s", spaces );
					if(cfg->fhgde) AGPrintLine( cfg->fhgde, tk );
				}
				if(new && fl->ff_file->UploadDate > cfg->newtime)
				{
					if ( cfg->fhnewtxt) FPrintf( cfg->fhnewtxt, "%s%s\n",   spaces, tk );
					if ( cfg->fhnewibm) FPrintf( cfg->fhnewibm, "%s%s\n\r", spaces, tk );
					if ( cfg->fhnewgde)
					{
						FPrintf( cfg->fhnewgde, "%s",   spaces );
						if(cfg->fhnewgde) AGPrintLine( cfg->fhnewgde, tk );
					}
				}
				tk = strtok( NULL, "\n" );
			}
		}
	}
	else
	{
			if ( cfg->fhtxt)    FPrintf(cfg->fhtxt, "%s\n", cfg->defdesc );
			if ( cfg->fhibm)    FPrintf(cfg->fhibm, "%s\n\r", cfg->defdesc );
			if ( cfg->fhgde)    AGPrintLine( cfg->fhgde, cfg->defdesc );
			if ( new && fl->ff_file->UploadDate > cfg->newtime )
			{
				if(cfg->fhnewtxt) FPrintf(cfg->fhnewtxt, "%s\n", cfg->defdesc);
				if(cfg->fhnewibm) FPrintf(cfg->fhnewibm, "%s\n\r", cfg->defdesc);
				if(cfg->fhnewgde) AGPrintLine( cfg->fhnewgde, cfg->defdesc );
			}
	}
}

BOOL
SizeSort ( LONG area, BOOL new )
{
	struct	ScanInfo	si;
	APTR							key;
	ULONG							desc;
	struct	List			list;
	struct	MyNode		*node;
	struct	fullFile	fl;
	int								files = 0, newfiles = 0, bytecount = 0, newbytes = 0;

	si.ScanFlag     = SA_CHRONO;
	si.Direction	= SA_FORWARDS;
	if(cfg->filebuffers < 64) si.Buffer = 64;
	else si.Buffer	= cfg->filebuffers;
	si.Area		= area;
	si.StartNumber	= 0;

	NewList ( &list ) ;

	if ( key = InitFileScanKey ( &si ) )
	{
		while ( desc = ScanFiles ( key, &fl ) )
		{
			++files;
			bytecount += fl.ff_file->FileSize;
			
			if(fl.ff_file->UploadDate > cfg->newtime)
			{
				++newfiles;
				newbytes += fl.ff_file->FileSize;
			}


			if ( node = (struct MyNode *) AllocMem ( sizeof ( struct MyNode ), MEMF_ANY ) )
			{
				node->fl.ff_file = fl.ff_file;
				node->fl.ff_text = fl.ff_text;
				node->desc       = desc;

				AddTail ( &list, (struct Node *) node ) ;
			}
		}
		CloseFileScanKey ( key ) ;

		if ( cfg->sortfwd )
			MySort ( &list, ForwardSort ) ;
		else
			MySort ( &list, BackwardSort ) ;

		while ( node = (struct MyNode *) RemHead ( &list ) )
		{
			MyOutputln ( &node->fl, new ) ;
			FreeMem ( node->fl.ff_text, node->desc ) ;
			FreeMem ( node->fl.ff_file, sizeof ( struct file ) ) ;
			FreeMem ( node, sizeof ( struct MyNode ) ) ;
		}
	}
	if(cfg->fhtxt)    FPrintf(cfg->fhtxt,    "%s\n%ld files %ld bytes in this area.\n\n", dashes, files, bytecount);
	if(cfg->fhibm)    FPrintf(cfg->fhibm,    "%s\n\r%ld files %ld bytes in this area.\n\r\n\r", dashes, files, bytecount);
	if(cfg->fhgde)    FPrintf(cfg->fhgde,    "%s\n%ld files %ld bytes in this area.\n\n", dashes, files, bytecount);
	if(new)
	{
		if(cfg->fhnewtxt) FPrintf(cfg->fhnewtxt, "%s\n%ld new files %ld bytes in this area.\n\n", dashes, newfiles, newbytes);
		if(cfg->fhnewibm) FPrintf(cfg->fhnewibm, "%s\n\r%ld new files %ld bytes in this area.\n\r\n\r", dashes, newfiles, newbytes);
		if(cfg->fhnewgde) FPrintf(cfg->fhnewgde, "%s\n%ld new files %ld bytes in this area.\n\n", dashes, newfiles, newbytes);
	}

	return ( TRUE ) ;
}


BOOL MyListFiles ( LONG area, BOOL new )
{
	struct	ScanInfo	si;
	struct	fullFile	fl;
	APTR		key;
	ULONG		desc;
	int			files = 0, newfiles = 0, bytecount = 0, newbytes = 0;

	if ( cfg->sortalpha )
		si.ScanFlag 	= SA_ALPHA;
	else
		si.ScanFlag     = SA_CHRONO;

	if ( cfg->sortfwd )
		si.Direction	= SA_FORWARDS;
	else
		si.Direction    = SA_BACKWARDS;

	if(cfg->filebuffers < 64) si.Buffer = 64;
	else si.Buffer	= cfg->filebuffers;
	si.Area		= area;
	si.StartNumber	= 0;

	if ( key = InitFileScanKey ( &si ) )
	{
		while ( desc = ScanFiles ( key, &fl ) )
		{
			MyOutputln ( &fl, new) ;

			++files;
			bytecount += fl.ff_file->FileSize;
			if(fl.ff_file->UploadDate > cfg->newtime)
			{
				++newfiles;
				newbytes += fl.ff_file->FileSize;
			}

			FreeMem ( fl.ff_file, sizeof ( struct file ) ) ;
			FreeMem ( fl.ff_text, desc ) ;
		}
		CloseFileScanKey ( key ) ;
	}
	if(cfg->fhtxt)    FPrintf(cfg->fhtxt,    "%s\n%ld files %ld bytes in this area.\n", dashes, files, bytecount);
	if(cfg->fhibm)    FPrintf(cfg->fhibm,    "%s\n\r%ld files %ld bytes in this area.\n\r", dashes, files, bytecount);
	if(cfg->fhgde)    FPrintf(cfg->fhgde,    "%s\n%ld files %ld bytes in this area.\n", dashes, files, bytecount);
	if(new)
	{
		if(cfg->fhnewtxt) FPrintf(cfg->fhnewtxt, "%s\n%ld new files %ld bytes in this area.\n", dashes, newfiles, newbytes);
		if(cfg->fhnewibm) FPrintf(cfg->fhnewibm, "%s\n\r%ld new files %ld bytes in this area.\n\r", dashes, newfiles, newbytes);
		if(cfg->fhnewgde) FPrintf(cfg->fhnewgde, "%s\n%ld new files %ld bytes in this area.\n", dashes, newfiles, newbytes);
	}
	return ( TRUE ) ;
}

/* Works with any Node (or MinNode) compatable struct pointer  */
/* size is sizeof the particular node compatable struct to add */
/* returns a pointer to the newnode or NULL if unsuccessfull   */
struct Node *AddNewNode(struct List *list, ULONG size)
{
struct Node *newnode;

	if(newnode = AllocMem(size, MEMF_CLEAR))
		AddTail((struct List *)list, (struct Node *)newnode);
	return(newnode);
}

/* Works with any Node (or MinNode) compatable struct pointer  */
/* size is sizeof the particular node compatable struct        */
/* NOTE - list header pointers will be invalid, you must reset */
/*        them yourself if you intend to use the list again!   */
VOID FreeAllNodes(struct List *list, ULONG size)
{
struct Node *worknode;
struct Node *nextnode;

	worknode = (struct Node *) (list->lh_Head);
	while(nextnode = (struct Node *) (worknode->ln_Succ))
	{
		FreeMem(worknode, size);
		worknode = nextnode;
	}

}

LONG CheckKey( struct SharedData	*Shared )
{
	char Buffer[108], *c;
	struct KeyFile *key;
	BPTR 	lock;
	FILE *in;
	ULONG i, j, k;

	/* Get the keyfile */

	lock=GetProgramDir(); /*** DO NOT UNLOCK THIS LOCK ***/
	NameFromLock(lock, Buffer,108);
	AddPart(Buffer, "Lightning.key", 108);
		
	if( key = (struct KeyFile *) AllocMem ( sizeof ( struct KeyFile ), MEMF_CLEAR ) )
	{
 		if(in=fopen(Buffer,"rb"))
 		{
			fread(key, sizeof(struct KeyFile), 1, in);
			fclose(in);

			strcpy(Buffer, key->Sysop);
			j = strlen(Buffer);
Forbid();
			for(c = &Buffer[0], i = 0, k = 0;i < j;c++, i++)
			{
				k += ((ULONG) *c) + i*2 + 100;
			}
			if(k == key->a[19])
			{
			 	/* Sysop name is good, now check BBS serial number */

				if( key->a[97] == Shared->KeyInfo->Serial )
				{
					/* BBS serial number matches too, must be ok */
					/* Return Lightning keyfile serial number    */
Permit();
					FreeMem ( key, sizeof ( struct KeyFile ) ) ;
					return( (LONG) key->a[132] );
				}
				else
				{
Permit();
					FreeMem ( key, sizeof ( struct KeyFile ) ) ;
					return( -2 );
				}
			}
			else
			{
Permit();
				FreeMem ( key, sizeof ( struct KeyFile ) ) ;
				return( -1 );
			}
Permit();
 		}
		FreeMem ( key, sizeof ( struct KeyFile ) ) ;
	}
	return( 0 );
}


void
main ( int argc, char **argv )
{
	int		i, a;
	LONG  serialnum;
  ULONG len;
  char  *tbuff, tpath[108];
	char 	configpath[108];
  char 	bragline[]  = "       Filelist created by Lightning FileLister v1.2 (c)1996 Rod Schnell";
	char 	regline[]   = "                       unregistered evaluation version";


	for ( i = 0; i < NUMARGS; i ++ )
		args[i] = NULL;

	if ( Args = ReadArgs ( "CONFIGFILE", args, NULL ) )
	{
		if(args[0]) strncpy(configpath, (UBYTE *)args[0], 108);
		else (strncpy(configpath, "Zeus:Config/Lightning/Lightning.cfg", 108));
		FreeArgs ( Args );
	}

	if ( PipelineBase = OpenLibrary ( "zeus.library", 0L ) )
	{
		if ( PipeUtilBase = OpenLibrary ( "zmf.library", 0L ) )
		{
			if ( IsPlFileThere ( ) )
			{
				struct PF_Globals *fg;
	
				if( cfg = (struct FL_Config *) AllocMem ( sizeof ( struct FL_Config ), MEMF_CLEAR ) )
				{
					if( cfg->areas = (struct List *) AllocMem(sizeof(struct List), MEMF_ANY) )
					{
						NewList( cfg->areas ); /*** IMPORTANT - FOR ALL LISTS ****/

						if( Shared = GetShared() )
						{
							serialnum = CheckKey( Shared );

							if(serialnum < 0)
							{
								Printf("LIGHTNING KEYFILE CHECK FAILED!\nAborting program...\n");
								goto Failed;
							}
							else
							{
								if(serialnum > 0)
									FPrintf(Output(), "Lightning FileLister v1.2 (c)1996 Rod Schnell\n serial# %lu registered to: %s\n\n", serialnum, Shared->KeyInfo->Name );
								else Printf("Lightning FileLister v1.2 (c)1996 Rod Schnell\n     unregistered evaluation version\n\n");
							}
							

/*  BBS Keyfile Information

		Shared->KeyInfo->Created;		/* DateStamp of Key creation 	        */
		Shared->KeyInfo->Serial;		/* Zeus serial number			            */
		Shared->KeyInfo->Name;	    /* Name of registered sysop 40 chars	*/
*/



							if(ReadCfg(configpath, cfg) == 0)
							{
								if ( fg = GetFileGlobals ( ) )
								{
									struct AreaNode *node;

									if(cfg->alltxt) cfg->fhtxt    = Open ( cfg->alltxt, MODE_NEWFILE ) ;
									if(cfg->allibm) cfg->fhibm    = Open ( cfg->allibm, MODE_NEWFILE ) ;
									if(cfg->allgde) cfg->fhgde    = Open ( cfg->allgde, MODE_NEWFILE ) ;
									if(cfg->newtxt) cfg->fhnewtxt = Open ( cfg->newtxt, MODE_NEWFILE ) ;
									if(cfg->newibm) cfg->fhnewibm = Open ( cfg->newibm, MODE_NEWFILE ) ;
									if(cfg->newgde) cfg->fhnewgde = Open ( cfg->newgde, MODE_NEWFILE ) ;
									if ( cfg->fhtxt || cfg->fhnewtxt || cfg->fhibm || cfg->fhnewibm || cfg->fhgde || cfg->fhnewgde)
									{
										if(cfg->fhtxt)
										{
											FPrintf ( cfg->fhtxt, "%s\n\n", bragline ) ;
											if(serialnum == 0) FPrintf ( cfg->fhtxt, "%s\n\n", regline ) ;
											else PrintReg ( cfg->fhtxt, FALSE, serialnum ) ;

											if ( InsertFile(cfg->fhtxt, "Header.txt" ) )
                    		FPrintf ( cfg->fhtxt, "\n" ) ;
										}
										if(cfg->fhibm)
										{
											FPrintf ( cfg->fhibm, "%s\n\r\n\r", bragline ) ;
											if(serialnum == 0) FPrintf ( cfg->fhibm, "%s\n\r\n\r", regline ) ;
											else PrintReg ( cfg->fhibm, TRUE, serialnum ) ;

											if ( InsertFile(cfg->fhibm, "Header.ibm" ) )
                    		FPrintf ( cfg->fhibm, "\n\r" ) ;
										}
										if(!(cfg->fhgde || cfg->fhnewgde))
											Printf("\n");

											/* For guide files, build area list first and @Endnode */
											/* Do not create Amigaguide area nodes in main loop    */

											if(cfg->fhgde)
											{
												FPrintf ( cfg->fhgde, "@DataBase  ALLFILES\n\n@Node Main \"ALLFILES\"\n%s\n", bragline);
												if(serialnum == 0) FPrintf ( cfg->fhgde, "%s\n", regline ) ;
												else PrintReg ( cfg->fhgde, FALSE, serialnum ) ;

												/* Guides need leading \n unlike trailing \n for InsertFile() */

												strncpy(tpath, cfg->insdir, 107);
												if(AddPart(tpath, "Header.guide", 108))
												{
                 					tbuff = ReadWholeFile ( tpath, &len ) ;
                 					if ( tbuff )
                 					{
                       			FPrintf ( cfg->fhgde, "%s\n", tbuff ) ;
                      	 		FreeMem ( tbuff, len ) ;
                 					}
                 				}

												if(cfg->about[0])
													FPrintf ( cfg->fhgde, "\n                   @{\" %-38s \" link ABOUTLFL}\n\n", cfg->about);
											}
											if(cfg->fhnewgde)
											{
												FPrintf ( cfg->fhnewgde, "@DataBase  NEWFILES\n\n@Node Main \"NEWFILES\"\n%s\n", bragline);
												if(serialnum == 0) FPrintf ( cfg->fhnewgde, "%s\n", regline ) ;
												else PrintReg ( cfg->fhnewgde, FALSE, serialnum ) ;

												strncpy(tpath, cfg->insdir, 107);
												if(AddPart(tpath, "NewHeader.guide", 108))
												{
                 					tbuff = ReadWholeFile ( tpath, &len ) ;
                 					if ( tbuff )
                 					{
                       			FPrintf ( cfg->fhnewgde, "%s\n", tbuff ) ;
                       			FreeMem ( tbuff, len ) ;
                 					}
                 				}

												if(cfg->about[0])
													FPrintf ( cfg->fhnewgde, "\n                   @{\" %-38s \" link ABOUTLFL}\n\n", cfg->about);
											}
											if ( node = (struct AreaNode *) GetSucc ( (struct Node *) fg->config->cfg_AreaList ) )
											{
												int t, tnum, lnum;
												char *tc, temp[108];

												i = a = 0;

												/* Walk the arealist, build my own list of Use and New areas */
												/* and build the AmigaGuide main node (with area buttons)    */

												do
												{
													if(FLnode = (struct FL_AreaNode *) AddNewNode(cfg->areas, sizeof(struct FL_AreaNode)))
													{
														FLnode->Number = node->area->number;
														if(cfg->areasstring[0] && serialnum > 0 )
														{
															tc = cfg->areasstring;
															t = tnum = lnum = 0;
															do
															{
																switch (*tc)
																{
																	case '-':
																		if( temp[0] ) lnum = atoi( temp );
																		else lnum = 1;
																		if( node->area->number == lnum )
																			FLnode->Use = TRUE;
																		tc++;
																		if(*tc == NULL && node->area->number >= lnum)
																			FLnode->Use = TRUE;
																		tc--;
																		t = 0;
																		break;
																	case ',':
																		if( temp[0] ) tnum = atoi( temp );
																		if( lnum > 0 )
																		{
																			if( node->area->number >= lnum && node->area->number <= tnum )
																				FLnode->Use = TRUE;
																			lnum = 0;
																		}
																		else if( node->area->number == tnum )
																			FLnode->Use = TRUE;
																		t = 0;
																		break;
																	default:
																		temp[t] = *tc;
																		temp[++t] = NULL;
																		tc++;
																		if(*tc == NULL )
																		{
																			if(temp[0])
																			{
																				tnum = atoi( temp );
																				if( lnum > 0 )
																				{
																					if( node->area->number >= lnum && node->area->number <= tnum )
																						FLnode->Use = TRUE;
																					lnum = 0;
																				}
																				else if( node->area->number == tnum )
																					FLnode->Use = TRUE;
																			}
																		}
																		tc--;
																		break;
																	}
																	tnum = 0;
																	tc++;
																	if( FLnode->Use == TRUE) break;
																} while( *tc != NULL ) ;
														
															}
															else FLnode->Use = TRUE;

															if( CheckNewFiles ( fg, node->area->number, cfg->newtime ) )
																FLnode->New = TRUE;
														}
														if ( FLnode->Use )
														{
															i++;
															if( cfg->fhgde )
															{
																FPrintf ( cfg->fhgde, "@{\" %-36s \" link %ld}", node->area->name, node->area->number);
																if( i % 2 ) FPrintf( cfg->fhgde, "  ");
																else    		FPrintf( cfg->fhgde, "\n");
															}
															if ( cfg->fhnewgde && FLnode->New == TRUE)
															{
																++a;
																FPrintf ( cfg->fhnewgde, "@{\" %-36s \" link %ld}", node->area->name, node->area->number);
																if ( a % 2 )  FPrintf ( cfg->fhnewgde, "  ");
																else    			FPrintf( cfg->fhnewgde, "\n" ) ;
															}
														}
													}
													while ( node = (struct AreaNode *) GetSucc ( (struct Node *) node ) ) ;
													if( i % 2 ) if(cfg->fhgde) FPrintf(cfg->fhgde, "\n");
													if( a % 2 ) if(cfg->fhnewgde ) FPrintf(cfg->fhnewgde, "\n");
												}
												if(cfg->fhgde)
												{

													strncpy(tpath, cfg->insdir, 107);
													if(AddPart(tpath, "Footer.guide", 108))
													{
                 						tbuff = ReadWholeFile ( tpath, &len ) ;
                 						if ( tbuff )
                 						{
                       				FPrintf ( cfg->fhgde, "\n%s", tbuff ) ;
                       				FreeMem ( tbuff, len ) ;
                 						}
                 					}

													FPrintf ( cfg->fhgde,		 "\n%s\n", bragline);
													if(serialnum == 0) FPrintf ( cfg->fhgde, "%s\n", regline ) ;
													else PrintReg ( cfg->fhgde, FALSE, serialnum ) ;
													FPrintf ( cfg->fhgde,		 "@EndNode\n\n");
											
													/* Create ABOUTLFL node */
											
													if(cfg->about)
													{
														FPrintf ( cfg->fhgde, "@Node ABOUTLFL \"%s\"\n", cfg->about);
														if ( !InsertFile(cfg->fhgde, "About" ) )
															Printf ( "Error reading About file!\n\n" ) ;
														FPrintf(cfg->fhgde, "@EndNode\n\n");
													}
												}
												if(cfg->fhnewgde)
												{

													strncpy(tpath, cfg->insdir, 107);
													if(AddPart(tpath, "NewFooter.guide", 108))
													{
                 						tbuff = ReadWholeFile ( tpath, &len ) ;
                 						if ( tbuff )
                 						{
                   		    		FPrintf ( cfg->fhnewgde, "\n%s", tbuff ) ;
                   		    		FreeMem ( tbuff, len ) ;
                 						}
                 					}

													FPrintf ( cfg->fhnewgde, "\n%s\n", bragline);
													if(serialnum == 0) FPrintf ( cfg->fhnewgde, "%s\n", regline ) ;
													else PrintReg ( cfg->fhnewgde, FALSE, serialnum ) ;
													FPrintf ( cfg->fhnewgde,		 "@EndNode\n\n");
													if(cfg->about)
													{
														FPrintf ( cfg->fhnewgde, "@Node ABOUTLFL \"%s\"\n", cfg->about);
														if ( !InsertFile(cfg->fhnewgde, "AboutNew" ) )
															Printf ( "Error reading AboutNew file!\n\n" ) ;
														FPrintf(cfg->fhnewgde, "@EndNode\n\n");
													}
												}
											if(cfg->fhnewtxt)
											{
												FPrintf ( cfg->fhnewtxt, "%s\n\n", bragline ) ;
												if(serialnum == 0) FPrintf ( cfg->fhnewtxt, "%s\n\n", regline ) ;
												else PrintReg ( cfg->fhnewtxt, FALSE, serialnum ) ;
												if ( InsertFile(cfg->fhnewtxt, "NewHeader.txt" ) )
               		     	FPrintf ( cfg->fhnewtxt, "\n" ) ;
											}
											if(cfg->fhnewibm)
											{
												FPrintf ( cfg->fhnewibm, "%s\n\r\n\r", bragline ) ;
												if(serialnum == 0) FPrintf ( cfg->fhnewibm, "%s\n\r\n\r", regline ) ;
												else PrintReg ( cfg->fhnewibm, TRUE, serialnum ) ;

												if ( InsertFile(cfg->fhnewibm, "NewHeader.ibm" ) )
                    			FPrintf ( cfg->fhnewibm, "\n\r" ) ;
											}
									
											/* Now, walk the area list, insert area headers/footers and list files as necessary */
									
											if ( node = (struct AreaNode *) GetSucc ( (struct Node *) fg->config->cfg_AreaList ) )
											{
												FLnode = (struct FL_AreaNode *) GetSucc ( (struct Node *) cfg->areas ); 

												do
												{

												/*** Check matching FLnode List nodes, Use and New variables ***/
												/*** to decide whether or not to list each area              ***/

												if(FLnode->Use)
												{
													Printf ( "[A[1KListing area: %lu %s\n", node->area->number, node->area->name ) ;

													if(cfg->fhtxt)
													{
														sprintf( tpath, "Header.%d.txt", node->area->number);
														if ( !InsertFile(cfg->fhtxt, tpath ) )
															FPrintf( cfg->fhtxt, "Area: %lu %s\n", node->area->number, node->area->name);
														FPrintf( cfg->fhtxt, "%s\nFilename             Size    Date  DnLd Description\n%s\n", dashes, dashes );
													}
													if(cfg->fhibm)
													{
														sprintf( tpath, "Header.%d.ibm", node->area->number);
														if ( !InsertFile(cfg->fhibm, tpath ) )
															FPrintf( cfg->fhibm, "Area: %lu %s\n\r", node->area->number, node->area->name);
 														FPrintf( cfg->fhibm, "%s\n\rFilename             Size    Date  DnLd Description\n\r%s\n\r", dashes, dashes );
 													}
													if(cfg->fhgde)
													{
														/* Guide Insert headers must be inserted AFTER creating the node */

														FPrintf(cfg->fhgde, "@Node %lu \"%s\"\n", node->area->number, node->area->name);
														sprintf( tpath, "Header.%d.guide", node->area->number);
														if ( !InsertFile(cfg->fhgde, tpath ) )
															FPrintf( cfg->fhgde, "Area: %lu %s\n", node->area->number, node->area->name);
														FPrintf(cfg->fhgde, "%s\nFilename             Size    Date  DnLd Description\n%s\n", dashes, dashes );
													}
													if(FLnode->New && cfg->fhnewtxt)
													{
														sprintf( tpath, "NewHeader.%d.txt", node->area->number);
														if ( !InsertFile(cfg->fhnewtxt, tpath ) )
															FPrintf( cfg->fhnewtxt, "Area: %lu %s\n", node->area->number, node->area->name);
														FPrintf( cfg->fhnewtxt, "%s\nFilename             Size    Date  DnLd Description\n%s\n", dashes, dashes );
													}
													if(FLnode->New && cfg->fhnewibm)
													{
														sprintf( tpath, "NewHeader.%d.ibm", node->area->number);
														if ( !InsertFile(cfg->fhnewibm, tpath ) )
															FPrintf( cfg->fhnewibm, "Area: %lu %s\n\r", node->area->number, node->area->name);
 														FPrintf( cfg->fhnewibm, "%s\n\rFilename             Size    Date  DnLd Description\n\r%s\n\r", dashes, dashes );
 													}
													if(FLnode->New && cfg->fhnewgde)
													{
														/* Guide Insert headers must be inserted AFTER creating the node */

														FPrintf(cfg->fhnewgde, "@Node %lu \"%s\"\n", node->area->number, node->area->name);
														sprintf( tpath, "NewHeader.%d.guide", node->area->number);
														if ( !InsertFile(cfg->fhnewgde, tpath ) )
															FPrintf( cfg->fhnewgde, "Area: %lu %s\n", node->area->number, node->area->name);
														FPrintf(cfg->fhnewgde, "%s\nFilename             Size    Date  DnLd Description\n%s\n", dashes, dashes );
													}

													/* File Sorting/Listing functions */
													if(cfg->sortsize) SizeSort ( node->area->number, FLnode->New ) ;
													else MyListFiles ( node->area->number, FLnode->New ) ;

													if(cfg->fhtxt)
													{
														sprintf( tpath, "Footer.%d.txt", node->area->number);
														if( !InsertFile(cfg->fhtxt, tpath ) )
															FPrintf( cfg->fhtxt, "\n" ) ;
													}
													if(cfg->fhibm)
													{
														sprintf( tpath, "Footer.%d.ibm", node->area->number);
														if ( !InsertFile(cfg->fhibm, tpath ) )
															FPrintf( cfg->fhibm, "\n" ) ;
 													}
													if(cfg->fhgde)
													{
														/* Guide footers MUST be done here BEFORE @EndNode */

														sprintf( tpath, "Footer.%d.guide", node->area->number);
														if ( !InsertFile(cfg->fhgde, tpath ))
															FPrintf( cfg->fhgde, "\n");
														FPrintf(cfg->fhgde, "@EndNode\n\n");
													}
													if(FLnode->New && cfg->fhnewtxt)
													{
														sprintf( tpath, "NewFooter.%d.txt", node->area->number);
														if ( !InsertFile(cfg->fhnewtxt, tpath ) )
															FPrintf( cfg->fhnewtxt, "\n");
													}
													if(FLnode->New && cfg->fhnewibm)
													{
														sprintf( tpath, "NewFooter.%d.ibm", node->area->number);
														if ( !InsertFile(cfg->fhnewibm, tpath ) )
															FPrintf( cfg->fhnewibm, "\n\r");
 													}
													if(FLnode->New && cfg->fhnewgde)
													{
														/* Guide footers MUST be done here BEFORE @EndNode */

														sprintf( tpath, "NewFooter.%d.guide", node->area->number);
														if ( !InsertFile(cfg->fhnewgde, tpath ))
															FPrintf( cfg->fhnewgde, "\n");
														FPrintf(cfg->fhnewgde, "@EndNode\n\n");
													}
												}
												FLnode = (struct FL_AreaNode *) GetSucc ( (struct Node *) FLnode );
											}
											while ( node = (struct AreaNode *) GetSucc ( (struct Node *) node ) ) ;
										}
										if(cfg->fhtxt)
										{
											InsertFile(cfg->fhtxt, "Footer.txt" );
											FPrintf ( cfg->fhtxt, "%s\n\n", bragline) ;
											if(serialnum == 0) FPrintf ( cfg->fhtxt, "%s\n\n", regline ) ;
											else PrintReg ( cfg->fhtxt, FALSE, serialnum ) ;
											Close( cfg->fhtxt );
											SetComment(cfg->alltxt, cfg->cmmalltxt);
										}
										if(cfg->fhibm)
										{
											InsertFile(cfg->fhibm, "Footer.ibm" );
											FPrintf ( cfg->fhibm, "%s\n\r\n\r", bragline ) ;
											if(serialnum == 0) FPrintf ( cfg->fhibm, "%s\n\r\n\r", regline ) ;
											else PrintReg ( cfg->fhibm, TRUE, serialnum ) ;
											Close( cfg->fhibm );
											SetComment(cfg->allibm, cfg->cmmallibm);
										}
										if(cfg->fhgde)
										{
											/* Bottom bragline for guide files must be done in first node loop above */
											Close( cfg->fhgde );
											SetComment(cfg->allgde, cfg->cmmallgde);
										}
										if(cfg->fhnewtxt)
										{
											InsertFile(cfg->fhnewtxt, "NewFooter.txt" );
											FPrintf ( cfg->fhnewtxt, "%s\n\n", bragline ) ;
											if(serialnum == 0) FPrintf ( cfg->fhnewtxt, "%s\n\n", regline ) ;
											else PrintReg ( cfg->fhnewtxt, FALSE, serialnum ) ;
											Close( cfg->fhnewtxt );
											SetComment(cfg->newtxt, cfg->cmmnewtxt);
										}
										if(cfg->fhnewibm)
										{
											InsertFile(cfg->fhnewibm, "NewFooter.ibm" );
											FPrintf ( cfg->fhnewibm, "%s\n\r\n\r", bragline ) ;
											if(serialnum == 0) FPrintf ( cfg->fhnewibm, "%s\n\r\n\r", regline ) ;
											else PrintReg ( cfg->fhnewibm, TRUE, serialnum ) ;
											Close( cfg->fhnewibm );
											SetComment(cfg->newibm, cfg->cmmnewibm);
										}
										if(cfg->fhnewgde)
										{
											/* Bottom bragline for guide files must be done in node loop above */
											Close( cfg->fhnewgde );
											SetComment(cfg->newgde, cfg->cmmnewgde);
										}
										Printf ( "Finished.\n" ) ;
									}
									else Printf("Couldn't open output file(s)!\n");
								}
								FreeAllNodes( cfg->areas, sizeof ( struct FL_AreaNode ) );
							}
						}
						else
							Printf( "Cannot get SharedData -- Is Zeus running?\n\n" );
						Failed:
						FreeMem ( cfg->areas, sizeof ( struct List ) ) ;
					}
					FreeMem ( cfg, sizeof ( struct FL_Config ) ) ;
				}
			}
			else
				Printf ( "PlFile is not running!\n" ) ;

			CloseLibrary ( PipeUtilBase ) ;
		}
		else
			Printf ( "Can't open zmf.library!\n" ) ;
		CloseLibrary ( PipelineBase ) ;
	}
	else
		Printf ( "Can't open pipeline library!\n" ) ;
}


