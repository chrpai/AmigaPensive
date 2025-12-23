/*
** CNet Listserver - ala Majordomo
** by Kelly Cochran
** Last updated: Sat Oct 17 20:25:25 1998
*/

#include "structs.h"

#define VERS "0"
#define REVS "10"

void __regargs __chkabort(void);
void __regargs __chkabort(void) { return; }

void process_item(struct ListEntry *list, struct MailItem *mi);
BOOL read_mail(struct MailItem *mi, char *filename);
void process_request(struct MailItem *mi);
BOOL load_config(char *listname, struct ListEntry **le);
void save_config(struct ListEntry *le, ULONG save_flags);
BOOL parse_user_line(char *line, struct UserEntry *ue);
void AddUserToList(struct List *list, struct UserEntry *temp_ue);
void AddHeaderToList(struct List *list, char *header);
void free_hdr_list(struct List **list);
void free_config(struct ListEntry *list);
void free_mail(struct MailItem *mi);
void reply2sender(struct MailItem *mi);
void send2list(struct MailItem *mi);
void saveitem(BPTR fp, char *file);
void archiveitem(struct ListEntry *list, struct MailItem *mi);
void log2sender(char *text, ...);
void log(char *text, ...);

struct UserEntry *FindUser(struct ListEntry *list, char *name, ULONG flags);
#define CND_UFLG_NONE       0L
#define CND_UFLG_USER    (1<<0)
#define CND_UFLG_POSTER  (1<<1)
#define CND_UFLG_PENDING (1<<2)
#define CND_UFLG_BANNED  (1<<3)
#define CND_UFLG_CONFIG  (1<<4)

int cmd_subscribe(void);
int cmd_confirm(void);
int cmd_unsubscribe(void);
int cmd_config(void);
int cmd_newconfig(void);
int cmd_lists(void);
int cmd_who(void);
int cmd_info(void);
int cmd_newinfo(void);
int cmd_help(void);

struct MailSpoolHeader 
{ 
   char destaddr [ 256 ]; 
   char from     [ 256 ]; 
} spoolhead;

struct CNdomoCmd CND_Cmds[] =
{
   "subscribe"  , cmd_subscribe  , 0, 2, CND_DA_YES,
   "confirm"    , cmd_confirm    , 2, 3, CND_DA_YES,
   "unsubscribe", cmd_unsubscribe, 0, 2, CND_DA_YES,
   "config"     , cmd_config     , 2, 2, CND_DA_YES,
   "newconfig"  , cmd_newconfig  , 2, 2, CND_DA_YES,
   "lists"      , cmd_lists      , 0, 0, CND_DA_REQ,
   "who"        , cmd_who        , 0, 1, CND_DA_YES,
   "info"       , cmd_info       , 0, 1, CND_DA_YES,
   "newinfo"    , cmd_newinfo    , 0, 1, CND_DA_YES,
   "help"       , cmd_help       , 0, 0, CND_DA_YES,
};

struct MailHeaderClass Headers[] =
{
  "From"         , HDR_FROM        ,
  "To"           , HDR_TO          ,
  "Date"         , HDR_DATE        ,
  "Subject"      , HDR_SUBJECT     ,
  "Reply-to"     , HDR_REPLYTO     ,
  "Organization" , HDR_ORGANIZATION,
  "Approved"     , HDR_APPROVED    ,
  "X-ListServer" , HDR_XLISTSERVER ,
  "Message-ID"   , HDR_MESSAGEID   ,
  "Received"     , HDR_RECEIVED    ,
  "X-DebugHeader", HDR_OTHER       ,
  NULL           , 0               ,
};

struct Library  *CNetBase  = NULL;
struct Library  *CNet4Base = NULL;
struct MainPort *myp       = NULL;

const  char *version   = "\0$VER: CNdomo "VERS"."REVS" [ALPHA/dnpT] "__AMIGADATE__;
char   tempfile [ 16 ] = "";
ULONG  filenum         = 0L;
struct ListEntry *le   = NULL;  /* Ok, so I'm lazy */

#define TEMPLATE "LIST/A,MAILFILE/A,REQUEST/S"

enum
{
   ARG_LIST = 0,
   ARG_MAILFILE,
   ARG_REQUEST,
   NUM_ARGS
};

int main(void)
{
   LONG args [ NUM_ARGS ];

   struct RDArgs *rda;

   filenum = (ULONG)FindTask(NULL);

   if(rda = ReadArgs(TEMPLATE, args, NULL))
   {
      if(CNetBase = OpenLibrary("cnet.library", 4))
      {
         if(CNet4Base = OpenLibrary("cnet4.library", 4))
         {
            Forbid();
            if(myp = (struct MainPort *)FindPort(CNETPORT))
            {
               Permit();
               if(args[ARG_LIST] && args[ARG_MAILFILE])
               {
                  struct MailItem mi;

                  memset(&mi, 0, sizeof(struct MailItem));

                  load_config((char *)args[ARG_LIST], &le);

                  if(read_mail(&mi, (char *)args[ARG_MAILFILE]))
                  {
                     if(args[ARG_REQUEST] || !stricmp("CNDOMO-PRIMARY", (char *)args[ARG_LIST]))
                     {
                        process_request(&mi);
                     }
                     else if(le)
                     {
                        process_item(le, &mi);
                     }
                     else
                     {
                        Printf("Problems loading "LISTDIR"/%s!\n", (char *)args[ARG_LIST]);
                     }

                     reply2sender(&mi);

                     if(le)
                     {
                        free_config(le);
                     }
                     free_mail(&mi);
                  }
               }
            }
            else
            {
               Permit();
            }
            CloseLibrary(CNet4Base);
         }
         CloseLibrary(CNetBase);
      }
   }
   else
   {
      Printf("Invalid usage!  Usage: CNDomo <list> <mailfile>\n");
   }

   if(rda)
   {
      FreeArgs(rda);
   }
}

void process_item(struct ListEntry *list, struct MailItem *mi)
{
   struct UserEntry *ue;

   if(mi && list)
   {
      if(FindUser(list, mi->From, CND_UFLG_BANNED))
      {
         log2sender("You are banned from %s\n", list->le_Name);
         log("[%s] Attempted post from banned user: %s", list->le_Name, mi->From);
         return;
      }

      if(list->le_Flags & LEF_CLOSED)
      {
         if(!(ue = FindUser(list, mi->From, CND_UFLG_USER)))
         {
            if(!(ue = FindUser(list, mi->From, CND_UFLG_POSTER)))
            {
               log2sender("This is a closed list.\n");
               log("[%s] Attempted post from non-member: %s", list->le_Name, mi->From);
               return;
            }
         }
      }

      if(list->le_Flags & LEF_MODERATED)
      {
         if(!(ue = FindUser(list, mi->From, CND_UFLG_POSTER)))
         {
            log2sender("This is a moderated list.\n");
            log("[%s] Attempted post from unauthorized user: %s", list->le_Name, mi->From);
            return;
         }
      }

      if(*tempfile && FileExists(tempfile))
         log2sender("\n");

      if(!(mi->Flags & MIF_LOOP))
      {
         send2list(mi);
         log("[%s] Post from %s", list->le_Name, mi->From);

         if(le->le_Flags & LEF_ARCHIVE)
         {
            archiveitem(list, mi);
         }

         list->le_Number++;
         save_config(list, CND_UFLG_CONFIG);
         CNetPutMsg(MAILTASKPORT, MT_RESCAN_MSG);

         if(list->le_Flags & LEF_SENDSPOOL)
         {
            CNetPutMsg(MAILTASKPORT, MT_SENDSPOOL_MSG);
         }
      }
      else
      {
         log("[%s] Loop detected! From: <%s>; To: <%s>", le->le_Name, mi->From, mi->To);
      }
   }
}

void process_request(struct MailItem *mi)
{
   return;
}

int cmd_subscribe(void)
{
   return 1;
}

int cmd_confirm(void)
{
   return 1;
}

int cmd_unsubscribe(void)
{
   return 1;
}

int cmd_config(void)
{
   return 1;
}

int cmd_newconfig(void)
{
   return 1;
}

int cmd_lists(void)
{
   return 1;
}

int cmd_who(void)
{
   return 1;
}

int cmd_info(void)
{
   return 1;
}

int cmd_newinfo(void)
{
   return 1;
}

int cmd_help(void)
{
   return 1;
}

BOOL read_mail(struct MailItem *mi, char *filename)
{
   BPTR fp;

   char line [ 1024 ] = "";

   struct MailHeaderClass *mhc;

   mi->Size = FileSize(filename);

   if(mi->Size && (fp = Open(filename, MODE_OLDFILE)))
   {
      while(FGets(fp, line, sizeof(line)))
      {
         char *temp,
              *head = line,
              *val;

         BOOL found = FALSE;

         mhc = Headers;

         if((*line == '\n') || (*line == '\r'))
            break;   /* end of headers */
         else
         {
            if(isspace(*line))
            {
               continue;   /* continuation of prior header */
            }
            else if(!(temp = strchr(line, ':')))
            {
               break;
            }

            *temp = '\0';  /* null the ":" */
            temp++;        /* walk past it */
            while(*temp && isspace(*temp))   /* eat leading spaces */
            {
               temp++;
            }

            val = temp; /* header value is there */

            temp += strlen(temp);   /* move temp to end of string */

            while(*temp && !isalnum(*temp))  /* eat trailing spaces */
            {
               --temp;
            }

            *(--temp) = '\0'; /* terminate string */

            while(!found && mhc->name)  /* find if it's a header we want */
            {
               if(!strcmp(mhc->name, head))  /* yup, bust from loop */
               {
                  found = TRUE;
                  continue;
               }
               mhc++;
            }

            if(found)  /* did we find one? */
            {
               int vallen = strlen(val);

               switch(mhc->type)
               {
                  case HDR_FROM: /* From: header */
                     {
                        char *tmp,
                             *tmp2 = NULL;

                        strncpy(mi->FullFrom, val, MIN(vallen, sizeof(mi->FullFrom))); /* copy full field */

                        while(tmp = strchr(val, '<'))
                           val = tmp2 = (++tmp);                 

                        if(tmp2)
                        {
                           val = tmp2;
                           if(tmp2 = strchr(val, '>'))
                              *tmp2 = '\0';
                        }

                        vallen = strlen(val);

                        strncpy(mi->From, val, MIN(vallen, sizeof(mi->From)));  /* copy only the address */
                     }
                     break;

                  case HDR_REPLYTO:
                     strncpy(mi->ReplyTo, val, MIN(vallen, sizeof(mi->ReplyTo)));
                     break;

                  case HDR_DATE:
                     strncpy(mi->Date, val,  MIN(vallen, sizeof(mi->Date)));
                     break;

                  case HDR_SUBJECT:
                     strncpy(mi->Subject, val,  MIN(vallen, sizeof(mi->Subject)));
                     break;

                  case HDR_MESSAGEID:
                     strncpy(mi->MessageID, val,  MIN(vallen, sizeof(mi->MessageID)));
                     break;

                  case HDR_ORGANIZATION:
                     strncpy(mi->Org, val,  MIN(vallen, sizeof(mi->Org)));
                     break;

                  case HDR_XLISTSERVER:
                     if(!strstr(val, "CNdomo"))
                     {
                        mi->Flags |= MIF_LOOP;
                     }
                     break;

                  case HDR_TO:
                     {
                        char *tmp,
                             *tmp2 = NULL;

                        while(tmp = strchr(val, '<'))
                           val = tmp2 = (++tmp);                 

                        if(tmp2)
                        {
                           val = tmp2;
                           if(tmp2 = strchr(val, '>'))
                              *tmp2 = '\0';
                        }

                        vallen = strlen(val);

                        strncpy(mi->To, val, MIN(vallen, sizeof(mi->To)));  /* copy only the address */
                     }
                     break;

                  case HDR_RECEIVED:
                     if(mi->RcvHdrs || (mi->RcvHdrs = init_list()))
                     {
                        AddHeaderToList(mi->RcvHdrs, val);
                     }

                  default:
                     break;
               }
            }
         }
      }  /* while(FGets()) */

      if(!stricmp(mi->To, mi->From))
      {
         mi->Flags |= MIF_LOOP;
      }

      mi->Size -= Seek(fp, 0, OFFSET_CURRENT);

      if(le && le->le_MaxPostSize && (mi->Size > le->le_MaxPostSize))
      {
         Close(fp);

         log2sender("Error: message too large\n");
         log("[%s] Message from %s too large (%ld bytes)", le->le_Name, mi->From, mi->Size);
         return FALSE;
      }
      else if(!le && (mi->Size > 102400))
      {
         Close(fp);

         log2sender("Error: message too large\n");
         log("[%s] Message from %s too large (%ld bytes)", "admin", mi->From, mi->Size);
         return FALSE;
      }

      if(mi->Message = AllocMem(mi->Size, MEMF_CLEAR))
      {
         Read(fp, mi->Message, mi->Size);
      }

      Close(fp);
   } /* if(fp) */
   else
   {
      return FALSE;
   }

   return TRUE;
}

void free_mail(struct MailItem *mi)
{
   if(mi->Message)
   {
      FreeMem(mi->Message, mi->Size);
      free_hdr_list(&mi->RcvHdrs);
   }
}

BOOL load_config(char *listname, struct ListEntry **le)
{
   struct ListEntry *list;

   if(list = (struct ListEntry *)AllocVec(sizeof(struct ListEntry), MEMF_CLEAR))
   {
      BPTR lfp,
           ufp,
           bfp;

      char buff [ 256 ] = "";

      *le = list; /* set pointer for return */

      sprintf(buff, LISTDIR"/%s/Config", listname);

      if(lfp = Open(buff, MODE_OLDFILE))
      {
         Read(lfp, list, sizeof(struct ListEntry));

         list->le_Users   = NULL;   /* NULL these out just in case.. */
         list->le_Posters = NULL;
         list->le_Pending = NULL;
         list->le_Banned  = NULL;

         Close(lfp);
      }
      else
      {
         return FALSE;
      }

      if(list->le_Flags & (LEF_MODERATED|LEF_CLOSED))  /* Moderated list? */
      {
         BPTR pofp;

         sprintf(buff, LISTDIR"/%s/AllowPost", list->le_Name);

         if(pofp = Open(buff, MODE_OLDFILE))
         {
            if(list->le_Posters = init_list())
            {
               struct UserEntry ue;

               while(FGets(pofp, buff, sizeof(buff)))
               {
                  if(parse_user_line(buff, &ue))
                  {
                     AddUserToList(list->le_Posters, &ue);
                  }
               }
            }
            Close(pofp);
         }
      }

      if(list->le_Flags & LEF_CONFIRM)
      {
         BPTR pfp;

         sprintf(buff, LISTDIR"/%s/Pending", list->le_Name);

         if(pfp = Open(buff, MODE_OLDFILE))
         {
            if(list->le_Pending = init_list())
            {
               struct UserEntry ue;

               while(FGets(pfp, buff, sizeof(buff)))
               {
                  if(parse_user_line(buff, &ue))
                  {
                     AddUserToList(list->le_Pending, &ue);
                  }
               }
            }
            Close(pfp);
         }
      }

      sprintf(buff, LISTDIR"/%s/Users", list->le_Name);

      if(ufp = Open(buff, MODE_OLDFILE))
      {
         if(list->le_Users = init_list())
         {
            struct UserEntry ue;

            while(FGets(ufp, buff, sizeof(buff)))
            {
               if(parse_user_line(buff, &ue))
               {
                  AddUserToList(list->le_Users, &ue);
               }
            }
         }
         Close(ufp);
      }

      sprintf(buff, LISTDIR"/%s/Banned", list->le_Name);

      if(bfp = Open(buff, MODE_OLDFILE))
      {
         if(list->le_Banned = init_list())
         {
            struct UserEntry ue;

            while(FGets(bfp, buff, sizeof(buff)))
            {
               if(parse_user_line(buff, &ue))
               {
                  AddUserToList(list->le_Banned, &ue);
               }
            }
         }
         Close(bfp);
      }

      return TRUE;
   }

   return FALSE;
}

void save_config(struct ListEntry *le, ULONG save_flags)
{
   BPTR fp;

   char buff [ 256 ] = "";

   if(save_flags & CND_UFLG_CONFIG)
   {
      sprintf(buff, LISTDIR"/%s/Config", le->le_Name);

      if(fp = Open(buff, MODE_OLDFILE))
      {
         Write(fp, le, sizeof(struct ListEntry));
         Close(fp);
      }
   }
   if(save_flags & CND_UFLG_USER)
   {
      /* would be userlist saving routines */
   }
   if(save_flags & CND_UFLG_POSTER)
   {
      /* would be userlist saving routines */
   }
   if(save_flags & CND_UFLG_PENDING)
   {
      /* would be userlist saving routines */
   }
   if(save_flags & CND_UFLG_BANNED)
   {
      /* would be userlist saving routines */
   }
}

void free_config(struct ListEntry *list)
{
   if(list)
   {
      if(list->le_Users)
      {
         free_list(&list->le_Users);
      }
      if(list->le_Posters)
      {
         free_list(&list->le_Posters);
      }
      if(list->le_Pending)
      {
         free_list(&list->le_Pending);
      }
      if(list->le_Banned)
      {
         free_list(&list->le_Banned);
      }

      FreeVec(list);
   }
}

BOOL parse_user_line(char *line, struct UserEntry *ue)
{
   char *tmp;

   if(!*line)
      return FALSE;

   line[strlen(line) - 1] = '\0';

   memset(ue, 0, sizeof(struct UserEntry));

   if(tmp = strchr(line, ':'))
   {
      *tmp = '\0';
      tmp++;
      strncpy(ue->ue_From, line, sizeof(ue->ue_From) - 1);
   }

   if(*tmp)
   {
      strncpy(ue->ue_Key, tmp, sizeof(ue->ue_Key) - 1);
   }

   return TRUE;
}

void AddUserToList(struct List *list, struct UserEntry *ue)
{
   struct UserEntryNode *uen;

   if(uen = (struct UserEntryNode *)AllocVec(sizeof(struct UserEntryNode), MEMF_CLEAR))
   {
      CopyMem(ue, (APTR)&uen->ue, sizeof(struct UserEntry));
      uen->ue_Node.ln_Name = uen->ue.ue_From;
      AddTail((struct List *)list, (struct Node *)uen);
   }
}

void AddHeaderToList(struct List *list, char *header)
{
   struct Node *temp;

   if(temp = (struct Node *)AllocVec(sizeof(struct Node), MEMF_CLEAR))
   {
      char *work = (char *)AllocVec(strlen(header), MEMF_CLEAR);

      if(work)
      {
         strcpy(work, header);
         temp->ln_Name = work;
      }
      AddTail((struct List *)list, (struct Node *)temp);
   }
}

void free_hdr_list(struct List **list)
{
   struct Node *temp;
   struct Node *work;

   if(*list)
   {
      work = (*list)->lh_Head;

      while(work && work->ln_Succ)
      {
         if(work->ln_Name)
         {
            FreeVec(work->ln_Name);
         }
         temp = work->ln_Succ;
         FreeVec(work);
         work = temp;
      }
      FreeVec(*list);

      *list = NULL;
   }
}

void reply2sender(struct MailItem *mi)
{
   if(*tempfile && FileExists(tempfile))
   {
      BPTR fp;

      char tmpf   [ 40 ] = "";
      char buffer [ 32 ] = "";

      do 
      {
         sprintf(tmpf, CNETOUTBOX"/Spool/%08x.mts", filenum);
         filenum++;
      } while(FileExists(tmpf));

      if(fp = Open(tmpf, MODE_NEWFILE))
      {
         struct MailSpoolHeader msh;

         memset(&msh, 0, sizeof(struct MailSpoolHeader));

         strcpy(msh.destaddr, *mi->ReplyTo ? mi->ReplyTo:mi->From);

         if(le)
         {
            strcpy(msh.from, le->le_From);
         }
         else
         {
            sprintf(msh.from, "%s@%s", "cndomo", myp->gc.MyUUCPName);
         }

         Write(fp, &msh, sizeof(struct MailSpoolHeader));
         FPrintf(fp, "Subject: %s\r\n", *mi->Subject ? mi->Subject:"<none>");

         if(*mi->MessageID)
         {
            FPrintf(fp, "Message-ID: %s\r\n", mi->MessageID);
         }

         GetRFCDate(buffer);
         FPrintf(fp, "Date: %s\r\n", buffer);

         FPuts(fp, "X-ListServ: CNdomo "VERS"."REVS"\r\n\n");

         saveitem(fp, tempfile);

         Close(fp);
      }
   }
}

void send2list(struct MailItem *mi)
{
   struct UserEntryNode   *uen = (struct UserEntryNode *)le->le_Users->lh_Head;
   struct MailSpoolHeader  msh;

   BPTR fp;

   char tmpf [ 40 ] = "";

   memset(&msh, 0, sizeof(struct MailSpoolHeader));

   if(le->le_Flags & LEF_SETLISTFROM)
   {
      strcpy(msh.from, le->le_From);
   }
   else
   {
      strcpy(msh.from, mi->FullFrom);
   }

   for(; uen && uen->ue_Node.ln_Succ; uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ)
   {
      strcpy(msh.destaddr, uen->ue.ue_From);

      do
      {
         sprintf(tmpf, CNETOUTBOX"/Spool/%08x.mts", filenum);
         filenum++;
      } while(FileExists(tmpf));

      if(fp = Open(tmpf, MODE_NEWFILE))
      {
         Write(fp, &msh, sizeof(struct MailSpoolHeader));

         if(mi->RcvHdrs)
         {
            struct Node *work = mi->RcvHdrs->lh_Head;

            while(work && work->ln_Succ)
            {
               if(work->ln_Name && *work->ln_Name)
               {
                  FPrintf(fp, "Received: %s\r\n", work->ln_Name);
               }
               work = work->ln_Succ;
            }
         }

         if(*mi->Date)
         {
            FPrintf(fp, "Date: %s\r\n", mi->Date);
         }
         else
         {
            char buffer [ 32 ] = "";
            GetRFCDate(buffer);
            FPrintf(fp, "Date: %s\r\n", buffer);
         }

         if(*le->le_ReplyTo)
            FPrintf(fp, "Reply-to: %s\r\n", le->le_ReplyTo);
         if(*le->le_ErrorTo)
            FPrintf(fp, "Errors-to: %s\r\n", le->le_ErrorTo);

         if(*mi->Org)
            FPrintf(fp, "Organization: %s\r\n", mi->Org);
         if(*mi->MessageID)
            FPrintf(fp, "Message-ID: %s\r\n", mi->MessageID);

         if(le->le_Flags & LEF_SETLISTFROM)
            FPrintf(fp, "Originally-From: %s\r\n", mi->FullFrom);

         FPrintf(fp, "Subject: %s\r\n", *mi->Subject ? mi->Subject:"<none>");

         FPuts(fp, "X-ListServ: CNdomo "VERS"."REVS"\r\n\n");

         Flush(fp);

         Write(fp, mi->Message, mi->Size);

         Close(fp);
      }
   }
}

void log2sender(char *text, ...)
{
   BPTR fp;

   if(!*tempfile)
   {
      do
      {
         sprintf(tempfile, "T:%08x.cnd", filenum);
         filenum++;
      } while(FileExists(tempfile));
   }

   if(fp = OpenAppend(tempfile))
   {
      VFPrintf(fp, text, (ULONG *)(&text + 1));
      Close(fp);
   }
}

void log(char *text, ...)
{
   BPTR fp;

   struct IsDate now;

   char buff [ 16 ] = "";

   CNetExplodeTime(CNetTime(), &now);

   if(fp = OpenAppend("SysData:Log/CNdomo"))
   {
      CNetPrintDate(&now, buff, NULL, "dd-mmm HH:MMa");
      FPrintf(fp, "%s: ", buff);
      VFPrintf(fp, text, (ULONG *)(&text + 1));
      FPuts(fp, "\n");
      Close(fp);
   }
}

void archiveitem(struct ListEntry *le, struct MailItem *mi)
{
   BPTR fp;

   char buff [ 256 ] = "";

   sprintf(buff, LISTDIR"/%s/Archive", le->le_Name);

   if(!FileExists(buff))
   {
      if(fp = CreateDir(buff))
      {
         UnLock(fp);
      }
      else
      {
         log("[%s] Unable to make Archive dir!", le->le_Name);
         return;
      }
   }

   sprintf(buff, LISTDIR"/%s/Archive/%04ld", le->le_Name, le->le_Number);

   if(fp = Open(buff, MODE_NEWFILE))
   {
      if(mi->RcvHdrs)
      {
         struct Node *work = mi->RcvHdrs->lh_Head;

         while(work && work->ln_Succ)
         {
            if(work->ln_Name && *work->ln_Name)
            {
               FPrintf(fp, "Received: %s\n", work->ln_Name);
            }
            work = work->ln_Succ;
         }
      }

      if(*mi->Date)
      {
         FPrintf(fp, "Date: %s\n", mi->Date);
      }
      else
      {
         char buffer [ 32 ] = "";
         GetRFCDate(buffer);
         FPrintf(fp, "Date: %s\n", buffer);
      }

      if(*le->le_ReplyTo)
         FPrintf(fp, "Reply-to: %s\n", le->le_ReplyTo);
      if(*le->le_ErrorTo)
         FPrintf(fp, "Errors-to: %s\n", le->le_ErrorTo);

      if(*mi->Org)
         FPrintf(fp, "Organization: %s\n", mi->Org);
      if(*mi->MessageID)
         FPrintf(fp, "Message-ID: %s\n", mi->MessageID);

      if(le->le_Flags & LEF_SETLISTFROM)
         FPrintf(fp, "Originally-From: %s\n", mi->FullFrom);

      FPuts(fp, "X-ListServ: CNdomo "VERS"."REVS"\n\n");

      FPrintf(fp, "Subject: %s\n", *mi->Subject ? mi->Subject:"<none>");

      Flush(fp);

      Write(fp, mi->Message, mi->Size);

      Close(fp);
   }
}

void saveitem(BPTR fp, char *filename)
{
   BPTR ffp;

   char buff [ 128 ] = "";

   if(ffp = Open(filename, MODE_OLDFILE))
   {
      while(FGets(ffp, buff, sizeof(buff)-1) && (*buff != 26))
      {
         FPuts(fp, buff);
      }
      Close(ffp);
   }
}

struct UserEntry *FindUser(struct ListEntry *list, char *name, ULONG flags)
{
   struct UserEntryNode *uen;

   switch(flags)
   {
      case CND_UFLG_USER:
         if(list->le_Users)
         {
            uen = (struct UserEntryNode *)le->le_Users->lh_Head;

            for(; uen && uen->ue_Node.ln_Succ; uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ)
            {
               if(!stricmp(name, uen->ue.ue_From))
               {
                  return (&uen->ue);
               }
            }
         }
         break;

      case CND_UFLG_POSTER:
         if(list->le_Posters)
         {
            uen = (struct UserEntryNode *)le->le_Posters->lh_Head;

            for(; uen && uen->ue_Node.ln_Succ; uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ)
            {
               if(!stricmp(name, uen->ue.ue_From))
               {
                  return (&uen->ue);
               }
            }
         }
         break;

      case CND_UFLG_PENDING:
         if(list->le_Pending)
         {
            uen = (struct UserEntryNode *)le->le_Pending->lh_Head;

            for(; uen && uen->ue_Node.ln_Succ; uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ)
            {
               if(!stricmp(name, uen->ue.ue_From))
               {
                  return (&uen->ue);
               }
            }
         }
         break;

      case CND_UFLG_BANNED:
         if(list->le_Banned)
         {
            uen = (struct UserEntryNode *)le->le_Banned->lh_Head;

            for(; uen && uen->ue_Node.ln_Succ; uen = (struct UserEntryNode *)uen->ue_Node.ln_Succ)
            {
               char pattern [ 260 ] = "";

               ParsePatternNoCase(uen->ue.ue_From, pattern, sizeof(pattern));

               if(MatchPatternNoCase(pattern, name))
               {
                  return (&uen->ue);
               }
            }
         }
         break;
   }

   return NULL;
}
