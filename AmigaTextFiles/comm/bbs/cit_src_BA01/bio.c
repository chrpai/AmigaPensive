/*
*       bio.c
*
* User Biography Functions.
*/
/*
*       history
*
* 91Jun14 HAW  Created.
*/
#include "ctdl.h"
#include <dos.h>
/*
*       contents
*
* BioDirectory()    Gets a list of user biographies.
* ClearBio()    This clears a bio from disk.
* EditBio()   Edits a biography by a user.
* GetBioInfo()    Gets a bio from disk.
* SaveBioInfo()   Save a bio to disk.
*/
extern CONFIG      cfg;   /* Lots an lots of variables    */
extern logBuffer   logBuf;    /* Person buffer    */
extern logBuffer   logTmp;    /* Person buffer    */
extern MessageBuffer     msgBuf;
extern char    haveCarrier;    /* Do we still got carrier?     */
extern int     thisLog;
extern char    *LCHeld, *WRITE_ANY, *READ_ANY;
extern char    outFlag;    /* Output flag     */
extern char    onConsole;
extern char    MsgEntryType;
/**
  InitBio()
  This function will check to see if the bioArea exists and
  will create it if it does not.
**/
void InitBio()
  {
  if( access(cfg.bioArea.saDirname, F_OK)) return;
  mkdir((char *)&cfg.bioArea.saDirname);
  }
/*
* EditBio()
*
* This function allows the editing of a biography.  If a biography already
* exists then an option is offered to edit that rather than create a new
* biography.  NB: "null" biographies are not saved but are rather completely
* deleted so they don't show up on .M?
*/
void EditBio()
  {
  char name[15];
  SYS_FILE bio;
  doCR();
  sPrintf(name, "%d.bio", thisLog);
  MakeBioName(bio, name);
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"EditBio:%s\n",bio);
    };
  if (access(bio, 0) == 0)
    {
    if (getYesNo("EDBIOS"))
      {
      GetBioInfo(thisLog);

      }
    else msgBuf.mbtext[0] = 0;

    }
  else msgBuf.mbtext[0] = 0;

  Output_Citadel_Message("BIONOV",NULL,NULL,NULL);
  HelpIfPresent("biosys.blb");
  MsgEntryType = BIO_ENTRY;
  Output_Citadel_Message("MYBIOS",NULL,NULL,NULL);
  doCR();
  CleanEnd(msgBuf.mbtext);
  mPrintf("%s", msgBuf.mbtext);
  outFlag = OUTOK;
  if (GetBalance(ASCII, msgBuf.mbtext, MAXTEXT-50) && onLine())
    {
    CleanEnd(msgBuf.mbtext);
    if (strLen(msgBuf.mbtext) < 3)
    ClearBio(thisLog);
    else SaveBioInfo(thisLog);

    }

  }
/*
* GetBioInfo()
*
* This function handles the mechanics of getting biographical info.  It
* handles the encryption.
*/
char GetBioInfo(int which)
  {
  char name[15];
  SYS_FILE bio;
  FILE *fd;
  long size;
  sPrintf(name, "%d.bio", which);
  MakeBioName(bio, name);
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"GetBio:%s\n",bio);
    };
  if ((fd = fopen(bio, READ_ANY)) != NULL)
    {
    totalBytes(&size, fd);
    size++;       /* include NULL byte */
    fread(msgBuf.mbtext, (int) size, 1, fd);
    crypte(msgBuf.mbtext, (int) size, which);
    fclose(fd);
    return TRUE;

    }
  msgBuf.mbtext[0] = 0;
  return FALSE;

  }
/*
* SaveBioInfo()
*
* This function handles the mechanics of saving biographical info.  This
* includes encrypting the bio before saving it.
*/
void SaveBioInfo(int which)
  {
  char name[15];
  SYS_FILE bio;
  FILE *fd;
  int size;
  sPrintf(name, "%d.bio", which);
  MakeBioName(bio, name);
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"SaveBio:%s\n",bio);
    };
  if ((fd = fopen(bio, WRITE_ANY)) != NULL)
    {
    size = strLen(msgBuf.mbtext) + 1; /* include NULL byte */
    crypte(msgBuf.mbtext, (int) size, which);
    fwrite(msgBuf.mbtext, (int) size, 1, fd);
    crypte(msgBuf.mbtext, (int) size, which);
    fclose(fd);

    }
  else msgBuf.mbtext[0] = 0;

  }
/*
* ClearBio()
*
* This clears out a biography.
*/
void ClearBio(int which)
  {
  char name[15];
  SYS_FILE bio;
  sPrintf(name, "%d.bio", which);
  MakeBioName(bio, name);
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"ClearBio:%s\n",bio);
    };
  unlink(bio);

  }
static  void ShowBioName(DirEntry *);
/*
* BioDirectory()
*
* This shows who's written biographies.  Rather ugly since we're between
* major releases.  If we do another major release we should clean out
* that call to MoveToBioDirectory().
*/
void BioDirectory()
  {
  if (cfg.BoolFlags.debug)
    {
    splitF(NULL,"BioArea:%s\n",cfg.bioArea.saDirname);
    };
  MoveToBioDirectory();
  if (wildCard(ShowBioName, "*.bio", FALSE, "", FALSE) == 0)
    {
    Output_Citadel_Message("NOBIOS",NULL,NULL,NULL);
    }
  else mPrintf("\b\b. \n ");
  homeSpace();

  }
/*
* ShowBioName()
*
* This internal function shows the name of a person with a bio.  It is called
* in connection with RunList() (see above).
*/
static void ShowBioName(DirEntry *entry)
  {
  char *dot;
  if ((dot = strchr(entry->unambig, '.')) == NULL) return;
  *dot = 0;
  getLog(&logTmp, atoi(entry->unambig));
  if (logTmp.lbflags.L_INUSE)
  mPrintf("%s, ", logTmp.lbname);

  }
