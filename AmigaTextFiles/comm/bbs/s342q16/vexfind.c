/*
 *				VexFind.c
 *
 * Find the system in VORTEX.SYS.
 */

/*
 *				History
 *
 * 90Jul29  HAW Initial creation.
 */

/*
 *				Contents
 *
 */

#include "ctdl.h"    /* header file  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include "exec/memory.h"
#include "exec/ports.h"
#include "exec/exec.h"

#define V_COPYRIGHT "Copyright (c) 1990, 1991 by Hue, Jr."
#define TITLE "VexFind V1.1\n%s\n", V_COPYRIGHT

#define USAGE	"Usage: VEXFIND \"<system name>\"\n\n  or\n\nVEXFIND\n"

FILE *vx;
void Find(char *name);
void ShowAll(void);
void ShowRoomInfo(int x);

extern CONFIG cfg;
extern rTable *roomTab;			/* RAM index of rooms		*/
extern aRoom  roomBuf;			/* room buffer			*/
extern FILE   *roomfl;			/* file descriptor for rooms	*/
extern int    thisRoom;			/* room currently in roomBuf	*/

char TabRead = FALSE;
/*
 * main()
 *
 * This is the main manager.
 */
int main(int, char **);
int main(argc, argv)
int  argc;
char **argv;
{
    extern char *READ_ANY;
    SYS_FILE filename;

    printf(TITLE);

    if (argc > 2) {
	printf(USAGE);
	exit(1);
    }

    if ((vx = fopen("VORTEX.SYS", READ_ANY)) == NULL) {
	if (!readSysTab(FALSE, TRUE)) {
	    printf("Couldn't find anything to work with.\n");
	    exit(1);
	}
	TabRead = TRUE;
	makeSysName(filename, "VORTEX.SYS", &cfg.netArea);
	if ((vx = fopen(filename, READ_ANY)) == NULL) {
	    printf("Couldn't find a VORTEX.SYS to work with.\n");
	    exit(1);
	}
    }

    if (argc == 1) ShowAll();
    else Find(argv[1]);
return 0;
}

typedef struct {
	label Id, Name;
	char InUse;
} VtRecord;

/*
 * ShowAll()
 *
 * This function shows all the systems listed in the vortex list.
 */
void ShowAll()
{
    VtRecord Vtx;
    int i = 0;

    while (fread(&Vtx, 1, sizeof Vtx, vx) > 0)
	printf("%3d. %-25s : %s\n", i++, Vtx.Name, Vtx.Id);
}

/*
 * Find()
 *
 * This function finds the named system in the vortex list.  First occurrence
 * only.
 */
void Find(char *name)
{
	VtRecord Vtx;
	int i = 0;

    if (!TabRead && readSysTab(FALSE, FALSE))	TabRead = TRUE;

    while (fread(&Vtx, 1, sizeof Vtx, vx) > 0) {
	if (stricmp(name, Vtx.Name) == 0) {
	    printf("%3d. %s\n", i, Vtx.Name);
	    ShowRoomInfo(i);
	    break;
	}
	i++;
    }
}

typedef struct {
	int	    vRoomNo;	/* Room we are associated with		*/
	AN_UNSIGNED vGen;	/* Allows checking for no longer in use */
	MSG_NUMBER  vHighest;   /* Highest msg # received		*/
	long	    vLastDate;  /* Date of last message received	*/
} VORTEX;

/*
 * ShowRoomInfo()
 *
 * This function tries to show which rooms are actually shared.
 */
void ShowRoomInfo(int x)
{
    char temp[10];
    SYS_FILE vortex;
    VORTEX data;
    int room;
    FILE *fd;

    if (TabRead) {
	sprintf(temp, "%d.vex", x);
	makeSysName(vortex, temp, &cfg.netArea);
	if ((fd = fopen(vortex, READ_ANY)) != NULL) {
	    while (fread(&data, sizeof data, 1, fd) >= 1) {
		room = data.vRoomNo;
		if (roomTab[room].rtflags.INUSE &&
			    roomTab[room].rtgen == data.vGen) {
		    printf("%s ", roomTab[room].rtname);
		    printf("Last message is %lu, ", data.vHighest);
		    printf("Date on last message was %s.\n",
						LastOn(data.vLastDate, FALSE));
		}
	    }
	    fclose(fd);
	}
    }
}

/*
 * crashout()
 *
 * Fatal error handler.
 */
void crashout(char *str)
{
    exit(printf("%s\n", str));
}
