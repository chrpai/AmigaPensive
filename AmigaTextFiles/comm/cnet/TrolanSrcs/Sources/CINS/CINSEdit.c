/*
** CINS User Configuration Module
** by Kelly Cochran
** Last Updated: Fri Mar 26 14:16:11 1999
*/

#ifndef CINSEDIT_C
#define CINSEDIT_C
#endif

#include "CINS.h"
#include "CINSEdit.h"
#include "CINSVDE.h"

const char *version = VERTAG;

void GetOut( void );

struct MainPort *myp;
struct PortData *z;
struct Library      *CNetBase  = NULL;
struct Library      *CNetCBase = NULL;
struct CNetCContext *context   = NULL;
struct CINSCfg  Config;
struct CINSUCfg UCfg;

char **cm    = NULL;
char **bm;
char *newptr = NULL;

void main( int argc, char **argv )
{
   if(argc < 2 || !(CNetCBase = OpenLibrary("cnetc.library", 4L)))
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

   if(!(CNetBase = OpenLibrary( "cnet.library", 4L )))
      GetOut();

   CINEdit_Main();
}

void CINEdit_Main(void)
{
   long  tmp1;

   setup();

   CNC_SetDoing(cm[10]);

   read_ucfg();

   if(z->user1.ANSI == 2)
   {
      if(write_user_VDE())
      {
         tmp1 = (long)z->uz[1];  /* Save old z->uz[x] values */

         z->uz[ 1 ] = TRUE;     /* Ghost for Expansion  */

         if(CNC_VisualDataEditor("CINS/UCfg",&UCfg,sizeof(struct CINSUCfg)))
         {
            write_netrc();
            write_ucfg();
         }

         z->uz[ 1 ] = tmp1;   /* Restore old z->uz[x] values   */
      }
      else
      {
         CNC_PutText("Unable to write VDE!\n");
      }
      GetOut();
   }

   command_loop();
}

void command_loop(void)
{
   char  ii;

   while(TRUE)
   {
      CNC_ReadGraphics("CINS:Menus/sys.Edit",FALSE);

      sprintf(z->ABuffer,cm[160],cm[153+UCfg.Shell],cm[158+UCfg.DirANSI]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[161],cm[153+UCfg.Muff],UCfg.IRCNick);
      CNC_PutA();

      CNC_PutText(cm[156]);

      ii = CNC_OneKey();

      checkcarrier();

      switch(ii)
      {
         case 'D':
            UCfg.DirANSI = CNC_PutQ(cm[132]);
            break;

         case 'F':
            edit_ncftp();
            break;

         case 'I':
            do_irc_nick();
            break;

         case 'M':
            UCfg.Muff = CNC_PutQ(cm[129]);
            break;

         case 'Q':
         {
            CNC_PutText(bm[2]);
            CNC_DoReturn();
            write_netrc();
            write_ucfg();
            GetOut();
         }

         case 'S':
            UCfg.Shell = CNC_PutQ(cm[128]);
            break;
      }

      CNC_DoReturn();
      CNC_DoReturn();
   }
}

void setup(void)
{
   short i,
         c,
         lines = 0,
         filesize;

   char *tmp;

   BPTR  fp,
         lock;

   memset(&Config, 0, sizeof(struct CINSCfg));
   memset(&UCfg  , 0, sizeof(struct CINSUCfg));

   read_config();

   if(fp = Open(CINSTXT, MODE_OLDFILE))
   {
      if(!(ExamineFH(fp, z->fib)))
      {
         CNC_PutText("n1Cannot find "CINSTXT"n1");
         Close(fp);
         GetOut();
      }

      filesize = z->fib->fib_Size;

      if((newptr = (char *) AllocVec(filesize, MEMF_CLEAR)))
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
         cm = (char **) AllocVec((lines + 1) * sizeof(char*), MEMF_CLEAR);

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

   checkcarrier();

   if(z->user1.Access < Config.Access)
   {
      CNC_PutText(cm[188]);
      GetOut();
   }

   sprintf(z->ABuffer, "Mail:Users/%s/.netrc", z->user1.UUCP);

   sprintf(UCfg.HomeDir, Config.UserDir, z->user1.UUCP);

   if(!FileExists(z->ABuffer))
   {
      new_user();
   }

   read_netrc();

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
}

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
}

void read_ucfg(void)
{
   BPTR  fp;

   char buff [ 64 ] = "";

   sprintf(z->ABuffer, CINSUCFGPAT, z->user1.UUCP);

   if(fp = Open(z->ABuffer, MODE_OLDFILE))
   {
      FGets(fp, buff, 64);
      UCfg.Shell = atoi(buff);
      buff[0] = 0;
      FGets(fp, buff, 64);
      UCfg.Muff = atoi(buff);
      buff[0] = 0;
      FGets(fp, buff, 64);
      UCfg.DirANSI = atoi(buff);
      buff[0] = 0;
      FGets(fp, buff, 64);
      strcpy(UCfg.IRCNick, buff);
      Close(fp);
   }
   else
   {
      UCfg.Shell   = Config.def_Shell;
      UCfg.DirANSI = Config.def_DirANSI;
      UCfg.Muff    = Config.def_Muff;
   }

   if(!*UCfg.IRCNick)
   {
      strcpy(UCfg.IRCNick,z->user1.UUCP);
   }
}

void write_ucfg(void)
{
   BPTR  fp;

   sprintf(z->ABuffer, CINSUCFGPAT, z->user1.UUCP);

   if(fp = Open(z->ABuffer,MODE_NEWFILE))
   {
      FPrintf(fp, "%ld\n", UCfg.Shell);
      FPrintf(fp, "%ld\n", UCfg.Muff);
      FPrintf(fp, "%ld\n", UCfg.DirANSI);
      FPrintf(fp, "%s", UCfg.IRCNick);
      Close(fp);
   }
}

void write_netrc(void)
{
   BPTR  fp;

   sprintf(z->ABuffer, "Mail:Users/%s/.netrc", z->user1.UUCP);

   if(fp = Open(z->ABuffer, MODE_NEWFILE))
   {
      FPrintf(fp, "%s\n", cm[33]);                   /* logfile */
      sprintf(z->ABuffer, "%s\n", cm[34]);           /* progress */
      FPrintf(fp, z->ABuffer, UCfg.ftp.Progress);
      FPrintf(fp, "#set local-dir %s\n", UCfg.HomeDir);  /* local dir */
      sprintf(z->ABuffer, "%s\n", cm[35]);            /* ansi-esc */
      FPrintf(fp, z->ABuffer, UCfg.ftp.ANSIEsc);
      sprintf(z->ABuffer, "%s\n", cm[36]);           /* auto-bin */
      FPrintf(fp, z->ABuffer, UCfg.ftp.AutoBin);
      FPrintf(fp, "%s\n", cm[37]);             /* logsize  */
      sprintf(z->ABuffer, "%s\n", cm[38]);           /* verbose  */
      FPrintf(fp, z->ABuffer, UCfg.ftp.Verbose);
      sprintf(z->ABuffer, "%s\n", cm[39]);           /* anon-open */
      FPrintf(fp, z->ABuffer, UCfg.ftp.AutoAnon);
      FPrintf(fp, "#set anon-password %s@%s\n", z->user1.UUCP, myp->gc.MyUUCPName);
      sprintf(z->ABuffer, "%s\n", cm[40]);           /* recent */
      FPrintf(fp, z->ABuffer, UCfg.ftp.Recent);
      sprintf(z->ABuffer, "%s\n", cm[41]);           /* tips */
      FPrintf(fp, z->ABuffer, UCfg.ftp.Tips);
      sprintf(z->ABuffer, "%s\n", cm[42]);           /* prompt */
      FPrintf(fp, z->ABuffer, cm[43]);
      sprintf(z->ABuffer, "%s\n", cm[44]);           /* mget prompting */
      FPrintf(fp, z->ABuffer, UCfg.ftp.MGet);
      Close(fp);
   }
}

void do_irc_nick(void)
{
   if(*UCfg.IRCNick)
   {
      strcpy(z->InBuffer,UCfg.IRCNick);
   }
   else
   {
      strcpy(z->InBuffer,z->user1.UUCP);
   }

   CNC_EnterLine(9,ELINE_HANDLESPECIAL | ELINE_USEINBUFF | ELINE_INPUTBOX,cm[114]);
   strcpy(UCfg.IRCNick,z->InBuffer);

   checkcarrier();

   if(!*UCfg.IRCNick)
   {
      strcpy(UCfg.IRCNick,z->user1.UUCP);
   }
}

void read_netrc(void)
{
   BPTR fp;

   char buf1 [ 256 ] = "",
        buf2 [ 256 ] = "";

   sprintf(buf1,"Mail:Users/%s/.netrc",z->user1.UUCP);

   if(fp = Open(buf1,MODE_OLDFILE))
   {
      FGets(fp,buf1,256);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,23,1);
      UCfg.ftp.Progress = atoi(buf2);

      FGets(fp,buf1,256);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,19,1);
      UCfg.ftp.ANSIEsc = atoi(buf2);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,18,1);
      UCfg.ftp.AutoBin = atoi(buf2);

      FGets(fp,buf1,256);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,14,1);
      UCfg.ftp.Verbose = atoi(buf2);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,16,1);
      UCfg.ftp.AutoAnon = atoi(buf2);

      FGets(fp,buf1,256);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,18,1);
      UCfg.ftp.Recent = atoi(buf2);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,11,1);
      UCfg.ftp.Tips = atoi(buf2);

      FGets(fp,buf1,256);

      FGets(fp,buf1,256);
      strmid(buf1,buf2,14,1);
      UCfg.ftp.MGet = atoi(buf2);

      Close(fp);
   }
}

void edit_ncftp(void)
{
   char  ii;

   if(UCfg.ftp.Recent > 1)
      UCfg.ftp.Recent = 1;

   if(UCfg.ftp.Tips > 1)
      UCfg.ftp.Tips = 1;

   if(UCfg.ftp.MGet > 1)
      UCfg.ftp.MGet = 1;

   if(UCfg.ftp.Recent > 1)
      UCfg.ftp.Recent = 1;

   CNC_PutText(cm[108]);

   while(1)
   {
      sprintf(z->ABuffer,cm[144],bm[Config.Line+UCfg.ftp.Progress]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[145],cm[153+UCfg.ftp.ANSIEsc]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[146],cm[153+UCfg.ftp.AutoBin]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[147],bm[Config.Line+5+UCfg.ftp.Verbose]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[148],cm[153+UCfg.ftp.AutoAnon]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[149],cm[153+UCfg.ftp.Recent]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[150],cm[153+UCfg.ftp.Tips]);
      CNC_PutA();
      sprintf(z->ABuffer,cm[151],cm[153+UCfg.ftp.MGet]);
      CNC_PutA();

      CNC_DoReturn();
      CNC_DoReturn();

      CNC_PutText(cm[156]);

      ii = CNC_OneKey();

      checkcarrier();

      switch(ii)
      {
         case 'Q':
            if(!CNC_PutQ(cm[157]))
               return;

            write_netrc();
            return;

         case '1':
            UCfg.ftp.Progress++;
            if(UCfg.ftp.Progress > 3)
               UCfg.ftp.Progress = 0;
            break;

         case '2':
            UCfg.ftp.ANSIEsc ? UCfg.ftp.ANSIEsc--:UCfg.ftp.ANSIEsc++;
            break;

         case '3':
            UCfg.ftp.AutoBin ? UCfg.ftp.AutoBin--:UCfg.ftp.AutoBin++;
            break;

         case '4':
            UCfg.ftp.Verbose++;
            if(UCfg.ftp.Verbose > 2)
               UCfg.ftp.Verbose = 0;
            break;

         case '5':
            UCfg.ftp.AutoAnon ? UCfg.ftp.AutoAnon--:UCfg.ftp.AutoAnon++;
            break;

         case '6':
            UCfg.ftp.Recent ? UCfg.ftp.Recent--:UCfg.ftp.Recent++;
            break;

         case '7':
            UCfg.ftp.Tips ? UCfg.ftp.Tips--:UCfg.ftp.Tips++;
            break;

         case '8':
            UCfg.ftp.MGet ? UCfg.ftp.MGet--:UCfg.ftp.MGet++;
            break;
      }

      CNC_PutText(cm[0]);
   }
}

BOOL write_user_VDE(void)
{
   BPTR fp;

   int i;

   for(i = 2400; bm[i]; i++)  /* only safe as long as bm[max_lines] == NULL */
   {
      if(strstr(bm[i], "CINS Stuff}"))
      {
         Config.Line = i+1;
         break;
      }
   }

   if(fp = Open("SysText:VDE/CINS/UCfg", MODE_NEWFILE))
   {
      UserConfig[  9 ].min = Config.Line;
      UserConfig[ 12 ].min = Config.Line + 5;
      FWrite(fp, (char*)UserConfig, sizeof(struct VDEentry), VDE_SIZE(UserConfig));
      Close(fp);
   }
   else
      return FALSE;
   return TRUE;
}

void GetOut( void )
{
   if(newptr)
   {
      FreeVec(newptr);
      if(cm)
      {
         FreeVec(cm);
      }
   }

   if(CNetBase)
   {
      CloseLibrary(CNetBase);
   }

   CNC_ShutDown(NULL);
   CNC_DisposeContext();
   CloseLibrary(CNetCBase);
   exit(0);
}
