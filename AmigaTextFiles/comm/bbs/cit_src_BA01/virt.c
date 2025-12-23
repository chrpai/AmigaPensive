/*                               virt.c
*
* Virtual room handler for Citadel-86.
*/
#include "ctdl.h"
/*
*                               history
*
* 91Nov15 HAW Restructured.
* 88Apr01 HAW Final cleanup before release.
*/
/*
*                               contents
*
*       VirtInit()              Initialize the virtual room stuff
*       InitVNode()             Init for a new node
*       UpdVirtStuff()          Updates the virtual stuff
*       VirtSummary()           Does real work of updating virtual stuff
*
*/
VirtualRoom *VRoomTab;
VirtNet     *VirtNetList;
char VirtualInUse = FALSE;
int  VirtSize, VNetSize;
extern NetBuffer netBuf;
extern CONFIG    cfg;
extern int       thisNet;
extern FILE      *netLog;
extern char      netDebug, TrError;
/*
* VirtInit()
*
* This function initializes the virtual room stuff, if available.  The virtual
* room stuff is created by the virtadmn utility, so this is a non-fatal,
* non-warning failure -- the sysop doesn't even know that this stuff ain't
* here.
*/
void VirtInit()
  {
  #ifndef NO_VIRTUAL_ROOMS
  FILE *fd;
  SYS_FILE fn;
  long size;
  extern char *R_W_ANY;
  makeVASysName(fn, "ctdlvrm.sys");
  if ((fd = safeopen(fn, R_W_ANY)) == NULL)
  return;       /* Depend on initializer to handle VirtualInUse */
  totalBytes(&size, fd);
  VRoomTab = (VirtualRoom *) GetDynamic((int) size);
  fread((char *) VRoomTab, (int) size, 1, fd);
  fclose(fd);
  VirtSize = (int) size / sizeof *VRoomTab;
  VirtualInUse = TRUE;
  makeVASysName(fn, "ctdlvnet.sys");
  if ((fd = safeopen(fn, R_W_ANY)) == NULL)
  crashout("ctdlvnet.sys is missing!!");
  totalBytes(&size, fd);
  VirtNetList = (VirtNet *) GetDynamic((int) size);
  VNetSize = size / sizeof *VirtNetList;
  fread(VirtNetList, (int) size, 1, fd);
  fclose(fd);
  #endif

  }
/*
* InitVNode()
*
* When a new node is added to the net list, this function initializes the
* virtual part of the new node.  This consists of enlarging the vnet table
* size if necessary, and initializing the room pointers to -1, indicating
* that none of them are in use.  Finally, the virtual tables on disk are
* updated.
*/
void InitVNode(int slot)
  {
  #ifndef NO_VIRTUAL_ROOMS
  int rover;
  if (!VirtualInUse) return ;
  if (slot >= VNetSize)
    {
    VirtNetList = realloc(VirtNetList, (slot+1) * sizeof *VirtNetList);
    VNetSize = slot + 1;

    }
  for (rover = 0; rover < VIRT_LIMIT; rover++)
  VirtNetList[slot].VirtList[rover].WhichVirt = -1;
  UpdVirtStuff();
  #endif

  }
/*
* UpdVirtStuff()
*
* This function updates the virtual data on disk.
*/
void UpdVirtStuff()
  {
  #ifndef NO_VIRTUAL_ROOMS
  FILE *fd;
  SYS_FILE fn;
  extern char *R_W_ANY;
  if (!VirtualInUse) return ;
  VirtSummary();
  makeVASysName(fn, "ctdlvrm.sys");
  if ((fd = safeopen(fn, R_W_ANY)) == NULL)
  crashout("ctdlvrm.sys is missing!");
  fwrite(VRoomTab, VirtSize, sizeof *VRoomTab, fd);
  fclose(fd);
  makeVASysName(fn, "ctdlvnet.sys");
  if ((fd = safeopen(fn, R_W_ANY)) == NULL)
  crashout("ctdlvnet.sys is missing!!");
  fwrite(VirtNetList, cfg.netSize, sizeof *VirtNetList,  fd);
  fclose(fd);
  #endif

  }
/*
* VirtSummary()
*
* This function handles post-call cleanup.
*/
void VirtSummary()
  {
  #ifndef NO_VIRTUAL_ROOMS
  int rover, another;
  for (rover = 0; rover < VirtSize; rover++)
    {
    if (VRoomTab[rover].vrChanged & LD_CHANGE)
      {
      VRoomTab[rover].vrHiLD++;
      for (another = 0; another < VIRT_LIMIT; another++)
      if (VirtNetList[thisNet].VirtList[another].WhichVirt == rover)
        {
        VirtNetList[thisNet].VirtList[another].LDSent =
        VRoomTab[rover].vrHiLD;
        break;

        }
      /*if (another == VIRT_LIMIT) splitF(netLog, "Unexpected mistake 1 in LDVirtSummary!\n"); */

      }
    if (VRoomTab[rover].vrChanged & LOC_CHANGE)
      {
      VRoomTab[rover].vrHiLocal++;
      for (another = 0; another < VIRT_LIMIT; another++)
      if (VirtNetList[thisNet].VirtList[another].WhichVirt == rover)

        {
        VirtNetList[thisNet].VirtList[another].LocSent =
        VRoomTab[rover].vrHiLocal;
        break;

        }
      /*if (another == VIRT_LIMIT) splitF(netLog, "Unexpected mistake 2 in LocVirtSummary!\n"); */

      }
    VRoomTab[rover].vrChanged = 0;

    }
  #endif

  }
