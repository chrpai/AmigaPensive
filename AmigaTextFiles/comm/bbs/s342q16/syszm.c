/*
 * *       syszm.c * * External protocol handlers.
 */
/*
 * *       history * * 89Aug14 HAW  Rewritten for versatility. * 88Nov13 HAW
 * Created.
 */
#define SYSTEM_DEPENDENT
#include "ctdl.h"
#include "dos.h"

int       Jsystem(char *);
long      Do_Zmodem(char *files, int direction);
                         /** returns TRUE if error **/
/**
   Contents
 AddExternProtocolOptions() to system menus
 AddNames()         Allows building strings of names
 AddOurOpts()       add  options to a menu list.
 DoesNumerous()     BATCH checking function.
 EatExtMessage()    Send a bundle of messages
 EatProtocol()      Eats a protocol definition line.
 ExternalProtocol() Handles all external protocols
 FindProtocolCode() finds a protocol value for id purposes
 FindProtoName()    returns the name of the ext protocol.
 FindProtocol()     work function for finding things.
 InitProtocols()   initializes external protocols
**/
char      MakeCmdLine(char *, char *, char *miscdata, int len);
void      AddName(DirEntry *);
int       ExternalProtocol(int, char, char *, char *, char);
typedef struct
  {
    char      Selector,
             *Name,
              Down,
             *CmdLine,
             *Display,
              Many;
    int       ProtVal;

  }
PROTOCOL;
char      GetSizes = FALSE;
int       ExCount = 4;
void     *EatProtocol(char *line);
void     *FindProtocol(PROTOCOL *, int *);
void      InitProtocols(void);
static SListBase ExtProtocols =
{
  NULL, FindProtocol, NULL, NULL, EatProtocol

};
static char FindCode = FALSE;
extern CONFIG cfg;
extern MessageBuffer msgBuf;
extern aRoom roomBuf;
extern char loggedIn;
extern int SystemPort;
extern char onConsole;              /*

                                             * Where we get stuff from
                                             */
extern logBuffer logBuf;
char      AddStringToMCL(char *target, char *source, int len);
void      EnglishWork(char *target, char Ups);
char      cmdline[125];             /* we have a limit  */
/*
 * * InitProtocols() * * This function initializes external protocols.
 */
void
InitProtocols()
{
  SYS_FILE  fn;

  makeSysName(fn, "ctdlprot.sys", &cfg.roomArea);
  MakeList(&ExtProtocols, fn, NULL);

}
/**
  EatProtocol()
This function eats a protocol line.
  Format:
 [selector] [1/M] [name] [u/d] [command line]
 [selector] [I]   [name] [u/d] [Internal Zmodem]
**/
void     *
EatProtocol(char *line)
{
  PROTOCOL *temp;
  char     *c,
           *err;

  if (cfg.BoolFlags.debug)
    splitF(NULL, "External:%s\n", line);
  err = line;
  NormStr(line);
  if (strLen(line) == 0)
    return NULL;
  temp = GetDynamic(sizeof *temp);
  temp->Selector = line[0];
  line += 2;
  temp->Many = toUpper(line[0]);
  /**
     Could be "1" or "M"
  **/
  switch (temp->Many)
    {
        case '1':       /*
                         * Single file, external protocol handler
                         */
        case 'M':       /*
                         * Multiple files, external protocol handler
                         */
        case 'I':       /*
                         * Internal Zmodem
                        */
          break;
        default:
          splitF(NULL, "Invalid Protocol in CTDLPROT.SYS\n%s\n", err);

    }
  line += 2;
  if ((c = strchr(line, ' ')) != NULL)
    *c = 0;
  temp->Name = strdup(line);
  line = lbyte(line) + 1;
  temp->Down = (toUpper(line[0]) == 'D') ? TRUE : FALSE;
  line += 2;
  temp->CmdLine = strdup(line);
  temp->ProtVal = ExCount++;
  sPrintf(cmdline, "%s%c\b%s", NTERM, temp->Selector, temp->Name);
  temp->Display = strdup(cmdline);
  return temp;

}
/**
 ExternalProtocol()
  This function handles external protocols.  It
  also checks for external message uploads/downloads.
**/
int
ExternalProtocol(int protocol, char upload, char *mask, char *phrase,
                     char Move)
{
  PROTOCOL *Prot;
  int       toReturn;
  extern long byteRate;
  extern int SystemPort;
  long realCount;
  SpecialMessage("Status:External Protocol");
  if (cfg.BoolFlags.debug)
    {
        splitF(NULL, "ExternalProtocol(%d,%s,%s,%s,%s)\n", protocol,
        (upload ? "TRUE" : "FALSE"), mask, phrase, (Move ? "TRUE" : "FALSE"));

    };
  if ((Prot = SearchList(&ExtProtocols, &protocol)) == NULL)
    {
        if (cfg.BoolFlags.debug) splitF(NULL, "SearchList failed to find Protocol\n");
        return TRAN_FAILURE;

    };
  if (Move && !setSpace(&roomBuf))
    {
        if (cfg.BoolFlags.debug)   splitF(NULL, "SetSpace Failed\n");
        return TRAN_FAILURE;

    };
  if (!upload)
    {
        msgBuf.mbtext[0] = 0;
        wildCard(AddName, mask, FALSE, phrase, TRUE);

    }
  else
    strCpy(msgBuf.mbtext, mask);
  if (Prot->Many == '1' || Prot->Many == 'M')   /*
                                                                 * old style
                                                                 */
    {
        if (cfg.BoolFlags.debug) splitF(NULL, "Making external command line\n");
        if (!MakeCmdLine(cmdline, Prot->CmdLine, msgBuf.mbtext, sizeof cmdline - 1))
          {
            if (strchr(mask, '>') != NULL || strchr(mask, '<') != NULL)
                {
                  if (cfg.BoolFlags.debug) splitF(NULL, "List too long\n");
                  Output_Citadel_Message("FILLTL", NULL, NULL, NULL);
                  if (Move)     homeSpace();
                  return TRAN_FAILURE;

                }
            else if (!MakeCmdLine(cmdline, Prot->CmdLine, mask, sizeof cmdline - 1))
                {
                  if (cfg.BoolFlags.debug) splitF(NULL, "List too long\n");
                  Output_Citadel_Message("FILLTL", NULL, NULL, NULL);
                  if (Move)  homeSpace();
                  return TRAN_FAILURE;

                }

          };

    };
  if (!upload)
    fileMessage(FL_START, mask, TRUE, protocol, 0l);
  if (loggedIn)
    printf("\n(%s)\n %s", logBuf.lbname, cmdline);
  if (Prot->Many == '1' || Prot->Many == 'M')   /* old style */
    {
        Jsystem(cmdline);

    }
  else

/**
   Internal Zmodem

**/
    {

    if( Do_Zmodem(msgBuf.mbtext, upload) )
       return TRAN_FAILURE ;


    };
  if (!upload)
    {
        GetSizes = TRUE;
        ExCount = 0;
        strCpy(msgBuf.mbtext, "   ");
        (void)wildCard(AddName, mask, Move, phrase, TRUE);
        GetSizes = FALSE;
        if (Move)         homeSpace();
        fileMessage(FL_EX_END, mask, TRUE, protocol, 0l         /*
                                                                                 * filled in
                                                                                 */ );
        toReturn = TRAN_SUCCESS;
  /* if( realCount > 0 )transRecDown(mask, realCount); */

    }
  else
    {
        toReturn = ((strcmp(mask, "msg") == 0) || (access(mask, 0) == 0))
          ? TRAN_SUCCESS : TRAN_FAILURE;

    }
  return toReturn;

}
UNS_16    intrates[] =
{
  30, 120, 240, 480, 960, 1440, 1920, 3840, 5760

};

/*
 * * MakeCmdLine() * * This function creates a command line, including
 * supported substitution * parameters.  This should be detailed in this
 * comment but isn't.
 */
char
MakeCmdLine(char *target, char *source, char *miscdata, int len)
{
  extern int thisLog;               /*

                                             * entry currently in logBuf
                                             */
  char     *c,
           *temp,
            NoOverflow = TRUE;
  int       i;
  long the_bps;
  for (i = 0, c = source; *c; c++)
    {
        if (i > len - 2)
          {
            NoOverflow = FALSE;
            break;

          }
        if (*c == '%')
          {
            target[i] = 0;
            c++;
            switch (*c)
                {
                  case 'a':     /* baud rate */
                    if (onConsole)
                        sPrintf(lbyte(target), "%d", 0);
                    else
                        sPrintf(lbyte(target), "%ld",
                                  (cfg.DepData.LockPort != -1) ?
                            (long)(intrates[cfg.DepData.LockPort] * 10) : byteRate * 10);
                    break;
                  case 'b':     /* bps */
                    if( onConsole )
                      {
                      the_bps = 0;
                      }
                    else if( cfg.DepData.LockPort == -1 )
                      {
                      the_bps = byteRate;
                      }
                    else
                      {
                      the_bps = (long)(intrates[cfg.DepData.LockPort]);
                      };
                    sPrintf(lbyte(target), "%ld", the_bps);
                    break;
                  case 'c':     /* port # */
                    sPrintf(lbyte(target), "%d", SystemPort);
                    break;
                  case 'e':     /* log number of the user */
                    sPrintf(lbyte(target), "%d", thisLog);
                    break;
                  case 'f':     /* ANSI code of the user  */
                    sPrintf(lbyte(target), "0");
                    break;
                  case 'g':     /* file mask */
                    NoOverflow = AddStringToMCL(target, miscdata, len);
                    break;
                  case 'h':
                    if (byteRate != 0)
                        sPrintf(lbyte(target), "COM%d", SystemPort);
                    else
                        strCat(target, "LOCAL");
                    break;
                  case 'i':
                    NoOverflow = AddStringToMCL(target, roomBuf.rbname, len);
                    break;
                  case 'j':
                    if ((temp = FindDirName(roomBuf.rbArea)) != NULL)
                        NoOverflow = AddStringToMCL(target, temp, len);
                    break;
                  case 'k':
                    sPrintf(lbyte(target), "%d", logBuf.lbwidth);
                    break;
                  case 'd':
                    strCat(target, logBuf.lbname);
                    break;

                }
            while (target[i])
                i++;

          }
        else
          target[i++] = *c;

    }
  target[i] = 0;
  if (cfg.BoolFlags.debug)
    splitF(NULL, "Command Line is %d long\n", strlen(target));
  return NoOverflow;

}
/*
 * * AddStringToMCL() * * This adds a string as needed, I guess.
 */
char
AddStringToMCL(char *target, char *source, int len)
{
  if (strlen(source) + strlen(target) < len - 2)
    {
        strCat(target, source);
        return TRUE;

    }
  else
    {
        strncpy(lbyte(target), source, len - strlen(target) - 1);
        target[len - 1] = 0;
        return FALSE;

    }

}
static char **TheOpts,
          IsUpload;

/*
 * * AddExternProtocolOptions() * * This function adds external protocol
 * options to system menus.
 */
void
AddExternProtocolOptions(char **Opts, char upload)
{
  void      AddOurOpts();

  TheOpts = Opts;
  IsUpload = upload;
  RunList(&ExtProtocols, AddOurOpts);

}
/*
 * * AddOurOpts() * * This function does the actual work of adding an option
 * to a menu list.
 */
void
AddOurOpts(PROTOCOL * d)
{
  if ((d->Down && IsUpload) ||
        (!d->Down && !IsUpload))
    return;
  ExtraOption(TheOpts, d->Display);

}
/*
 * * FindProtocolCode() * * This function finds a protocol value for id
 * purposes.
 */
int
FindProtocolCode(int c, char upload)
{
  PROTOCOL *Prot;

  FindCode = TRUE;
  IsUpload = upload;
  if (isalpha(c))
    c = toUpper(c);
  Prot = SearchList(&ExtProtocols, &c);
  FindCode = FALSE;
  if (Prot == NULL)
    return -1;
  return Prot->ProtVal;

}
/*
 * * FindProtoName() * * This function returns the name of the external
 * protocol.
 */
char     *
FindProtoName(int protocol)
{
  PROTOCOL *Prot;

  if ((Prot = SearchList(&ExtProtocols, &protocol)) == NULL)
    return NULL;
  return Prot->Name;

}
/*
 * * FindProtocol() * * This work function helps find a protocol in a list.
 */
void     *
FindProtocol(PROTOCOL * d, int *val)
{
  if (FindCode)
    {
        if (*val == d->Selector && IsUpload != d->Down)
          return d;

    }
  else if (*val == d->ProtVal)
    return d;
  return NULL;

}
/*
 * * DoesNumerous() * * Does this protocol support BATCH downloads?
 */
char
DoesNumerous(int protocol)
{
  PROTOCOL *Prot;

  if ((Prot = SearchList(&ExtProtocols, &protocol)) == NULL)
    return FALSE;
  return (char) (Prot->Many == 'M' || Prot->Many == 'I');

}
/*
 * * EatExtMessage() * * This function eat a message upload using an
 * external protocol.  It returns * TRAN_SUCCESS on success, TRAN_FAILURE
 * otherwise.
 */
int
EatExtMessage(int protocol)
{
  struct FileInfoBlock *info;
  int       result;
  extern char TDirBuffer[];
  char      dir[80];

  MakeTempDir();
  if ((result = ExternalProtocol(protocol, TRUE, "msg", "", FALSE)) == TRAN_SUCCESS)
    {
        chdir(TDirBuffer);
        info = (struct FileInfoBlock *) calloc(1, sizeof(struct FileInfoBlock));

        if (info == NULL)
          {
            splitF(NULL, "Unable to get memory for info buffer(syszm)\n");

          }
        else
          {
            result = dfind(info, ALL_FILES, 0); /*
                                                                 * get the filename
                                                                 */
            if (result == 0)
                {
                  if (cfg.BoolFlags.debug)
                    splitF(NULL, "Ingesting:", info->fib_FileName);
                  ingestFile(info->fib_FileName, &msgBuf);
                  unlink(info->fib_FileName);

                }
            else
                splitF(NULL, "No file found!(syszm)\n");
            free(info);

          };

    }
  else
    {
        splitF(NULL, "Transfer failed(syszm)\n");
        if (cfg.BoolFlags.debug)
          {
            (void) getcd(0, dir);
            splitF(NULL, " Dir:%s\n Other:%s\n", dir, TDirBuffer);

          };

    };
  homeSpace();
  rmdir(TDirBuffer);
  return result;

}
/*
 * * AddName() * * This function is used in conjunction with wildCard to
 * generate a list * of names, optionally with filesizes in parenthesis
 * 4/line.  This is used * to generate a command line or a report in
 * FILELOG.SYS.
 */
void
AddName(DirEntry * file)
{
  if (strLen(msgBuf.mbtext) > MAXTEXT - 200)
    {
        if (GetSizes)
          strCat(msgBuf.mbtext, ".");

    }
  else
    {
        sPrintf(lbyte(msgBuf.mbtext), " %s", file->unambig);
        if (GetSizes)
          {
            sPrintf(lbyte(msgBuf.mbtext), " (%ld)", file->FileSize);
            if (++ExCount == 4)
                {
                  strCat(msgBuf.mbtext, "\n   ");
                  ExCount = 0;

                }

          }

    }

}
static char *msgText,
          UpsOnly;

/*
 * * UpProtsEnglish() * * This generates a list of upload protocol names.
 * It is used by the help * system.
 */
void
UpProtsEnglish(char *target)
{
  EnglishWork(target, TRUE);

}
/*
 * * DownProtsEnglish() * * This function generates a list of download
 * protocol names for use by the * help system.
 */
void
DownProtsEnglish(char *target)
{
  EnglishWork(target, FALSE);

}
/*
 * * EnglishWork() * * This function does the real work of generating names.
 */
void      List_Names(PROTOCOL *);
void
EnglishWork(char *target, char Ups)
{
  char     *c;

  target[0] = 0;
  msgText = target;
  UpsOnly = Ups;
  RunList(&ExtProtocols, List_Names);
  if ((c = strrchr(target, ',')) == NULL)
    strCpy(target, "None.");
  else
    strCpy(c, ".");

}
/*
 * * List_Names() * * This function helps build the list of protocols
 * available.
 */
void
List_Names(PROTOCOL * d)
{
  char     *c;

  if (UpsOnly != d->Down)
    {
        c = lbyte(msgText);
        sPrintf(c, "<%c>%s, ", d->Selector, (d->Selector == d->Name[0]) ?
                  d->Name + 1 : d->Name);

    }

}
/*
 * * ExternalTransfer() * * This function is used by the network for
 * external protocol transfers.
 */
char
ExternalTransfer(int protocol, char *filename)
{
  PROTOCOL *Prot;
  char     *name,
           *fn;

  if ((Prot = SearchList(&ExtProtocols, &protocol)) == NULL)
    return FALSE;
  fn = strdup(filename);
  if ((name = strrchr(fn, '\\')) != NULL)
    {
        *name++ = 0;
        chdir(fn);

    }
  else
    {
        name = fn;

    }
  if (!MakeCmdLine(cmdline, Prot->CmdLine, name, sizeof cmdline - 1))
    {
        free(fn);
        return FALSE;

    }
  Jsystem(cmdline);
  free(fn);
  homeSpace();
  return TRUE;

}
