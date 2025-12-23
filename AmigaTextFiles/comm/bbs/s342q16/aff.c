/*
 *				Aff.C
 *
 * Automatic File Forwarding utility.
 */

/*
 *				History
 *
 * 90May15  HAW V1 Created.
 */

/*
 *				Contents
 *
 *	crashout()		general crash routine
 *	InitSystem()		open up appropriate data base files
 *	main()			main manager
 *	ParseFileMap()		reads in CtdlAff.Sys
 *	EatMapLine()		eats a line from a file map
 *	RewriteFileMap()	writes out the updated file map
 *	WriteFileList()		work fn that writes out a ctdlaff.sys entry
 *	SystemWrite()		work fn writes out the indicated system
 *	SendThemFiles()		setts up the .sfl files
 *	UpdateSFLs()		updates the sfl file for the given system
 *	WriteSFL()		This will write the .sfl file
 *	CopyToTemp()		copies a name to the temporary list
 *	ShowArguments()		usage
 *	FileCmp()		helps find the string in a list
 *	CheckSystems()		used to help search for a given system in list
 */

#include "ctdl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <dos.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"

#ifndef NO_STAT
#include "sys\stat.h"
#endif

long getdateof(char *file);

extern FILE *upfd;

extern CONFIG    cfg;
extern NetBuffer netBuf;
extern int       thisNet;
extern FILE      *netfl;
extern NetTable  *netTab;
extern char      *READ_ANY, *WRITE_ANY, *WRITE_TEXT;

void InitSystem(void);
void ShowArguments(void);
void ParseFileMap(void);
void SendThemFiles(void);
void RewriteFileMap(void);

/*
 * crashout()
 *
 * This is general crash routine.  Needed by getNet, etc.
 */
void crashout(str)
char *str;
{
    printf(str);
    exit(1);
}

/*
 * InitSystem()
 *
 * This is used to open up appropriate data base files, and initialize buffers.
 */
void InitSystem()
{
    SYS_FILE name;

    /* Open database files. */
    makeSysName(name, "ctdlnet.sys", &cfg.netArea);
    openFile(name, &netfl);

    /* Now initialize data buffers. */
    initNetBuf(&netBuf);
}

/*
 * main()
 *
 * The main manager for Aff.
 */
int main(int, char **);
int main(argc, argv)
int  argc;
char **argv;
{

    printf("AFF V1.3\nAutomatic File Forwarding\n%s\n", COPYRIGHT);

    if (access(LOCKFILE, 0) != ERROR) {
	printf("Please do not run AFF using Outside Commands.\n");
	exit(1);
    }

    if (argc > 1) {
	ShowArguments();
	exit(1);
    }

    if (!readSysTab(FALSE, FALSE)) {
	printf("Couldn't read system table.\n");
	exit(1);
    }

    InitSystem();       /* Open needed data files       */

    ParseFileMap();

    RewriteFileMap();	/* rewrite updated CtdlAff.Sys next	*/

    SendThemFiles();	/* now send them files (if any) 	*/

    writeSysTab();

    return 0;
}

#define FILE_NAME	0
#define SYSTEM_NAME	1
#define NO_FILE_NAME	2

char MapState = FILE_NAME;
void *EatMapLine(char *line);
void *CheckSystems();
SListBase Files = { NULL, NULL, NULL, NULL, EatMapLine };
SListBase Systems = { NULL, CheckSystems, NULL, NULL, NULL };

/* this structure is used to rebuild ctdlaff.sys */
typedef struct {
	char *Filename;
	long Date;
	SListBase SystemList;
} FileList;

/* this structure is used in SystemList, above */
typedef struct {
	char *Systemname;
	long Date;
} SystemRec;

/* this structure is used for efficient updates of *.sfl */
typedef struct {
	char *Systemname;
	SListBase NewFiles;	/* but this is just a list of strings */
} SystemUpdate;

/*
 * ParseFileMap()
 *
 * This function will read in CtdlAff.Sys.
 */
void ParseFileMap()
{
    SYS_FILE fn;

    makeSysName(fn, "ctdlaff.sys", &cfg.netArea);
    if (!MakeList(&Files, fn, NULL))
	crashout("Couldn't find CtdlAff.Sys.");
}

/*
 * EatMapLine()
 *
 * This function will eat a line from a file map.  Actions depend on
 * MapState:
 *
 *	FILE_NAME: looking for a filename to parse.
 *	SYSTEM_NAME: looking for a system to send file to.
 *	NO_FILE_NAME: File for this doesn't exist, so ignore entries.
 *
 *	File is formatted:
 *	<filename>
 *	<system name>:<number>
 *	<system name>:<number>
 *	...
 *	<blank line>
 *	<filename>
 */
void *EatMapLine(char *line)
{
    static FileList *data;	/* keep static so we know what's current*/
    char *colon, work[200];
    SystemRec *sysdata;
    SystemUpdate *sysupdate, *System;
#ifndef NO_STAT
    struct stat buf;
#endif

    switch (MapState) {
    case FILE_NAME:
	if (strLen(line) == 0) return NULL;	/* discard blank lines	*/
	if (access(line, 0) != 0) {		/* if file doesn't exist*/
	    MapState = NO_FILE_NAME;		/* then don't process	*/
	    printf("Warning: file %s not found.\n", line);
	}
	else MapState = SYSTEM_NAME;	/* file exists */
	data = GetDynamic(sizeof *data);
	data->Filename = strdup(line);
	if (MapState == SYSTEM_NAME) {
#ifndef NO_STAT
	    stat(line, &buf);
	    data->Date = buf.st_mtime;
#else
	    data->Date = getdateof(line);
#endif
	}
	InitListValues(&data->SystemList, NULL, NULL, NULL, NULL);
	return data;
    case NO_FILE_NAME:
	if (strLen(line) == 0) {	/* blank line, return to normal */
	    MapState = FILE_NAME;
	    return NULL;
	}
	if ((colon = strchr(line, ':')) != NULL) {
	    *colon++ = 0;
	    NormStr(colon);		/* kill leading spaces on date */
	    NormStr(line);		/* kill trailing spaces */
	    sysdata = GetDynamic(sizeof *sysdata);
	    sysdata->Systemname = strdup(line);
	    sysdata->Date = atol(colon);
	    AddData(&data->SystemList, sysdata, NULL, FALSE);
	}
	return NULL;
    case SYSTEM_NAME:
	if (strLen(line) == 0) {	/* blank line, return to normal */
	    MapState = FILE_NAME;
	}
	else {
	    if ((colon = strchr(line, ':')) != NULL) {
		*colon++ = 0;
		NormStr(colon);		/* kill leading spaces on date */
		NormStr(line);		/* kill trailing spaces */
		sysdata = GetDynamic(sizeof *sysdata);
		sysupdate = GetDynamic(sizeof *sysupdate);
		sysdata->Systemname = sysupdate->Systemname = strdup(line);
		sysdata->Date = atol(colon);
		AddData(&data->SystemList, sysdata, NULL, FALSE);
		if (data->Date > sysdata->Date) {
		    if ((System = SearchList(&Systems, sysdata->Systemname))
								== NULL) {
			System = sysupdate;
			InitListValues(&System->NewFiles,NULL,NULL,NULL,NULL);
			AddData(&Systems, System, NULL, FALSE);
		    }
		    else free(sysupdate);
		    strcpy(work, data->Filename);
		    AddData(&System->NewFiles, strdup(work), NULL, FALSE);
		    if (searchNameNet(line, &netBuf) != ERROR)
			sysdata->Date = data->Date;	/* update record */
		}
	    }
	}
	return NULL;			/* always return NULL here	*/
    default: crashout("Parsing failure!");
    }
    return NULL;
}

static FILE *fd;
/*
 * RewriteFileMap()
 *
 * This function writes out the file map.
 */
void RewriteFileMap()
{
    SYS_FILE fn;
    void WriteFileList();

    makeSysName(fn, "ctdlaff.sys", &cfg.netArea);
    if ((fd = fopen(fn, WRITE_TEXT)) == NULL)
	crashout("Couldn't update CtdlAff.Sys!");
    RunList(&Files, WriteFileList);
    fclose(fd);
}

/*
 * WriteFileList()
 *
 * This is a work fn that writes out a ctdlaff.sys entry.
 */
void WriteFileList(FileList *data)
{
	void SystemWrite();

	fprintf(fd, "%s\n", data->Filename);
	RunList(&data->SystemList, SystemWrite);
	fprintf(fd, "\n");	/* blank line separator */
}

/*
 * SystemWrite()
 *
 * This work fn writes out the indicated system to fd.
 */
void SystemWrite(SystemRec *data)
{
	fprintf(fd, "    %s : %ld\n", data->Systemname, data->Date);
}

/*
 * SendThemFiles()
 *
 * This function manages setting up the .sfl files.
 */
void SendThemFiles()
{
	void UpdateSFLs();

	RunList(&Systems, UpdateSFLs);
}

void *FileCmp();
SListBase TempList = { NULL, FileCmp, NULL, NoFree, NULL };
/*
 * UpdateSFLs()
 *
 * This updates the sfl file for the given system.  It works to ensure
 * duplicates are not accidentally sent - that explains the complexity.
 */
void UpdateSFLs(SystemUpdate *data)
{
	void CopyToTemp(), WriteSFL();
	int slot;
	SYS_FILE fn;
	char temp[10], realname[100];

	if ((slot = searchNameNet(data->Systemname, &netBuf)) == ERROR) {
		printf("Warning: no system '%s' known.\n", data->Systemname);
		return;
	}
	printf("System %s:", data->Systemname);
	RunList(&data->NewFiles, CopyToTemp);
	printf(".\n");

	sPrintf(temp, "%d.sfl", slot);
	makeSysName(fn, temp, &cfg.netArea);

	if ((upfd = safeopen(fn, READ_ANY)) != NULL) {
		while (getSLNet(realname, upfd)) {
			/* NormalName(&buf, realname); */
			AddData(&TempList, strdup(realname), NULL, TRUE);
		}
		fclose(upfd);
		unlink(fn);
	}

	if ((upfd = safeopen(fn, WRITE_ANY)) != NULL) {
		RunList(&TempList, WriteSFL);
		fclose(upfd);
	}

	netBuf.nbflags.send_files = TRUE;
	putNet(slot, &netBuf);
	KillList(&TempList);
}

/*
 * WriteSFL()
 *
 * This will update the .sfl file.
 */
void WriteSFL(char *name)
{
	struct fl_send buf;
	void SetBuffer(char *name, struct fl_send *buf);

	SetBuffer(name, &buf);
	putSLNet(buf, upfd);
}

/*
 * CopyToTemp()
 *
 * This function will copy a name to the temporary list with no dups.
 */
void CopyToTemp(char *name)
{
	printf(" %s", name);
	AddData(&TempList, name, NULL, TRUE);
}

/*
 * ShowArguments()
 *
 * This will show off arguments.
 */
void ShowArguments()
{
	printf("usage: Aff\n");
}

/*
 * FileCmp()
 *
 * This function helps find the designated string in a list of strings.
 */
void *FileCmp(char *n1, char *n2)
{
	if (strCmpU(n1, n2) == SAMESTRING) return n1;
	return NULL;
}

/*
 * CheckSystems()
 *
 * This function is used to help search for a given system in a list.
 */
void *CheckSystems(SystemRec *s, char *t)
{
	if (stricmp(s->Systemname, t) == SAMESTRING) return s;
	return NULL;
}

long getdateof(char *file)
  {
  long thetime;
  struct FileInfoBlock *info;
  info = (struct FileInfoBlock *)calloc(1, sizeof(struct FileInfoBlock));
  if( dfind(info,file,0) == 0 )
    {
    thetime = info->fib_Date.ds_Days*24*3600;
    thetime+= info->fib_Date.ds_Minute;
    }
  else  thetime = 0;
  free(info);
  return thetime;
  }
