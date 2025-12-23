/*
*       netcache.c
*
* Networking functions for handling network cache (fast transfers).
*/
/*
*       history
*
* 91Sep20 HAW Created.
*/
#include "ctdl.h"
#include <dos.h>
#define MAP_FILE  "map.$$$"
#define FAST_TEMPLATE "fast$$$.%s"
#define FAST_RECEIVE  "fast_tr.%s"
#define NETMSGS   "netmsgs.%s"
/*
*       contents
*
*/
char RecMassTransfer;
char MassTransferSent;
char Zmodem = 0;
static int MTCompVal;


extern char logNetResults;
extern char netDebug;


extern CONFIG    cfg;   /* Lots an lots of variables    */
extern char      *SR_Sent;
extern MessageBuffer   msgBuf;
extern char      inReceive;
extern NetBuffer netTemp;
extern char      inNet;
extern NetBuffer netBuf;
extern FILE  *netLog;
extern rTable    *roomTab;
extern FILE  *upfd;
extern int   callSlot;
extern NetTable  *netTab;
extern int       thisNet;
extern char  *READ_ANY, *APPEND_ANY;
/*
* FileMap
*
* This structure is used to map between a filename and the room it's
* carrying.
*/
typedef struct
  {
  char *FileName;
  char *RoomName;

  }
FileMap;
/*
* Mappings
*
* This is a list of mappings for our current work.
*/
static void FreeMapping();
SListBase Mappings =
  {
  NULL, NULL, NULL, FreeMapping, NULL

  };
/*
* SendFastTransfer()
*
* We want to use Facility 21, the mass transfer of messages.
*/
char SendFastTransfer()
  {
  long   size;
  extern long netBytes;
  extern AN_UNSIGNED RecBuf[SECTSIZE + 5];
  struct cmd_data cmds;
  char BaseArcName[15], Outgoing;
  int protocol;
  char ArcFileName[100];
  char CacheDir[70];
  int RoomOutgoing(SharedRoom *room, int system, int index, int roomslot,  char *d);
  int UnCacheRoom(SharedRoom *room, int system, int index, int roomslot,  void *d);

  if ( logNetResults && netDebug )splitF(netLog, "SendFastTransfer:%sabled\n", netBuf.nbflags.MassTransfer ? " En" : "Dis");
  if (!netBuf.nbflags.MassTransfer)    return FALSE;
  if (!netBuf.nbflags.local &&
  !(netBuf.nbflags.spine || inReceive))
    {
    if( logNetResults )
      {
      if( netDebug )
        {
        if( !netBuf.nbflags.local )splitF(netLog, "SendFastTransfer:FALSE, Not Local\n");
        if(  netBuf.nbflags.spine )splitF(netLog, "SendFastTransfer:FALSE, A Spine\n");
        if(             inReceive )splitF(netLog, "SendFastTransfer:FALSE, We have been called\n");
        };
      splitF(netLog, "Mass Transfer flag ignored\n");
      };
    return FALSE;

    }
  if (!CompAvailable(GetCompression(thisNet)))
    {
    if (netDebug)splitF(netLog, "SendFastTransfer:FALSE, No compression set\n");
    return FALSE;

    }
  /* do we have rooms to send??? */
  Outgoing = FALSE;
  EachSharedRoom(thisNet, RoomOutgoing, VirtualRoomOutgoing, &Outgoing);
  if (!Outgoing)
    {
    MassTransferSent = TRUE;
    EachSharedRoom(thisNet, UnCacheRoom, UnCacheVirtualRoom, NULL);
    if ( logNetResults && netDebug )splitF(netLog, "SendFastTransfer:FALSE, No rooms to send\n");
    return FALSE;

    }
  zero_struct(cmds);
  sPrintf(cmds.fields[0], "%d", GetCompression(thisNet));
  sPrintf(cmds.fields[1], "%d", ZM_PROTOCOL);
  strCpy(cmds.fields[2], "0");
  strCpy(cmds.fields[3], "-1");
  cmds.command = FAST_MSGS;
  protocol = ZM_PROTOCOL;
  if (!Zmodem || FindProtocolCode(Zmodem, FALSE) == -1 ||
  !sendNetCommand(&cmds, "mt"))
    {
    sPrintf(cmds.fields[1], "%d", DEFAULT_PROTOCOL);
    protocol = DEFAULT_PROTOCOL;
    if (!sendNetCommand(&cmds, "mt"))
      {
      if( logNetResults && netDebug )splitF(netLog, "Fast Transfer refused\n");
      return FALSE;

      }

    }
  if (SendMapFile())
    {
    if (!MapFileAccepted())
      {
      if ( logNetResults && netDebug )splitF(netLog, "SendFastTransfer:FALSE, MAP File problem\n");
      return FALSE;

      }
    CacheSystem(thisNet, FALSE);
    outMod(ACK);
    sPrintf(BaseArcName, NETMSGS, CompExtension(GetCompression(thisNet)));
    NetCacheName(ArcFileName, thisNet, BaseArcName);
    if( logNetResults && netDebug )splitF(netLog, "Mass Transfer: %s\n",ArcFileName);
    if (protocol == DEFAULT_PROTOCOL)
      SendHostFile(ArcFileName);
    else
      {
      ExternalTransfer(FindProtocolCode(Zmodem, FALSE), ArcFileName);
      while (MIReady()) inp();
      pause(80);   /* keep the other system from eating our NAK */

      }
    if (gotCarrier())
      {
      ITL_Receive(NULL, FALSE, TRUE, putFLChar, fclose);
      if (RecBuf[0] == BAD || !gotCarrier())
        {
        if ( logNetResults && netDebug )splitF(netLog, "SendFastTransfer:FALSE, lost carrier, or bad transmission\n");
        return FALSE;
        };
      MassTransferSent = TRUE;
      EachSharedRoom(thisNet, UnCacheRoom, UnCacheVirtualRoom, NULL);
      MakeNetCacheName(CacheDir, thisNet);
      if (logNetResults && netDebug)splitF(netLog, "CacheDir:%s\n",CacheDir);
      if (ChangeToCacheDir(CacheDir) == 0)
        {
        netBytes = 0l;
        wildCard(getSize, BaseArcName, FALSE, "", FALSE);
        size = netBytes;
        netBytes = 0l;
        wildCard(getSize, CACHED_FILES, FALSE, "", FALSE);
        wildCard(DelFile, ALL_FILES, FALSE, "", FALSE);
        if( logNetResults && netDebug )splitF(netLog, "MT: %ld => %ld\n", netBytes, size);

        }
      homeSpace();
      return TRUE;

      }

    }
  if ( logNetResults && netDebug )splitF(netLog, "SendFastTransfer:FALSE, problem with sendmapfile()\n");
  return FALSE;

  }
/*
* RoomOutgoing()
*
* This is responsible for deciding if the given shared room has outgoing
* material in a room eligible for mass transfers.
*/
int RoomOutgoing(SharedRoom *room, int system, int index, int roomslot,
char *d)
  {
  if (strLen(roomTab[roomslot].rtname) && HasOutgoing(system, index))
    {
    *d = TRUE;
    return ERROR;

    }
  return TRUE;

  }
/*
* SendMapFile()
*
* This sends a file containing the names of rooms to share and the filenames
* in the upcoming archive file to map the rooms to.  Format is a series of
* pairs of lines (UNIX style), first the roomname and then the filename.
* blank line signals end of file.
*/
char SendMapFile()
  {
  int NormalRoomMap(SharedRoom *room, int system, int index, int roomslot,
  void *d);
  if (!ITL_Send(STARTUP)) return FALSE;
  EachSharedRoom(thisNet, NormalRoomMap, VirtualRoomMap, NULL);
  ITL_Line("");
  return ITL_Send(FINISH);

  }
/*
* NormalRoomMap()
*
* This sends the given room's name and cache file name if there is an
* outgoing file pending or one will be built.
*/
int NormalRoomMap(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  char work[20];
  if (strLen(roomTab[roomslot].rtname) && HasOutgoing(system, index))
    {
    ITL_Line(roomTab[roomslot].rtname);
    sPrintf(work, CACHE_END_NAME, roomslot);
    ITL_Line(work);

    }
  return TRUE;

  }
/*
* ITL_Line()
*
* Sends the specified line to the SendITLchar function.
*/
void ITL_Line(char *data)
  {
  while (*data)
    {
    sendITLchar((int) *data);
    data++;

    }
  sendITLchar((int) '\n');

  }
/*
* DelFile()
*
* This function kills the named file.
*/
void DelFile(DirEntry *f)
  {
  unlink(f->unambig);

  }
/*
* MapFileAccepted()
*
* This function is responsible for discovering if the map file is acceptable.
*/
char MapFileAccepted()
  {
  FILE *fd;
  char toReturn = TRUE, work[NAMESIZE + 5];
  ToTempArea();
  if (ITL_Receive("moo", FALSE, TRUE, putFLChar, fclose) == ITL_SUCCESS)
    {
    if ((fd = safeopen("moo", READ_ANY)) == NULL)
    toReturn = FALSE;
    else
      {
      GetAString(work, sizeof work, fd);
      if (strLen(work) != 0) toReturn = FALSE;
      fclose(fd);
      unlink("moo");

      }

    }
  else toReturn = FALSE;
  KillTempArea();
  return toReturn;

  }
#define TOP_COMP  4
/*
* netFastTran()
*
* Accept an archive of files?
*/
void netFastTran(struct cmd_data *cmds)
  {
  char CheckMap(char *fn, char talk);
  int MTProtocol;
  SYS_FILE fn;
  char work[15];
  MTCompVal  = atoi(cmds->fields[0]);
  MTProtocol = atoi(cmds->fields[1]);
  if (RecMassTransfer || MTCompVal > TOP_COMP || MTCompVal < 0 ||
  !DeCompAvailable(MTCompVal))
    {
    reply(BAD, "nope");
    return;

    }
  if (MTProtocol == ZM_PROTOCOL)
    {
    if (!Zmodem || FindProtocolCode(Zmodem, TRUE) == -1)
      {
      reply(BAD, "No protocol");
      return;

      }

    }
  else if (MTProtocol != DEFAULT_PROTOCOL)
    {
    reply(BAD, "No protocol");
    return;

    }
  if (strCmp(cmds->fields[2], "0") != SAMESTRING ||
  strCmp(cmds->fields[3], "-1") != SAMESTRING)
    {
    reply(BAD, " Field 2 & Field 3 no match");
    return;

    }
  reply(GOOD, "");
  makeSysName(fn, MAP_FILE, &cfg.netArea);
  if ( logNetResults && netDebug)splitF(netLog, "MAP File:%s\n",fn);
  if (ITL_Receive(fn, FALSE, TRUE, putFLChar, fclose) != ITL_SUCCESS)
    {
    return ;

    }
  KillList(&Mappings);
  if (CheckMap(fn, TRUE))
    {
    if( logNetResults )splitF(netLog, "Accepting mass transfer %d\n", MTProtocol);
    if (receive(120) == NAK) return;

    sPrintf(work, FAST_RECEIVE, CompExtension(MTCompVal));  /*** bug fix ***/
/*    makeSysName(fn, work, &cfg.netArea); */
    ChangeToCacheDir((char *)&cfg.netArea);           /*** bug fix ***/

    if ( logNetResults && netDebug )splitF(netLog, "AMT File(netarea):%s\n",work);
    if (MTProtocol == DEFAULT_PROTOCOL)
      {
      if ( logNetResults && netDebug )splitF(netLog, "Protocol: DEFAULT\n",work);
      if (ITL_Receive(work,FALSE,TRUE, putFLChar, fclose) != ITL_SUCCESS)
        {
        return ;

        }

      }
    else if (MTProtocol == ZM_PROTOCOL)
      {
      if ( logNetResults && netDebug )splitF(netLog, "Protocol: Zmodem/External\n",work);
      ExternalTransfer(FindProtocolCode(Zmodem, TRUE), work);
      while (MIReady()) inp();

      }
    if (access(fn, 0) == 0 )
      {
      reply(GOOD, "");
      /* recovery setup in case of power failure, etc */
      sPrintf(fn, "%s:%d", FAST_RECEIVE, MTCompVal);
      UpdateRecoveryFile(fn);
      RecMassTransfer = TRUE;
      }
    else
      {
      reply(BAD, "No file");
      if( logNetResults )splitF(netLog, "File not received:%s\n",fn);
      makeSysName(fn, MAP_FILE, &cfg.netArea);
      unlink(fn);

      }

    }
  homeSpace();
  }
/*
* CheckMap()
*
* Checks the map of room names to see if we refuse any.
*/
char CheckMap(char *fn, char talk)
  {
  FILE *fd;
  char work[2 * NAMESIZE];
  char toReturn = TRUE, bad;
  FileMap *data;
  RoomSearch arg;
  if (talk && !ITL_Send(STARTUP)) return FALSE;
  if ((fd = safeopen(fn, READ_ANY)) != NULL)
    {
    while (GetAString(work, sizeof work, fd) != NULL)
      {
      bad = TRUE;
      if (strLen(work) == 0) break;
      if (strLen(work) < NAMESIZE)
        {
        strCpy(arg.Room, work);
        if (RoomRoutable(&arg))
          {
          bad = FALSE;
          data = GetDynamic(sizeof *data);
          data->RoomName = strdup(work);
          GetAString(work, sizeof work, fd);
          data->FileName = strdup(work);
          if ( logNetResults && netDebug )splitF(netLog, "MAP Data:RoomName:%s FileName:%s\n"
          ,data->RoomName, data->FileName);
          AddData(&Mappings, data, NULL, FALSE);

          }

        }
      /* this form lets us respond to several errors with one chunk of code */
      if (bad)
        {
        toReturn = FALSE;
        if (talk) ITL_Line(work);
        GetAString(work, sizeof work, fd);

        }

      }
    fclose(fd);

    }
  else
    {
    printf("Couldn't open %s!\n", fn);
    if (talk) ITL_Line("moo");
    toReturn = FALSE;

    }
  if (talk)
    {
    ITL_Line("");
    ITL_Send(FINISH);

    }
  return toReturn;

  }
/*
* FreeMapping()
*
* Frees a mapping structure.
*/
static void FreeMapping(FileMap *data)
  {
  free(data->FileName);
  free(data->RoomName);
  free(data);

  }
/*
* KillCacheFiles()
*
* This function kills all cache information concerning a node.  It is called
* when a node is deleted from the nodelist, NOT when caching is simply turned
* off.
*/
void KillCacheFiles(int which)
  {
  char ArcFileName[100];
  MakeNetCacheName(ArcFileName, which);
  /* we check this due to the statement after - kill all files! */
  if (ChangeToCacheDir(ArcFileName) == 0)
  wildCard(DelFile, ALL_FILES, FALSE, "", FALSE);
  homeSpace();
  rmdir(ArcFileName);

  }
/*
* ReadFastFiles()
*
* This function reads in all of the messages from an Arc file.
*/
void ReadFastFiles()
  {
  SYS_FILE fn;
  char work[15];
  void EatMsgFile();
  if (!RecMassTransfer) return;
  RecMassTransfer = FALSE;
  sprintf(work, FAST_RECEIVE, CompExtension(MTCompVal));
  makeSysName(fn, work, &cfg.netArea);
  if ( logNetResults && netDebug)splitF(netLog, "RFF:%s(netarea)\n",fn);
  NetDeCompress(MTCompVal, fn);
  RunList(&Mappings, EatMsgFile);
  KillNetDeCompress();
  if (!cfg.BoolFlags.debug) unlink(fn);
  makeSysName(fn, MAP_FILE, &cfg.netArea);
  if (!cfg.BoolFlags.debug) unlink(fn);
  KillList(&Mappings);
  homeSpace();
  }
/*
* EatMsgFile()
*
* This eats a message file extracted from an archive file.
*/
void EatMsgFile(FileMap *data)
  {
  char fn[80];
  char vfn[70];
  int VirtNo;
  RoomSearch arg;
  extern char *SharingRefusal[];
  arg.reason = NO_ROOM;
  strCpy(arg.Room, data->RoomName);
  if (!RoomRoutable(&arg))
    {
    if( logNetResults )splitF(netLog, "Ooops - can't find %s\n", data->RoomName);
    return;

    }
  if (arg.virtual)
    {
    SetUpForVirtuals(arg.index, &VirtNo, vfn);
    MakeDeCompressedFilename(fn, data->FileName);
    VirtualCopyFileToFile(fn, vfn);

    }
  else
    {
    MakeDeCompressedFilename(fn, data->FileName);
    ReadNetRoomFile(arg.index, fn);

    }

  }
/*
* CacheMessages()
*
* This function is tasked with building cache files as needed.  We loop
* through all the systems in the system list.  Those for which the
* MassTransfer flag is active will cause the system to see if there are
* any messages in the message base that are not in the cache for that
* system.  If any are found, the messages are added to their respective
* files (or create as necessary) and then the compression program is run for
* that particular cache.  Flags must be set appropriately, of course.
*/
char CacheUpdated;
void CacheMessages(MULTI_NET_DATA whichNets, char VirtOnly)
  {
  int rover;
  for (rover = 0; rover < cfg.netSize; rover++)
    {
    if ((netTab[rover].ntMemberNets & whichNets))
    CacheSystem(rover, VirtOnly);

    }

  }
/*
* CacheSystem()
*
* This function caches a single system.  It is separated from CacheMessages()
* so we can cache systems on an individual basis.
*/
void CacheSystem(int system, char VirtOnly)
  {
  int CacheRoom(SharedRoom *room, int system, int index, int roomslot, void *d);
  extern void (*NetPrintTarget)(char *format, ...);
  char ArcFileName[60], Files[60], BaseName[40];
  if (!netTab[system].ntflags.MassTransfer) return;
  if (!gotCarrier()) DisableModem(TRUE);
  CacheUpdated = FALSE;
  NetPrintTarget = ToFile;
  EachSharedRoom(system, (VirtOnly) ? NULL : CacheRoom,
  CacheVirtualRoom, NULL);
  if (CacheUpdated)
    {
    putNet(system, &netBuf);
    if (CompAvailable(GetCompression(system)))
      {
      sPrintf(BaseName, NETMSGS, CompExtension(GetCompression(thisNet)));
      NetCacheName(ArcFileName, system, BaseName);
      if ( logNetResults && netDebug )splitF(netLog, "ArcFileName:%s\n",ArcFileName);
      NetCacheName(Files, system, CACHED_FILES);
      if ( logNetResults && netDebug )splitF(netLog, "Files:%s\n",Files);
      Compress(GetCompression(system), Files, ArcFileName);
      if (access(ArcFileName, 0) != 0)
        {
        sPrintf(msgBuf.mbtext, "Compress failed for %s?", netBuf.netName);
        if( logNetResults )splitF(netLog, "ERROR: %s\n", msgBuf.mbtext);
        aideMessage("Net Aide", FALSE);

        }

      }
    UpdVirtStuff();

    }
  if (!gotCarrier()) EnableModem(TRUE);
  NetPrintTarget = mTrPrintf;

  }
/*
* CacheRoom()
*
* This caches a room's messages as necessary.
*/
int CacheRoom(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  extern char PrTransmit;
  char oldNet;
  int MsgCount;
  extern NetInfo NetStyle;
  char work[10], tempNm[3*NAMESIZE], commnd, doit, *name;
  if (roomTab[roomslot].rtlastMessage > room->lastMess &&
  strLen(roomTab[roomslot].rtname))
    {
    if (thisNet != system)
    getNet(system, &netBuf);
    Addressing(system, index, &commnd, &NetStyle.addr1, &NetStyle.addr2,
    &NetStyle.addr3, &name, &doit);
    NetStyle.sendfunc = putFLChar;
    PrTransmit = FALSE;
    sPrintf(work, CACHE_END_NAME, roomslot);
    NetCacheName(tempNm, system, work);
    if ((upfd = safeopen(tempNm, APPEND_ANY)) != NULL)
      {
      oldNet = inNet;
      inNet = NET_CACHE;
      MsgCount = showMessages(NEWoNLY, FALSE,
      room->lastMess, NetRoute);
      inNet = oldNet;
      fclose(upfd);
      SetHighValues(index);
      if (MsgCount == 0 && !GetFA(room->mode))
      unlink(tempNm);
      else if (MsgCount != 0)
        {
        SetFA(netBuf.netRooms[index].mode);
        CacheUpdated = TRUE;

        }

      }
    PrTransmit = TRUE;

    }
  return TRUE;

  }
/*
* RecoverMassTransfer()
*
* This function is charged with recovering a mass transfer file that was
* not processed in the last net session due to a crash of some sort.
*/
void RecoverMassTransfer(char *line)
  {
  SYS_FILE fn;
  char *colon;
  makeSysName(fn, MAP_FILE, &cfg.netArea);
  RecMassTransfer = TRUE;
  if ( logNetResults && netDebug)splitF(netLog, "Recover from:%s\n",fn);
  CheckMap(fn, FALSE);
  if ((colon = strchr(line, ':')) == NULL) return;
  MTCompVal = atoi(colon + 1);
  ReadFastFiles();

  }
/*
* UnCacheRoom()
*
* This turns off the cached flag for a given room shared thing.
*/
int UnCacheRoom(SharedRoom *room, int system, int index, int roomslot,
void *d)
  {
  if (strLen(roomTab[roomslot].rtname))
    {
    UnSetFA(netBuf.netRooms[index].mode);
    /* this is valid only so long as we uncache on-line, so in case ... */
    if (inNet != NON_NET) SR_Sent[index] = 1;

    }
  return TRUE;

  }
