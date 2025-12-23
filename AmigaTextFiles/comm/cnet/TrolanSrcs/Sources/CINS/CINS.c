/*
** CINS Main Executable
** by Kelly Cochran
** Last Update: Fri Mar 26 13:11:14 1999
*/

#include "CINS.h"
#include "CINS_protos.h"

#define UNREG_DELAY 15

#define COMMANDS    17

#define MENU_QUIT   -1
#define MENU_PREV   -2
#define MENU_NEXT   -3
#define MENU_MAN    -4
#define MENU_NONE   -5
#define MENU_NOTE   -6
#define MENU_ADD    -7
#define MENU_DELE   -8

struct MainPort *myp;      
struct PortData *z;
struct Library  *CNetBase  = NULL;
struct Library  *CNetCBase = NULL;
struct Library  *CINSBase  = NULL;

struct CNetCContext *context = NULL;

struct Process *myTask = NULL;

struct CINSCfg  Config;
struct CINSUCfg UCfg;
char   **bm;
char   **cm;

const char *version = VERTAG;

APTR orig_window   = NULL; /* cinsproc->pr_WindowPtr */

struct List *tnel  = NULL;
struct List *ftpel = NULL;

char  *newptr = NULL;

char  cmdline [ COMMANDS ] [ 256 ];

void main( int argc, char **argv )
{
   struct Library *SocketBase;

   if((argc < 2) || !(CNetCBase = OpenLibrary("cnetc.library", 4L)))
   {
      Printf("Unable to setup CNetC library interface!\n");
      exit(0);
   }

   if(!(context = CNC_InitContext(argv[1])))
   {
      CloseLibrary(CNetCBase);
      Printf("Unable to setup CNetC messaging!\n");
      exit(0);
   }

   z   = context->z;
   myp = context->myp;
   bm  = z->bm;

   memset(&Config, 0, sizeof(struct CINSCfg));
   memset(&UCfg  , 0, sizeof(struct CINSUCfg));

   if(!(CNetBase = OpenLibrary("cnet.library", 3L )))
   {
      GetOut();
   }

   if(!(CINSBase = OpenLibrary(CINSLIBNAME, 4L)))
   {
      GetOut();
   }

   if(!(SocketBase = OpenLibrary("bsdsocket.library", 3)))
   {
      CNC_PutText("\nCould not open bsdsocket.library!\nTCP stack not loaded!  Exiting...");
      GetOut();
   }
   else
   {
      CloseLibrary(SocketBase);
   }

   if(myTask = (struct Process *)FindTask(NULL))   /* My proc! */
   {
      orig_window = myTask->pr_WindowPtr;    /* save old ptr       */
      myTask->pr_WindowPtr = (void *)-1;     /* prevent requestors */
   }

   CINS_Main();
}

void CINS_Main(void)
{
   check_user_prefs(TRUE);

   setup();

   CNC_SetDoing(cm[2]);

   UCfg.TimeEnter = z->TimeOnLine;

   CNC_ReadGraphics("CINS:Text/sys.News", FALSE);

   if(UCfg.Shell)
   {
      do_shell();
      GetOut();
   }

   commands();

   command_loop();
}

void commands(void)
{
   CNC_ReadGraphics("CINS:Menus/sys.Main", FALSE);
}

void command_loop(void)
{
   short ftpin;

   char  ii;

   while(TRUE)
   {
      CNC_SetDoing(cm[3]);

      CNC_PutText(cm[4]);

      ii = CNC_OneKey();

      checkcarrier();

      switch(ii)
      {
         case '!':   /* Config   */
            if(z->SysMaint)
            {
               CNC_PutText(cm[10]);
               CNC_PutText(cmdline[15]);
            }
            break;

         case 'A':   /* Archie   */
            CNC_SetDoing(cm[8]);
            CNC_PutText(cm[8]);
            CNC_DoReturn();
            archie();
            CINS_FreeList(&Config.archie);
            break;

         case 'B':   /* Account Balance   */
            if(UCfg.uae.Rate)
            {
               CNC_PutText(cm[9]);
               view_balance();
            }
            break;

         case 'C':   /* User Config */
            CNC_SetDoing(cm[10]);
            CNC_PutText(cm[11]);
            CNC_PutText(cmdline[10]);
            check_user_prefs(FALSE);
            break;

         case 'D':   /* Directory Maint   */
            CNC_SetDoing(cm[12]);
            CNC_PutText(cm[12]);
            CNC_DoReturn();
            CNC_PutText(cmdline[11]);
            break;

         case 'F':   /* FTP   */
            CNC_SetDoing(cm[13]);
            CNC_PutText(cm[14]);
            CNC_DoReturn();

            if(UCfg.uae.FTPFree && (check_dir_size() > UCfg.uae.FTPFree))
            {
               CNC_PutText(cm[122]);
               break;
            }

            ftpin = check_ftp_open();

            if(ftpin)
            {
               if(!FileExists("CINS:FTP/Menu"))
               {
                  do_ftp_exe("");
                  break;
               }

               do_ftp_menu();
               CINS_FreeList(&Config.ftp);
               CINS_FreeList(&ftpel);
               if((Config.FTPin > 0) && (ftpin))
                  check_ftp_close();
            }

            if(UCfg.uae.FTPFree && (check_dir_size() > UCfg.uae.FTPFree))
            {
               CNC_PutText(cm[122]);
            }
            break;
 
         case 'G':   /* Gopher   */
            CNC_SetDoing(cm[15]);
            CNC_PutText(cm[15]);
            CNC_DoReturn();
            do_gopher();
            break;

         case 'H':   /* Help  */
            CNC_PutText(cm[16]);
            view_help();
            break;

         case 'I':   /* IRC   */
            CNC_SetDoing(cm[17]);
            CNC_PutText(cm[17]);
            CNC_DoReturn();
            do_irc();
            break;

         case 'L':   /* Lynx  */
            CNC_SetDoing(cm[18]);
            CNC_PutText(cm[18]);
            CNC_DoReturn();
            do_lynx(Config.URL);
            break;

         case 'N':   /* Netstat  */
            CNC_SetDoing(cm[19]);
            CNC_PutText(cm[19]);
            CNC_DoReturn();
            do_netstat();
            break;

         case 'Q':   /* Quit  */
         case 'X':
            CNC_PutText(bm[2]);
            GetOut();

         case 'P':   /* Ping  */
            CNC_SetDoing(cm[20]);
            CNC_PutText(cm[20]);
            CNC_DoReturn();
            do_ping();
            break;

         case 'R':   /* Redraw   */
         case '?':
            CNC_PutText(cm[21]);
            commands();
            break;

         case 'S':   /* Shell */
            CNC_PutText(cm[22]);
            CNC_DoReturn();
            do_shell();
            break;

         case 'T':   /* Telnet   */
            CNC_SetDoing(cm[25]);
            CNC_PutText(cm[26]);
            CNC_DoReturn();

            if(!FileExists("CINS:Telnet/Menu"))
            {
               do_telnet_exe("");
               break;
            }

            do_telnet_menu();
            CINS_FreeList(&Config.telnet);
            CINS_FreeList(&tnel);
            break;
         
         case 'V':   /* View Fingers   */
            CNC_SetDoing(cm[23]);
            CNC_PutText(cm[23]);
            CNC_DoReturn();
            do_finger();
            break;

         case 'W':   /* Web Maint */
            CNC_SetDoing(cm[28]);
            CNC_PutText(cm[28]);
            CNC_DoReturn();
            CNC_PutText(cmdline[16]);
            break;

         case '1':   /* Talk  */
            CNC_SetDoing(cm[24]);
            CNC_PutText(cm[24]);
            CNC_DoReturn();
            do_talk();
            break;

         case '2':   /* Traceroute  */
            CNC_SetDoing(cm[27]);
            CNC_PutText(cm[27]);
            CNC_DoReturn();
            do_traceroute();
            break;

         default:
            CNC_DoReturn();
            break;
      }
   }
}  /* command_loop() */

void do_shell(void)
{
   char temp [ 64 ] = "";

   short i;

   while(TRUE)
   {
      CNC_SetDoing(cm[22]);

      sprintf(z->ABuffer, cm[5], cm[22]);

      CNC_EnterLine(65, ELINE_NONE, z->ABuffer);

      checkcarrier();

      if(CNC_CommonCommands())
      {
         if(z->Logoff)
         {
            GetOut();
         }
         continue;
      }
      else
         i = CNC_FindCommand(Config.Menu);

      if(!i)
      {
         if(*z->pitem[0])
         {
            sprintf(z->ABuffer, bm[310], z->pitem[0]);
            CNC_PutA();
         }
         continue;
      }

      switch(i)
      {
         case 1:  /* Quit        */
            return;

         case 2:  /* Talk        */
            CNC_SetDoing(cm[24]);
            if(!z->npitems)
               do_talk();
            else
               do_talk_exe(z->pitem[0]);
            break;

         case 3:  /* Traceroute  */
            CNC_SetDoing(cm[27]);
            if(!z->npitems)
            {
               do_traceroute();
            }
            else
            {
               do_traceroute_exe(z->pitem[0]);
            }
            break;

         case 4:  /* Telnet      */
            CNC_SetDoing(cm[26]);
            if(!z->npitems)
            {
               do_telnet_manual();
               break;
            }
            sprintf(temp, "%s", z->pitem[0]);

            if(z->npitems > 1)
            {
               strcat(temp, " ");
               strcat(temp, z->pitem[1]);
            }
            do_telnet_exe(temp);
            break;

         case 5:  /* FTP         */
            CNC_SetDoing(cm[14]);
            if(UCfg.uae.FTPFree && (check_dir_size() > UCfg.uae.FTPFree))
            {
               CNC_PutText(cm[122]);
               break;
            }
            if(!z->npitems)
            {
               do_ftp_manual();
            }   
            else
            {
               do_ftp_exe(z->pitem[0]);
            }
            if(UCfg.uae.FTPFree && (check_dir_size() > UCfg.uae.FTPFree))
            {
                  CNC_PutText(cm[122]);
            }
            break;

         case 6:  /* Netstat     */
            CNC_SetDoing(cm[19]);
            do_netstat();
            break;

         case 7:  /* Lynx        */
            CNC_SetDoing(cm[18]);
            do_lynx(*z->pitem[0] ? z->pitem[0] : Config.URL);
            break;

         case 8:  /* Archie      */
            CNC_SetDoing(cm[8]);
            archie();
            CINS_FreeList(&Config.archie);
            break;

         case 9:  /* IRC         */
            CNC_SetDoing(cm[17]);
            if(!z->npitems)
            {
               do_irc();
               CINS_FreeList(&Config.irc);
            }
            else
            {
               do_irc_exe(z->pitem[0], z->npitems > 1 ? z->pitem[1] : Config.IRC);
               break;
            }
            break;

         case 10: /* Finger      */
            CNC_SetDoing(cm[23]);
            if(!z->npitems)
               do_finger();
            else
               do_finger_exe(z->pitem[0]);
            break;

         case 11: /* Ping        */
            CNC_SetDoing(cm[20]);
            if(!z->npitems)
               do_ping();
            else
               do_ping_exe(z->pitem[0]);
            break;

         case 12: /* ?           */
            CNC_ReadFile("CINS:Menu/CINShell", TRUE);
            break;

         case 13: /* Config      */
            CNC_SetDoing(cm[10]);
            CNC_PutText(cmdline[10]);
            check_user_prefs(FALSE);
            break;

         case 14: /* DirMaint    */
            CNC_SetDoing(cm[12]);
            CNC_PutText(cmdline[11]);
            break;

         case 15: /* Gopher      */
            CNC_SetDoing(cm[15]);
            if(!z->npitems)
            {
               do_gopher();
               break;
            }

            sprintf(temp, "%s", z->pitem[0]);

            if(z->npitems > 1)
            {
               strcat(temp, " ");
               strcat(temp, z->pitem[1]);
            }
            do_gopher_exe(temp);
            break;

         case 16: /* Web Maint */
            CNC_SetDoing(cm[28]);
            CNC_PutText(cmdline[16]);
            break;

         default:
            sprintf(z->ABuffer, bm[310], z->pitem[0]);
            CNC_PutA();
            break;           
      }
   }
}  /* do_shell()  */

void setup(void)
{
   char  buf1 [ 256 ] = "";

   short c,
         i,
         lines = 0,
         filesize;

   BPTR  fp,
         lock;

   char *tmp;

   read_config();

   if(fp = Open(CINSCMD, MODE_OLDFILE))
   {
      FGets(fp, buf1, 256);
      FGets(fp, buf1, 256);

      for(i = 0; FGets(fp, buf1, sizeof(buf1)) && (i < COMMANDS);)
      {
         if(*buf1 != ';')
         {
            strncpy(cmdline[i], buf1, strlen(buf1) - 1);
            i++;
         }
      }
      Close(fp);
      if(i < COMMANDS)
      {
         CNC_PutText("\nMisconfiguration in "CINSCMD"!\nPlease alert your sysop!");
         GetOut();
      }
   }
   else
   {
      CNC_PutText("n1Cannot find "CINSCMD"n1");
      GetOut();
   }

   if(fp = Open(CINSTXT, MODE_OLDFILE))
   {
      if(!(ExamineFH(fp, z->fib)))
      {
         CNC_PutText("n1Cannot find "CINSTXT"n1");
         Close(fp);
         GetOut();
      }

      filesize = z->fib->fib_Size;

      if((newptr = (char *) AllocVec(filesize,MEMF_CLEAR)))
      {
         i = Read(fp, newptr, filesize);
         Close(fp);
         tmp = newptr;
         for(c = 0; c < i; c++)
         {
            if(*tmp == '\n')
            {
               lines++;
               *tmp = '\0';
            }
            tmp++;
         }
         cm = (char **)AllocVec((lines + 1) * sizeof(char*), MEMF_CLEAR);

         tmp = newptr;
         for(c = 0; c < lines; tmp++)
         {
            if(*tmp == '\0')
            {
               tmp++;
               cm[c] = tmp;
               c++;
            }
         }
      }
   }

   if(z->user1.Access < Config.Access)
   {
      CNC_PutText(cm[118]);
      GetOut();
   }

#ifndef CNET4
   check_uucp();
#endif

   sprintf(UCfg.HomeDir, Config.UserDir, z->user1.UUCP);

   sprintf(z->ABuffer, "Mail:Users/%s/.netrc", z->user1.UUCP);

   if(!FileExists(z->ABuffer))
   {
      new_user();
   }

   if(chdir(UCfg.HomeDir) != 0)
   {
      if(lock = CreateDir(UCfg.HomeDir))
      {
         UnLock(lock);
      }
      else
      {
         CNC_PutText(cm[117]);
         GetOut();
      }
   }
}  /* setup() */

void check_user_prefs(int ent)
{
   int   i;

   BPTR  fp;

   char  buff [ 64 ] = "";

   if(ent)
   {
      memset(&UCfg, 0, sizeof(struct CINSUCfg));
      for(i = 0; i < myp->HiPort + 1; i++)
      {
         UCfg.IUC[i] = z->IUCBits[i];
      }
   }

   sprintf(buff, CINSUCFGPAT, z->user1.UUCP);

   if(fp = Open(buff, MODE_OLDFILE))
   {
      FGets(fp, buff, sizeof(buff));
      UCfg.Shell = atoi(buff);
      buff[0] = 0;
      FGets(fp, buff, sizeof(buff));
      UCfg.Muff  = atoi(buff);
      buff[0] = 0;
      FGets(fp, buff, sizeof(buff));
      UCfg.DirANSI = atoi(buff);
      buff[0] = 0;
      FGets(fp, buff, sizeof(buff));
      strcpy(UCfg.IRCNick, buff);
      Close(fp);
   }
   else
   {
      UCfg.Shell   = Config.def_Shell;
      UCfg.DirANSI = Config.def_DirANSI;
      UCfg.Muff    = Config.def_Muff;
   }

   sprintf(z->ABuffer, CINSUSERLIMITPAT, z->user1.UUCP);

   if(fp = Open(z->ABuffer, MODE_OLDFILE))
   {
      FGets(fp, buff, sizeof(buff));
      UCfg.uae.FTPFree = atol(buff);
      Close(fp);
   }

   if(UCfg.Muff)
   {
      z->MuffAll = TRUE;
   }
} /* check_user_prefs */

void new_user(void)
{
   BPTR  fp,
         lock;

   char  buff [ 256 ] = "";

   CNC_PutText(cm[6]);
   if(lock = CreateDir(UCfg.HomeDir))
      UnLock(lock);

   CopyMem(&Config.def_ftp, (APTR)&UCfg.ftp, sizeof(struct NcFTPCfg)); /* Copy defaults */

   sprintf(z->ABuffer, "Mail:Users/%s/.netrc", z->user1.UUCP);

   if(fp = Open(z->ABuffer, MODE_NEWFILE))
   {
      sprintf(z->ABuffer, "%s\n", cm[33]);     /* logfile */
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[34]);           /* progress */
      sprintf(z->ABuffer, buff, UCfg.ftp.Progress);
      FPrintf(fp, z->ABuffer);
      FPrintf(fp, "#set local-dir %s\n", UCfg.HomeDir);  /* local dir */
      sprintf(buff, "%s\n", cm[35]);     /* ansi-esc */
      sprintf(z->ABuffer, buff, UCfg.ftp.ANSIEsc);
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[36]);           /* auto-bin */
      sprintf(z->ABuffer, buff, UCfg.ftp.AutoBin);
      FPrintf(fp, z->ABuffer);
      sprintf(z->ABuffer, "%s\n", cm[37]);           /* logsize  */
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[38]);           /* verbose  */
      sprintf(z->ABuffer, buff, UCfg.ftp.Verbose);
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[39]);           /* anon-open */
      sprintf(z->ABuffer, buff, UCfg.ftp.AutoAnon);
      FPrintf(fp, z->ABuffer);
      FPrintf(fp,"#set anon-password %s@%s\n", z->user1.UUCP, myp->gc.MyUUCPName);
      sprintf(buff, "%s\n", cm[40]);           /* recent */
      sprintf(z->ABuffer, buff, UCfg.ftp.Recent);
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[41]);           /* tips */
      sprintf(z->ABuffer, buff, UCfg.ftp.Tips);
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[42]);
      sprintf(z->ABuffer, buff, cm[43]);       /* prompt */
      FPrintf(fp, z->ABuffer);
      sprintf(buff, "%s\n", cm[44]);           /* mget prompting */
      sprintf(z->ABuffer, buff, UCfg.ftp.MGet);
      FPrintf(fp, z->ABuffer);
      Close(fp);
   }

   if(Config.AddNewToDB)
   {
      struct Library *MiamiBase;

      sprintf(z->ABuffer, cm[181], z->user1.UUCP, z->id, z->user1.Handle, UCfg.HomeDir);

      if(MiamiBase = OpenLibrary("miami.library", 0))
      {
         Config.StackType = ST_MIAMI;

         if(MiamiBase->lib_Version > 9)
         {
/*
** Code unable to operate due to Miami 2.x's ChangeDB
** not operating on the users section of its DB
** NOTE: This does work with Miami 3.x!
*/
            if(fp = OpenAppend("ENVARC:MiamiChangeDB"))
            {
               FPrintf(fp, cm[182], z->ABuffer);
               Close(fp);
            }
         }
         CloseLibrary(MiamiBase);
      }
      else
      {
         if(FileExists("AmiTCP:db/passwd"))
         {
            Config.StackType = ST_AMITCP;
            if(fp = OpenAppend("AmiTCP:db/passwd"))
            {
               FPuts(fp, z->ABuffer);
               Close(fp);
            }
         }
      }
   }
} /* new_user */

void do_ping(void)
{
   CNC_EnterLine(24, ELINE_NOLEADSPACE | ELINE_INPUTBOX, cm[46]);

   checkcarrier();

   if(!*z->InBuffer)
      return;

   do_ping_exe(z->InBuffer);
} /* do_ping */

void do_ping_exe(char *site)
{
   CNC_DoReturn();
   if(!z->SysMaint)
      sprintf(z->ABuffer, cmdline[8], site);
   else
      sprintf(z->ABuffer, cmdline[9], site);

   CNC_PutA();
} /* do_ping_exe */

void do_talk(void)
{
   CNC_EnterLine(40, ELINE_NOLEADSPACE | ELINE_INPUTBOX, cm[47]);

   checkcarrier();

   if(!*z->InBuffer)
      return;

   CNC_DoReturn();

   do_talk_exe(z->InBuffer);
} /* do_talk */

void do_talk_exe(char *user)
{
   sprintf(z->ABuffer, cmdline[3], user);
   CNC_PutA();
} /* do_talk_exe */

void do_gopher(void)
{
   sprintf(z->ABuffer, cm[119], Config.Gopher);

   CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_INPUTBOX, z->ABuffer);

   checkcarrier();

   if(!*z->InBuffer)
      do_gopher_exe(Config.Gopher);
   else
      do_gopher_exe(z->InBuffer);
} /* do_gopher */

void do_gopher_exe(char *site)
{
   sprintf(z->ABuffer, cmdline[12], site);
   CNC_PutA();
} /* do_gopher_exe */

void do_finger(void)
{
   CNC_EnterLine(40, ELINE_NOLEADSPACE | ELINE_INPUTBOX, cm[48]);

   checkcarrier();

   if(!*z->InBuffer)
      return;

   do_finger_exe(z->InBuffer);
} /* do_finger */

void do_finger_exe(char *user)
{
   CNC_DoReturn();
   sprintf(z->ABuffer, cmdline[4], user);
   CNC_PutA();
} /* do_finger_exe */

void do_traceroute(void)
{
   CNC_EnterLine(40, ELINE_NOLEADSPACE | ELINE_INPUTBOX, cm[49]);

   checkcarrier();

   if(!*z->InBuffer)
      return;

   do_traceroute_exe(z->InBuffer);
} /* do_traceroute */

void do_traceroute_exe(char *site)
{
   sprintf(z->ABuffer, cmdline[6], site);
   CNC_PutA();
} /* do_traceroute_exe */

void do_netstat(void)
{
   if(z->SysMaint)
      CNC_PutText(cmdline[7]);
   else
      CNC_PutText(bm[1091]);
} /* do_netstat */

void do_telnet_menu(void)
{
   BPTR  fp;

   struct TelnetMenu *work;

   short c,
         site,
         start = 0;

   static short i = 0;

   struct TelnetMSite tnms;

   if(!Config.telnet)
   {
      if(!(Config.telnet = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      if(!(fp = Open("CINS:Telnet/Menu", MODE_OLDFILE)))
      {
         sprintf(z->ABuffer, bm[635], "CINS:Telnet/Menu");
         CNC_PutA();
         return;
      }

      while(FGets(fp, tnms.Menu, sizeof(tnms.Menu)) != NULL)
      {
         tnms.Menu[strlen(tnms.Menu)-1] = '\0';
         CINS_AddTelnetName(Config.telnet, &tnms);
         i++;
      }

      Close(fp);
   }

   CNC_DoReturn();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 0)); c++)
      {
         work = (struct TelnetMenu *)CINS_FindOrdinal(Config.telnet, start + c);
         if(work && (work->tnm_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[52], c+1, work->tnm_Site.Menu);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[195]);
         else
            CNC_PutText(cm[194]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[53]);
      else
         CNC_PutText(cm[54]);

      site = do_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_NONE:
            break;

         case MENU_NOTE:
            do_telnet_notebook();
            save_telnet_notebook();
            break;

         case MENU_MAN:
            do_telnet_manual();
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            CINS_FreeList(&Config.telnet);
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            do_telnet_submenu(site);
            break;
      }
   }
} /* do_telnet_menu */

void do_telnet_submenu(int menu)
{
   BPTR  fp;

   struct TelnetEntry *work;
   struct TelnetSite   tns;

   char  buff [ 256 ] = "";

   short c,
         site,
         start = 0;

   static short i = 0;

   if(tnel)
   {
      CINS_FreeList(&tnel);
      tnel = NULL;
   }

   if(!tnel)
   {
      if(!(tnel = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      sprintf(buff, "CINS:Telnet/%d", menu + 1);

      if(!(fp = Open(buff, MODE_OLDFILE)))
      {
         sprintf(z->ABuffer, bm[635], buff);
         CNC_PutA();
         return;
      }

      while(FGets(fp, tns.Name, sizeof(tns.Name)) != NULL)    /* Site short desc */
      {
         FGets(fp, tns.Host , sizeof(tns.Host));   /* site hostname/IP */
         FGets(fp, tns.Comm1, sizeof(tns.Comm1));  /* site long desc 1 */
         FGets(fp, tns.Comm2, sizeof(tns.Comm2));  /* site long desc 2 */
         tns.Name [strlen(tns.Name) -1] = '\0';
         tns.Host [strlen(tns.Host) -1] = '\0';
         tns.Comm1[strlen(tns.Comm1)-1] = '\0';
         tns.Comm2[strlen(tns.Comm2)-1] = '\0';
         i++;
         CINS_AddTelnetDataName(tnel, &tns);
      }

      Close(fp);
   }

   CNC_DoReturn();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 0)); c++)
      {
         work = (struct TelnetEntry *)CINS_FindOrdinal(tnel, start + c);
         if(work && (work->tn_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[52], c+1, work->tn_Site.Name);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[195]);
         else
            CNC_PutText(cm[194]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[53]);
      else
         CNC_PutText(cm[54]);

      site = do_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_NONE:
            break;

         case MENU_NOTE:
            do_telnet_notebook();
            save_telnet_notebook();
            break;

         case MENU_MAN:
            do_telnet_manual();
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            CINS_FreeList(&tnel);
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct TelnetEntry *)CINS_FindOrdinal(tnel, site);
            if(work && (work->tn_Node.ln_Succ != NULL))
            {
               sprintf(z->ABuffer, cm[115], work->tn_Site.Name, work->tn_Site.Comm1, work->tn_Site.Comm2);
               CNC_PutA();
               do_telnet_exe(work->tn_Site.Host);
               break;
            }
      }
   }
} /* do_telnet_menu */

void do_telnet_manual(void)
{
   CNC_EnterLine(60,ELINE_NOLEADSPACE | ELINE_INPUTBOX,cm[50]);

   checkcarrier();

   if(!*z->InBuffer)
      return;

   CNC_DoReturn();
   do_telnet_exe(z->InBuffer);
} /* do_telnet_manual */

void do_telnet_notebook(void)
{
   struct TelnetEntry *work;
   struct TelnetSite   tns;

   short c,
         site,
         start = 0;

   static short i = 0;

   char ii;

   BPTR fp;

   if(!UCfg.tnnl)
   {
      if(!(UCfg.tnnl = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      sprintf(z->ABuffer, "%s/.Telnet", UCfg.HomeDir);

      if(fp = Open(z->ABuffer, MODE_OLDFILE))
      {
         while(FGets(fp, tns.Name, sizeof(tns.Name)))
         {
            FGets(fp, tns.Host , sizeof(tns.Host));   /* site hostname/IP */
            FGets(fp, tns.Comm1, sizeof(tns.Comm1));  /* site long desc 1 */
            FGets(fp, tns.Comm2, sizeof(tns.Comm2));  /* site long desc 2 */
            tns.Name [strlen(tns.Name) -1] = '\0';
            tns.Host [strlen(tns.Host) -1] = '\0';
            tns.Comm1[strlen(tns.Comm1)-1] = '\0';
            tns.Comm2[strlen(tns.Comm2)-1] = '\0';
            i++;
            CINS_AddTelnetDataName(UCfg.tnnl, &tns);
         }
         i--;
         Close(fp);
      }
   }

   sprintf(z->ABuffer, cm[175], cm[26]);
   CNC_PutA();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct TelnetEntry *)CINS_FindOrdinal(UCfg.tnnl, start + c);
         if(work && (work->tn_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[52], c+1, work->tn_Site.Name);
            CNC_PutA();
         }
      }

      CNC_DoReturn();

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[197]);
         else
            CNC_PutText(cm[196]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[166]);
      else
         CNC_PutText(cm[167]);

      site = do_notebook_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_ADD:
            CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[169]);
            if(*z->InBuffer)
            {
               strcpy(tns.Name, z->InBuffer);
               CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[170]);
               if(*z->InBuffer)
               {
                  strcpy(tns.Host, z->InBuffer);
                  CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[171]);
                  strcpy(tns.Comm1, z->InBuffer);
                  CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[172]);
                  strcpy(tns.Comm2, z->InBuffer);
                  CINS_AddTelnetDataName(UCfg.tnnl, &tns);
                  i++;
               }
            }
            checkcarrier();
            break;

         case MENU_DELE:
            CNC_PutText(cm[163]);
            ii = CNC_OneKey();

            checkcarrier();

            site = (ii - '1') + start;

            if((site > -1) && site < (i + 1))
            {
               if(CINS_RemoveNode(UCfg.tnnl, start + site))
               {
                  sprintf(z->ABuffer, cm[164], ii - '0');
                  CNC_PutA();
                  i--;
               }
               else
               {
                  sprintf(z->ABuffer, cm[165], ii - '0');
                  CNC_PutA();
               }
            }
            break;

         case MENU_NONE:
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct TelnetEntry *)CINS_FindOrdinal(UCfg.tnnl, site);
            if(work && (work->tn_Node.ln_Succ != NULL))
            {
               sprintf(z->ABuffer, cm[115], work->tn_Site.Name, work->tn_Site.Comm1, work->tn_Site.Comm2);
               CNC_PutA();
               do_telnet_exe(work->tn_Site.Host);
               break;
            }
      }
   }
} /* do_telnet_notebook */

void save_telnet_notebook(void)
{
   BPTR fp;

   struct TelnetEntry *work;

   sprintf(z->ABuffer, "%s/.Telnet", UCfg.HomeDir);

   if(UCfg.tnnl)
   {
      work = (struct TelnetEntry *)UCfg.tnnl->lh_Head;

      if(fp = Open(z->ABuffer, MODE_NEWFILE))
      {
         while(work && work->tn_Node.ln_Succ)
         {
            FPrintf(fp, "%s\n", work->tn_Site.Name);
            FPrintf(fp, "%s\n", work->tn_Site.Host);
            FPrintf(fp, "%s\n", work->tn_Site.Comm1);
            FPrintf(fp, "%s\n", work->tn_Site.Comm2);
            work = (struct TelnetEntry *)work->tn_Node.ln_Succ;
         }
         Close(fp);
      }
   }
} /* save_telnet_notebook */

void do_telnet_exe(char *site)
{
   CNC_SetDoing(cm[26]);
   sprintf(z->ABuffer, cmdline[0], site);
   CNC_PutA();
} /* do_telnet_exe */

int do_menu(void)
{
   char ii;

   ii = CNC_OneKey();

   checkcarrier();

   switch(ii)
   {
      case 'M':
         CNC_PutText(cm[186]); /* Manual */
         return MENU_MAN;

      case 'N':
         CNC_PutText(cm[187]); /* Next */
         return MENU_NEXT;

      case 'O':
         CNC_PutText(cm[189]); /* Notebook */
         return MENU_NOTE;

      case 'P':
         CNC_PutText(cm[188]); /* Previous */
         return MENU_PREV;

      case 'Q':
         CNC_PutText(bm[2]);   /* Quit */
         CNC_DoReturn();
         return MENU_QUIT;

      default:
         if(isdigit(ii))   /* Normal */
         {
            sprintf(z->ABuffer, cm[190], ii - '0');
            CNC_PutA();
            return (ii - '1');
         }
         else
            return MENU_NONE;
   }
   return 0;
} /* do_menu */

int check_ftp_open(void)
{
   char fvar [ 4 ] = "";

   int ftpi;

   GetVar("CNFTP", fvar, 3, GVF_GLOBAL_ONLY);
   ftpi = atoi(fvar);

   if(Config.FTPin > ftpi)
   {
      ftpi++;
      sprintf(z->ABuffer, "%ld", ftpi);

      SetVar("CNFTP", z->ABuffer, 3, GVF_GLOBAL_ONLY);

      return TRUE;
   }
   else if(Config.FTPin)
   {
      CNC_PutText(cm[57]);
      return FALSE;
   }
   return TRUE;
} /* check_ftp_open */

void check_ftp_close(void)
{
   char fvar [ 4 ] = "";

   int ftpi;

   GetVar("CNFTP", fvar, 3, GVF_GLOBAL_ONLY);

   ftpi = atoi(fvar);

   sprintf(z->ABuffer, "%ld", ftpi);

   SetVar("CNFTP", z->ABuffer, 3, GVF_GLOBAL_ONLY);
} /* check_ftp_close */

void do_ftp_menu(void)
{
   BPTR  fp;

   struct FTPMenu *work;

   short c,
         site,
         start = 0;

   static short i = 0;

   struct FTPMSite ftpms;

   if(!Config.ftp)
   {
      if(!(Config.ftp = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      if(!(fp = Open("CINS:FTP/Menu", MODE_OLDFILE)))
      {
         CNC_DoReturn();
         sprintf(z->ABuffer, bm[635], "CINS:FTP/Menu");
         CNC_PutA();
         return;
      }

      while(FGets(fp, ftpms.Menu, sizeof(ftpms.Menu)) != NULL)
      {
         ftpms.Menu[strlen(ftpms.Menu)-1] = '\0';
         CINS_AddFTPName(Config.ftp, &ftpms);
         i++;
      }

      Close(fp);
   }

   CNC_DoReturn();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 0)); c++)
      {
         work = (struct FTPMenu *)CINS_FindOrdinal(Config.ftp, start + c);
         if(work && (work->ftpm_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[52], c+1, work->ftpm_Site.Menu);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[195]);
         else
            CNC_PutText(cm[194]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[53]);
      else
         CNC_PutText(cm[54]);

      site = do_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_NONE:
            break;

         case MENU_NOTE:
            do_ftp_notebook();
            save_ftp_notebook();
            break;

         case MENU_MAN:
            do_ftp_manual();
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            CINS_FreeList(&Config.ftp);
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            do_ftp_submenu(site);
            break;
      }
   }
} /* do_ftp_menu */

void do_ftp_submenu(int menu)
{
   BPTR  fp;

   struct FTPEntry *work;
   struct FTPSite   ftps;

   char  buff [ 256 ] = "";

   short c,
         site,
         start = 0;

   static short i = 0;

   if(ftpel)
   {
      CINS_FreeList(&ftpel);
      ftpel = NULL;
   }

   if(!ftpel)
   {
      if(!(ftpel = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      sprintf(buff, "CINS:FTP/%d", menu + 1);

      if(!(fp = Open(buff, MODE_OLDFILE)))
      {
         sprintf(z->ABuffer, bm[635], buff);
         CNC_PutA();
         return;
      }

      while(FGets(fp, ftps.Name, sizeof(ftps.Name)) != NULL)    /* site short desc */
      {
         FGets(fp, ftps.Host , sizeof(ftps.Host));   /* site hostname/IP */
         FGets(fp, ftps.Comm1, sizeof(ftps.Comm1));  /* site long desc 1 */
         FGets(fp, ftps.Comm2, sizeof(ftps.Comm2));  /* site long desc 2 */
         ftps.Name [strlen(ftps.Name) -1] = '\0';
         ftps.Host [strlen(ftps.Host) -1] = '\0';
         ftps.Comm1[strlen(ftps.Comm1)-1] = '\0';
         ftps.Comm2[strlen(ftps.Comm2)-1] = '\0';
         i++;
         CINS_AddFTPDataName(ftpel, &ftps);
      }

      i--;

      Close(fp);
   }

   CNC_DoReturn();
   CNC_DoReturn();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct FTPEntry *)CINS_FindOrdinal(ftpel, start + c);
         if(work && (work->ftp_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[52], c+1, work->ftp_Site.Name);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[195]);
         else
            CNC_PutText(cm[194]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[53]);
      else
         CNC_PutText(cm[54]);

      site = do_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_NONE:
            break;

         case MENU_NOTE:
            do_ftp_notebook();
            save_ftp_notebook();
            break;

         case MENU_MAN:
            do_ftp_manual();
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            CINS_FreeList(&ftpel);
            return;

         default:
            site += start;
            if(site > i)
               break;
            work = (struct FTPEntry *)CINS_FindOrdinal(ftpel, site);
            if(work && (work->ftp_Node.ln_Succ != NULL))
            {
               sprintf(z->ABuffer, cm[115], work->ftp_Site.Name, work->ftp_Site.Comm1, work->ftp_Site.Comm2);
               CNC_PutA();
               do_ftp_exe(work->ftp_Site.Host);
               break;
            }
      }
   }
} /* do_ftp_submenu */

void do_ftp_manual(void)
{
   CNC_EnterLine(60,ELINE_NOLEADSPACE | ELINE_INPUTBOX, cm[51]);

   checkcarrier();

   if(!*z->InBuffer)
      return;

   CNC_DoReturn();
   do_ftp_exe(z->InBuffer);
} /* do_ftp_manual */

void do_ftp_notebook(void)
{
   struct FTPEntry *work;
   struct FTPSite   ftps;

   short c,
         site,
         start = 0;

   static short i = 0;

   char ii;

   BPTR fp;

   if(!UCfg.ftpnl)
   {
      if(!(UCfg.ftpnl = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      sprintf(z->ABuffer, "%s/.FTP", UCfg.HomeDir);

      if(fp = Open(z->ABuffer, MODE_OLDFILE))
      {
         while(FGets(fp, ftps.Name, sizeof(ftps.Name)))
         {
            FGets(fp, ftps.Host , sizeof(ftps.Host));   /* site hostname/IP */
            FGets(fp, ftps.Comm1, sizeof(ftps.Comm1));  /* site long desc 1 */
            FGets(fp, ftps.Comm2, sizeof(ftps.Comm2));  /* site long desc 2 */
            ftps.Name [strlen(ftps.Name) -1] = '\0';
            ftps.Host [strlen(ftps.Host) -1] = '\0';
            ftps.Comm1[strlen(ftps.Comm1)-1] = '\0';
            ftps.Comm2[strlen(ftps.Comm2)-1] = '\0';
            i++;
            CINS_AddFTPDataName(UCfg.ftpnl, &ftps);
         }
         i--;
         Close(fp);
      }
   }

   sprintf(z->ABuffer, cm[175], cm[14]);
   CNC_PutA();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct FTPEntry *)CINS_FindOrdinal(UCfg.ftpnl, start + c);
         if(work && (work->ftp_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[52], c+1, work->ftp_Site.Name);
            CNC_PutA();
         }
      }

      CNC_DoReturn();

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[197]);
         else
            CNC_PutText(cm[196]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[166]);
      else
         CNC_PutText(cm[167]);

      site = do_notebook_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_ADD:
            CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[169]);
            if(*z->InBuffer)
            {
               strcpy(ftps.Name, z->InBuffer);
               CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[170]);
               if(*z->InBuffer)
               {
                  strcpy(ftps.Host, z->InBuffer);
                  CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[171]);
                  strcpy(ftps.Comm1, z->InBuffer);
                  CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[172]);
                  strcpy(ftps.Comm2, z->InBuffer);
                  CINS_AddFTPDataName(UCfg.ftpnl, &ftps);
                  i++;
               }
            }
            checkcarrier();
            break;

         case MENU_DELE:
            CNC_PutText(cm[163]);
            ii = CNC_OneKey();

            checkcarrier();

            site = (ii - '1') + start;

            if((site > -1) && site < (i + 1))
            {
               if(CINS_RemoveNode(UCfg.ftpnl, start + site))
               {
                  sprintf(z->ABuffer, cm[164], ii - '0');
                  CNC_PutA();
                  i--;
               }
               else
               {
                  sprintf(z->ABuffer, cm[165], ii - '0');
                  CNC_PutA();
               }
            }
            break;

         case MENU_NONE:
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct FTPEntry *)CINS_FindOrdinal(UCfg.ftpnl, site);
            if(work && (work->ftp_Node.ln_Succ != NULL))
            {
               sprintf(z->ABuffer, cm[115], work->ftp_Site.Name, work->ftp_Site.Comm1, work->ftp_Site.Comm2);
               CNC_PutA();
               do_ftp_exe(work->ftp_Site.Host);
               break;
            }
      }
   }
} /* do_ftp_notebook */

void save_ftp_notebook(void)
{
   BPTR fp;

   struct FTPEntry *work;

   sprintf(z->ABuffer, "%s/.FTP", UCfg.HomeDir);

   if(UCfg.ftpnl)
   {
      work = (struct FTPEntry *)UCfg.ftpnl->lh_Head;

      if(fp = Open(z->ABuffer, MODE_NEWFILE))
      {
         while(work && work->ftp_Node.ln_Succ)
         {
            FPrintf(fp, "%s\n", work->ftp_Site.Name);
            FPrintf(fp, "%s\n", work->ftp_Site.Host);
            FPrintf(fp, "%s\n", work->ftp_Site.Comm1);
            FPrintf(fp, "%s\n", work->ftp_Site.Comm2);
            work = (struct FTPEntry *)work->ftp_Node.ln_Succ;
         }
         Close(fp);
      }
   }
} /* save_ftp_notebook */

void do_ftp_exe(char *site)
{
   CNC_SetDoing(cm[14]);
   sprintf(z->ABuffer, cmdline[1], site);
   CNC_PutA();
} /* do_ftp_exe */

void archie(void)
{
   BPTR  fp;

   short matches = 20;

   char  srch   [  3 ] = "-s",
         nice   [  6 ] = "-N4",
         one    [  3 ] = "",
         search [ 30 ] = "** NONE **",
         server [ 60 ] = "";

   char buff [ 256 ] = "";

   char  ii;

   strcpy(server, Config.Archie);

   while(TRUE)
   {
      CNC_PutText(cm[59]);

      CNC_PutText(cm[60]);

      if(!strcmp(srch, "-c"))
         CNC_PutText(cm[61]);
      else if(!strcmp(srch, "-e"))
         CNC_PutText(cm[62]);
      else if(!strcmp(srch, "-r"))
         CNC_PutText(cm[63]);
      else if(!strcmp(srch, "-s"))
         CNC_PutText(cm[64]);

      CNC_PutText(cm[67]);

      if(!strcmp(nice, "-N4"))
         CNC_PutText(cm[68]);
      else if(!strcmp(nice, "-N50"))
         CNC_PutText(cm[69]);
      else if(!strcmp(nice, "-N100"))
         CNC_PutText(cm[70]);

      sprintf(z->ABuffer, cm[73], !strcmp(one, "-l") ? cm[74]:cm[75]);
      CNC_PutA();

      sprintf(z->ABuffer, cm[77], matches);
      CNC_PutA();
      sprintf(z->ABuffer, cm[78], server);
      CNC_PutA();
      sprintf(z->ABuffer, cm[79], search);
      CNC_PutA();

      CNC_PutText(cm[80]);
      CNC_PutText(cm[81]);
      CNC_PutText(cm[82]);

      ii = CNC_OneKey();

      checkcarrier();

      switch(ii)
      {
         case '1':
            archie_pref(srch);
            break;

         case '2':
            archie_results(nice);
            break;

         case '3':
            archie_form(one);
            break;

         case '4':
            matches = archie_matches();
            break;

         case '5':
            archie_server(server);
            break;

         case '6':
            archie_search(search);
            break;

         case '7':
            if(!strcmp(search, "** NONE **"))
               archie_search(search);

            CNC_PutText(cm[83]);

            if(!strcmp(nice, "-N4"))
            {
               CNC_PutText(cm[84]);
               CNC_ReadGraphics("CINS:Text/sys.LongTime", FALSE);
               sprintf(z->ABuffer, "#4 %s -h %s -m %d %s -N3 %s %s}", cmdline[13], server, matches, one, srch, search);
               CNC_PutA();
            }

            if(!strcmp(nice, "-N100"))
            {
               sprintf(buff, "%s/_CINS_Arch", UCfg.HomeDir);

               if(fp = Open(buff, MODE_NEWFILE))
               {
                  FPrintf(fp, "%s >%s/%s -h %s -m%ld %s -N3 %s %s\n", cmdline[13], UCfg.HomeDir, z->user1.UUCP, server, matches, one, srch, search);
                  sprintf(z->ABuffer, "%s/%s", UCfg.HomeDir, z->user1.UUCP);
                  FPrintf(fp, bm[2132], z->ABuffer, cm[130], myp->gc.MyUUCPName, cm[131], z->user1.UUCP);
                  FPrintf(fp, "Wait 10\n");
                  FPrintf(fp, "Delete %s/%s\n", UCfg.HomeDir, z->user1.UUCP);

                  if(!myp->gc.MonitorUUMail)
                     FPrintf(fp, cmdline[14]);

                  Close(fp);
               }

               sprintf(buff, "run Execute %s/_CINS_Arch", UCfg.HomeDir);
               SystemTags(buff, TAG_END);
            }

            if(!strcmp(nice, "-N50"))
            {
               sprintf(buff, "run %s >%s/Archie.txt -h %s -m%d %s -N3 %s %s", cmdline[13], UCfg.HomeDir, server, matches, one, srch, search);
               SystemTags(buff, TAG_END);
            }
            CNC_PutText(cm[85]);

            CNC_PutText(cm[86]);

            checkcarrier();
            break;

         case 'Q':
            CNC_PutText(bm[2]);
            CNC_DoReturn();
            return;
      }
   }
} /* archie */

void archie_pref(char *srch)
{
   char ii;

   CNC_PutText(cm[87]);
   CNC_PutText(cm[88]);
   CNC_PutText(cm[89]);
   CNC_PutText(cm[90]);
   CNC_PutText(cm[93]);

   ii = CNC_OneKey();

   checkcarrier();

   switch(ii)
   {
      case 'C':
         srch[1] = 'c';
         break;

      case 'E':
         srch[1] = 'e';
         break;

      case 'R':
         srch[1] = 'r';
         break;

      case 'S':
         srch[1] = 's';
         break;
   }
} /* archie_prefs */

void archie_results(char *nice)
{
   char ii;

   CNC_PutText(cm[94]);
   CNC_PutText(cm[95]);
   CNC_PutText(cm[96]);
   CNC_PutText(cm[99]);

   checkcarrier();

   ii = CNC_OneKey();

   switch(ii)
   {
      case '1':
         strcpy(nice, "-N4");
         break;

      case '2':
         strcpy(nice, "-N50");
         break;

      case '3':
         strcpy(nice, "-N100");
         break;
   }
} /* archie_results */

void archie_form(char *one)
{
   if(!strcmp(one, "-l"))
      strcpy(one, "");
   else
      strcpy(one, "-l");
} /* archie_form */

int archie_matches(void)
{
   int matches;

   CNC_EnterLine(3, ELINE_ALLCAPS | ELINE_NUMBERSONLY, cm[100]);

   checkcarrier();

   matches = atoi(z->InBuffer);

   if(matches < 5)
      matches = 5;

   if(matches > 95)
      matches = 95;

   return matches;
} /* archie_matches */

void archie_server(char *server)
{
   BPTR  fp;

   short c,
         site,
         start = 0;

   static short i = 0;

   struct ArchieEntry *work;
   struct ArchieSite   arcs;

   sprintf(z->ABuffer, cm[101], server);
   CNC_PutA();

   if(!Config.archie)
   {
      if(!(Config.archie = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      if(!(fp = Open(CINSARCHIECFG, MODE_OLDFILE)))
      {
         sprintf(z->ABuffer, bm[635], CINSARCHIECFG);
         CNC_PutText(z->ABuffer);
         GetOut();
      }

      while(FGets(fp, arcs.Site, sizeof(arcs.Site)) != NULL)
      {
         arcs.Site[strlen(arcs.Site)-1] = 0;
         i++;
         CINS_AddArchieName(Config.archie, &arcs);
      }

      i--;

      Close(fp);
   }

   CNC_PutText(cm[103]);

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, start + c);
         if(work && (work->arc_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[102], c+1, work->arc_Site.Site);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[195]);
         else
            CNC_PutText(cm[194]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[53]);
      else
         CNC_PutText(cm[54]);

      site = do_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_NONE:
            break;

         case MENU_NOTE:
            if(do_archie_notebook(server))
            {
               save_archie_notebook();
               return;
            }
            save_archie_notebook();
            break;

         case MENU_MAN:
            do_archie_manual(server);
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct ArchieEntry *)CINS_FindOrdinal(Config.archie, site);
            if(work && (work->arc_Node.ln_Succ != NULL))
            {
               strcpy(server, work->arc_Site.Site);
               return;
            }
      }
   }
} /* archie_server */

void do_archie_manual(char *server)
{
   CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[105]);
   if(*z->InBuffer)
      strcpy(server, z->InBuffer);

   checkcarrier();

   return;
} /* do_archie_manual */

BOOL do_archie_notebook(char *server)
{
   struct ArchieEntry *work;
   struct ArchieSite   arcs;

   short c,
         site,
         start = 0;

   static short i = 0;

   char ii;

   BPTR fp;

   if(!UCfg.arcnl)
   {
      if(!(UCfg.arcnl = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return FALSE;
      }

      sprintf(z->ABuffer, "%s/.Archie", UCfg.HomeDir);

      if(fp = Open(z->ABuffer, MODE_OLDFILE))
      {
         while(FGets(fp, arcs.Site, sizeof(arcs.Site)))
         {
            arcs.Site[strlen(arcs.Site)-1] = 0;
            i++;
            CINS_AddArchieName(UCfg.arcnl, &arcs);
         }
         i--;
         Close(fp);
      }
   }

   sprintf(z->ABuffer, cm[175], cm[8]);
   CNC_PutA();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct ArchieEntry *)CINS_FindOrdinal(UCfg.arcnl, start + c);
         if(work && (work->arc_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[102], c+1, work->arc_Site.Site);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[197]);
         else
            CNC_PutText(cm[196]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[166]);
      else
         CNC_PutText(cm[167]);

      site = do_notebook_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_ADD:
            CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[169]);
            if(*z->InBuffer)
            {
               strcpy(arcs.Site, z->InBuffer);
               CINS_AddArchieName(UCfg.arcnl, &arcs);
               i++;
            }
            checkcarrier();
            break;

         case MENU_DELE:
            CNC_PutText(cm[163]);
            ii = CNC_OneKey();

            checkcarrier();

            site = (ii - '1') + start;

            if((site > -1) && site < (i + 1))
            {
               if(CINS_RemoveNode(UCfg.arcnl, start + site))
               {
                  sprintf(z->ABuffer, cm[164], ii - '0');
                  CNC_PutA();
                  i--;
               }
               else
               {
                  sprintf(z->ABuffer, cm[165], ii - '0');
                  CNC_PutA();
               }
            }
            break;

         case MENU_NONE:
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            return FALSE;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct ArchieEntry *)CINS_FindOrdinal(UCfg.arcnl, site);
            if(work && (work->arc_Node.ln_Succ != NULL))
            {
               strcpy(server, work->arc_Site.Site);
               return TRUE;
            }
      }
   }
} /* do_archie_notebook */

void save_archie_notebook(void)
{
   BPTR fp;

   struct ArchieEntry *work;

   sprintf(z->ABuffer, "%s/.Archie", UCfg.HomeDir);

   if(UCfg.arcnl)
   {
      work = (struct ArchieEntry *)UCfg.arcnl->lh_Head;

      if(fp = Open(z->ABuffer, MODE_NEWFILE))
      {
         while(work && work->arc_Node.ln_Succ)
         {
            FPrintf(fp, "%s\n", work->arc_Site.Site);
            work = (struct ArchieEntry *)work->arc_Node.ln_Succ;
         }
         Close(fp);
      }
   }
} /* save_archie_notebook */

void archie_search(char *search)
{
   CNC_EnterLine(30, ELINE_NOOLMS | ELINE_FILENAME, cm[104]);
   strcpy(search, z->InBuffer);

   checkcarrier();

   if(!*search)
      strcpy(search,"** NONE **");
} /* archie_search */

void view_help(void)
{
   short q;

   while(TRUE)
   {
      q = TRUE;

      CNC_PutText(cm[107]);

      DirRead("CINS:Help");

      CNC_EnterLine(20, ELINE_ALLCAPS, bm[157]);

      checkcarrier();

      if((*z->InBuffer == 'Q') || (!*z->InBuffer))
         return;

      if(!strcmp(z->InBuffer, "MENU"))
         q = FALSE;

      switch(q)
      {
         case FALSE:
            break;

         case TRUE:
            view_help_file(z->InBuffer);
            break;
      }
   }
} /* view_help */

void view_help_file(char *file)
{
   sprintf(z->ABuffer, "CINS:Help/%s", file);

   if(FileExists(z->ABuffer))
   {
      CNC_PutText(cm[108]);
      CNC_ReadFile(z->ABuffer, FALSE);
   }
   else
   {
      sprintf(z->ABuffer, bm[156], file);
      CNC_PutA();
   }
   CNC_PutText(cm[86]);
} /* do_help_file */

int do_notebook_menu(void)
{
   char ii;

   ii = CNC_OneKey();

   checkcarrier();

   switch(ii)
   {
      case 'A':
         CNC_PutText(cm[191]); /* Add */
         return MENU_ADD;

      case 'D':
         CNC_PutText(cm[192]); /* Delete */
         return MENU_DELE;

      case 'N':
         CNC_PutText(cm[187]); /* Next */
         return MENU_NEXT;

      case 'P':
         CNC_PutText(cm[188]); /* Previous */
         return MENU_PREV;

      case 'Q':
         CNC_PutText(bm[2]);   /* Quit */
         CNC_DoReturn();
         return MENU_QUIT;

      default:
         if((ii - '0') > 0)   /* Normal */
         {
            if(isdigit(ii))
            {
               sprintf(z->ABuffer, cm[190], ii - '0');
               CNC_PutA();
               return (ii - '1');
            }
         }
         else
            return MENU_NONE;
   }
   return 0;
} /* do_notebook_menu */

void do_lynx(char *url)
{
   BPTR  fp;

   char  buff [ 15 ] = "";

   chdir(UCfg.HomeDir);

   sprintf(buff, "T:ALynx.%d", z->InPort);

   if(fp = Open(buff, MODE_NEWFILE))
   {
      FPrintf(fp, "Set HOME %s\n", UCfg.HomeDir);
      FPrintf(fp, "Set LINES %ld\n", z->user1.TermLength);
      FPrintf(fp, "Set COLS %ld\n", z->user1.TermWidth);
      sprintf(z->ABuffer, cmdline[2], Config.URL);
      FPrintf(fp, z->ABuffer);
      Close(fp);
   }

   CNC_PutText(cm[110]);
   sprintf(z->ABuffer, "#4 Execute %s}", buff);
   CNC_PutA();

   DeleteFile(buff);
} /* do_lynx */

void do_irc(void)
{
   char  nick   [  12 ] = "",
         server [  50 ] = "";

   int   c,
         site,
         start = 0;

   static short i = 0;

   BOOL  ii = TRUE;

   struct IRCEntry *work;
   struct IRCSite   ircs;

   BPTR  fp;

   CNC_PutText(cm[108]);
   CNC_DoReturn();

   if(!Config.irc)
   {
      if(!(Config.irc = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return;
      }

      if(fp = Open(CINSIRCCFG, MODE_OLDFILE))
      {
         CNC_DoReturn();
         CNC_DoReturn();
         while(FGets(fp, ircs.Site, sizeof(ircs.Site)) != NULL)
         {
            FGets(fp, ircs.Host, sizeof(ircs.Host));
            ircs.Site[strlen(ircs.Site) - 1] = 0;
            ircs.Host[strlen(ircs.Host) - 1] = 0;
            i++;
            CINS_AddIRCName(Config.irc, &ircs);
         }
         Close(fp);

         i--;

         sprintf(z->ABuffer, cm[112], Config.IRC);
         CNC_PutA();
      }
      else
      {
         strcpy(server, Config.IRC);
         ii = FALSE;
      }
   }

   CNC_ReadGraphics("CINS:Text/sys.IRC", FALSE);

   strcpy(nick, z->user1.UUCP);

   if(!*UCfg.IRCNick)
      do_irc_nick(nick);
   else
      strcpy(nick, UCfg.IRCNick);

   while(ii)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, start + c);
         if(work && (work->irc_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[111], c+1, work->irc_Site.Site, work->irc_Site.Host);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[195]);
         else
            CNC_PutText(cm[194]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[53]);
      else
         CNC_PutText(cm[54]);

      site = do_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_NONE:
            strcpy(server, Config.IRC);
            ii = FALSE;
            break;

         case MENU_NOTE:
            ii = !do_irc_notebook(server);
            save_irc_notebook();
            break;

         case MENU_MAN:
            do_irc_manual(server);
            ii = FALSE;
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            CINS_FreeList(&Config.irc);
            return;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct IRCEntry *)CINS_FindOrdinal(Config.irc, site);
            if(work && (work->irc_Node.ln_Succ != NULL))
            {
               strcpy(server, work->irc_Site.Host);
            }
            ii = FALSE;
            break;
      }
   }

   do_irc_exe(nick, server);

   CINS_FreeList(&Config.irc);
   Config.irc = NULL;
} /* do_irc */

void do_irc_exe(char *nick,char *server)
{
   sprintf(z->ABuffer, cmdline[5], server, nick, 
           z->user1.PName ? z->user1.Handle:z->user1.RealName, z->user1.UUCP);
   CNC_PutA();
} /* do_irc_exe */

void do_irc_nick(char *nick)
{
   strcpy(z->InBuffer, nick);

   CNC_EnterLine(10, ELINE_HANDLESPECIAL | ELINE_USEINBUFF | ELINE_INPUTBOX, cm[114]);
   strcpy(nick, z->InBuffer);

   checkcarrier();

   if(!*nick)
      strcpy(nick, z->user1.UUCP);
} /* do_irc_nick */

void do_irc_manual(char *server)
{
   CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[105]);
   strcpy(server, z->InBuffer);

   checkcarrier();

   if(!*server)
      strcpy(server, Config.IRC);

   return;
} /* do_irc_manual */

BOOL do_irc_notebook(char *server)
{
   struct IRCEntry *work;
   struct IRCSite   ircs;

   short c,
         site,
         start = 0;

   static short i = 0;

   char ii;

   BPTR fp;

   if(!UCfg.ircnl)
   {
      if(!(UCfg.ircnl = CINS_InitList()))
      {
         CNC_PutText(bm[600]);
         return FALSE;
      }

      sprintf(z->ABuffer, "%s/.IRC", UCfg.HomeDir);

      if(fp = Open(z->ABuffer, MODE_OLDFILE))
      {
         while(FGets(fp, ircs.Site, sizeof(ircs.Site)))
         {
            FGets(fp, ircs.Host, sizeof(ircs.Host));
            ircs.Site[strlen(ircs.Site)-1] = 0;
            ircs.Host[strlen(ircs.Host)-1] = 0;
            i++;
            CINS_AddIRCName(UCfg.ircnl, &ircs);
         }
         i--;
         Close(fp);
      }
   }

   sprintf(z->ABuffer, cm[175], cm[17]);
   CNC_PutA();

   while(TRUE)
   {
      for(c = 0; (c < 9) && ((start + c) < (i + 1)); c++)
      {
         work = (struct IRCEntry *)CINS_FindOrdinal(UCfg.ircnl, start + c);
         if(work && (work->irc_Node.ln_Succ != NULL))
         {
            sprintf(z->ABuffer, cm[111], c+1, work->irc_Site.Site, work->irc_Site.Host);
            CNC_PutA();
         }
      }

      if(!start)  /* starting at 0 */
      {
         if(c < 9)
            CNC_PutText(cm[197]);
         else
            CNC_PutText(cm[196]);
      }
      else if((c == 9) && ((start + c) != (i + 1)))
         CNC_PutText(cm[166]);
      else
         CNC_PutText(cm[167]);

      site = do_notebook_menu();

      CNC_DoReturn();

      switch(site)
      {
         case MENU_ADD:
            CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[169]);
            if(*z->InBuffer)
            {
               strcpy(ircs.Site, z->InBuffer);
               CNC_EnterLine(30, ELINE_NOLEADSPACE | ELINE_FILENAME, cm[170]);
               strcpy(ircs.Host, z->InBuffer);
               CINS_AddIRCName(UCfg.ircnl, &ircs);
               i++;
            }
            checkcarrier();
            break;

         case MENU_DELE:
            CNC_PutText(cm[163]);
            ii = CNC_OneKey();

            checkcarrier();

            site = (ii - '1') + start;

            if((site > -1) && site < (i + 1))
            {
               if(CINS_RemoveNode(UCfg.ircnl, start + site))
               {
                  sprintf(z->ABuffer, cm[164], ii - '0');
                  CNC_PutA();
                  i--;
               }
               else
               {
                  sprintf(z->ABuffer, cm[165], ii - '0');
                  CNC_PutA();
               }
            }
            break;

         case MENU_NONE:
            break;

         case MENU_PREV:
            if(start > 1)
               start -= 9;
            break;

         case MENU_NEXT:
            if(start <= (i-9))
               start += 9;
            break;

         case MENU_QUIT:
            return FALSE;

         default:
            site += start;
            if(site > i + 1)
               break;
            work = (struct IRCEntry *)CINS_FindOrdinal(UCfg.ircnl, site);
            if(work && (work->irc_Node.ln_Succ != NULL))
            {
               strcpy(server, work->irc_Site.Host);
               return TRUE;
            }
      }
   }
} /* do_irc_notebook */

void save_irc_notebook(void)
{
   BPTR fp;

   struct IRCEntry *work;

   sprintf(z->ABuffer, "%s/.IRC", UCfg.HomeDir);

   if(UCfg.ircnl)
   {
      work = (struct IRCEntry *)UCfg.ircnl->lh_Head;

      if(fp = Open(z->ABuffer, MODE_NEWFILE))
      {
         while(work && work->irc_Node.ln_Succ)
         {
            FPrintf(fp, "%s\n", work->irc_Site.Site);
            FPrintf(fp, "%s\n", work->irc_Site.Host);
            work = (struct IRCEntry *)work->irc_Node.ln_Succ;
         }
         Close(fp);
      }
   }
} /* save_irc_notebook */

void view_balance(void)
{
   float rate,
         spent,
         timespent;

   timespent = ((float)z->TimeOnLine - (float)UCfg.TimeEnter)/10;

   rate = (float)UCfg.uae.Rate/100;
   spent = timespent * rate;

   sprintf(z->ABuffer, cm[133], timespent);
   CNC_PutA();
   sprintf(z->ABuffer, cm[134], rate);
   CNC_PutA();
   sprintf(z->ABuffer, cm[135], spent/100);
   CNC_PutA();
} /* view_balance */


long check_dir_size(void)
{
   BPTR lock;

   long size;

   if(!(lock = Lock(UCfg.HomeDir, ACCESS_READ)))
   {
      sprintf(z->ABuffer, cm[109], IoErr());
      CNC_PutA();
      return 0;
   }

   size = DirectorySize(UCfg.HomeDir, lock);
   UnLock(lock);

   return size;
} /* check_dir_size */

void GetOut( void )
{
   int   i;

   float timespent,
         spent = 0;

   CNC_DoReturn();

   if(Config.archie)
      CINS_FreeList(&Config.archie);
   if(Config.ftp)
      CINS_FreeList(&Config.ftp);
   if(ftpel)
      CINS_FreeList(&ftpel);
   if(Config.irc)
      CINS_FreeList(&Config.irc);
   if(Config.telnet)
      CINS_FreeList(&Config.telnet);
   if(tnel)
      CINS_FreeList(&tnel);
   if(UCfg.tnnl)
      CINS_FreeList(&UCfg.tnnl);
   if(UCfg.ftpnl)
      CINS_FreeList(&UCfg.ftpnl);
   if(UCfg.ircnl)
      CINS_FreeList(&UCfg.ircnl);
   if(UCfg.arcnl)
      CINS_FreeList(&UCfg.arcnl);

   if(myTask && orig_window)   /* restore old windowptr */
   {
      myTask->pr_WindowPtr = orig_window;
   }

   if((UCfg.Muff == 1) && (z->MuffAll == 1))
   {
      z->MuffAll = FALSE;
   }

   for(i = 0; i < myp->HiPort + 1; i++)
   {
      z->IUCBits[i] = UCfg.IUC[i];
   }

   timespent = ((float)z->TimeOnLine - (float)UCfg.TimeEnter)/10;

   if(UCfg.uae.Rate)
   {
      spent = timespent * ((float)UCfg.uae.Rate/100);
      z->user1.Balance -= (int)spent;
      if(cm)
      {
         sprintf(z->ABuffer, cm[106], timespent, spent/100);
         CNC_PutA();
      }
   }

   if(CINSBase)
   {
      CloseLibrary(CINSBase);
      CINSBase = NULL;
   }

   if(newptr)
   {
      FreeVec(newptr);
      if(cm)
      {
         sprintf(z->ABuffer, cm[120], spent/100, timespent);
         WriteLog(z, Config.Log, z->ABuffer, NULL);
         FreeVec(cm);
      }
   }

   if(CNetBase)
   {
      CloseLibrary(CNetBase);
      CNetBase = NULL;
   }

   CNC_ShutDown(NULL);
   CNC_DisposeContext();
   CloseLibrary(CNetCBase);

   exit(0);
} /* GetOut */

void DirRead(char *dirname)
{
   short i = 1;

   LONG lock;

   if(!(lock = Lock(dirname, ACCESS_READ)))
   {
      sprintf(z->ABuffer, cm[109], IoErr());
      CNC_PutA();
      return;
   }

   if(Examine(lock, z->fib))
   {
      CNC_DoReturn();

      while(ExNext(lock, z->fib))
      {
        sprintf(z->ABuffer, cm[126], z->fib->fib_FileName);
        CNC_PutA();
        if(i++ == 4)
           CNC_DoReturn();
      }
      CNC_DoReturn();
   }

   UnLock(lock);
   return;
} /* DirRead */
