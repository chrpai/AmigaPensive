/*
*       sysarc.c
*
* This file contains the system dependent code for deARCing files for
* download.
*/
/*
*       history
*
* 91Oct07 HAW Netcache stuff.
* 88Jun08 HAW Created.
*/
#define SYSTEM_DEPENDENT
#include "ctdl.h"
#include "dos.h"
/*#include "sys\stat.h" */
/* #include "process.h" */
/*

*       Contents
*
* ArcInit()   initialize for deARCing
* CompAvailable()   is compression available for this type?
* SendArcFiles()    get arc name, send contents specified
* # MakeTempDir()   Create and move into a temporary dir
*
*   # == local for this implementation only
*/

extern char logNetResults;
extern char netDebug;
extern FILE  *netLog;

extern aRoom roomBuf;
extern MessageBuffer msgBuf;
extern CONFIG  cfg;
extern char whichIO;
char TDirBuffer[120];
char cmdbuf[120];  /* command line buffer */
void KillDeCompress(void);


/*
* This table must be kept in sync with Formats[] in Misc.c
*/
DeCompElement DeComp[] =
  {
    { "delha.sys", "L. Lha",   NULL, NULL, NULL },
    { "dezip.sys", "P. Zip",   NULL, NULL, NULL },
    { "dezoo.sys", "Z. Zoo",   NULL, NULL, NULL },
    { "dearc.sys", "A. Arc",   NULL, NULL, NULL },

  };

int Jsystem(char *);
static void CheckCompressedData(char *name, int index);
static void MakeTempName(void);
/*
* ArcInit()
*
* This initializes this module for handling the various variants of
* compression/decompression.
*/
void ArcInit()
  {
  SYS_FILE name;
  int i;
  for (i = 0; i < NumElems(DeComp); i++)
      {
    makeSysName(name, DeComp[i].FileName, &cfg.roomArea);
    CheckCompressedData(name, i);

    }

  }
/*
* CheckCompressedData()
*
* This function does an initial check for existence of a configuration file.
*/
static void CheckCompressedData(char *name, int index)
  {
  FILE *fd;
  char work[80];
  if (cfg.BoolFlags.debug) splitF(NULL,"Open Attemp:%s\n",name);
  if ((fd = fopen(name, "r")) == NULL)
      {
    return;

    }
  if (cfg.BoolFlags.debug) splitF(NULL,"Opened:%s\n",name);
  if (GetAString(work, sizeof work, fd) != NULL)
      {
      if (cfg.BoolFlags.debug) splitF(NULL,"Read 1:%s\n",work);
      if (strlen(work) != 0)DeComp[index].DeWork = strdup(work);
      if (GetAString(work, sizeof work, fd) != NULL)
        {
        if (cfg.BoolFlags.debug) splitF(NULL,"Read 2:%s\n",work);
        if (strlen(work) != 0) DeComp[index].IntWork = strdup(work);
        if (GetAString(work, sizeof work, fd) != NULL)
          {
          if (cfg.BoolFlags.debug) splitF(NULL,"Read 3:%s\n",work);
          if (strlen(work) != 0) DeComp[index].CompWork = strdup(work);
          };
        };

      };
  fclose(fd);

  }
/*
* SendArcFiles()
*
* This function gets arc filename, send specified contents.
*/
void SendArcFiles(int protocol)
  {
  SYS_FILE name;
  char     *temp;
  int  NumFiles, format;
  extern long netBytes;
  getNormStr("ARCHFN", name, SIZE_SYS_FILE, 0);
  if (strLen(name) == 0) return;
  if (!setSpace(&roomBuf)) return;
  temp = getcwd(NULL, 100);
  sPrintf(msgBuf.mbtext, "%s/%s", temp, name);
  homeSpace();
  if (access(msgBuf.mbtext, 4) == -1)
    {
    for (NumFiles = 0; NumFiles < NumElems(DeComp); NumFiles++)
      {
      sPrintf(msgBuf.mbtext, "%s/%s.%s", temp,name, Formats[NumFiles].Format);
      if (access(msgBuf.mbtext, 4) == 0) break;

      };

    };
  if( access(msgBuf.mbtext,4) == -1 )
    {
    mPrintf("%s does not exist.\n ", name);
    free(temp);
    return;

    };
  if ((format = CompressType(msgBuf.mbtext)) == -1)
  format = 0;
  if (DeComp[format].DeWork == NULL)
      {
    mPrintf("Sorry, de%sing not supported here.\n ",
    Formats[format].Format);
    return;

    }
  sPrintf(msgBuf.mbtext, "%s %s/%s ",
  DeComp[format].DeWork, temp, name);
  getNormStr("ARCHMS", msgBuf.mbtext, 100, 0);
  if (strLen(msgBuf.mbtext) == 0) strCpy(msgBuf.mbtext, ALL_FILES);
  MakeTempDir();      /* Create and drop into */
  mPrintf("One moment, please ...\n ");
  sprintf(cmdbuf, "%s %s/%s %s",
  DeComp[format].DeWork, temp, name, msgBuf.mbtext);
  Jsystem(cmdbuf);
  free(temp);         /* Don't need this any longer, so kill it now */
  netBytes = 0l;
  NumFiles = wildCard(getSize, ALL_FILES, FALSE, "", FALSE);
  if (NumFiles <= 0)
      {
    mPrintf("Sorry, no files matched that deARC command.\n ");

    }
  else
      {
    if ( getYesNo("MREFFI") )
    if (TranAdmin(protocol, NumFiles))
    TranSend(protocol, transmitFile, ALL_FILES, "", FALSE);
    }
  KillDeCompress();  /* clean up the temp directory */
  }

/*
 * KillDeCompress()
 *
 * This function clears out the files from the decompression.
 * We assume we're IN the dir to be cleared.
 */
void KillDeCompress()
  {
  char temp[120];
  homeSpace();
  chdir(TDirBuffer);
  getcwd(temp, 120);
  if( strcmp(temp,TDirBuffer) == 0 )   /* make sure we are where we want to delete files */
    {
    if (!cfg.BoolFlags.debug) wildCard(DelFile, ALL_FILES, FALSE, "", FALSE);
    }
  else
    {
    splitF(netLog,"Error:  Could not set directory and delete temp dir/files\n");
    splitF(netLog,"Temp dir:%s\n",TDirBuffer);
    splitF(netLog,"Curr dir:%s\n",temp);
    }
  homeSpace();
  if (!cfg.BoolFlags.debug) rmdir(TDirBuffer);
  }
/*
 * KillNetDeCompress()
 *
 * This function clears out the files from the net session decompression.
 *
 */
void KillNetDeCompress()
  {
  char temp[120], temp2[120];
  homeSpace();
  MakeNetCache(temp2);
  chdir(temp2);
  getcwd(temp, 120);
  if( strcmp(temp,temp2) == 0 )   /* make sure we are where we want to delete files */
    {
    if (!cfg.BoolFlags.debug) wildCard(DelFile, ALL_FILES, FALSE, "", FALSE);
    }
  else
    {
    splitF(netLog,"Error:  Could not set current directory and delete temp files\n");
    splitF(netLog,"Temporary dir:%s\n",temp2);
    splitF(netLog,"  Current dir:%s\n",temp);
    }
  homeSpace();
  }



/*
* MakeTempDir()
*
* This function will create and move into a temporary directory.
* This will always be off the homearea.
*/
void MakeTempDir()
  {
  homeSpace();
  MakeTempName();
  if (mkdir(TDirBuffer) != 0) mPrintf("System error with mkdir!\n ");
  chdir(TDirBuffer);
  getcwd(TDirBuffer, 120);

  }
/*
* MakeTempName()
*
* This function gets a temporary name.
*/
static void MakeTempName()
  {
  static char *seed = "tmp_%04d";
  int i = 0;
  sPrintf(TDirBuffer, seed, i++);
  while (access(TDirBuffer, 0) != -1) sPrintf(TDirBuffer, seed, i++);
  }

#define BAT_FILE  "%s\nif warn \n echo \"1\" >%s\nelse\n echo \"0\" >%s\nendif\n"

/*
* FileIntegrity()
*
* This function does a file integrity check.
* Note: we don't use spawn() since it adds 3K to the .EXE.
*/
char FileIntegrity(char *filename)
  {
  char *c, *d, check[90], bad, NameUsed = FALSE;
  int format;
  FILE *fd;
  if ((c = strchr(filename, '.')) != NULL)
      {
    if ((format = CompressType(filename)) == -1) return TRUE;
    if (format > COMP_MAX - 1)                   return TRUE;
    if (DeComp[format].IntWork == NULL)          return TRUE;
    if (!getYesNo("INTEGR"))return TRUE;
    Output_Citadel_Message("ONEMOM",NULL, NULL, NULL);
    MakeTempName();
    strCat(TDirBuffer, ".bat");
    if (cfg.BoolFlags.debug) splitF(NULL,"File Open:%s\n",TDirBuffer);
    fd = fopen(TDirBuffer, "w");
    sprintf(check,"Execute %s",TDirBuffer);
    MakeTempName();
    /*
    * Manually copy.  If we encounter "%g" then replace it with the
    * name of the file to test.  If we never encounter it, then
    * remember to append it to the end of the string after the copy
    * has finished (NameUsed).
    */
    for (c = DeComp[format].IntWork, d = msgBuf.mbtext; *c; c++)
        {
      if (*c == '%' && *(c + 1) == 'g')
          {
        strCpy(d, filename);
        NameUsed = TRUE;
        while (*d) d++;
        c++;      /* this will get us over the %g. */

        }
      else *d++ = *c;

      }
    *d++ = ' ';   /* harmless space padding. */
    *d = 0;
    if (!NameUsed) strCat(msgBuf.mbtext, filename);
    fprintf(fd, BAT_FILE, msgBuf.mbtext, TDirBuffer, TDirBuffer);
    fclose(fd);
    Jsystem(check);
    unlink(check);
    if (cfg.BoolFlags.debug) splitF(NULL,"File Open:%s\n",TDirBuffer);
    bad = TRUE;
    if ((fd = fopen(TDirBuffer, "r")) != NULL)
      {
      if (fgets(check, sizeof check, fd) != NULL) bad = *check != '0';
      fclose(fd);
      unlink(TDirBuffer);
      };
    if (bad) return (char)!getYesNo("FAILIC");
    else Output_Citadel_Message("PASSED",NULL, NULL, NULL);

    }
  return TRUE;

  }
/*
* CompAvailable()
*
* This function determines if the given compression type has a compression
* method defined.
*/
char CompAvailable(char CompType)
  {
  return (char)(DeComp[CompType - 1].CompWork != NULL);

  }
/*
* NetDeCompress()
*
* This decompresses files for network dissection.
*/
void NetDeCompress(char CompType, SYS_FILE fn)
  {
  char *c;
  SYS_FILE dir;
  char pwd[100];
  if(  netDebug )splitF(netLog, "ComptType:%d File:%s\n", (int)CompType, fn);
  if (DeComp[CompType - 1].DeWork == NULL)
    {
    splitF(netLog," No method for decompression, NetDeCompress aborted\n");
    return;
    };
  strCpy(dir, fn);
  c = &dir[strlen(dir) ];  /* get the directory path */
  while(  *c != '/'  && *c != ':' )
    {
    c--;    /* move to the ':' or '/' */
    };
  c++;
  *c = '\0';               /* terminate the directory spec */
  c  = &fn[strlen(dir)];   /* get filename alone */
  chdir(dir);              /* set the default directory    */
  getcwd(pwd, 100);        /* get the absolute path */
  if(  netDebug )splitF(netLog, "Compressed file dir:%s\n", pwd);
  sPrintf(lbyte(pwd), pwd[strlen(pwd) - 1] == '/' ? "%s" : "/%s", c);
  sprintf(cmdbuf,"%s \"%s\"", DeComp[CompType - 1].DeWork, pwd);
  Jsystem(cmdbuf);         /* and unpack the archive */
  }
/*
* Compress()
*
* This compresses the given files into the given archive file using the
* given file.
*/
void Compress(char CompType, char *Files, char *ArcFileName)
  {
  if (!CompAvailable(CompType)) return;
  sprintf(cmdbuf, "%s %s %s", DeComp[CompType - 1].CompWork, ArcFileName, Files);
  Jsystem(cmdbuf);

  }
/*
* GetUserCompression()
*
* This function handles a request for a compression protocol.  This can be
* called either for a sysop (when selecting for Mass Transfers) or by a user
* (eventually) for .RC.
*
* This returns the protocol selected (LHA_COMP, etc) or NO_COMP if none
* was selected or if none are available.
*/
int GetUserCompression()
  {
  int  rover, count = 0, x;
  MenuId   id;
  char *CompOpts[] =
      {
    " ", " ", " ", " ", ""
    };
    for (rover = 0; rover < NumElems(DeComp); rover++) {
    if (DeComp[rover].CompWork != NULL) {
    count++;
    ExtraOption(CompOpts, DeComp[rover].MenuEntry);
    if (whichIO == MODEM) mPrintf("%s\n ", DeComp[rover].MenuEntry);
    }
    }
    if (count == 0) {
    SysopError(NO_MENU, "No Compression methods");
    return NO_COMP;
    }
    id = RegisterSysopMenu("Compress.mnu", CompOpts, " Compression ");
    SysopMenuPrompt(id, "\n Compression method: ");
    x = GetSysopMenuChar(id);
    CloseSysopMenu(id);
    switch (x) {
    case 'P': return ZIP_COMP;
    case 'Z': return ZOO_COMP;
    case 'L': return LHA_COMP;
    case 'A': return ARC_COMP;
    default:  return NO_COMP;
    }
    }
