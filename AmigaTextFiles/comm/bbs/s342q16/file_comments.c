/************************************************************************/
/*   File Comments Processing                                           */
/************************************************************************/
#define SYSTEM_DEPENDENT
#define TIMER_FUNCTIONS_NEEDED
#include "ctdl.h"
#include "dos.h"
#include "time.h"
#include "string.h"
#include "stdarg.h"

/************************************************************************/
/*      Globals -- there shouldn't be anything here but statics and     */
/*      externs.                                                        */
/************************************************************************/
static char *Phrase;
extern char *READ_TEXT;
extern char *WRITE_TEXT;
static FILE *FileTag;                  /* For the file tags            */
#define ONE_DAY         86400L
#define D_OFFSET        252460800L

int Jsystem(char *);
void Do_Stack_Check(void);


extern MessageBuffer   msgBuf;
extern CONFIG    cfg;            /* Lots an lots of variables    */
void *ChPhrase(DirEntry *e, DirEntry *d);
static char  File_Extension(char *name, char *extension);
static void Change_Extension(char *name, char *extension);
static char *Find_Extension(char *name);

static char filenamebuffer[100];


#define ONE_DAY         86400L
#define D_OFFSET        252460800L

static char *Find_Extension(char *name)
  {
  char *ptr, *lptr;
  ptr = name;  /* point to last character */
  while( ( lptr = strchr(ptr,'.') ) != NULL )
    {
    ptr = lptr;
    if( *ptr == '.' ) ptr++;
    };
  return ptr;
  }

static void Change_Extension(char *name, char *extension)
  {
  char *ptr;
  int lfn, lex;
  /**
    Find the Extension of the name
  **/
  lfn  = strlen(name);
  ptr  = Find_Extension(name);
  lex  = strlen(ptr);
  if( lfn > lex )lfn -= lex;  /* length of filename without extention */
  memset(filenamebuffer,'\0',100);
  strncpy(filenamebuffer,name,lfn);
  strcat(filenamebuffer,extension);
  return;
  }

static char  File_Extension(char *name, char *extension)
  {
  char *ptr;
  int lnm, lext;
  /**
    1) check to see that we have a valid pointer and that
       the string is long enough to match
  **/
  if( name == NULL ) return (char)TRUE;
  lnm =  strlen(name);
  lext=  strlen(extension);
  if( lnm < lext ) return (char)TRUE;
  /**
    Find the Extension of the name
  **/
  ptr = Find_Extension(name);
  lnm = strlen(ptr);
  if( lnm != lext ) return (char)FALSE;
  if( stricmp(ptr, extension) == 0 ) return (char)TRUE;
  return (char)FALSE;
  }

/************************************************************************/
/*      CitGetFileList() Get a list of files from the current "area"    */
/************************************************************************/
void CitGetFileList(char *mask, SListBase *list, long before, long after,
char *phrase)
  {
  struct FileInfoBlock *FlBlock;
  extern char     *monthTab[13];
  char            *w, *work, *sp, again, buf[10];
  DirEntry        *fp;
  int             done;
  long            time;
  struct tm         *v;
  Do_Stack_Check();
  w = work = strdup(mask);
  FlBlock = malloc(sizeof *FlBlock);
  do
    {
    again = (sp = strchr(work, ' ')) != NULL;      /* space is separator */
    if (again)
      {
      *sp = 0;

      }
    /* Do all scanning for illegal requests here */
    if (!ValidDirFileName(work))    continue;
    for (done = dfind(FlBlock, work, 0);  !done;    done = dnext(FlBlock))
      {
      if( File_Extension(FlBlock->fib_FileName, "readme") )
        continue; /* skip all readme files */
      /* format date to our standards */
      /* DosToNormal(buf, FlBlock->ff_fdate);  */
      time = D_OFFSET + (long)(ONE_DAY * FlBlock->fib_Date.ds_Days) +
      (long)(FlBlock->fib_Date.ds_Minute * 60) +
      (long)(FlBlock->fib_Date.ds_Tick / 50);
      v = localtime(&time);
      sPrintf(buf, "%d%s%02d", v->tm_year, monthTab[v->tm_mon+1],
      v->tm_mday);
      /* now read it so we can handle date specs */
      ReadDate(buf, &time);
      /* add to list iff dates inactive or we meet the specs */
      if ((before != -1l && time > before) || (after  != -1l && time < after))
        continue;
      fp = (DirEntry *) GetDynamic(sizeof *fp);
      fp->unambig = strdup(FlBlock->fib_FileName);
      strCpy(fp->FileDate, buf);
      fp->FileSize = FlBlock->fib_Size;
      AddData(list, fp, NULL, TRUE);

      }
    if (again) work = sp+1;

    }
  while (again);
  free(w);
  free(FlBlock);
  if (strLen(phrase) != 0)
    {
    Phrase = phrase;
    StFileComSearch();      /* so's we can do phrase searches */
    list->CheckIt = ChPhrase;   /* COVER YOUR EYES I'M CHEATING! */
    KillData(list, list);
    list->CheckIt = DirCheck;   /* COVER YOUR EYES I'M CHEATING! */
    EndFileComment();

    }

  }

/************************************************************************/
/*      ChPhrase() does something to do with finding phrases in comments*/
/************************************************************************/
void *ChPhrase(DirEntry *e, DirEntry *d)
  {
  Do_Stack_Check();
  if (FindFileComment(e->unambig))
    {
    if (matchString(msgBuf.mbtext, Phrase, lbyte(msgBuf.mbtext)) == NULL) return d;

    }
  else return d;
  return NULL;

  }

/************************************************************************/
/*      CopyFile() copies a file                                        */
/************************************************************************/
char CopyFile(char *oldname, aRoom *roomData)
  {
  int len;
  #ifdef NO_STAT
  BPTR F_Lock;
  struct FileInfoBlock *FIBlk;
  char F_Ok;
  LONG F_Status;
  #else
  struct stat buf;
  #endif
  char *temp;
  char buffer[256];
  char F_Node[32],F_Ext[32];
  Do_Stack_Check();
  if ((temp = FindDirName(roomData->rbArea)) == NULL)
    {
    Output_Citadel_Message("ERRDRM",NULL,NULL,NULL);
    return FALSE;

    }
  #ifdef NO_STAT
  F_Ok = FALSE;
  F_Lock = Lock(oldname,-2);
  if (F_Lock != NULL)
    {
    FIBlk=(struct FileInfoBlock *)malloc(sizeof(struct FileInfoBlock));
    if (FIBlk != NULL)
      {
      F_Status = Examine(F_Lock, FIBlk);
      if (F_Status != 0)
        {
        if (FIBlk->fib_DirEntryType < 0)
        F_Ok = TRUE;

        }
      free(FIBlk);

      }
    UnLock(F_Lock);

    }
  if (!F_Ok)
    {
    Output_Citadel_Message("ISNCPY",(long)oldname,NULL,NULL);
    return FALSE;

    }
  #else
  if (stat(oldname, &buf) != 0 || !(buf.st_mode & S_IFREG))
    {
    Output_Citadel_Message("ISNCPY",(long) oldname,NULL,NULL);
    return FALSE;

    }
  #endif
  sPrintf(buffer, "copy >NIL: %s %s", oldname, temp);
  Jsystem(buffer);  /* move file */
  Change_Extension(oldname,"readme");
  if( access(filenamebuffer,R_OK) == 0 )
    {
    sPrintf(buffer, "copy >NIL: %s %s", filenamebuffer, temp);
    Jsystem(buffer);  /* move readme */
    ZeroMsgBuffer(&msgBuf);
    ingestFile(filenamebuffer,&msgBuf);
    };
  strsfn(oldname,NULL,NULL,F_Node,F_Ext);
  temp = FindDirName(roomData->rbArea);
  sPrintf(oldname, "%s%c%s", F_Node,(F_Ext[0] != 0) ? '.' : '\0', F_Ext);
  len = strlen(temp);
  if ( temp[len-1] == ':' || temp[len-1] == '/' )
    {
    sPrintf(buffer, "%s%s",temp,oldname);
    }
  else    sPrintf(buffer, "%s/%s",temp,oldname);
  return ((char)(access(buffer, 0) == 0));

  }

/************************************************************************/
/*      StFileComSearch() Start File Comment search: see SYSDEP.DOC.    */
/************************************************************************/
int StFileComSearch()
  {
  return TRUE;  /* a stub, nothing to do  */
  }

/************************************************************************/
/*      FindFileComment() find file comment for file: see SYSDEP.DOC.   */
/************************************************************************/
int FindFileComment(char *fileName)
  {
  char Found_Comment;
  Do_Stack_Check();
  ZeroMsgBuffer(&msgBuf);
  Change_Extension(fileName,"readme");
  Found_Comment = ingestFile(filenamebuffer,&msgBuf);
  doCR();               /* formatting without blank lines.              */
  return ( Found_Comment );
  }

/************************************************************************/
/*      EndFileComment() end session of reading file comments.          */
/************************************************************************/
void EndFileComment()
  {
  return;   /* just a stub */
  }

/************************************************************************/
/*      updFiletag()  updates a file tag                                */
/************************************************************************/
void updFiletag(char *fileName, char *desc)
  {
  Do_Stack_Check();
  Change_Extension(fileName, "readme");
  if ((FileTag = safeopen(filenamebuffer, WRITE_TEXT)) == NULL)
    {
    Output_Citadel_Message("UNKWNP",(long)filenamebuffer,NULL,NULL);
    }
  else
    {
    fprintf(FileTag, "%s\n",  desc);
    fclose( FileTag);
    }
  return;
  }
