/**
  Citadel Message procesing routines
**/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctdl.h"

extern CONFIG      cfg;   /* Lots an lots of variables    */
extern logBuffer   logBuf;    /* Person buffer    */

struct variables
  {
  char *ourname;     /* name of the variable */
  char type;         /* type of variable */
  union
    {
    UNS_16 *where; /* offset into codeBuf */
    char   *addr;  /* char data pointer */
    void   (*func1)(char *);
    char  *(*func2)(void);
    long   (*func3)(void);
    long   id;
    } object;
  };


#define CHAR_PTR    0   /* item is a character string pointer */
#define CFGB_PTR    1   /* item is offset into codeBuf        */
#define FUNC1_PT    2   /* Function: void  (func1 *)(char *target) */
#define FUNC2_PT    3   /* Function: char *(func2 *)(void) */
#define FUNC3_PT    4   /* Function: long  (func3 *)(void) */
#define IARG1_PT    5   /* Argument # 1 Integer    */
#define SARG1_PT    6   /* Argument # 1 String ptr */
#define IARG2_PT    7   /* Argument # 2 Integer    */
#define SARG2_PT    8   /* Argument # 2 String ptr */
#define IARG3_PT    9   /* Argument # 3 Integer    */
#define SARG3_PT   10   /* Argument # 3 String ptr */
#define NEW_LINE   11   /* special: add a new line and space*/
#define INT_PTR    12   /* integer data item */
#define SPACE_CH   13   /* special: add a space */
#define BACK_SPC   14   /* special: add a ^H character to the messge */
extern char *SysVers;
extern char  *VERSION;
extern char lastuser[];

#define MAXVARS (29)

static long message_sector_value(void);

struct variables variable[MAXVARS] =
  {
    { "variant",     CHAR_PTR, (unsigned short *)&VARIANT_NAME    },
    { "version",     CHAR_PTR, (unsigned short *)&VERSION_NAME    },
    { "sysvers",     CHAR_PTR, (unsigned short *)&SYSDEP_NAME     },
    { "baseroom",    CFGB_PTR, (unsigned short *)&cfg.bRoom       },
    { "sysop",       CHAR_PTR, (unsigned short *)&cfg.SysopName   }, /* 5 */
    { "nodetitle",   CFGB_PTR,                  &cfg.nodeTitle    },
    { "nodename",    CFGB_PTR,                  &cfg.nodeName     },
    { "nodedomain",  CFGB_PTR,                  &cfg.nodeDomain   },
    { "nodeid",      CFGB_PTR,                  &cfg.nodeId       },
    { "mainfloor",   CFGB_PTR,                  &cfg.MainFloor    }, /*10*/
    { "curruser",    CHAR_PTR, (unsigned short *)&logBuf.lbname   },
    { "ulprotocols", FUNC1_PT, (unsigned short *)UpProtsEnglish   },
    { "dlprotocols", FUNC1_PT, (unsigned short *)DownProtsEnglish },
    { "doorlist",    FUNC1_PT, (unsigned short *)DoorHelpListing  },
    { "lastuser",    CHAR_PTR, (unsigned short *)&lastuser        },/*15*/
    { "privileges",  FUNC2_PT, (unsigned short *)Display_Privledges},
    { "callcount",   FUNC3_PT, (unsigned short *)Get_Call_Count   },
    { "ia1",         IARG1_PT, (unsigned short *) 1               },
    { "sa1",         SARG1_PT, (unsigned short *) 1               },
    { "ia2",         IARG2_PT, (unsigned short *) 2               }, /*20*/
    { "sa2",         SARG2_PT, (unsigned short *) 2               },
    { "ia3",         IARG3_PT, (unsigned short *) 3               },
    { "sa3",         SARG3_PT, (unsigned short *) 3               },
    { "currtime",    FUNC2_PT, (unsigned short *)Current_Time     },
    { "currdate",    FUNC2_PT, (unsigned short *)formDate         }, /*25*/
    { "messages",    FUNC3_PT, (unsigned short *)message_sector_value },
    { "s",           SPACE_CH, NULL                               },
    { "b",           BACK_SPC, NULL                               },
    { "n",           NEW_LINE, NULL                               }

/***
    { "lastcall"    CHAR_PTR,(unsigned short *) NULL },
    { "events"      CHAR_PTR(unsigned short *) NULL },
    { "rooms"       CHAR_PTR(unsigned short *) NULL },
    { "roomsused"   CHAR_PTR(unsigned short *) NULL },
    { "lastmessage" CHAR_PTR(unsigned short *) NULL },
    { "logs"        CHAR_PTR(unsigned short *) NULL },
    { "logsused"    CHAR_PTR(unsigned short *) NULL },
    { "lenmessage"  CHAR_PTR(unsigned short *) NULL },
**/
  };

struct  Cit_Msg
  {
  struct Cit_Msg *link;/* link to common hashed messages */
  char MsgId[9];       /* Message Identifier */
  char MsgText[72];    /* Message Text */
  };

#define MAX_MSGS  ( 523 ) /* good sized prime number */

static struct Cit_Msg *Message_Table[MAX_MSGS];   /* dynamic message table */

/**
  The message file is made up of a count of the messages
  plus one line per message.  Each message is made up of a
  message ID(8 characters), plus the actual text(71 characters or less).
**/

static long message_sector_value()
  {
  return (long)cfg.maxMSector;
  }


static int Load_Items(FILE *fp);
static int Hashed_Msg(char *text);
void Load_Citadel_Messages(void);

static int Hashed_Msg(char *text)
  {
  int result;
  int cnt;
  for( result=0, cnt=0; text[cnt] != '\0'; cnt++)
    result = ( result << 1 ) + toupper(text[cnt]);
  result %= MAX_MSGS;
  return result;
  }

static int Load_Items(FILE *fp)
  {
  int cnt;
  char *p;
  char Text[200];        /* Message Text */
  struct Cit_Msg *ptr;  /* hashed Entry Value */
  struct Cit_Msg *tptr; /* hashed Entry Value for duplicate check */
  /**
    Get the ID, hash it, then add it to the table
    File format:
    # comment lines are ignored, and start with "#"
    1      8       10                  79
    IIIIIIII<space>TTTTTTTTTTTTTTTTTTTTTT<newline>
  **/
  while( fgets(Text,sizeof(Text),fp) )
    {
    if( strlen(Text) > 79 )
      {
      printf("WARNING: CIT_MESSAGE.SYS text line too long, Truncated:\n%s\n",Text);
      Text[79] = '\0';
      };
    if( Text[0] == '#' )continue;                /* skip comment characters */
    if( (p=strchr(Text,'\n')) != NULL)*p = '\0'; /* get rid of newline      */
    Text[8] = '\0';
    ptr = calloc(1,sizeof(struct Cit_Msg));     /* get an element          */
    if( ptr == NULL)return 2;                   /* No memory available     */
    strcpy(ptr->MsgId,   &Text[0]);
    strcpy(ptr->MsgText, &Text[9]);             /* build entry             */
    cnt        = Hashed_Msg(&Text[0]);
    tptr       = Message_Table[cnt];
/**
  Look up the message to be sure we don't have a duplicate
**/
    while( tptr != NULL )
      {
      if( stricmp(tptr->MsgId, ptr->MsgId) == 0 )break;
      tptr = tptr->link;
      };
    if( tptr != NULL )
      printf("Error:  Duplicate Message Code(%s) in Cit_Message.Sys\n",Text);
    ptr->link  = Message_Table[cnt];
    Message_Table[cnt] = ptr;
    };
  return 0;
  }

#define MAX_LINE (120)

extern MessageBuffer     msgBuf;
static void Fix_Line(char *line, char *buffer, long arg1, long arg2, long arg3);

static void Fix_Line(char *line, char *buffer, long arg1, long arg2, long arg3)
  {
  struct variables *vptr;
  char *ptr;
  char *tmp;
  long inttmp;
  int index = strlen(buffer);
  while( *line != '\0' )
    {
    if( *line == '^' )
      {
      int i;
      ptr = ++line;
      buffer[index] = '\0';
      for( i=0; i < MAXVARS; i++)
        {
        vptr = &variable[i];
        if( strncmp(vptr->ourname, ptr, strlen(vptr->ourname)) == 0)
          {
          switch (vptr->type)
            {
            case BACK_SPC: /* insert a backspace character in message */
               buffer[index] = '\b';
               break;
            case NEW_LINE: /* put in a newline and space */
              strcat(buffer,"\n ");
              break;
            case SPACE_CH: /* insert a space */
              strcat(buffer," ");
              break;
            case CHAR_PTR: /* char pointer, direct access */
              strcat(buffer,vptr->object.addr);
              break;
            case CFGB_PTR: /* make a char pointer, then direct access */
              strcat(buffer,&cfg.codeBuf[*vptr->object.where]);
              break;
            case FUNC1_PT: /* call the function, direct to buffer */
              (vptr->object.func1)(lbyte(buffer));
              break;
            case FUNC2_PT: /* call the function, direct access */
              tmp = (vptr->object.func2)();
              strcat(buffer, tmp );
              break;
            case FUNC3_PT: /* call the function, convert number into buffer */
              inttmp = (vptr->object.func3)();
              sprintf(lbyte(buffer),"%ld", inttmp );
              break;
            case INT_PTR:  /* integer argument */
              sprintf(lbyte(buffer), "%ld, *vptr->object.addr");
              break;
            case IARG1_PT: /* argument in call */
              sprintf(lbyte(buffer),"%ld",arg1);
              break;
            case SARG1_PT: /* argument is */
              if( arg1 != NULL )sprintf(lbyte(buffer),"%s",arg1);
              break;
            case IARG2_PT: /* argument in call */
              sprintf(lbyte(buffer),"%ld",arg2);
              break;
            case SARG2_PT: /* argument is */
              if( arg1 != NULL )sprintf(lbyte(buffer),"%s",arg2);
              break;
            case IARG3_PT: /* argument in call */
              sprintf(lbyte(buffer),"%ld",arg3);
              break;
            case SARG3_PT: /* argument is  */
              if( arg1 != NULL )sprintf(lbyte(buffer),"%s",arg3);
              break;

            };
          line += strlen(vptr->ourname);
          index = strlen(buffer);
          break;
          }
        };
      if( i == MAXVARS )buffer[index++] = *line++;
      }
    else buffer[index++] = *line++;
    };
  buffer[index] = '\0';
  }

static struct Cit_Msg *Look_Up(char *code);

static struct Cit_Msg *Look_Up(char *code)
  {
  struct Cit_Msg *ptr;
  ptr = Message_Table[Hashed_Msg(code)];
  if( ptr ==  NULL )return NULL;
  while( ptr != NULL )
    {
    if( stricmp(ptr->MsgId, code) == 0)return ptr;
    ptr = ptr->link;
    };
  return NULL;
  }

char *msgbuffer=NULL;   /* Initialized from available memory */

void Output_Citadel_Message(char *pcode,long arg1, long arg2, long arg3)
  {
  char code[9];
  FILE *op;
  char line[MAX_LINE];
  struct Cit_Msg *ptr;
/**
  translate the code into a message
**/
  if( msgbuffer == NULL )    msgbuffer = GetDynamic(MAXTEXT);
  strncpy(code,pcode,6);
  code[6] = '\0';
  strcat(code,expert ? "EX" : "NO");
  code[8] = '\0';
  ptr = Look_Up(code);     /* find the code or return NULL */
  if( ptr == NULL && expert )
    {
    code[6] = 'N';
    code[7] = 'O';
    ptr =  Look_Up(code);
    };
  if( ptr == NULL )
    {
    mPrintf("\n ***Error Code %s\n",code);
    }
  else
    {
    if( ptr->MsgText[0] == '@')
      {
      if( (op=fopen(&ptr->MsgText[1],"r")) == NULL )
        {
        mPrintf("\n *** Error Code %s, filename:%s not found\n",code,&ptr->MsgText[1]);
        }
      else
        {
        msgbuffer[0] = '\0';
        while (fgets(line, MAX_LINE, op) )
          {
          char *p;
          if( (p=strchr(line,'\n')) != NULL)*p = '\0'; /* get rid of newline      */
          Fix_Line(line,msgbuffer, arg1,arg2, arg3);
          if( ( strlen(msgbuffer) + strlen(line) + 500 ) > MAXTEXT )
            {
            mPrintf("%s", msgbuffer);
            msgbuffer[0] = '\0';
            };
          };
        fclose(op);
        };
      }
    else
      {
      msgbuffer[0] = '\0';
      Fix_Line(ptr->MsgText,msgbuffer, arg1, arg2, arg3);
      };
    if( strlen(msgbuffer) > 0 ) mPrintf("%s",msgbuffer);

    }
  }

void Load_Citadel_Messages()
  {
  int code;       /* Error Code */
  char temp[80];  /* input filename  */
  FILE *fp;       /* input file pointer */
/*  SpecialMessage("Message Initialization"); */
  makeSysName(temp,"Cit_Messages.sys", &cfg.msgArea);
  if( (fp=fopen(temp,/* READ_TEXT*/ "r")) != NULL )
    {
    memset(Message_Table,'\0',sizeof(struct Cit_Msg *) * MAX_MSGS );
    code = Load_Items( fp );
    fclose(fp);
    }
  else code = 1;      /* file will not open */
  if( code != 0 )
    {
    printf(" Filename:%s\n",temp);
    printf(" Load Messages Error code %d\n",code);
    };
  }
extern int       outPut;
extern char      outFlag;
extern char      Showing;
extern char      ReverseMessage;
extern char      pullMessage;
extern int       thisRoom;
extern char      journalMessage;
extern char      MsgStreamEnter;
extern char      echo;           /* Output flag  */
extern char      echoChar;
extern char      loggedIn;       /* Logged in flag      */
extern SListBase Moderators;
extern char      pause_whichMess;
char  Pause_Message_Check()
  {
  /**
    If this is a user, and the user has Pause at end of message
    in their configuration, we will pause here asking for a character.
    We will eat all input first(just incase of line noise).
    A "S" will attempt to stop the message stream.
    A "N" will attempt to skip to next message.
    Basically, we duplicate MABORT() here...
  **/
   char c, toReturn, oldEcho;
   toReturn = FALSE;
   if( outFlag == IMPERVIOUS || outFlag == NET_CALL
    || outPut == DISK )return toReturn;
   if( loggedIn )
     {
     if( logBuf.lbflags.MSG_PAUSE
      && ( ( pause_whichMess != NEWoNLY
          && thisRoom == MAILROOM ) || thisRoom != MAILROOM) )
       {
       if( aide ||
           ( strCmpU(logBuf.lbname, AskForNSMap(&Moderators, thisRoom)) == SAMESTRING
             && strLen(logBuf.lbname) != 0 ))
         {
         Output_Citadel_Message("SYSPAU",NULL, NULL, NULL);
         }
       Output_Citadel_Message("MSGPAU",NULL, NULL, NULL);
       oldEcho = echo;
       echo    = NEITHER;
       echoChar= 0;
       while (MIReady())   inp();   /* eat noise */
       c = toUpper(modIn());        /* accept one character */
       switch (c)
         {
         case 'D':  /* delete the message if you have privileges */
           if( Showing == MSGS &&
               ( aide ||
                 ( strCmpU(logBuf.lbname, AskForNSMap(&Moderators, thisRoom)) == SAMESTRING
                 && strLen(logBuf.lbname) != 0)))
              {
              pullMessage = TRUE;
              toReturn = TRUE;
              };
            break;
          case 'S' : /* stop displaying messages */
            outFlag  = OUTSKIP;
            toReturn = TRUE;
            break;
         case 'J':  /* Journal the Message */
           if( Showing == MSGS &&
               ( aide ||
                 ( strCmpU(logBuf.lbname, AskForNSMap(&Moderators, thisRoom)) == SAMESTRING
                 && strLen(logBuf.lbname) != 0 ) ) )
              {
              journalMessage = TRUE;
              toReturn       = TRUE;
              };
            break;
         case 'E':  /* Enter a Reply */
            if( Showing == MSGS && HasWritePrivs())
              {
              MsgStreamEnter = TRUE;
              outFlag        = OUTSKIP;
              toReturn       = TRUE;
              };
            break;
         case 'R':  /* Reverse the flow */
            if( Showing == MSGS )
              {
              ReverseMessage = TRUE;
              toReturn       = TRUE;
              };
            break;
         };
       echo = oldEcho;
       }
     else if( Showing == MSGS && thisRoom == MAILROOM && pause_whichMess == NEWoNLY)toReturn = TRUE;
     };
  return toReturn;
  }
