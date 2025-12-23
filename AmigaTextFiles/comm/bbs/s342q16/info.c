/*
*				Info.C
*
* Handles almost all aspects of the Information command of C-86
*/
#include "ctdl.h"
/*
*				history
*
* 89Aug0? HAW	Created.
*/
/*
*				Contents
*
*	ReadCitInfo()		Reads info from file.
*	EatInfo()		Eats an entry from file (work fn).
*	CheckInfo()		Equivalency check for list.
*	FreeInfo()		Frees an info structure.
*	EditInfo()		Adds new information.
*	GetInfo()		Get information for a room.
*	doInfo()		Dump info on room.
*	WriteOutInformation()	Writes information to disk.
*	KillInfo()		Kills info of dead room.
*	ChangeInfoName()	Called for room name change.
*	AllInfo()		.Known Info.
*	InfoShow()		work fn to show information.
*	WriteNewInfo()		Writes entry to disk.
*/
#define ROOM_HEADER	"#room"
extern CONFIG  cfg;
extern struct floor  *FloorTab;
extern rTable    *roomTab;
extern MessageBuffer msgBuf;
extern logBuffer logBuf;
extern aRoom   roomBuf;		/* Room buffer			*/
extern char    haveCarrier;	/* Have carrier?		*/
extern char    onConsole;	/* How about on Console?	*/
extern char    outFlag;		/* Output flag			*/
/*
* Data Format
*
* All room information is kept in a single file named ctdlinfo.sys.  It's
* straight text, in which each record begins with a line that looks like
*
*    #room <roomname>
*
* and ends with a completely blank line (a space on that line will make it
* non-blank, thus allowing blank lines to separate paragraphs in long
* information entries).
*
* As a later note, this format is inadequate.  Direct editing of this file
* can throw away those blank spots.  A better record ending convention, such
* as a line that says simply '#end', would probably be for the best.  Maybe
* do that in the next major release.
*/
typedef struct
  {
  label Roomname;
  char  *Info;

  }
RoomInfo;
static void *EatInfo(char *line);
void *CheckInfo(RoomInfo *d1, RoomInfo *d2);
static void FreeInfo(RoomInfo *d);
char *GetInfo(label name);
static int InfoShow(int r);
static SListBase InfoList =
  {
  NULL, CheckInfo, NULL, FreeInfo, EatInfo

  };
static FILE *fd;
/*
* ReadCitInfo()
*
* This function will read in the C-86 room info file.
*/
void ReadCitInfo()
  {
  SYS_FILE fn;
  makeSysName(fn, "ctdlinfo.sys", &cfg.roomArea);
  if ((fd = safeopen(fn, "r")) != NULL)
    {
    MakeList(&InfoList, "", fd);
    fclose(fd);

    }

  }
/*
* EatInfo()
*
* This eats some info from the information file.  This function is a bit
* non-obvious because it is called from the generic MakeList() function,
* which generally expects one record/per line format.  Therefore, we have
* to cheat a bit in order to read in the extra lines and still return a
* meaningful record.
*
* Basically, if the line sent in is not the beginning of a room info entry,
* NULL is returned.  Otherwise, we cheat by reading in the rest of the
* entry from the global FILE pointer and return the entire record.
*/
static void *EatInfo(char *line)
  {
  RoomInfo *temp;
  char     *c;
  if (strncmp(line, ROOM_HEADER, strLen(ROOM_HEADER)) != SAMESTRING)return NULL;
  temp = GetDynamic(sizeof *temp);
  strCpy(temp->Roomname, line + 1 + strLen(ROOM_HEADER));
  msgBuf.mbtext[0] = 0;
  c = msgBuf.mbtext;
  while (fgets(c, 100, fd) != NULL && *c != '\n')
    {
    c = lbyte(msgBuf.mbtext);

    };
  temp->Info = strdup(msgBuf.mbtext);
  return temp;

  }
/*
* CheckInfo()
*
* This function checks to see if two infos reference the same room.
*/
void *CheckInfo(RoomInfo *d1, RoomInfo *d2)
  {
  return ((strCmpU(d1->Roomname, d2->Roomname)
  != SAMESTRING) ? NULL : d1);

  }
/*
* FreeInfo()
*
* This function will free an info structure.
*/
static void FreeInfo(RoomInfo *d)
  {
  free(d->Info);
  free(d);

  }
/*
* EditInfo()
*
* This function adds new information.  Once the new information is acquired
* the entire list is written back out to disk.
*/
void EditInfo()
  {
  RoomInfo *d;
  char *c;
  extern char MsgEntryType, EndWithCR;
  d = GetDynamic(sizeof *d);
  strCpy(d->Roomname, roomBuf.rbname);
  if ((c = GetInfo(roomBuf.rbname)) != NULL)
    {
    if (getYesNo("EDCURI"))
    strCpy(msgBuf.mbtext, c);
    else
    msgBuf.mbtext[0] = 0;

    }
  else msgBuf.mbtext[0] = 0;
  MsgEntryType = INFO_ENTRY;
  Output_Citadel_Message("INFOED",NULL, NULL, NULL);
  doCR();
  CleanEnd(msgBuf.mbtext);
  mPrintf("%s", msgBuf.mbtext);
  outFlag = OUTOK;
  if (GetBalance(ASCII, msgBuf.mbtext, MAXTEXT-50) && onLine())
    {
    CleanEnd(msgBuf.mbtext);
    d->Info = strdup(msgBuf.mbtext);
    if (c != NULL || strLen(msgBuf.mbtext) != 0)
      {
      AddData(&InfoList, d, NULL, TRUE);
      WriteOutInformation();

      }
    else free(d);

    }

  }
/*
* GetInfo()
*
* This function will get the information for a given room.
*/
char *GetInfo(label name)
  {
  char *result;
  RoomInfo d, *c;
  strCpy(d.Roomname, name);
  result =  (c = SearchList(&InfoList, &d)) == NULL ? NULL : c->Info;
  return result;

  }
/*
* doInfo()
*
* This function dumps info on current room.
*/
char doInfo()
  {
  char *c;
  extern char journalMessage;
  mPrintf("\n \n ");
  if ((c = GetInfo(roomBuf.rbname)) != NULL)
    {
    mPrintf("%s", c);
    doCR();
    if (journalMessage)
      {
      if (redirect(NULL))
        {
        mPrintf("%s", c);
        doCR();
        undirect();

        }
      journalMessage = FALSE;

      }

    }
  else Output_Citadel_Message("NOINFO",(long)roomBuf.rbname,NULL, NULL);
  return GOOD_SELECT;

  }
/*
* WriteOutInformation()
*
* This function writes out information to disk.
*/
void WriteOutInformation()
  {
  void WriteNewInfo();
  SYS_FILE fn;
  makeSysName(fn, "ctdlinfo.sys", &cfg.roomArea);
  fd = safeopen(fn, "w");
  RunList(&InfoList, WriteNewInfo);
  fclose(fd);

  }
/*
* KillInfo()
*
* This function kills some info.  This is used when a room is killed.
*/
void KillInfo(char *name)
  {
  RoomInfo d;
  if (GetInfo(name) != NULL)
    {
    strCpy(d.Roomname, name);
    KillData(&InfoList, &d);
    WriteOutInformation();

    }

  }
/*
* ChangeInfoName()
*
* This function is called when a room name is changed.
*/
void ChangeInfoName(char *newname)
  {
  RoomInfo d, *c;
  strCpy(d.Roomname, roomBuf.rbname);
  if ((c = SearchList(&InfoList, &d)) != NULL)
    {
    strCpy(c->Roomname, newname);
    WriteOutInformation();

    }

  }
/*
* AllInfo()
*
* This function implements .Known Info.
*/
void AllInfo()
  {
  int rover, roomNo;
  extern int TopFloor;
  extern char ShowNew, SelNew;
  doCR();
  if (FloorMode)
    {
    for (rover = 0; rover < TopFloor; rover++)
      {
      roomNo = FirstRoom(rover);
      ShowNew = 2;
      SelNew = TRUE;
      if (FloorRunner(roomNo, CheckFloor) != ERROR)
        {
        mPrintf("[%s]", FloorTab[rover].FlName);
        doCR();
        FloorRunner(roomNo, InfoShow);

        }
      ShowNew = FALSE;
      SelNew = FALSE;

      }

    }
  else tableRunner(InfoShow, TRUE);

  }
/*
* InfoShow()
*
* This function will actually show the information for a room.
*/
static int InfoShow(int r)
  {
  char *c;
  int  rover;
  extern int DirAlign;
  extern char AlignChar;
  if ((c = GetInfo(roomTab[r].rtname)) != NULL) CleanEnd(c);
  if (c != NULL && strLen(c) != 0)
    {
    mPrintf("%s ", roomTab[r].rtname);
    for (rover = strLen(roomTab[r].rtname); rover < 22; rover++)
    oChar('.');
    oChar(' ');
    DirAlign = 22;
    AlignChar = ' ';
    mPrintf("%s", c);
    DirAlign = 0;
    doCR();

    }
  return 0;

  }
/*
* WriteNewInfo()
*
* This function will write out a chunk of new information.
*/
void WriteNewInfo(RoomInfo *d)
  {
  CleanEnd(d->Info);
  fprintf(fd, "#room %s\n%s\n\n", d->Roomname, d->Info);

  }
