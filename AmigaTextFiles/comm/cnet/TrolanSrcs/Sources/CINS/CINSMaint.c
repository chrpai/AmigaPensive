/*
** CINS Directory Maintenance Module
** by Kelly Cochran
** Last Updated: Fri Mar 26 13:33:51 1999
*/

#include "CINS.h"
#include "CINSMaint.h"

const char *version = VERTAG;

void dir_purge(void);
void file_purge(void);
void user_dir_check(char *dirname, int ag);
void read_config(void);
void usage(void);
void GetOut(void);

struct MainPort *myp      = NULL;
struct Library  *CNetBase = NULL;
struct CINSCfg  Config;

void main(void)
{
   struct RDArgs __aligned *myargs;

   struct MyArgs
   {
      ULONG dir;
      ULONG file;
   };

   struct MyArgs args;

   memset(&args  , 0, sizeof(struct MyArgs));
   memset(&Config, 0, sizeof(struct CINSCfg));

   if(!(CNetBase = OpenLibrary("cnet.library", 4)) )
      exit(0);

   Forbid();
   if (!(myp=(struct MainPort *)FindPort( "cnetport" )) )
   {
      Permit();
      Printf("Could not find CONTROL process!\r\n");
      GetOut();
   }
   Permit();

   read_config();

   if(myargs = ReadArgs("D=DIRPURGE/S,F=FILEPURGE/S", (LONG *)&args, NULL))
   {
      if(args.dir)
         dir_purge();

      if(args.file)
         file_purge();

      FreeArgs(myargs);
   }

   GetOut();
}


void dir_purge(void)
{
   Printf("Not yet implemented\n");
   return;
}

#if 0
   struct UDirEnt *ud;
   struct FileInfoBlock *fib;

   char buffer [ 256 ] = "";

   int   acchi,
         accnum,
         i,
         j;

   BPTR  lock;

   BOOL  found;

   acchi  = myp->Nums[0];           /* High account #             */
   accnum = myp->Nums[1];           /* Number of active accounts  */

   if(ud = AllocVec(sizeof(struct UDirEnt) * accnum, MEMF_CLEAR))
                                    /* Allocate RAM for array of UUCP ids */
   {
      for(i = 0, j = 0; i < acchi; i++)
      {
         if((*myp->Key[i].Handle != '!') || (*myp->Key[i].UUCP == '\0'))
                                    /* ! and NULL are not valid handles  =) */
         {
            strcpy(ud[j].UUCP, myp->Key[i].UUCP);   /* copy key value to array element */
            j++;                                   /* increment array value           */
         }
      }
   }
   else
   {
      Printf("Insufficient memory\n");
      return;
   }

   if(!(fib = AllocDosObject(DOS_FIB, NULL)))
   {                 /* Allocate a FIB */
      FreeVec(ud);
      Printf("Insufficient memory\n");
      return;
   }

   if(lock = Lock(Config.UserDir, ACCESS_READ))    /* Get a lock on the dir   */
   {
      if(Examine(lock,fib))                  /* pull a FIB from it      */
      {
         while(ExNext(lock,fib))             /* while I can pull a FIB  */
         {
            if(fib->fib_DirEntryType > 0)    /* if it's a directory     */
            {
               found = FALSE;                   /* not found yet           */

               Printf("Checking %s...", fib->fib_FileName);

               for(i = 0; i < j; i++)              /* loop thru array of ids  */
               {
                  if(ud[i].Found == TRUE)          /* we've already found this one  */
                     continue;

                  if(!stricmp(ud[i].UUCP, fib->fib_FileName)) /* we found it! */
                  {
                     ud[i].Found = TRUE;        /* keep from trying this one again  */
                     Printf("found.\n", ud[i].UUCP);
                     found = TRUE;              /* keep from saying we missed it */
                     break;                     /* break from the loop           */
                  }
               }
               if(found != TRUE)                /* didn't find a damn thing      */
               {
                  sprintf(buffer, "Delete %s/%s ALL FORCE", Config.UserDir, fib->fib_FileName);
                  Printf("SystemTags(buffer, TAG_END);");
               }
            }
         }
      }
      UnLock(lock);     /* unlock the directory */
   }

   FreeVec(ud);         /* free the array       */
   FreeDosObject(DOS_FIB, fib);        /* free the FIB         */

   return;
}
#endif /* commented for non-working factor */

void file_purge(void)
{
   long id;

   int ag;

   LONG lock;

   struct FileInfoBlock *UDirFIB;
   struct UserData      *udata;

   char  dirbuff  [ 128 ] = "",
         dirbuf2  [ 128 ] = "";

   if(!(UDirFIB = AllocDosObject(DOS_FIB, NULL)))
   {
      Printf("Not Enough memory\n");
      return;
   }

   for(id = 0; id < myp->Nums[0]; id++)
   {
      if(!(udata = LockAccount(id)))
         continue;

      sprintf(dirbuff, Config.UserDir, udata->UUCP);
      ag = udata->Access;
      UnLockAccount(id, FALSE);

      if(Config.AE[ag].PurgeDays)
      {
         if(lock = Lock(dirbuff, ACCESS_READ))
         {
            if(Examine(lock, UDirFIB))
            {
               while(ExNext(lock, UDirFIB))
               {
                  if(UDirFIB->fib_DirEntryType > 0)
                  {
                     strcpy(dirbuf2, dirbuff);
                     AddPart(dirbuf2, UDirFIB->fib_FileName, sizeof(dirbuf2));
                     user_dir_check(dirbuf2, ag);
                  }
               }
            }
            UnLock(lock);
         }
      }
   }

   FreeDosObject(DOS_FIB, UDirFIB);
}

void user_dir_check(char *dirname, int ag)
{
   struct DateStamp     *now;
   struct FileInfoBlock *UFIB;

   char  fname [ 256 ] = "";

   LONG  lock;

   if(!(UFIB = AllocDosObject(DOS_FIB, NULL)))
   {
      Printf("Not enough memory\n");
      return;
   }

   lock = Lock(dirname, ACCESS_READ);

   now = __timecvt(time(NULL));
   
   if(Examine(lock, UFIB))
   {
      while(ExNext(lock, UFIB))
      {
         if((CompareDates(&UFIB->fib_Date, now) > Config.AE[ag].PurgeDays) && (UFIB->fib_DirEntryType < 1) && (*UFIB->fib_FileName != '.'))
         {
            strcpy(fname, dirname);
            AddPart(fname, UFIB->fib_FileName, sizeof(fname));
            Printf("OLD FILE! Filename: %s\n", fname);
            DeleteFile(fname);
/*
            WriteLog(NULL, Config.Log, 
*/
         }
      }
   }

   FreeDosObject(DOS_FIB, UFIB);

   UnLock(lock);
}

void GetOut(void)
{
   if(CNetBase)
      CloseLibrary(CNetBase);

   if(Config.AE)
      FreeVec(Config.AE);

   exit(0);
}

void read_config(void)
{
   BPTR  fp;

   if((fp = Open(CINSCFG, MODE_OLDFILE)) && (Read(fp, &Config, sizeof(struct CINSCfg))))
   {
      Close(fp);
   }
   else
   {
      Printf("Could not open "CINSCFG" for read!\n");
      GetOut();
   }

   Config.AE = NULL;

   if(!(Config.AE = AllocVec(sizeof(struct CINSAccEntry) * 32, MEMF_CLEAR)))
   {
      Printf("Not enough memory to allocate Accounting Entry data\n");
      GetOut();
   }

   if((fp = Open(CINSACC, MODE_OLDFILE)) && (Read(fp, Config.AE, sizeof(struct CINSAccEntry) *32)))
   {
      Close(fp);
   }
}
