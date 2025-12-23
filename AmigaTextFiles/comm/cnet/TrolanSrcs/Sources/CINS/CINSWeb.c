/*
** CINS Web Directory Management Module
** by Kelly Cochran
** Last Updated: Fri Mar 26 14:19:49 1999
*/

#ifndef CINSWEB_C
#define CINSWEB_C
#endif

#include "CINS.h"
#include "CINSWeb.h"

const char *version = VERTAG;

void GetOut( void );

struct MainPort *myp;  /* Pointer to CNet port--ALL info!   */
struct PortData *z;
struct Library      *CNetBase   = NULL;
struct Library      *CINSBase   = NULL;
struct Library      *CNetCBase  = NULL;
struct CNetCContext *context    = NULL;
struct CINSCfg   Config;
struct CINSUCfg  UCfg;
struct List     *dellist = NULL;

char     **bm;
char     **cm;
char     *newptr = NULL;

void main( int argc, char **argv )
{
   if((argc < 2) || !(CNetCBase = OpenLibrary("cnetc.library", 4L)))
   {
      Printf("Unable to setup CNetC library!\n");
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

   if( !(CNetBase = OpenLibrary( "cnet.library", 4L )) )
   {
      GetOut();
   }

   if(!(CINSBase = OpenLibrary(CINSLIBNAME, 4)))
   {
      CloseLibrary(CNetBase);
      GetOut();
   }

   /* put your program here */

   CINSWeb_Main();
}

void CINSWeb_Main(void)
{
   setup();

   CNC_SetDoing(cm[28]);

   if(UCfg.uae.WebFree && (check_dir_size() > UCfg.uae.WebFree))
   {
      CNC_PutText(cm[122]);
   }

   if(!UCfg.DirANSI)
   {
      CNC_ReadGraphics("CINS:Menus/sys.WebDir", FALSE);
   }
   else
   {
      ANSI_loop();
   }

   command_loop();
}

void command_loop(void)
{
   BPTR fp;

   char buff [ 256 ] = "",
        cbuf [ 128 ] = "",
        file [  64 ] = "";

   short i;

   while(TRUE)
   {
      getcwd(cbuf, 128);

      sprintf(z->ABuffer, cm[5], cm[28]);

      CNC_EnterLine(60, ELINE_NOLEADSPACE, z->ABuffer);

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
      {
         i = CNC_FindCommand(CINSWEB_MENU);
      }

      if(!i)
      {
         if(*z->pitem[0])
         {
            sprintf(z->ABuffer, bm[310], z->pitem[0]);
            CNC_PutA();
         }
         continue;
      }

      ParseTrash(z->pitem[0]);
      ParseTrash(z->pitem[1]);

      switch(i)
      {
         case CINSWEB_QUIT:  /* Quit     */
            GetOut();

         case CINSWEB_HELP:  /* ?        */
            CNC_ReadFile("CINS:Menu/CINSWeb", TRUE);
            break;

         case CINSWEB_LIST:  /* List     */
            if(z->npitems)
            {
               sprintf(buff, "%s%s", z->pitem[0], z->pitem[1]);

               if(strchr(buff, '*') != NULL)
               {
                  pat_match(buff, PAT_LIST);
                  break;
               }
            }

            DirRead(2);
            break;

         case CINSWEB_DELETE:  /* DELete   */
            if(z->npitems)
            {
               sprintf(file, "%s%s", z->pitem[0], z->pitem[1]);
               sprintf(buff, "%s/%s", cbuf, file);
            }
            else
            {
               break;
            }

            if(strchr(buff, '*') != NULL)
            {
               sprintf(buff, "%s%s", z->pitem[0], z->pitem[1]);
               pat_match(buff, PAT_DEL);
               break;
            }

            if((!FileExists(buff)) || (*z->pitem[0] == '.'))
            {
               sprintf(z->ABuffer, bm[1066], buff);
               CNC_PutA();
               break;
            }

            CNC_DoReturn();
            QueryDelete(buff);
            break;

         case CINSWEB_EDIT:  /* Edit     */
            if(z->npitems)
            {
               sprintf(file, "%s%s", z->pitem[0], z->pitem[1]);
               sprintf(buff, "%s/%s", cbuf, file);
            }
            else
            {
               break;
            }

            z->vis_subj = file;

            if(fp = Open(buff, MODE_OLDFILE))
            {
               PrepEditor(fp);
               Close(fp);
               CNC_CallEditor(z->user1.MyPrivs.EditorLines, TRUE);
            }
            else
            {
               CNC_CallEditor(z->user1.MyPrivs.EditorLines, FALSE);
            }

            if(z->edbuff > 0)
            {
               if(fp = Open(buff, MODE_NEWFILE))
               {
                  SaveEditor(fp, TRUE);
                  Close(fp);
               }
            }

            break;

         case CINSWEB_READ:  /* Read     */
            sprintf(buff, "%s/%s%s", cbuf, z->pitem[0], z->pitem[1]);

            if((!FileExists(buff)) || (*z->pitem[0] == '.'))
            {
               sprintf(z->ABuffer, bm[1066], buff);
               CNC_PutA();
               break;
            }

            CNC_ReadFile(buff, FALSE);
            break;

         case CINSWEB_TAG:  /* *        */
            sprintf(buff, "%s/%s%s", cbuf, z->pitem[0], z->pitem[1]);

            if(strchr(buff, '*') != NULL)
            {
               sprintf(buff, "%s%s", z->pitem[0], z->pitem[1]);
               pat_match(buff, PAT_TAG);
               break;
            }

            sprintf(buff, "%s/%s%s", cbuf, z->pitem[0], z->pitem[1]);
            CNC_SelectAndDownload(buff, FALSE);
            break;

         case CINSWEB_DL:  /* DOwnload */
            sprintf(buff, "%s/%s%s", cbuf, z->pitem[0], z->pitem[1]);

            if(strchr(buff, '*') != NULL)
            {
               sprintf(buff, "%s%s", z->pitem[0], z->pitem[1]);
               pat_match(buff, PAT_DL);
               break;
            }

            CNC_SelectAndDownload(buff, TRUE);
            break;

         case CINSWEB_UL:  /* UPload   */
            if(!(UCfg.uae.Flags & CINSAEF_UPLOADWEB))
            {
               CNC_PutText(cm[125]);
               break;
            }

            if(!z->npitems)
            {
               sprintf(buff, "%s/", cbuf);
            }
            else
            {
               sprintf(buff, "%s/%s%s", cbuf, z->pitem[0], z->pitem[1]);
            }

            CNC_ExtSetMinFree(100000);
            CNC_ExtSetProtocol(NULL);
            CNC_ExtUpload(buff);
            break;

         case CINSWEB_DIR:  /* DIR      */
            if(z->npitems)
            {
               sprintf(buff, "%s%s", z->pitem[0], z->pitem[1]);

               if(strchr(buff, '*') != NULL)
               {
                  pat_match(buff, PAT_DIR);
                  break;
               }
            }

            DirRead(1);
            break;

         case CINSWEB_CD: /* CD       */
            if(!(UCfg.uae.Flags & CINSAEF_CD))
            {
               sprintf(z->ABuffer, bm[310], z->pitem[0]);
               CNC_PutA();
               break;
            }
            sprintf(buff, "%s%s", z->pitem[0], z->pitem[1]);
            do_cd(buff);
            break;

         default:
            if(*z->pitem[0])
            {
               sprintf(z->ABuffer, bm[310], z->pitem[0]);
               CNC_PutA();
            }
            continue;
      }
   }
}

void ANSI_loop(void)
{
   BPTR fp;

   short start = 0,
         max,
         c;
   static short i;

   char  ii;

   char buff [ 64 ] = "";

   struct DirEntryList *work = NULL;

   LONG size = 0;

   if(!(dellist = CINS_InitList()))
   {
      CNC_PutText(bm[600]);
      return;
   }

   max = ReadDirEntries(&size, dellist);

   while(TRUE)
   {
      CNC_PutText(cm[108]);

      if(size > 1048576)
      {
         sprintf(buff, cm[185], (float)size/1048576);
      }
      else if(size > 1024)
      {
         sprintf(buff, cm[184], (float)size/1024);
      }
      else
      {
         sprintf(buff, cm[183], size);
      }

      sprintf(z->ABuffer, cm[121], buff);
      CNC_PutA();

      for(i = 0; (i < 9) && ((start + 1) < (max)); i++)
      {
         work = (struct DirEntryList *)CINS_FindOrdinal(dellist, start + i);
         if(work && (work->del_Node.ln_Succ))
         {
            sprintf(z->ABuffer, cm[141], i+1, work->del_DEN.FileName, work->del_DEN.Size, work->del_DEN.Date);
            CNC_PutA();
         }
      }

      CNC_ReadGraphics("CINS:Menu/sys.CINSWebANSI",FALSE);

      CNC_PutText(cm[142]);

      ii = CNC_OneKey();

      checkcarrier();

      switch(ii)
      {
         case '*':   /* TAG */
            CNC_PutText(cm[137]);

            ii = CNC_OneKey();
            checkcarrier();
            if(ii == '*')
            {
               sprintf(z->ABuffer, "%c", ii);
               CNC_PutA();
               for(c = start;(c < start + 9) && (c < max); c++)
               {
                  work = (struct DirEntryList *)CINS_FindOrdinal(dellist, c);
                  if(work)
                  {
                     if(!CNC_SelectAndDownload(work->del_DEN.FNReal, FALSE))
                     {
                        size -= work->del_DEN.Size;
                        Remove((struct Node *)work);
                        FreeVec(work);
                        max--;
                     }
                  }
               }
               break;
            }
            if(isdigit(ii))
            {
               sprintf(z->ABuffer, "%c", ii);
               CNC_PutA();
               ii = (start + (ii - '0'));
               work = (struct DirEntryList *)CINS_FindOrdinal(dellist, ii - 1);
               if(work)
               {
                  if(!CNC_SelectAndDownload(work->del_DEN.FNReal, FALSE))
                  {
                     size -= work->del_DEN.Size;
                     Remove((struct Node *)work);
                     FreeVec(work);
                     max--;
                  }
               }
            }
            break;

         case 'D':   /* Download */
            CNC_PutText(cm[138]);

            ii = CNC_OneKey();
            checkcarrier();

            if(isdigit(ii))
            {
               sprintf(z->ABuffer, "%c", ii);
               CNC_PutA();
               ii = (start + (ii - '0'));
               work = (struct DirEntryList *)CINS_FindOrdinal(dellist, ii - 1);
               if(work)
               {
                  if(!CNC_SelectAndDownload(work->del_DEN.FNReal, TRUE))
                  {
                     size -= work->del_DEN.Size;
                     Remove((struct Node *)work);
                     FreeVec(work);
                     max--;
                  }
               }
            }
            break;

         case 'E':   /* Edit */
            CNC_PutText(cm[136]);

            ii = CNC_OneKey();
            checkcarrier();

            if(ii == 'N')  /* new file */
            {
               struct DirEntryNode den;
               BOOL  dupe = FALSE;

               CNC_PutText(cm[179]);
               memset(&den, 0, sizeof(struct DirEntryNode));
               CNC_EnterLine(64, ELINE_NOOLMS, cm[178]);
               checkcarrier();
               if(!*z->InBuffer)
               {
                  break;
               }
               ParseTrash(z->InBuffer);
               strcpy(den.FileName, z->InBuffer);
               sprintf(den.FNReal, "%s/%s", UCfg.WebDir, den.FileName);
               CNC_MakeDate(&z->Today, den.Date);
               for(c = 0; c < max; c++)
               {
                  work = (struct DirEntryList *)CINS_FindOrdinal(dellist, c);
                  if(!stricmp(work->del_Node.ln_Name, den.FileName))
                  {
                     dupe = TRUE;
                     break;
                  }
               }
               if(!dupe)
               {
                  CINS_AddDirEntryName(dellist, &den);
                  max++;
                  work = (struct DirEntryList *)CINS_FindOrdinal(dellist, max - 1);
               }
            }
            else if(isdigit(ii)) /* old file */
            {
               sprintf(z->ABuffer, "%c", ii);
               CNC_PutA();
               ii = (start + (ii - '0'));
               work = (struct DirEntryList *)CINS_FindOrdinal(dellist, ii - 1);
            }
            if(work) /* valid? */
            {
               z->vis_subj = work->del_DEN.FileName;
               if(fp = Open(work->del_DEN.FNReal, MODE_OLDFILE))
               {
                  PrepEditor(fp);
                  Close(fp);
                  CNC_CallEditor(z->user1.MyPrivs.EditorLines, TRUE);
               }
               else
               {
                  CNC_CallEditor(z->user1.MyPrivs.EditorLines, FALSE);
               }

               if(z->edbuff > 0) /* if editor !empty */
               {
                  if(fp = Open(work->del_DEN.FNReal, MODE_NEWFILE))
                  {
                     SaveEditor(fp, TRUE);
                     Close(fp);
                     size -= work->del_DEN.Size;   /* update filesize data */
                     work->del_DEN.Size = FileSize(work->del_DEN.FNReal);
                     CNC_MakeDate(&z->Today, work->del_DEN.Date); /* fix date */
                     size += work->del_DEN.Size;
                  }
               }
            }
            break;

         case 'K':   /* Kill/Delete */
            CNC_PutText(cm[139]);

            ii = CNC_OneKey();
            checkcarrier();

            if(isdigit(ii))
            {
               sprintf(z->ABuffer, "%c", ii);
               CNC_PutA();
               ii = (start + (ii - '0'));
               work = (struct DirEntryList *)CINS_FindOrdinal(dellist, ii - 1);
               if(work)
               {
                  sprintf(z->ABuffer, cm[143], work->del_DEN.FileName);
                  if(CNC_PutQ(z->ABuffer))
                  {
                     DeleteFile(work->del_DEN.FNReal);
                     size -= work->del_DEN.Size;
                     Remove((struct Node *)work);
                     FreeVec(work);
                     max--;
                  }
               }
            }
            break;

         case 'N':   /* Next */
            if(start < (max-9))
               start += 9;
            break;

         case 'P':   /* Previous */
            if(start > 1)
               start -= 9;
            break;

         case 'Q':   /* Quit */
            CNC_PutText(bm[2]);
            CNC_DoReturn();
            GetOut();

         case 'R':   /* Read */
            CNC_PutText(cm[140]);

            ii = CNC_OneKey();
            checkcarrier();

            if(isdigit(ii))
            {
               sprintf(z->ABuffer, "%c", ii);
               CNC_PutA();
               ii = (start + (ii - '0'));
               CNC_DoReturn();
               work = (struct DirEntryList *)CINS_FindOrdinal(dellist, ii - 1);
               if(work)
               {
                  if(!CNC_ReadFile(work->del_DEN.FNReal, FALSE))
                  {
                     size -= work->del_DEN.Size;
                     Remove((struct Node *)work);
                     FreeVec(work);
                     max--;
                  }
               }
               CNC_DoReturn();
               CNC_PutText(bm[1591]);
            }
            break;

         case 'U':   /* Upload */
            if(!(UCfg.uae.Flags & CINSAEF_UPLOADWEB))
            {
               CNC_PutText(cm[125]);
               break;
            }
            sprintf(z->ABuffer, "%s/", UCfg.WebDir);         
            CNC_ExtSetMinFree(100000);
            CNC_ExtSetProtocol(NULL);
            CNC_ExtUpload(z->ABuffer);
            break;

         default:    /* Anything else */
            continue;
      }
   }
}

int ReadDirEntries(LONG *size, struct List *list)
{
   BPTR  lock;

   int i = 0;

   struct IsDate        CDate;
   struct DirEntryNode  den;

   memset(&CDate, 0, sizeof(struct IsDate));
   memset(&den, 0, sizeof(struct DirEntryNode));

   if(lock = Lock(UCfg.WebDir, ACCESS_READ))
   {
      if(Examine(lock, z->fib))
      {
         while(ExNext(lock,z->fib))
         {
            if(*z->fib->fib_FileName == '.' || (z->fib->fib_DirEntryType > 0))
            {
               continue;
            }
            FileDate(&z->fib->fib_Date, &CDate);
            CNC_MakeDate(&CDate, den.Date);
            sprintf(den.FNReal, "%s/%s", UCfg.WebDir, z->fib->fib_FileName);
            sprintf(den.FileName, z->fib->fib_FileName);
            den.Size = z->fib->fib_Size;
            CINS_AddDirEntryName(list, &den);
            *size += z->fib->fib_Size;
            i++;
         }
      }
      UnLock(lock);
   }
   return i;
}

void setup(void)
{
   BPTR fp;

   char *tmp;
   short i,
         c,
         lines = 0;

   long filesize;

   memset(&Config, 0, sizeof(struct CINSCfg));
   memset(&UCfg  , 0, sizeof(struct CINSUCfg));

   read_config();

   if(fp = Open(CINSTXT, MODE_OLDFILE))
   {
      if(!(ExamineFH(fp, z->fib)))
      {
         CNC_PutText("n1Cannot find "CINSTXT"n1");
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
         for(c = 0;c < lines; tmp++)
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

   check_user_prefs();

   check_web_dir();

   checkcarrier();

   if(z->user1.Access < Config.Access)
   {
      CNC_PutText(cm[118]);
      GetOut();
   }

   sprintf(UCfg.HomeDir, Config.UserDir, z->user1.UUCP);

   if(chdir(UCfg.WebDir) != 0)
   {
      CNC_PutText(cm[117]);
      GetOut();
   }

   getcwd(UCfg.WebDir, 64);
}

void check_user_prefs(void)
{
   BPTR  fp;

   char buff [ 64 ] = "";

   sprintf(z->ABuffer, CINSUCFGPAT, z->user1.UUCP);

   if(fp = Open(z->ABuffer, MODE_OLDFILE))
   {
      FGets(fp, buff, sizeof(buff));
      UCfg.Shell = *buff - '0';
      buff[0] = 0;
      FGets(fp, buff, sizeof(buff));
      UCfg.Muff  = *buff - '0';
      buff[0] = 0;
      FGets(fp, buff, sizeof(buff));
      UCfg.DirANSI = *buff - '0';
      buff[0] = 0;
      FGets(fp, buff, sizeof(buff));
      strcpy(UCfg.IRCNick, buff);
      Close(fp);
   }

   sprintf(z->ABuffer, CINSWEBLIMITPAT, z->user1.UUCP);

   if(fp = Open(z->ABuffer, MODE_OLDFILE))
   {
      FGets(fp, buff, sizeof(buff));
      UCfg.uae.WebFree = atol(buff);
      Close(fp);
   }
}

void check_web_dir(void)
{
   BPTR fp;

   char buff [ 256 ] = "";

   *z->ABuffer = '\1';

   if(fp = Open(CINSWEB, MODE_OLDFILE))
   {
      while(FGets(fp, buff, sizeof(buff)))
      {
         buff[strlen(buff) - 1] = '\0';
         sprintf(z->ABuffer, buff, z->user1.UUCP);
         if(FileExists(z->ABuffer))
         {
            strcpy(UCfg.WebDir, z->ABuffer);
            *z->ABuffer = '\0';
            break;
         }
      }
      Close(fp);
   }

   if(*z->ABuffer)
   {
      if(myp->s1 < 5010)
      {
         sprintf(z->ABuffer, CINSDEFWEBDIRPAT, z->user1.UUCP);
      }
      else
      {
         sprintf(z->ABuffer, CINS5DEFWEBDIRPAT, z->user1.UUCP);
      }
      if(!FileExists(z->ABuffer))
      {
         if(fp = CreateDir(z->ABuffer))
            UnLock(fp);
      }
      sprintf(UCfg.WebDir, z->ABuffer);
   }
}

void do_cd(char *path)
{
   char  currdir [ 128 ] = "";

   int   i;

   i = strlen(UCfg.WebDir);

   getcwd(currdir, 128);
   strcat(currdir, "/");

   if(!strcmp(path, ".."))
   {
      strcat(currdir, "/");
   }
   else
   {
      strcat(currdir, path);
   }

   if(currdir[i+1] == '/')
   {
      CNC_PutText(cm[123]);
      return;
   }

   if(chdir(currdir) != 0)
   {
      CNC_PutText(cm[123]);
   }
   else
   {
      sprintf(z->ABuffer, bm[1765], getcwd(currdir, 128));
      CNC_PutA();
   }
}

void DirRead(short type)
{
   char buff [ 90 ] = "",
        tbuf [ 60 ] = "";

   short i = 1;

   LONG lock;
   struct IsDate  CDate;

   memset(&CDate, 0, sizeof(struct IsDate));

   if(!(lock = Lock(getcwd(buff, 90), ACCESS_READ)))
   {
      sprintf(z->ABuffer, cm[109], IoErr());
      CNC_PutA();
      return;
   }

   sprintf(z->ABuffer, bm[638], getcwd(buff,90));
   CNC_PutA();
   CNC_DoReturn();

   if(Examine(lock, z->fib))
   {
      switch(type)
      {
         case 1:
            while(ExNext(lock, z->fib))
            {
               if(*z->fib->fib_FileName == '.')
               {
                  continue;
               }
               sprintf(z->ABuffer,cm[126], z->fib->fib_FileName);
               CNC_PutA();
               if(i++ == 4)
               {
                  CNC_DoReturn();
               }
            }
            CNC_DoReturn();
            break;

         case 2:
            while(ExNext(lock, z->fib))
            {
               if(*z->fib->fib_FileName == '.')
               {
                  continue;
               }
               FileDate(&z->fib->fib_Date, &CDate);
               CNC_MakeDate(&CDate, tbuf);
               sprintf(z->ABuffer, bm[639], z->fib->fib_FileName);
               CNC_PutA();
               sprintf(z->ABuffer, z->fib->fib_DirEntryType > 0 ? bm[640]:bm[641], z->fib->fib_Size);
               CNC_PutA();
               CNC_PutText(tbuf);
               CNC_DoReturn();
            }
            break;
      }
   }

   UnLock(lock);
   return;
}

void pat_match(char *file, int action)
{
   char tbuf [ 90 ] = "";

   struct IsDate     CDate;

   long ioerr = 0;
   int  i     = 0;

   memset(&CDate, 0, sizeof(struct IsDate));

   MatchFirst(file, z->ap);

   CNC_DoReturn();

   while(!ioerr)
   {
      if((z->ap->ap_Info.fib_DirEntryType > 0) && ((action != PAT_LIST) && (action != PAT_DIR)))
      {
         MatchNext(z->ap);
         ioerr = IoErr();
         continue;
      }         

      switch(action)
      {
         case PAT_DEL:
            if(!QueryDelete(z->ap->ap_Info.fib_FileName))
            {
               return;
            }
            break;

         case PAT_DIR:
            if(*z->ap->ap_Info.fib_FileName == '.')
            {
               continue;
            }
            sprintf(z->ABuffer, cm[126], z->ap->ap_Info.fib_FileName);

            strcat(tbuf, z->ABuffer);
            if(i++ == 4)
            {
               CNC_PutText(tbuf);
               CNC_DoReturn();
               tbuf [ 0 ] = 0;
            }
            break;

         case PAT_DL:
            CNC_SelectAndDownload(z->ap->ap_Info.fib_FileName,TRUE);
            break;            

         case PAT_LIST:
            if(*z->ap->ap_Info.fib_FileName == '.')
            {
               continue;
            }
            FileDate(&z->ap->ap_Info.fib_Date, &CDate);
            CNC_MakeDate(&CDate, tbuf);
            sprintf(z->ABuffer, bm[639], z->ap->ap_Info.fib_FileName);
            CNC_PutA();
            sprintf(z->ABuffer, z->ap->ap_Info.fib_DirEntryType > 0 ? bm[640]:bm[641], z->ap->ap_Info.fib_Size);
            CNC_PutA();
            CNC_PutText(tbuf);
            CNC_DoReturn();
            break;

         case PAT_TAG:
            CNC_SelectAndDownload(z->ap->ap_Info.fib_FileName,FALSE);
            break;            
      }

      MatchNext(z->ap);

      ioerr = IoErr();
   }

   if(ioerr != ERROR_NO_MORE_ENTRIES)
   {
      CNC_PutText(bm[193]);
   }

   MatchEnd(z->ap);

   if((action == PAT_DIR) && (tbuf != NULL))
   {
      CNC_PutText(tbuf);
      CNC_DoReturn();
   }
}

int QueryDelete(char *file)
{
   char ii;

   if(z->DoQuick)
   {
      DeleteFile(file);
      sprintf(z->ABuffer, bm[364], file);
      CNC_PutA();
      return 1;
   }

   sprintf(z->ABuffer, cm[127], file);
   CNC_PutA();

   ii = CNC_OneKey();
         
   checkcarrier();

   if(ii == 'Q')
   {
      CNC_PutText(bm[2]);
      return 0;
   }
   else if(ii == 'Y')
   {
      CNC_PutText(bm[1]);
      DeleteFile(file);
   }
   else
   {
      CNC_PutText(bm[0]);
      return -1;
   }

   return 1;
}

void ParseTrash(char *arg)
{
   char *tmp = arg;

   for(; *tmp; tmp++)
   {
      if((*tmp == '/') || (*tmp == ':'))
      {
         *tmp = '\0';
      }
      tmp++;
   }
}      

long check_dir_size(void)
{
   BPTR lock;

   char buff [ 32 ] = "";

   long size;

   if(!(lock=Lock(UCfg.WebDir, ACCESS_READ)))
   {
      sprintf(z->ABuffer, cm[109], IoErr());
      CNC_PutA();
      return 0;
   }

   size = DirectorySize(UCfg.WebDir, lock);

   UnLock(lock);

   if(size > 1048576)
   {
      sprintf(buff, cm[185], (float)size/1048576);
   }
   else if(size > 1024)
   {
      sprintf(buff, cm[184], (float)size/1024);
   }
   else
   {
      sprintf(buff, cm[183], size);
   }

   sprintf(z->ABuffer, cm[121], buff);
   CNC_PutA();

   return size;
}

void GetOut( void )
{
   if(dellist)
   {
      CINS_FreeList(&dellist);
   }

   if(newptr)
   {
      FreeVec(newptr);
      if(cm)
      {
         FreeVec(cm);
      }
   }

   if(CINSBase)
   {
      CloseLibrary(CINSBase);
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
