/*
 *				vasys.c
 *
 * This holds the system dependent code for the virtual room administrator.
 */

/*
 *				history
 *
 * 88Jan25 HAW  Created.
 */

#define SYSTEM_DEPENDENT
#define V_ADMIN 1

#include "ctdl.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
/*#include "sys\stat.h" */

extern VirtualRoom *VRoomTab;

/*
 *				Contents
 */

/*
 * splitF()
 *
 * This function formats format+args to file and console.
 */
void splitIt(char *format, ...)
{
    va_list argptr;
    char garp[1000];
    extern FILE *VaLog;

    va_start(argptr, format);
    vsprintf(garp, format, argptr);
    va_end(argptr);
    printf(garp);
if (strLen(garp) > 999) {
    printf("splitIt overflow!!!!");
    exit(3);
}
    if (VaLog != NULL) {
	fprintf(VaLog, garp);
	fflush(VaLog);
    }
}

/*
 *  CheckDir(char *name, char *msg)
 *
 * This function performs error checking to see if a directory already exists
 * and emits a warning if so.  If it isn't there then it's a fatal error.
 */
void CheckDir(char *name, char *msg)
{
    if (access(name, 0) != 0)
	crashout(msg);
    else
	printf("Warning: %s already existed?\n", name);
}

/*
 * ChkVirtArea()
 *
 * This is the initializer of the virtual room stuff.
 */
void ChkVirtArea()
  {
  if (access("virtual",0) != 0)
    {  /* doesn't exist, create it */
	  if (mkdir("virtual") != 0)
	  CheckDir("virtual", "Couldn't create the 'virtual' directory!");
    };
  }

/*
 * SetVirtAreas()
 *
 * This sets up the subdirectory structure for the given room number.
 */
void SetVirtAreas(int RoomNo)
{
    SYS_FILE sub;

    sPrintf(sub, "%d", RoomNo);

    if (chdir("virtual") != 0) crashout("chdir failure in SetVirtAreas??");
    if (mkdir(sub) != 0) CheckDir(sub, "Couldn't create a subdir in virtual!");
    sPrintf(sub, "%d\\%s", RoomNo, LD_DIR);
    if (mkdir(sub) != 0) CheckDir(sub, "Couldn't create a subdir in virtual!");
    sPrintf(sub, "%d\\%s", RoomNo, LOCAL_DIR);
    if (mkdir(sub) != 0) CheckDir(sub, "Couldn't create a subdir in virtual!");
    chdir("..");
}

/*
 * KillVirtAreas()
 *
 * This kills the subdirectory structure.
 */
void KillVirtAreas(int RoomNo)
{
    char sub[SIZE_SYS_FILE + 30];
    MSG_NUMBER names;

    if (chdir("virtual") != 0) crashout("chdir failure in KillVirtAreas??");
    if (!getYesNo("Kill manually")) {
	for (names = VRoomTab[RoomNo].vrLoLocal;
		names <= VRoomTab[RoomNo].vrHiLocal; names++) {
	    sPrintf(sub, "%d\\%s\\%ld", RoomNo, LOCAL_DIR, names);
	    unlink(sub);
	}

	for (names = VRoomTab[RoomNo].vrLoLD; names <= VRoomTab[RoomNo].vrHiLD;
			names++) {
	    sPrintf(sub, "%d\\%s\\%ld", RoomNo, LD_DIR, names);
	    unlink(sub);
	}

	sPrintf(sub, "%d\\%s", RoomNo, LOCAL_DIR);
	if (rmdir(sub) != 0) crashout("system bug: couldn't remove a dir!");

	sPrintf(sub, "%d\\%s", RoomNo, LD_DIR);
	if (rmdir(sub) != 0) crashout("system bug: couldn't remove a dir!");

	sPrintf(sub, "%d", RoomNo);
	if (rmdir(sub) != 0) crashout("system bug: couldn't remove a dir!");
    }
    chdir("..");
}

