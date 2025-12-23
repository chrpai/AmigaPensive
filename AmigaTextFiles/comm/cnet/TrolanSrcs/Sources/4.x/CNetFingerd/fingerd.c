/*
** CNet/4 Fingerd
** by Kelly Cochran
** Last Updated: Mon Jan 04 20:30:30 1999
*/

#include "fingerd.h"
#include <cnetndk/cnetcgi_lib.h>
#include <cnetndk/cnet4lib_protos.h>

#define BIG_BUFFER 1024

void FRemoveMCI(char *in)
{
   short numcr;
   short x;

   char buff [ BIG_BUFFER ] = "";

   char *tmp = buff;

   while(*in != '\0')
   {
      x = 0;
      if(*in == 0x19)   // CTRL-Y
      {
         in++;
         switch(toupper(*in))
         {
            case 'N':
               in++;
               numcr = atoi(in);
               in++;
               while(x<numcr)
               {
                  *tmp = '\r';
                  tmp++;
                  *tmp = '\n';
                  tmp++;
                  x++;
               }
               break;
            default:
               in += 2;
         }
      }
      else if(*in == 0x11)
      {
         while( (*in != '}') && (*in != '\0') )
         {
            in++;
         }
         in++;
      }
      else
      {
         *tmp = *in;
         tmp++;
         in++;
      }
   }
   *tmp = '\0';

   fwrite(buff, strlen(buff), 1, stdout);
}  /* FRemoveMCI */

void PrintFile(char *file)
{
   BPTR  fp;

   char b [ BIG_BUFFER ];

   if(fp = Open(file, MODE_OLDFILE))
   {
      printf("\r\n");
      while(FGets(fp, b, sizeof(b)) != NULL)
      {
         FRemoveMCI(b);
      }
      Close(fp);
   }
}  /* PrintFile */

void FDisplay(long uid, char *name)
{
   char buffer [ BIG_BUFFER ] = "",
        date   [  40 ],
        buf1   [  30 ];

   struct UserData *tudata;
   struct UserData udata;

   uid++;

   if(tudata = LockAccount(uid))
   {
      CopyMem(tudata, (APTR)&udata, sizeof(struct UserData));
      UnLockAccount(uid, FALSE);
      if(*name && (udata.PName))
      {
         printf(buffer, "Invalid account %s.\r\n", name);
         return;
      }
      sprintf(buffer, myp->FirstTT->bm[114], uid); /* account # */
      FRemoveMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[115], udata.Handle); /* handle */
      FRemoveMCI(buffer);
      printf("EMailAddr: %s@%s\r\n", udata.UUCP, myp->gc.MyUUCPName); /* email */
      if(!udata.PName)   /* priv name check */
      {
         sprintf(buffer, myp->FirstTT->bm[116],
              udata.RealName, *myp->FirstTT->bm[1141+udata.Sex]); /* name/gender */
         FRemoveMCI(buffer);
      }
      if(!udata.PAddress) /* priv address check */
      {
         sprintf(buffer, myp->FirstTT->bm[117], udata.Address); /* address */
         FRemoveMCI(buffer);
      }
      sprintf(buffer, myp->FirstTT->bm[118], udata.CityState); /* city state */
      FRemoveMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[119], udata.Country, udata.ZipCode);
          /* country, zip */
      FRemoveMCI(buffer);
      if(!udata.PVoice) /* priv voice phone check */
      {
         sprintf(buffer, myp->FirstTT->bm[120], udata.VoiceNo);
             /* voice phone */
         FRemoveMCI(buffer);
      }
      if(!udata.PData) /* priv data phone check */
      {
         sprintf(buffer, myp->FirstTT->bm[126], udata.PhoneNo,
              myp->FirstTT->bm[1812+udata.PhoneVerified]); /* data phone */
         FRemoveMCI(buffer);
      }
      if(!udata.PAge) /* priv age check */
      {
         CNetPrintDate(&udata.Birthdate, date, NULL, "www dd-mmm-yyyy");
         sprintf(buffer, myp->FirstTT->bm[122], date, CalcAge(&udata)); /* birthdate */
         FRemoveMCI(buffer);
      }
      CNetPrintDate(&udata.FirstCall, date, NULL, NULL);
      sprintf(buffer, myp->FirstTT->bm[123], date); /* first call */
      FRemoveMCI(buffer);
      CNetPrintDate(&udata.LastCall, date, NULL, NULL);
      sprintf(buffer, myp->FirstTT->bm[124], date); /* last call */
      FRemoveMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[125],
         myp->FirstTT->mm[myp->FirstTT->menustart[30]+udata.CompType]);
             /* comp type */
      FRemoveMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[145], udata.HighBaud);   /* high baud */
      FRemoveMCI(buffer);
      printf("\r\nThis user is %scurrently online.\r\n",(udata.z &&
         !((udata.z->IUCBits[udata.z->InPort] & PORTRIGHT_HIDDEN) ||
           (udata.z->HideAll))) ? "":"not ");

      if(new_mail_check(udata.UUCP, date))
      {
         printf("\r\nNew mail received on %s.\r\n", date);
      }
      else
      {
         printf("\r\nNo new mail.\r\n");
      }

      sprintf(buf1, "Mail:Users/%s/_plan", myp->Key[uid-1].UUCP); /* finger file */

      PrintFile(buf1);
   }

} /* FDisplay */

void do_who(void)
{
   int  i,
        len;
   char buffer [ BIG_BUFFER ] = "",
        date   [  40 ];
   char *flag;

   sprintf(buffer, "Who's online %s\r\n", myp->gc.MySystemName);

   len = strlen(buffer);

   for(i=0; i < (80-len)/2; i++)
      printf(" ");
   printf(buffer);

   sprintf(buffer, "%s", myp->FirstTT->bm[898]);  /* Port Handle, etc */
   FRemoveMCI(buffer);
   sprintf(buffer, "%s", myp->FirstTT->bm[899]);  /* spacer line */
   FRemoveMCI(buffer);
   printf("\r\n");

   for(i=0; i <= myp->HiPort; i++)
   {
      date[0] = '\0';
      flag = " ";
      if (!myp->pc[i].online)
         continue;
      if (!myp->PortZ[i]->id || (myp->PortZ[i]->IUCBits[i] & PORTRIGHT_HIDDEN) || (myp->PortZ[i]->HideAll))
      {
         if (!myp->gc.SkipIdlePorts)  /* SkipIdlePorts false? */
         {
            char *who = myp->FirstTT->bm[902];
            sprintf(buffer,myp->FirstTT->bm[900],i,flag);
            FRemoveMCI(buffer);
            if(*myp->PortZ[i]->PDE->sp.IdleWho)       /* Idle who? */
            {
               who = myp->PortZ[i]->PDE->sp.IdleWho;
            }
            sprintf(buffer, myp->FirstTT->bm[901], who, date, 0);
            FRemoveMCI(buffer);
            sprintf(buffer, myp->FirstTT->bm[904], "", "");
            printf("  ");
            FRemoveMCI(buffer);
            sprintf(buffer, myp->FirstTT->bm[905], myp->PortZ[i]->MyDoing);
            FRemoveMCI(buffer);
            printf("\r\n");
         }
         continue;
      }
      if(myp->PortZ[i]->user1.MyPrivs.ABits2 & SUPERUSER_FLAG)
         flag = myp->FirstTT->bm[947];
      if(myp->PortZ[i]->user1.MyPrivs.ABits & CONFCON_FLAG)
         flag = myp->FirstTT->bm[924];
      sprintf(buffer,myp->FirstTT->bm[900], i, flag);
      FRemoveMCI(buffer);
      PrintHour(&myp->PortZ[i]->user1.ConnectDate,date);
      sprintf(buffer, myp->FirstTT->bm[901],
         myp->PortZ[i]->user1.Handle, date, (myp->PortZ[i]->CurrentCPS)/10);
      FRemoveMCI(buffer);
      printf(" ");
      sprintf(buffer, myp->FirstTT->bm[904],
          myp->PortZ[i]->user1.CityState, myp->PortZ[i]->user1.Country);
      FRemoveMCI(buffer);
      printf(" ");
      if(WhoIdleTime(myp->PortZ[i], date, 20))
      {
         sprintf(buffer, myp->FirstTT->bm[905], date);
      }
      else
      {
         sprintf(buffer, myp->FirstTT->bm[905], myp->PortZ[i]->MyDoing);
      }
      FRemoveMCI(buffer);
      printf("\r\n");
      if(*myp->PortZ[i]->user1.Banner)
      {
         sprintf(buffer, myp->FirstTT->bm[903], myp->PortZ[i]->user1.Banner);
         FRemoveMCI(buffer);
      }
   }
   return;
} /* do_who */

void do_finger(char *uids)
{
   BPTR  lock;

   long i;
   char *handle;
   char buf [ BIG_BUFFER ] = "";

   if(*uids && (strchr(uids, '/') || strchr(uids, ':')))
   {
      printf("Invalid account %s.\r\n",uids);
      return;
   }

   if (!*uids | (!stricmp("WHO", uids)))
   {
      do_who();
      return;
   }
   i = atol(uids) - 1;
   if (i >= 0)
   {
      if (!(i < myp->Nums[0]) || (*myp->Key[i].Handle == '!') || !*myp->Key[i].Handle)
      {
         printf("Invalid account #%s.\r\n",uids);
         return;
      }
      FDisplay(i,"");
      return;
   }
   else
   {
      if(!FileExists(FINGER_DIR))
      {
         if(lock = CreateDir(FINGER_DIR))
            UnLock(lock);
      }
      sprintf(buf,FINGER_DIR"/%s",uids);
      if(FileExists(buf))
      {
         PrintFile(buf);
         return;
      }
      for(i = 0; i < myp->Nums[0]; i++)
      {
         handle = myp->Key[i].Handle;
         if (!*handle)
            break;
         if (*handle == '!')
            continue;
         if (!stricmp(handle, uids) || !stricmp(myp->Key[i].UUCP, uids))
         {
            FDisplay(i, "");
            return;
         }
         if (!stricmp(myp->Key[i].RealName, uids))
         {
            FDisplay(i, uids);
            return;
         }
      }
   }
   printf("Invalid account %s.\r\n",uids);
} /* do_finger */

void main(void)
{
   char fromstdin[30];
   char c;
   int i = 0;

   int sock = init_inet_daemon();   /* get socket from inetd */
   if (sock >= 0)
      set_socket_stdio(sock);       /* set it as stdio */

   Forbid();
   if(myp = (struct MainPort *)FindPort("cnetport"))
   {
      Permit();
      if(CNetBase = OpenLibrary("cnet.library",4))
      {
         if(CNet4Base = OpenLibrary("cnet4.library",4))
         {
            c = getc(stdin);
            while((c != '\n') && (c != '\r') && (c != EOF))
            {
               fromstdin[i] = c;
               i++;
               c = getc(stdin);
            }
            fromstdin[i] = '\0';
            do_finger(fromstdin);

            openlog("fingerd", LOG_PID, LOG_DAEMON);
            syslog(LOG_INFO, "lookup: %s", fromstdin);
            closelog();

            CloseLibrary(CNet4Base);
         }
         else
            printf("Unable to open cnet4.library.\r\n");

         CloseLibrary(CNetBase);
      }
      else
         printf("Unable to open cnet.library.\r\n");
   }
   else
   {
      Permit();
      printf("Could not find CONTROL process!\r\n\r\n");
   }

   exit(0);
} /* main */
