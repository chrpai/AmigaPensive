/*
** CNet/4 POP3 Daemon
** by Kelly Cochran
** Last Change: Mon Jan 11 19:08:45 1999
*/

#include "pop3d.h"

static struct POP_Cmd POPCmds[] =
{
  STATE_IDLE, "quit" , cmd_quit , 0, 0, { STATE_QUIT, STATE_QUIT },
  STATE_IDLE, "user" , cmd_user , 1, 1, { STATE_IDLE, STATE_AUTH },
  STATE_IDLE, "auth" , cmd_auth , 1, 2, { STATE_IDLE, STATE_IDLE },
  STATE_IDLE, "apop" , cmd_apop , 2, 2, { STATE_IDLE, STATE_IDLE },
  STATE_AUTH, "quit" , cmd_quit , 0, 0, { STATE_QUIT, STATE_QUIT },
  STATE_AUTH, "pass" , cmd_pass , 1, 1, { STATE_IDLE, STATE_USER },
  STATE_AUTH, "rpop" , cmd_rpop , 1, 1, { STATE_IDLE, STATE_USER },
  STATE_AUTH, "capa" , cmd_capa , 0, 0, { STATE_AUTH, STATE_AUTH },
  STATE_USER, "quit" , cmd_quit , 0, 0, { STATE_QUIT, STATE_QUIT },
  STATE_USER, "stat" , cmd_stat , 0, 0, { STATE_USER, STATE_USER },
  STATE_USER, "top"  , cmd_top  , 2, 2, { STATE_USER, STATE_USER },
  STATE_USER, "list" , cmd_list , 0, 1, { STATE_USER, STATE_USER },
  STATE_USER, "retr" , cmd_retr , 1, 1, { STATE_USER, STATE_USER },
  STATE_USER, "rset" , cmd_rset , 0, 0, { STATE_USER, STATE_USER },
  STATE_USER, "noop" , NULL     , 0, 1, { STATE_USER, STATE_USER },
  STATE_USER, "uidl" , cmd_uidl , 0, 1, { STATE_USER, STATE_USER },
  STATE_USER, "euidl", cmd_euidl, 0, 1, { STATE_USER, STATE_USER },
  STATE_USER, "xtnd" , cmd_xtnd , 1, 8, { STATE_USER, STATE_USER },
  STATE_USER, "dele" , cmd_dele , 1, 1, { STATE_USER, STATE_USER },
  STATE_USER, "capa" , cmd_capa , 0, 0, { STATE_USER, STATE_USER },
  0         , NULL   , NULL     , 0, 0, { STATE_IDLE, STATE_IDLE },
};

static struct XTND_Cmd XTNDCmds[] =
{
   "xlst", cmd_xlst, 1, 2,
   "xmit", cmd_xmit, 0, 0,
   NULL  , NULL    , 0, 0,
};

static struct HeaderDef HdrDef[] =
{
   "From:"          , HDR_FROM,
   "Date:"          , HDR_DATE,
   "Subject:"       , HDR_SUBJECT,
   "To:"            , HDR_TO,
   "X-UIDL:"        , HDR_UIDL,
   "Content-Length:", HDR_CONTENT_LENGTH,
   "Received:"      , HDR_RECEIVED,
/* Additional headers to check against should go here */
   NULL              , NULL,
};

/* Constants */

const char *version = "\0$VER: CNet/4-POP3d "VER" "__AMIGADATE__;

const char *states[] = { NULL,
                         "IDLE",
                         "AUTHORIZATION",
                         "TRANSACTION",
                         "QUIT",
                         NULL
                       };

/* NOTE: state is kept local to main(commandloop), but may be determined
   by the following
   STATE_IDLE : curr_user_id == 0
   STATE_AUTH : curr_user_id != 0 && pop3_ml.mh4_List == 0
   STATE_USER : pop3_ml.mh4_List != 0
*/

const char *mh4file  = "Mail:Users/%s/FOLDERS/INBOX/_mhead4";
const char *mtxtfile = "Mail:Users/%s/FOLDERS/INBOX/_mtext4";
const char *hdrfile  = "Mail:Users/%s/HEADER/%ld";

struct ExecBase   *SysBase;
struct DosLibrary *DOSBase      = NULL;
struct Library    *CNetBase     = NULL;
struct Library    *CNet4Base    = NULL;
struct Library    *CNetMailBase = NULL;
struct Library    *SocketBase   = NULL;
struct MainPort   *myp          = NULL;
struct UserData    curr_user;
struct MailList    pop3_ml;
SHORT  curr_user_id         = 0;
int    SOCK;

int __saveds main(void)
{
   SysBase = (*((struct ExecBase **)4));

   if(!(DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 37)))
   {
      return (RETURN_FAIL);
   }

   if(SocketBase = OpenLibrary("bsdsocket.library", 4))
   {
      if(!init_inetd_server(&SOCK))
      {
         return (DERR_OBTAIN);
      }

      if((CNetBase = OpenLibrary("cnet.library", 4))
          && (CNet4Base = OpenLibrary("cnet4.library", 4))
          && (CNetMailBase = OpenLibrary("cnetmail.library", 4)))
      {
         struct timerequest *tr;

         Forbid();

         if(myp = (struct MainPort *)FindPort("cnetport"))
         {
            Permit();

            openlog("pop3d", LOG_PID, LOG_DAEMON);

            if(myp->s3 < 30000)
            {
               pop_msg(POP_ERR, "Invalid Serial");

               return (RETURN_FAIL);
            }

            if(tr = init_timer())
            {
               struct POP_Cmd     *curr_cmd;

               char buff [ POP3_BUFFER_SIZE ] = "";
               char *args[ MAX_OPTIONS ];

               int i,
                   x;

               ULONG mysigs;

               struct timeval     mytv;
               struct sockaddr_in remote_addr;

               LONG  addrlen = sizeof(remote_addr);

               int num_fds    = 0;
               int curr_state = STATE_IDLE;

               fd_set myset;

               if(getpeername(SOCK, (struct sockaddr *) &remote_addr, &addrlen) < 0)
               {
                  pop_log(LOG_ERR, "getpeername() failed.  Possible spoof?");
               }
               else
               {
                  pop_log(LOG_INFO, "Connection from %.128s", Inet_NtoA(remote_addr.sin_addr.s_addr));
               }

               FD_ZERO(&myset);
               FD_SET(SOCK, &myset);

               GetRFCDate(buff);
               tr->tr_time.tv_secs    = POP3_TIMEOUT;
               tr->tr_node.io_Command = TR_ADDREQUEST;
               SendIO((struct IORequest *)tr);

               pop_msg(POP_OK, "%s CNet/4 POP3 "VER" at %s", myp->gc.MyUUCPName, buff);

               buff[0] = '\0';
               do
               {
                  mysigs = (1L << tr->tr_node.io_Message.mn_ReplyPort->mp_SigBit) | SIGBREAKF_CTRL_C;

                  for(;;)
                  {
                     mytv.tv_sec  = POP3_TIMEOUT;
                     mytv.tv_usec = 0L;
                     if((num_fds = WaitSelect(SOCK + 1, &myset, NULL, NULL, &mytv, &mysigs)) < 0)
                     {
                        Signal(FindTask(NULL), SIGBREAKF_CTRL_C);
                        continue;
                     }
                     else
                     {
                        break;
                     }
                  }
                  if(mysigs & SIGBREAKF_CTRL_C)
                  {
                     if(curr_user.IDNumber)
                     {
                        write_mh4(pop3_ml.mh4_List, &curr_user, FALSE);
                     }
                     break;
                  }
                  if(mysigs & (1L << tr->tr_node.io_Message.mn_ReplyPort->mp_SigBit))
                  {
                     pop_msg(POP_ERR, "Timeout (%ld secs)", POP3_TIMEOUT);
                     if(curr_user.IDNumber)
                     {
                        write_mh4(pop3_ml.mh4_List, &curr_user, FALSE);
                     }
                     break;
                  }
                  if(num_fds)
                  {
                     if((x = pop_get_msg(SOCK, buff)) < 0)
                     {
                        Signal(FindTask(NULL), SIGBREAKF_CTRL_C);
                     }
                     x = strlen(buff);

                     if((buff[x-1] == '\n') || (buff[x-1] == '\r'))
                     {
                        i = cmd_parse(buff, args);
                        curr_cmd = command_parser(args[0], &curr_state, i);
                        buff[0] = '\0';
                        if(curr_cmd)
                        {
                           if(!curr_cmd->function)   /* special case for NOOP */
                           {
                              pop_msg(POP_OK, "");
                           }
                           else
                           {
                              curr_state = curr_cmd->result[curr_cmd->function(i, args)];
                           }
                           AbortIO((struct IORequest *)tr);
                           WaitIO((struct IORequest *)tr);
                           tr->tr_time.tv_secs    = POP3_TIMEOUT;
                           tr->tr_node.io_Command = TR_ADDREQUEST;
                           SendIO((struct IORequest *)tr);
                           SetSignal(0L, (1 << tr->tr_node.io_Message.mn_ReplyPort->mp_SigBit));
                        }
                     }
                  }
               } while(curr_state != STATE_QUIT);

               AbortIO((struct IORequest *)tr);
               WaitIO((struct IORequest *)tr);
               delete_timer(tr);
            }
            pop_log(LOG_INFO, "Connection closed");
            closelog();
         }
         else
         {
            Permit();
            /* no control */
         }
         CloseLibrary(SocketBase);
      }
      if(CNetMailBase)
         CloseLibrary(CNetMailBase);
      if(CNet4Base)
         CloseLibrary(CNet4Base);
      if(CNetBase)
         CloseLibrary(CNetBase);
   }

   CloseLibrary((struct Library *)DOSBase);

   return (RETURN_OK);
} /* main */

void __regargs __chkabort(void) { return; }  /* ^C denial */

int cmd_parse(char *buffer, char **args)
{
   char *tmp = buffer;

   int i = 0;

   for(;;i++)
   {
      if(isspace(*tmp))
         tmp++;
      if(*tmp == 0)
         break;
      args[i] = tmp;

      while(!isspace(*tmp) && *tmp)
         tmp++;

      if(tmp)
      {
         *tmp = 0;
         tmp++;
      }

      if(i == 0)
      {
         cmd_lower(args[0]);
         if((*args[0] == 'p') && !strcmp(args[0], "pass"))  /* don't wanna tweak a passwd */
         {
            i++;
            args[i] = tmp;
            if(strlen(tmp) > 0)
            {
               tmp = tmp + strlen(tmp) - 1;
               while(*tmp == 0xa || *tmp == 0xd)
               {
                  *tmp = 0;
                  tmp--;
               }
            }
            return (1);
         }
      }
   }
   return (i-1);
} /* cmd_parse */

void cmd_lower(char *buff)
{
   char *tmp = buff;

   for(; *tmp; tmp++, buff++)
   {
      *tmp = tolower(*buff);
   }
} /* cmd_lower */

void arpa_date(ULONG date, char *buff)
{
   struct IsDate id;

   CNetExplodeTime(date, &id);

   CNetPrintDate(&id, buff, NULL, "www, dd mmm yy HH:MM:SS");
} /* arpa_date */

int cmd_quit(int argc, char **argv)
{
   if(curr_user.IDNumber)
   {
      if(write_mh4(pop3_ml.mh4_List, &curr_user, TRUE))
      {
         pop_msg(POP_OK, "Goodbye");
      }
      else
      {
         pop_msg(POP_ERR, "Problem deleting messages");
         pop_log(LOG_ERR, "Problem deleting messages");
      }
   }
   else
   {
      pop_msg(POP_OK, "Goodbye");
   }

   return POP_SUCCESS;
} /* cmd_quit */

int cmd_pass(int argc, char **argv)
{
   struct UserData *udata;

   if(curr_user_id > 0)
   {
      if(udata = LockAccount(curr_user_id))
      {
         if(!stricmp(argv[1], udata->PassWord))
         {
            CopyMem(udata, (APTR)&curr_user, sizeof(struct UserData));
            UnLockAccount(curr_user_id, FALSE);
            if(AttemptSemaphore(&(GetMailSems()[curr_user_id-1])))
            {
               pop_msg(POP_OK, "Password accepted.  Mailbox open with %ld messages", LoadMHList(&curr_user));
               return POP_SUCCESS;
            }
            else
            {
               struct Task *owner = GetMailSems()[curr_user_id-1].ss_Owner;

               pop_log(LOG_ERR, "Mailbox already in use for account %.64s (%ld); Owning task: %s (0x%lx)", curr_user.Handle, curr_user_id, owner->tc_Node.ln_Name, owner);
               curr_user_id = 0;
               memset(&curr_user, 0, sizeof(struct UserData));
               pop_msg(POP_ERR, "[IN-USE] Mailbox in use by another process");
               return POP_FAILURE;
            }
         }
         else
         {
            pop_log(LOG_ERR, "Bad password for account %.64s (%ld)", udata->Handle, curr_user_id);
         }
         udata->LogonAttempts++;
         UnLockAccount(curr_user_id, TRUE);
      }
      else
      {
         pop_log(LOG_ERR, "UserData lock on account %ld failed", curr_user_id);
      }
   }

   curr_user_id = 0;
   memset(&curr_user, 0, sizeof(struct UserData));
   pop_msg(POP_ERR, "Invalid password");

   return POP_FAILURE;
} /* cmd_pass */

int cmd_user(int argc, char **argv)
{
   memset(&curr_user, 0, sizeof(struct UserData));
   curr_user_id = 0;

   if((argc > 0) && *argv[1])
   {
      curr_user_id = CNetAddressToAccount(argv[1]);
      pop_msg(POP_OK, "Password required for %.64s", argv[1]);
      return POP_SUCCESS;
   }
   pop_msg(POP_ERR, "Unknown authorization error");
   pop_log(LOG_ERR, "Unknown error in USER state");

   return POP_FAILURE;
} /* cmd_user */

int cmd_apop(int argc, char **argv)
{
   pop_msg(POP_ERR, "This command is not implemented");
   return POP_FAILURE;
} /* cmd_apop */

int cmd_rpop(int argc, char **argv)
{
   pop_msg(POP_ERR, "This command is not implemented");
   return POP_FAILURE;
} /* cmd_rpop */

int cmd_top(int argc, char **argv)
{
   struct MailNode *work;

   int i;

   i = atoi(argv[1]);
   if((i > 0) && (i < pop3_ml.num_Items + 1))
   {
      work = FindMNode(pop3_ml.mh4_List, i);
   }
   else
   {
      pop_msg(POP_ERR, "Message %ld does not exist", i);
      return POP_FAILURE;
   }

   if(work)
   {
      BPTR fp;

      char buff [ POP3_BUFFER_SIZE ] = "";

      int mail_size;
      int lines = atoi(argv[2]);

      if(lines < 0)
      {
         pop_msg(POP_ERR, "Invalid argument");
      }
      mail_size = print_headers(work, FALSE);

      sprintf(buff, mtxtfile, curr_user.UUCP);
      if(fp = Open(buff, MODE_OLDFILE))
      {
         Seek(fp, work->mn_MH4.text, OFFSET_BEGINNING);
         while(lines && (mail_size > 0))
         {
            mail_size -= parse_mail_output(fp, mail_size);
            lines--;
         }
         Close(fp);
         pop_msg(POP_NULL, ".");
      }
      else
      {
         pop_msg(POP_ERR, "Error accessing message");
      }
   }
   return POP_SUCCESS;
} /* cmd_top */

int cmd_list(int argc, char **argv)
{
   int i = 1;

   struct MailNode *work = (struct MailNode *)pop3_ml.mh4_List->lh_Head;

   if(argc > 0)
   {
      i = atoi(argv[1]);
      if((i > 0) && (i < pop3_ml.num_Items + 1))
      {
         work = FindMNode(pop3_ml.mh4_List, i);
      }
      else
      {
         pop_msg(POP_ERR, "Message %ld does not exist", i);
         return POP_FAILURE;
      }

      if(work)
      {
         if(work->Deleted)
         {
            pop_msg(POP_ERR, "Message %ld has been deleted", i);
            return POP_FAILURE;
         }
         else
         {
            pop_msg(POP_OK, "%ld %ld", i, work->mn_MH4.Length);
            return POP_SUCCESS;
         }
      }
   }

   pop_msg(POP_OK, "%ld messages (%ld octets)", pop3_ml.num_Items - pop3_ml.Deleted, pop3_ml.Bytes);

   while(work && work->mn_Node.ln_Succ)
   {
      if(!work->mn_MH4.Killed)
      {
         if(!work->Deleted)
         {
            pop_msg(POP_NULL, "%ld %ld", i++, work->mn_MH4.Length);
         }
      }
      work = (struct MailNode *)work->mn_Node.ln_Succ;
   }
   pop_msg(POP_NULL, ".");
   return POP_SUCCESS;
} /* cmd_list */

int cmd_stat(int argc, char **argv)
{
   pop_msg(POP_OK, "%ld %ld", pop3_ml.num_Items - pop3_ml.Deleted, pop3_ml.Bytes);

   return POP_SUCCESS;
} /* cmd_stat */

int cmd_rset(int argc, char **argv)
{
   struct MailNode *work = (struct MailNode *)pop3_ml.mh4_List->lh_Head;

   while(work && work->mn_Node.ln_Succ)
   {
      if(work->Deleted)
      {
         work->Deleted   = FALSE;
         pop3_ml.Bytes  += work->mn_MH4.Length;
         pop3_ml.Deleted--;
      }
      work = (struct MailNode *)work->mn_Node.ln_Succ;
   }
   pop_msg(POP_OK, "Mailbox has %ld messages (%ld octects)", pop3_ml.num_Items, pop3_ml.Bytes);

   return POP_SUCCESS;
} /* cmd_rset */

int cmd_uidl(int argc, char **argv)
{
   argc++;
   argv[2] = argv[1];
   argv[1] = "X-UIDL";
   argv[0] = NULL;         /* Used as a tag to tell xlst to not preserve headers */

   return(cmd_xlst(argc, argv));
} /* cmd_uidl */

#if 0    /* original cmd_uidl */
int cmd_uidl(int argc, char **argv)
{
   int i = 1;

   struct MailNode *work = (struct MailNode *)pop3_ml.mh4_List->lh_Head;

   if(argc > 0)
   {
      i = atoi(argv[1]);

      if((i > 0) && (i < pop3_ml.num_Items + 1))
      {
         work = FindMNode(pop3_ml.mh4_List, i);
      }
      else
      {
         pop_msg(POP_ERR, "Message %ld does not exist", i);
         return POP_FAILURE;
      }

      if(work)
      {
         if(work->Deleted)
         {
            pop_msg(POP_ERR, "Message %ld has been deleted", i);
            return POP_FAILURE;
         }
         else
         {
            pop_msg(POP_OK  , "");
            pop_msg(POP_NULL, "%ld %.128s", i, find_header(work, "X-UIDL", FALSE));
            pop_msg(POP_NULL, ".");
            return POP_SUCCESS;
         }
      }
   }

   pop_msg(POP_OK, "");
   while(work && work->mn_Node.ln_Succ)
   {
      if(!work->mn_MH4.Killed)
      {
         if(!work->Deleted)
         {
            pop_msg(POP_NULL, "%ld %.128s", i, find_header(work, "X-UIDL", FALSE));
         }
        
         i++;
      }
      work = (struct MailNode *)work->mn_Node.ln_Succ;
   }
   pop_msg(POP_NULL, ".");
   return POP_SUCCESS;
} /* cmd_uidl */
#endif

int cmd_euidl(int argc, char **argv)
{
   char temp [ 1024 ] = "";

   int i = 1;

   struct MailNode *work = (struct MailNode *)pop3_ml.mh4_List->lh_Head;

   if(argc > 0)
   {
      i = atoi(argv[1]);

      if((i > 0) && (i < pop3_ml.num_Items + 1))
      {
         work = FindMNode(pop3_ml.mh4_List, i);
      }
      else
      {
         pop_msg(POP_ERR, "Message %ld does not exist", i);
         return POP_FAILURE;
      }

      if(work)
      {
         if(work->Deleted)
         {
            pop_msg(POP_ERR, "Message %ld has been deleted", i);
            return POP_FAILURE;
         }
         else
         {
            strcpy(temp, find_header(work, "X-UIDL", FALSE));
            pop_msg(POP_OK  , "");
            pop_msg(POP_NULL, "%ld %ld %.128s %.128s", i, work->mn_MH4.Length, temp, find_header(work, "From", FALSE));
            pop_msg(POP_NULL, ".");
            return POP_SUCCESS;
         }
      }
   }

   pop_msg(POP_OK, "");

   while(work && work->mn_Node.ln_Succ)
   {
      if(!work->mn_MH4.Killed)
      {
         if(!work->Deleted)
         {
            strcpy(temp, find_header(work, "X-UIDL", FALSE));
            pop_msg(POP_NULL, "%ld %ld %.128s %.128s", i, work->mn_MH4.Length, temp, find_header(work, "From", FALSE));
         }
        
         i++;
      }
      work = (struct MailNode *)work->mn_Node.ln_Succ;
   }
   pop_msg(POP_NULL, ".");
   return POP_SUCCESS;
} /* cmd_euidl */

int cmd_retr(int argc, char **argv)
{
   if(argc == 1)
   {
      struct MailNode *work;

      int mail_item = atoi(argv[1]);
      if((mail_item > 0) && (mail_item < pop3_ml.num_Items + 1))
      {
         work = FindMNode(pop3_ml.mh4_List, mail_item);
      }
      else
      {
         pop_msg(POP_ERR, "Message %ld does not exist", mail_item);
         return POP_FAILURE;
      }
      if(work)
      {
         BPTR fp;

         char buff [ 256 ] = "";

         if(work->Deleted)
         {
            pop_msg(POP_ERR, "Message %ld marked as deleted", mail_item);
            return POP_FAILURE;
         }
         sprintf(buff, mtxtfile, curr_user.UUCP);
         if(fp = Open(buff, MODE_OLDFILE))
         {
            long mail_size = print_headers(work, TRUE);

            Seek(fp, work->mn_MH4.text, OFFSET_BEGINNING);

            while(mail_size > 0)
            {
               mail_size -= parse_mail_output(fp, mail_size);
            }
            pop_msg(POP_NULL, ".");
            work->mn_MH4.ReadDate = CNetTime();
            work->mn_MH4.flags   |= MAIL_OPENED;
            if(work->mn_MH4.flags & MAIL_RECEIPT)
            {
               file_retrcpt(work, &curr_user);
               work->mn_MH4.flags &= ~MAIL_RECEIPT;
            }
            Close(fp);
         }
         else
         {
            pop_msg(POP_ERR, "Error accessing mailbox");
         }
         return POP_SUCCESS;
      }
   }
   return POP_FAILURE;
} /* cmd_retr */

int cmd_dele(int argc, char **argv)
{
   if(argc == 1)
   {
      struct MailNode *work;

      int mail_item = atoi(argv[1]);

      if((mail_item > 0) && (mail_item < pop3_ml.num_Items + 1))
      {
         work = FindMNode(pop3_ml.mh4_List, mail_item);
      }
      else
      {
         pop_msg(POP_ERR, "Message %ld does not exist", mail_item);
         return POP_FAILURE;
      }
      if(work)
      {
         if(work->Deleted)
         {
            pop_msg(POP_ERR, "Message %ld already marked for deletion", mail_item);
            return POP_FAILURE;
         }
         else
         {
            pop_msg(POP_OK, "Message %ld marked for deletion", mail_item);
            work->Deleted  = TRUE;
            pop3_ml.Bytes -= work->mn_MH4.Length;
            pop3_ml.Deleted++;
         }
      }
   }
   return POP_SUCCESS;
} /* cmd_dele */

int cmd_capa(int argc, char **argv)
{
   pop_msg(POP_OK, "Capability list follows");
   pop_msg(POP_NULL, "TOP");     /* Yay, we have top        */
   pop_msg(POP_NULL, "USER");    /* I hope we have user..   */
   pop_msg(POP_NULL, "UIDL");    /* UIDL is always handy    */
   pop_msg(POP_NULL, "EXPIRE NEVER");  /* As CNet doesn't have an expiration for email currently, this is safe */
   pop_msg(POP_NULL, "RESP-CODES"); /* Simple addition, may as well advertise it */
   pop_msg(POP_NULL, ".");

   return POP_SUCCESS;
} /* cmd_capa */

int cmd_xtnd(int argc, char **argv)
{
   struct XTND_Cmd *command;

   cmd_lower(argv[1]);

   if(command = sub_command_parser(argc - 1, &argv[1]))  /* shift arg count */
   {
      if(command->function)
      {
         return (command->function(argc - 1, &argv[1]));
      }
      pop_msg(POP_OK, "");
   }

   return POP_FAILURE;
} /* cmd_xtnd */

int cmd_xlst(int argc, char **argv)
{
   int i = 1;

   struct MailNode *work = (struct MailNode *)pop3_ml.mh4_List->lh_Head;

   if(argc > 1)
   {
      i = atoi(argv[2]);

      if((i > 0) && (i < pop3_ml.num_Items + 1))
      {
         work = FindMNode(pop3_ml.mh4_List, i);
      }
      else
      {
         pop_msg(POP_ERR, "Message %ld does not exist", i);
         return POP_FAILURE;
      }

      if(work)
      {
         if(work->Deleted)
         {
            pop_msg(POP_ERR, "Message %ld has been deleted", i);
            return POP_FAILURE;
         }
         else
         {
            char *tmp = find_header(work, argv[1], argv[0] ? TRUE:FALSE);

            pop_msg(POP_OK  , "Headers following");

            if(*tmp) /* if header found, display */
               pop_msg(POP_NULL, "%ld %.256s", i, tmp);

            pop_msg(POP_NULL, ".");
            return POP_SUCCESS;
         }
      }
   }

   pop_msg(POP_OK, "Headers following");

   while(work && work->mn_Node.ln_Succ)
   {
      if(!work->mn_MH4.Killed)
      {
         if(!work->Deleted)
         {
            char *tmp = find_header(work, argv[1], argv[0] ? TRUE:FALSE);

            if(*tmp) /* if header found, display */
               pop_msg(POP_NULL, "%ld %.256s", i, tmp);
         }
        
         i++;
      }
      work = (struct MailNode *)work->mn_Node.ln_Succ;
   }
   pop_msg(POP_NULL, ".");
   return POP_SUCCESS;
} /* cmd_xlst */

int cmd_xmit(int argc, char **argv)
{
   pop_msg(POP_ERR, "This function not implemented");
   return POP_FAILURE;
} /* cmd_xmit */

struct XTND_Cmd *sub_command_parser(int argc, char **argv)
{
   struct XTND_Cmd *work = XTNDCmds;

   while(work->command != NULL)
   {
      if(!strcmp(work->command, argv[0]))
      {
         if(work->min_ops > argc)
         {
            pop_msg(POP_ERR, "Too few arguments for XTND %.32s command", argv[0]);
            return NULL;
         }
         if(work->max_ops < argc)
         {
            pop_msg(POP_ERR, "Too many arguments for XTND %.32s command", argv[0]);
            return NULL;
         }
         return (work);
      }
      work++;
   }
   pop_msg(POP_ERR, "Invalid XTND command");

   return NULL;
} /* sub_command_parser */

struct POP_Cmd *command_parser(char *cmd, int *state, int args)
{
   struct POP_Cmd *work = POPCmds;

   while(work->command != NULL)
   {
      if(!strcmp(work->command, cmd) && (work->req_state == *state))
      {
         if(work->min_ops > args)
         {
            pop_msg(POP_ERR, "Too few arguments for command");
            if(!strcmp(work->command, "pass"))
            {
               *state = STATE_IDLE;
            }
            return NULL;
         }
         if(work->max_ops < args)
         {
            pop_msg(POP_ERR, "Too many arguments for command");
            return NULL;
         }
         return (work);
      }
      work++;
   }
   if(cmd && *cmd)
   {
      pop_msg(POP_ERR, "Invalid command for %s state", states[*state]);
   }
   else
   {
      pop_msg(POP_ERR, "Null command");
   }
   if(*state == STATE_AUTH)
   {
      *state = STATE_IDLE;
   }
   return NULL;
} /* command_parser */

struct List *init_list(void)
{
   struct List *temp;

   if(temp = (struct List *)AllocVec(sizeof(struct List), MEMF_CLEAR))
   {
      NewList(temp);
      return (temp);
   }

   return NULL;
} /* init_list */

void AddMHToList(struct List *list, struct MailHeader4 *mh4)
{
   struct MailNode *temp;

   if(temp = (struct MailNode *)AllocVec(sizeof(struct MailNode), MEMF_CLEAR))
   {
      CopyMem(mh4, (APTR)&temp->mn_MH4, sizeof(struct MailHeader4));
      temp->mn_Node.ln_Name = temp->mn_MH4.From;
      AddTail(list, (struct Node *)temp);
   }
} /* AddMHToList */

int LoadMHList(struct UserData *udata)
{
   BPTR   fp;

   struct MailHeader4 mh4;

   char buff [ 256 ] = "";

   memset(&mh4, 0, sizeof(struct MailHeader4));

   if(pop3_ml.mh4_List = init_list())
   {
      sprintf(buff, mh4file, curr_user.UUCP);

      if(fp = Open(buff, MODE_OLDFILE))
      {
         while(Read(fp, &mh4, sizeof(struct MailHeader4)) == sizeof(struct MailHeader4))
         {
            AddMHToList(pop3_ml.mh4_List, &mh4);
            if(!mh4.Killed)
            {
               pop3_ml.num_Items++;
               pop3_ml.Bytes += mh4.Length;
            }
         }
         Close(fp);
      }
   }

   return (pop3_ml.num_Items);
} /* LoadMHList */

void free_list(struct List **list)
{
   if(*list)
   {
      struct Node *work = (*list)->lh_Head,
                  *temp;

      while(work && work->ln_Succ)
      {
         temp = work->ln_Succ;
         FreeVec(work);
         work = temp;
      }
      FreeVec(*list);
      *list = NULL;
   }
} /* free_list */

struct MailNode *FindMNode(struct List *list, int node)
{
   if(list)
   {
      struct MailNode *work = (struct MailNode *)list->lh_Head;

      while((node > 0) && work && work->mn_Node.ln_Succ)
      {
         if(!work->mn_MH4.Killed)
         {
            node--;
         }

         if(node > 0)
         {
            work = (struct MailNode *)work->mn_Node.ln_Succ;
         }
      }
      return (work);
   }
   return NULL;
} /* FindMNode */

int write_mh4(struct List *list, struct UserData *udata, int quit)
{
   BPTR fp;

   struct MailNode *work = (struct MailNode *)list->lh_Head;

   char buff [ 128 ] = "";

   if(!udata->IDNumber)
   {
      return POP_SUCCESS;
   }

   sprintf(buff, mh4file, udata->UUCP);

   if(pop3_ml.Bytes == 0)
   {
      DeleteFile(buff);
      sprintf(buff, mtxtfile, udata->UUCP);
      DeleteFile(buff);
   }
   else
   {
      if(fp = Open(buff, MODE_NEWFILE))
      {
         while(work && work->mn_Node.ln_Succ)
         {
            if(work->Deleted && (quit == TRUE))
            {
               work->mn_MH4.Killed = TRUE;

               if(work->mn_MH4.RFCHead)
               {
                  sprintf(buff, hdrfile, udata->UUCP, work->mn_MH4.RFCHead);
                  DeleteFile(buff);
               }
            }
            Write(fp, (APTR)&work->mn_MH4, sizeof(struct MailHeader4));
            work = (struct MailNode *)work->mn_Node.ln_Succ;
         }
         Close(fp);
      }
   }

   if(quit == TRUE)
   {
      ObtainSemaphore(&myp->SEM[18]);
      myp->SAM[4][1] -= pop3_ml.Deleted;
      if(curr_user_id == 1)
      {
         myp->SAM[4][2] -= pop3_ml.Deleted;
      }
      ReleaseSemaphore(&myp->SEM[18]);
   }

   ReleaseSemaphore(&(GetMailSems()[curr_user_id-1]));
   memset(udata, 0, sizeof(struct UserData));
   curr_user_id = 0;

   if(list)
   {
      free_list(&list);
   }

   return POP_SUCCESS;
} /* write_mh4 */

void file_retrcpt(struct MailNode *mn, struct UserData *udata)
{
   BPTR mh4fp,
        mtfp;

   ULONG Time = CNetTime();

   struct MailHeader4 mh4;
   struct MailToList  mtl;
   struct UserData   *rr_udata;

   char buf1 [ 256 ] = "",
        buf2 [ 256 ] = "",
        buf3 [ 256 ] = "";

   sprintf(buf1, MTT_PATTERN, Time);
   sprintf(buf2, CMH_PATTERN, Time);
   sprintf(buf3, MTL_PATTERN, Time);

   memset(&mh4, 0, sizeof(struct MailHeader4));
   memset(&mtl, 0, sizeof(struct MailToList));

   if(mh4fp = Open(buf2, MODE_NEWFILE))
   {
      if(mtfp = Open(buf1, MODE_NEWFILE))
      {
         if(rr_udata = LockAccount(mn->mn_MH4.ByAccount))
         {
            if(*mn->mn_MH4.NetAddress)
            {
               strcpy(mtl.Name, mn->mn_MH4.NetAddress);
               mtl.MailType = MAILTO_FIDO; /* FIDO Netmail */
            }
            else
            {
               strcpy(mtl.UUCP, rr_udata->UUCP);
               mtl.MailType = MAILTO_HANDLE; /* Local/Handle */
            }
            FPuts(mtfp, buf2);
            FPuts(mtfp, "\n");
            FWrite(mtfp, (APTR)&mtl, 1, sizeof(struct MailToList));

            sprintf(mh4.Subject, myp->FirstTT->bm[765], *mn->mn_MH4.Subject ? mn->mn_MH4.Subject : "NO SUBJECT");  /* Message Header */
            strcpy(mh4.From, udata->Handle);
            mh4.Date      = Time;
            mh4.Number    = Time;
            mh4.ByID      = udata->IDNumber;
            mh4.ByAccount = IDToAccount(udata->IDNumber);
            if(!*mn->mn_MH4.NetAddress)
            {
               mh4.ToID      = rr_udata->IDNumber;
               mh4.ToAccount = IDToAccount(rr_udata->IDNumber);
            }
            mh4.Length    = strlen(myp->FirstTT->bm[764]) + strlen(buf2) - 1; 
            Write(mh4fp, (APTR)&mh4, sizeof(struct MailHeader4));

            CNetPrintTime(mh4.Date, buf2, NULL, udata);
            FPrintf(mh4fp, myp->FirstTT->bm[764], buf2);  /* Message body   */

            UnLockAccount(IDToAccount(rr_udata->IDNumber), FALSE);
         }
         Close(mtfp);
      }
      Close(mh4fp);
   }
   Rename(buf1, buf3);
} /* file_retrcpt */

int print_headers(struct MailNode *mn, BOOL msg)
{
   char neta [ 256 ] = "";
   char date [  64 ] = "";

   int mail_size = 0;
   int rfchead   = 0;

   if(mn)
   {
      mail_size = mn->mn_MH4.Length;

      if(mn->mn_MH4.RFCHead)
      {
         sprintf(neta, hdrfile, curr_user.UUCP, mn->mn_MH4.RFCHead);
         rfchead = FileSize(neta);
      }

      if(rfchead)
      {
         pop_msg(POP_OK, "%ld octets", rfchead + (msg ? mail_size:0));
         PrintHeaderToSock(neta, rfchead);
      }
      else
      {
         pop_msg(POP_OK, "%ld octets", mail_size);
         if(!*mn->mn_MH4.NetAddress)
            sprintf(neta, "%s@%s", CNetIDToUUCP(mn->mn_MH4.ByID), myp->gc.MyUUCPName);
         else
            strcpy(neta, mn->mn_MH4.NetAddress);

         arpa_date(mn->mn_MH4.Date, date);

         pop_msg(POP_NULL, "%s from %s by localhost; %s", HdrDef[HDR_RECEIVED].Name, myp->gc.MyUUCPName, date);
         pop_msg(POP_NULL, "%s \"%s\" <%s>", HdrDef[HDR_FROM].Name, mn->mn_MH4.From, neta);
         pop_msg(POP_NULL, "%s %s@%s", HdrDef[HDR_TO].Name, curr_user.UUCP, myp->gc.MyUUCPName);
         pop_msg(POP_NULL, "%s %s", HdrDef[HDR_SUBJECT].Name, *mn->mn_MH4.Subject ? mn->mn_MH4.Subject:"NO SUBJECT");
         pop_msg(POP_NULL, "%s %s", HdrDef[HDR_DATE].Name, date);
         pop_msg(POP_NULL, "%s %ld", HdrDef[HDR_CONTENT_LENGTH].Name, mn->mn_MH4.Length);
         pop_msg(POP_NULL, "%s %s", HdrDef[HDR_UIDL].Name, find_header(mn, "X-UIDL", FALSE));
         pop_msg(POP_NULL, "");
      }
   }

   return (mail_size);
} /* print_headers */

struct timerequest *init_timer(void)
{
   LONG error;
   struct timerequest *tr;
   struct MsgPort     *timerport;

   if(timerport = CreatePort(0, 0))
   {
      if(tr = (struct timerequest *)CreateExtIO(timerport, sizeof(struct timerequest)))
      {
         error = OpenDevice(TIMERNAME, UNIT_VBLANK, (struct IORequest *)tr, NULL);

         if(!error)
         {
            return (tr);
         }
         delete_timer(tr);
      }
      DeletePort(timerport);
   }

   return (NULL);
} /* init_timer */

void delete_timer(struct timerequest *tr)
{
   if(tr != NULL)
   {
      struct MsgPort *tp = tr->tr_node.io_Message.mn_ReplyPort;

      if(tp)
      {
         DeletePort(tp);
      }
      CloseDevice((struct IORequest *)tr);
      DeleteExtIO((struct IORequest *)tr);
   }

   return;
} /* delete_timer */

int pop_msg(int status, char *fmt, ...)
{
   int outlen,
       sent;

   char oput [ POP3_BUFFER_SIZE + 32 ] = "";
   char *tmp = oput;

   /* These correspond to the POP_#? enumerated values */
   char *resp[] =
   {
      "",
      "+OK ",
      "-ERR "
   };

   va_list ap;

   strcpy(oput, resp[status]);

   tmp += strlen(oput);

   va_start(ap, fmt);
   vsprintf(tmp, fmt, ap);
   va_end(ap);

   strcat(oput, "\r\n");

   outlen = strlen(oput);

   if((sent = send(SOCK, oput, outlen, 0)) != outlen)
   {
      return (0);
   }

   return (sent);
} /* pop_msg */

int pop_get_msg(int sock, char *buffer)
{
   int  nrec,
        ncurr,
        recstart,
        retval = 0;

   char tmpbuf[ POP3_BUFFER_SIZE + 1 ] = "";

   if ((nrec = recv(sock, tmpbuf, POP3_BUFFER_SIZE - 2, MSG_PEEK)) < 1)
      return(-1);

   recstart = 0;
   while ((tmpbuf[recstart] != '\n') && (recstart < (POP3_BUFFER_SIZE-2)) && (recstart < nrec))
      recstart++;

   recv(sock, tmpbuf, recstart + 1, 0);
   if (tmpbuf[recstart] == '\n')
      retval = 1;
   tmpbuf[recstart] = 0;

   ncurr = strlen(buffer);

   nrec = 0;
   while ((nrec < recstart) && ((ncurr + nrec) < (POP3_BUFFER_SIZE-2)))
      buffer[ncurr+nrec] = tmpbuf[nrec++];
   buffer[ncurr+nrec] = 0;
   return(retval);
} /* pop_get_msg */

long parse_mail_output(BPTR fp, long mail_size)
{
   char tmp  [ POP3_BUFFER_SIZE + 8 ] = ""; /* arbitrary play space */
   char *tmp2;

   long read_size;

   memset(tmp, 0, sizeof(tmp));
   read_size = Read(fp, tmp, mail_size > POP3_BUFFER_SIZE ? POP3_BUFFER_SIZE:mail_size);
   if(tmp2 = strchr(tmp, '\n'))
   {
      Seek(fp, tmp2 + 1 - (tmp + read_size), OFFSET_CURRENT);
      read_size = tmp2 - tmp + 1;
      *tmp2 = '\0';
   }
   else
   {
      tmp2 = tmp + read_size;
      while((tmp2 > tmp) && !isspace(*tmp2))
      {
         tmp2--;
      }
      if(isspace(*tmp2))
      {
         Seek(fp, tmp2 + 1 - (tmp + read_size), OFFSET_CURRENT);
         read_size = tmp2 - tmp + 1;
         *tmp2 = '\0';
      }
   }

   pop_msg(POP_NULL, "%s%s", (*tmp == '.') ? ".":"", tmp);

   return (read_size);
} /* parse_mail_output */

BOOL init_inetd_server(int *SOCK)
{
   struct Process       *me = (struct Process *)FindTask(NULL);
   struct DaemonMessage *dm = (struct DaemonMessage *)me->pr_ExitData;

   if(dm == NULL)
   {
      return FALSE;
   }

   *SOCK = ObtainSocket(dm->dm_Id, dm->dm_Family, dm->dm_Type, 0);

   if(*SOCK < 0)
   {
      return FALSE;
   }
  
   return TRUE;
} /* init_inetd_server */

void pop_log(int pri, char *fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   vsyslog(pri, fmt, (LONG *)ap);
   va_end(ap);
} /* pop_log */

void openlog(const char *ident, int logstat, int logfac)
{
  SocketBaseTags(SBTM_SETVAL(SBTC_LOGTAGPTR), ident,
		 SBTM_SETVAL(SBTC_LOGSTAT), logstat,
		 SBTM_SETVAL(SBTC_LOGFACILITY), logfac,
		 TAG_END);
} /* openlog */

void closelog(void)
{
  SocketBaseTags(SBTM_SETVAL(SBTC_LOGTAGPTR), NULL,
		 TAG_END);
} /* closelog */

ULONG PrintHeaderToSock(char *file, ULONG file_size)
{
   BPTR fp;

   ULONG header_size = file_size;

   if(fp = Open(file, MODE_OLDFILE))
   {
      while(header_size > 0)
      {
         header_size -= parse_header_output(fp, header_size);
      }

      Close(fp);
   }
   return (file_size);
} /* PrintfFileToSock */

long parse_header_output(BPTR fp, long mail_size)
{
   char tmp  [ HEADER_BUFFER + 8 ] = ""; /* arbitrary play space */
   char *tmp2;

   long read_size,
        total_read = 0L;

   BOOL line_done = FALSE,
        cont      = FALSE;

   memset(tmp, 0, sizeof(tmp));

   while(!line_done)
   {
      read_size = Read(fp, tmp, mail_size > HEADER_BUFFER ? HEADER_BUFFER:mail_size);
      if(tmp2 = strchr(tmp, '\n'))
      {
         Seek(fp, tmp2 + 1 - (tmp + read_size), OFFSET_CURRENT);
         read_size = tmp2 - tmp + 1;
         *tmp2 = '\0';
         line_done = TRUE;
      }
      else
      {
         tmp2 = tmp + read_size;
         while((tmp2 > tmp) && !isspace(*tmp2))
         {
            tmp2--;
         }
         if(isspace(*tmp2))
         {
            Seek(fp, tmp2 + 1 - (tmp + read_size), OFFSET_CURRENT);
            read_size = tmp2 - tmp + 1;
            *tmp2 = '\0';
         }
      }

      pop_msg(POP_NULL, "%s%s", cont ? "\t":"", tmp);

      cont        = !line_done;
      total_read += read_size;
   }

   return (total_read);
} /* parse_header_output */

char *find_header(struct MailNode *mn, char *header, BOOL preserve)
{
   static char reply [ 1024 ];
   char date [  64 ] = "";
   char neta [ 256 ] = "";

   reply[0] = '\0';

   if(mn)
   {
      if(mn->mn_MH4.RFCHead)  /* do we have a header? If so, check it */
      {
         char hdr [ 1024 ] = "";

         sprintf(hdr, hdrfile, curr_user.UUCP, mn->mn_MH4.RFCHead);

         if(FileExists(hdr))
         {
            BPTR fp;
            char *tmp = hdr;

            if(fp = Open(hdr, MODE_OLDFILE))
            {
               while(FGets(fp, hdr, sizeof(hdr) - 1))
               {
                  if(!strincmp(hdr, header, strlen(header)))
                  {
                     if(!preserve && (tmp = strchr(hdr, ':')))
                     {
                        tmp += 2;
                     }
                     strcpy(reply, tmp);
                     reply[strlen(reply) - 1] = '\0'; /* whack the trailing \n */
                     break;
                  }
               }
               Close(fp);

               if(*reply)  /* find something? */
                  return (reply);
            }
         }
      }

      /* must not have.. guess we fake it */

      if(!*mn->mn_MH4.NetAddress)
         sprintf(neta, "%s@%s", CNetIDToUUCP(mn->mn_MH4.ByID), myp->gc.MyUUCPName);
      else
         strcpy(neta, mn->mn_MH4.NetAddress);

      arpa_date(mn->mn_MH4.Date, date);

      {
         struct HeaderDef *work = &HdrDef[0];

         BOOL found = FALSE;

         while(work && work->Name)
         {
            if(!strincmp(header, work->Name, strlen(header)))
            {
               found = TRUE;
               break;
            }
            work++;
         }

         if(found)
         {
            if(preserve)
            {
               sprintf(reply, "%s ", HdrDef[work->value].Name);
            }

            switch(work->value)
            {
               case HDR_UIDL:
                  sprintf(reply, "%s%ld/%ld", reply, mn->mn_MH4.Number, mn->mn_MH4.ByID);
                  break;

               case HDR_FROM:
                  sprintf(reply, "%s\"%s\" <%s>", reply, mn->mn_MH4.From, neta);
                  break;

               case HDR_TO:
                  sprintf(reply, "%s%s@%s", reply, curr_user.UUCP, myp->gc.MyUUCPName);
                  break;

               case HDR_SUBJECT:
                  sprintf(reply, "%s%s", reply, *mn->mn_MH4.Subject ? mn->mn_MH4.Subject:"NO SUBJECT");
                  break;

               case HDR_DATE:
                  sprintf(reply, "%s%s", reply, date);
                  break;

               case HDR_CONTENT_LENGTH:
                  sprintf(reply, "%s%ld", reply, mn->mn_MH4.Length);
                  break;

               case HDR_RECEIVED:
                  sprintf(reply, "%sfrom %s by localhost; %s", reply, myp->gc.MyUUCPName, date);
                  break;
            }
         }
      }

      return (reply);
   }
} /* find_header */

int strincmp(char *s1, char *s2, int n)
{
   int i;
   char c1,
        c2;

   for(i = 0; i < n; i++, s1++, s2++)
   {
      c1 = tolower(*s1);
      c2 = tolower(*s2);
      if(c1 < c2) return (-1);
      if(c1 > c2) return (1);
      if(!c1)     return 0;
   }
   return 0;
} /* strincmp */
