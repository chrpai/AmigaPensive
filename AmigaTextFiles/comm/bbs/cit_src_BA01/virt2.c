/*
*				virt2.c
*
* Virtual room handler for Citadel-86, part 2.
*/
#include "ctdl.h"
/*
*				history
*
* 91Nov15 HAW Restructured.
* 89Apr04 HAW Created.
*/
/*
*				contents
*
*	VNeedCall()		Checks to see if a node needs a call
*	VirtRoomRoutable()	Checks to see if a room is routable
*	SendVirtual()		Sends a virtual room to another system
*	RecVirtualRoom()	Receives a virtual room
*	SetUpForVirtuals()	Gets set up for a virtual room reception
*	DoVirtuals()		Go through all shared virtual rooms for a sys
*	SendVirtualRoom()	Manages sending a virtual room out
*	DumpVRoom()		Dumps a shared virtual room setup on console
*	UnCacheVirtualRoom()	Unsets the cache bit
*	VRNeedCall()		Check to see if there is outgoing for a room
*	VirtualRoomOutgoing()	Same?
*	VirtualRoomMap()	Sends the room/file map for a virtual room
*	CacheVirtualRoom()	Caches a virtual room
*
*/
extern VirtualRoom *VRoomTab;
extern VirtNet     *VirtNetList;
extern char VirtualInUse;
extern int  VirtSize, VNetSize;
extern NetBuffer netBuf;
extern CONFIG    cfg;
extern int       thisNet;
extern FILE      *netLog;
extern char      inNet, netDebug, TrError;
extern char	 *READ_ANY, *APPEND_ANY;
extern NetTable  *netTab;
static int SendVirtualNormalMessages(int VirtIndex, int VirtNo,int (*SendFunc)(int c));
static char VNeedSend(int system, int rover, int which, char NotPeon);
static char VirtualOutgoingMessages(VirtPoint *VirtP);
static int ThrowAll(int which, char *distance, MSG_NUMBER start, MSG_NUMBER end,
int (*SendFunc)(int c));
/*
* VNeedCall()
*
* This function checks to see if list element needs calling.
*/
char VNeedCall(VirtPoint *VirtP)
  {
  #ifndef NO_VIRTUAL_ROOMS
  switch (VGetMode(VirtP->mode))
    {
    /*
    * For active backbone, we assume that we are always called via
    * roomsShared(), which will keep us from calling more than once.
    * Since an Active should always call once, we return TRUE here
    * without actually checking values.
    */
    case ACTIVE_BACKBONE:
    return (char)(inNet == NORMAL_NET);
    case PASS_BACKBONE:   return FALSE;
    case PEON:
    return VirtualOutgoingMessages(VirtP);

    }
  return FALSE;
  #else
  return FALSE;
  #endif

  }
/*
* VirtualOutgoingMessages()
*
* Are there outgoing messages, regardless of caching & room status?
*/
static char VirtualOutgoingMessages(VirtPoint *VirtP)
  {
  if (VirtP->LDSent < VRoomTab[VirtP->WhichVirt].vrHiLD ||
  VGetFA(VirtP->mode))
  return TRUE;
  if (VGetMode(VirtP->mode) != PEON)
  if (VirtP->LocSent < VRoomTab[VirtP->WhichVirt].vrHiLocal)
  return TRUE;
  return FALSE;

  }
/*
* VirtRoomRoutable()
*
* This checks to see if the given room is the one we're looking for as
* specified in the d parameter.
*/
int VirtRoomRoutable(VirtualRoom *room, int system, int index, int which,
void *d)
  {
  RoomSearch *arg;
  arg = d;
  arg->reason = RS_INIT;
  if (strCmpU(room->vrName, arg->Room) == SAMESTRING)
    {
    arg->virtual = TRUE;
    arg->room = which;
    arg->index = index;
    arg->reason = FOUND;
    return ERROR;

    }
  return TRUE;

  }
/*
* SendVirtual()
*
* This function manages sending a room to another system.
*/
int SendVirtual(int VirtIndex, char *d1, char *d2, char *d3)
  {
  char	work[15];
  int		VirtNo, count = 0;
  VirtNo = VirtNetList[thisNet].VirtList[VirtIndex].WhichVirt;
  if (VGetFA(VirtNetList[thisNet].VirtList[VirtIndex].mode))
    {
    sPrintf(work, V_CACHE_END_NAME, VirtIndex);
    if (SendPrepAsNormal(work, &count))
    VUnSetFA(VirtNetList[thisNet].VirtList[VirtIndex].mode);

    }
  splitF(netLog, "Sending %s (virtual) ", VRoomTab[VirtNo].vrName);
  count += SendVirtualNormalMessages(VirtIndex, VirtNo, sendITLchar);
  return count;

  }
/*
* SendVirtualNormalMessages()
*
* Sends normal virtual messages to some destination.
*/
static int SendVirtualNormalMessages(int VirtIndex, int VirtNo,
int (*SendFunc)(int c))
  {
  int		count;
  MSG_NUMBER	StartMsg;
  /* Send all the new LD messages received */
  StartMsg = VirtNetList[thisNet].VirtList[VirtIndex].LDSent;
  count=ThrowAll(VirtNo, LD_DIR, StartMsg, VRoomTab[VirtNo].vrHiLD, SendFunc);
  if (TrError == TRAN_SUCCESS)
  VirtNetList[thisNet].VirtList[VirtIndex].LDSent =
  VRoomTab[VirtNo].vrHiLD;
  if (VGetMode(VirtNetList[thisNet].VirtList[VirtIndex].mode) != PEON)
    {
    StartMsg = VirtNetList[thisNet].VirtList[VirtIndex].LocSent;
    count += ThrowAll(VirtNo, LOCAL_DIR, StartMsg,
    VRoomTab[VirtNo].vrHiLocal, SendFunc);
    if (TrError == TRAN_SUCCESS)
    VirtNetList[thisNet].VirtList[VirtIndex].LocSent =
    VRoomTab[VirtNo].vrHiLocal;

    }
  VRoomTab[VirtNo].vrChanged |= SENT_DATA;
  return count;

  }
/*
* ThrowAll()
*
* This function sends a virtual room to another system.
*/
static int ThrowAll(int which, char *distance, MSG_NUMBER start, MSG_NUMBER end,
int (*SendFunc)(int c))
  {
  #ifndef NO_VIRTUAL_ROOMS
  MSG_NUMBER  rover;
  int		count=0;
  char	fn[100];
  extern FILE *netMisc;
  extern PROTO_TABLE Table[];
  extern int	TransProtocol;
  for (rover = start + 1; rover <= end; rover++)
    {
    CreateVAName(fn, which, distance, rover);
    if ((netMisc = safeopen(fn, READ_ANY)) != NULL)
      {
      while (getMessage(getNetChar, TRUE, FALSE, TRUE))
        {
        count++;
        prNetStyle(1, SendFunc, TRUE, "");

        }
      fclose(netMisc);

      }

    }
  return count;
  #else
  return 0;
  #endif

  }
/*
* RecVirtualRoom()
*
* This function receives a virtual room directly from another system.
*/
int RecVirtualRoom(int VirtIndex, char ReplyFirst)
  {
  #ifndef NO_VIRTUAL_ROOMS
  int		VirtNo;
  char	fn[50];
  extern FILE *upfd;
  extern char *W_R_ANY;
  SetUpForVirtuals(VirtIndex, &VirtNo, fn);
  splitF(netLog, "Receiving %s (virtual)\n", VRoomTab[VirtNo].vrName);
  return (ITL_StartRecMsgs(fn, ReplyFirst, TRUE, NULL) == ITL_SUCCESS)
  ? TRUE : FALSE;
  #else
  return TRUE;
  #endif

  }
/*
* SetUpForVirtuals()
*
* This sets up a filename for a virtual room.
*/
void SetUpForVirtuals(int VirtIndex, int *VirtNo, char *fn)
  {
  MSG_NUMBER  rover;
  char *distance;
  *VirtNo = VirtNetList[thisNet].VirtList[VirtIndex].WhichVirt;
  if (VGetMode(VirtNetList[thisNet].VirtList[VirtIndex].mode) != PEON)
    {
    distance = LD_DIR;
    rover = VRoomTab[*VirtNo].vrHiLD + 1l;
    VRoomTab[*VirtNo].vrChanged |= LD_CHANGE;

    }
  else
    {
    distance = LOCAL_DIR;
    rover = VRoomTab[*VirtNo].vrHiLocal + 1l;
    VRoomTab[*VirtNo].vrChanged |= LOC_CHANGE;

    }
  CreateVAName(fn, *VirtNo, distance, rover);

  }
static char VirtualCheck(int system, int rover, int which, int *cmd);
/*
* DoVirtuals()
*
* This function sends rooms to another system, if needed.
*/
void DoVirtuals(int system,
int (*virtfunc)(VirtualRoom *room, int system, int index, int which, void *d),
void *d)
  {
  #ifndef NO_VIRTUAL_ROOMS
  int rover, which;
  if (!VirtualInUse) return ;
  for (rover = 0; rover < VIRT_LIMIT; rover++)
    {
    which = VirtNetList[system].VirtList[rover].WhichVirt;
    if (which >= VirtSize || which < 0 || !VRoomInuse(which))
      {
      VirtNetList[system].VirtList[rover].WhichVirt = -1;
      continue;

      }
    if ((*virtfunc)(VRoomTab + which, system, rover, which, d) == ERROR)
    break;

    }
  #endif

  }
/*
* SendVirtualRoom()
*
* This function transfers a virtual room's contents to another system.
*/
int SendVirtualRoom(VirtualRoom *room, int system, int index, int which,void *d)
  {
  char doit;
  int  cmd;
  extern char		MassTransferSent;
  if (!gotCarrier()) return ERROR;
  doit = VirtualCheck(system, index, which, &cmd);
  if (doit && !MassTransferSent)
    {
    ITL_optimize(TRUE);
    findAndSend(cmd, NULL, NULL, NULL, index, SendVirtual,
    room->vrName, RecVirtualRoom);

    }
  return TRUE;

  }
/*
* VirtualCheck()
*
* This handles deciding if a room should be sent.
*/
static char VirtualCheck(int system, int index, int which, int *cmd)
  {
  char doit;
  extern char inReceive;
  doit = TRUE;
  switch (VGetMode(VirtNetList[system].VirtList[index].mode))
    {
    case PEON:
    *cmd = NET_ROOM;
    if (VirtNetList[system].VirtList[index].LDSent >=
    VRoomTab[which].vrHiLD &&
    !VGetFA(VirtNetList[system].VirtList[index].mode))
    doit = FALSE;
    break;
    case PASS_BACKBONE:
    if (!inReceive || VRoomTab[which].vrChanged & SENT_DATA)
    doit = FALSE;
    else
    *cmd = NET_ROOM;
    break;
    case ACTIVE_BACKBONE:
    *cmd = (netBuf.nbflags.local) ? NET_ROOM : NET_ROUTE_ROOM;
    break;
    default:
    splitF(netLog, "Error in virtuals for %s!\n", VRoomTab[which].vrName);
    doit = ERROR;

    }
  return doit;

  }
/*
* VNeedSend()
*
* Determines if we need to send messages in a virtual regardless of the
* mode, except we know it's a backbone situation (so check both local and
* backbones).
*/
static char VNeedSend(int system, int rover, int which, char NotPeon)
  {
  return(char) (VirtNetList[system].VirtList[rover].LDSent <
  VRoomTab[which].vrHiLD ||
  (NotPeon && VirtNetList[system].VirtList[rover].LocSent <
  VRoomTab[which].vrHiLocal));

  }
/*
* DumpVRoom()
*
* This function dumps information concerning a virtual room.
*/
int DumpVRoom(VirtualRoom *room, int system, int index, int which, void *d)
  {
  #ifndef NO_VIRTUAL_ROOMS
  mPrintf("%s: ", room->vrName);
  switch (VGetMode(VirtNetList[system].VirtList[index].mode))
    {
    case PEON: mPrintf("PEON"); break;
    case ACTIVE_BACKBONE:
    mPrintf("ACTIVE BACKBONE"); break;
    case PASS_BACKBONE:
    mPrintf("PASSIVE BACKBONE"); break;

    }
  mPrintf(" relationship (last bb sent %ld, hi bb %ld",
  VirtNetList[system].VirtList[index].LDSent, room->vrHiLD);
  if (VGetMode(VirtNetList[system].VirtList[index].mode) != PEON)
    {
    mPrintf(", last peon sent %ld, hi peon %ld",
    VirtNetList[system].VirtList[index].LocSent, room->vrHiLocal);

    }
  mPrintf("):%d", VirtNetList[system].VirtList[index].mode);
  if (VGetFA(VirtNetList[system].VirtList[index].mode))
  mPrintf(" *");
  mPrintf("\n ");
  return TRUE;
  #endif

  }
/*
* UnCacheVirtualRoom()
*
* Unset the flag indicating the room is cached for this system.
*/
int UnCacheVirtualRoom(VirtualRoom *room, int system, int index, int which,
void *d)
  {
  VUnSetFA(VirtNetList[system].VirtList[index].mode);
  return TRUE;

  }
/*
* VRNeedCall()
*
* This discerns if the given virtual room has outgoing.
*/
int VRNeedCall(VirtualRoom *room, int system, int index, int which,
char *d)
  {
  if (VNeedCall(VirtNetList[system].VirtList + index))
    {
    *d = TRUE;
    return ERROR;

    }
  return TRUE;

  }
/*
* VirtualRoomOutgoing()
*
* This checks to see if the given virtual room has outgoing stuff, regardless
* of room backbone status (we're already connected).
*/
int VirtualRoomOutgoing(VirtualRoom *room, int system, int index, int which,
char *d)
  {
  if (VirtualOutgoingMessages(VirtNetList[system].VirtList + index))
    {
    *d = TRUE;
    return ERROR;

    }
  return TRUE;

  }
/*
* VirtualRoomMap()
*
* This sends the name and cache file name of a virtual room.
*/
int VirtualRoomMap(VirtualRoom *room, int system, int index, int which,
void *d)
  {
  char work[20], NotPeon;
  NotPeon = (VGetMode(VirtNetList[system].VirtList[index].mode) != PEON);
  if (VGetFA(VirtNetList[system].VirtList[index].mode) ||
  VNeedSend(system, index, which, NotPeon))
    {
    ITL_Line(room->vrName);
    sprintf(work, V_CACHE_END_NAME, index);
    ITL_Line(work);

    }
  return TRUE;

  }
/*
* CacheVirtualRoom()
*
* This function should cache a virtual room.
*/
int CacheVirtualRoom(VirtualRoom *room, int sys, int index, int which, void *d)
  {
  int		count;
  char	work[15], NotPeon;
  char	tempNm[3*NAMESIZE];
  extern char PrTransmit, CacheUpdated;
  PrTransmit = FALSE;
  NotPeon = (VGetMode(VirtNetList[sys].VirtList[index].mode) != PEON);
  if (VNeedSend(sys, index, which, NotPeon))
    {
    if (sys != thisNet)
    getNet(sys, &netBuf);
    sPrintf(work, V_CACHE_END_NAME, index);
    NetCacheName(tempNm, thisNet, work);
    if ((upfd = safeopen(tempNm, APPEND_ANY)) != NULL)
      {
      count = SendVirtualNormalMessages(index, which, putFLChar);
      fclose(upfd);
      if (count == 0 &&
      !VGetFA(VirtNetList[sys].VirtList[index].mode))
      unlink(tempNm);
      else
        {
        VSetFA(VirtNetList[sys].VirtList[index].mode);
        if (count) CacheUpdated = TRUE;

        }

      }

    }
  PrTransmit = TRUE;
  return TRUE;

  }
