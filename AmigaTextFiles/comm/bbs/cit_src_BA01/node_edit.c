#include "ctdl.h"

extern CONFIG    cfg;
extern NetBuffer netBuf, netTemp;
extern FILE      *netfl;
extern int       thisNet;
extern NetTable  *netTab;
extern aRoom     roomBuf;		/* room buffer			*/
extern FILE      *roomfl;		/* file descriptor for rooms	*/
extern int       thisRoom;		/* room currently in roomBuf	*/
extern rTable	 *roomTab;		/* RAM index of rooms		*/
extern VirtualRoom *VRoomTab;
extern VirtNet     *VirtNetList;
extern char VirtualInUse;


void main()
  {
  printf(" Node editor Version 1.00\n");
  printf("...Initializing\n"
  if (!readSysTab(FALSE, TRUE))
   {
   printf("Couldn't read system table.\n");
   exit(10);
   }
  InitSystem();   /* open the database */

  }
/*
* InitSystem()
*
* This function will open up appropriate data base files, and initialize
* buffers.
*/
void InitSystem()
  {
  SYS_FILE name;
  extern FILE *logfl;
  extern logBuffer logTmp, logBuf;
  /* Open database files. */
  makeSysName(name, "ctdlnet.sys", &cfg.netArea);
  openFile(name, &netfl);
  makeSysName(name, "ctdllog.sys", &cfg.logArea);
  openFile(name, &logfl);
  initLogBuf(&logTmp);
  initLogBuf(&logBuf);
  InitMsgBase();
  /* Now initialize data buffers. */
  initNetBuf(&netBuf);
  initNetBuf(&netTemp);
  initNetBuf(&nBuf1);
  initNetBuf(&nBuf2);
  makeSysName(name, "ctdlalsi.sys", &cfg.roomArea);
  MakeList(&FwdAliasii, name, upfd);

  }
