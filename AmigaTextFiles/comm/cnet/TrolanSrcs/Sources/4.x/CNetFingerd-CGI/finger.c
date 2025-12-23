/*
** CNet-aware finger CGI script
** by Kelly Cochran
** Last updated: Tue Nov 18 23:05:32 1997
*/

#include "finger.h"
#include <cnetndk/cnetcgi_lib.h>

#define BIG_BUFFER 1024

void printMCI(char *text)
{
   short num,
         x,
         font = 0;
   char  buff[ BIG_BUFFER ] = "";
   char  buf2 [ 32 ] = "";

   char *tmp = buff;

   int color [ 8 ] = { 0x000000, 0xA01010, 0x20A020, 0xE09020,
                       0x0020B0, 0xA000B0, 0x50A0C0, 0xFFFFFF };

   while(*text != '\0')
   {
      x = 0;
      if(*text == 0x19)
      {
         text++;
         switch(toupper(*text))
         {
            case 'N':
               text++;
               num = atoi(text);
               text++;
               while(x < num)
               {
                  *tmp='\n';
                  tmp++;
                  x++;
               }
               break;

            case 'C':
               font++;
               text++;
               num = atoi(text);
               text++;
               if(num > 7)
                  num -= 8;
               sprintf(buf2, "<FONT COLOR=#%06X>", color[num]);
               while(x < strlen(buf2))
               {
                  *tmp = buf2[x];
                  tmp++;
                  x++;
               }
               break;

            default:
               text += 2;
         }
      }
      else if(*text == '{')
      {
         while( (*text != '}') && (*text != '\0') )
            text++;
      }
      else
      {
         *tmp = *text;
         tmp++;
         text++;
      }
   }
   *tmp = '\0';
   fwrite(buff, strlen(buff), 1, stdout);
   for(x = 0; x < font; x++)
   {
      printf("</FONT>");
   }
}  /* printMCI */

void FDisplay(long uid, char *name)
{
   char buffer [ BIG_BUFFER ] = "";
   char date   [  40 ];
   char buf1   [  30 ];
   struct UserData *tudata;
   struct UserData udata;

   uid++;

   if(tudata = LockAccount(uid))
   {
      CopyMem(tudata, (APTR)&udata, sizeof(struct UserData));
      UnLockAccount(uid, FALSE);

      if(*name && (udata.PName))
      {
         printf("Invalid account %s.\n", name);
         return;
      }

      printf("<PRE>\n");
      sprintf(buffer, myp->FirstTT->bm[114], uid); /* account # */
      printMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[115], udata.Handle); /* handle */
      printMCI(buffer);
      printf("EMailAddr: <A HREF=mailto:%s@%s>%s@%s</A>\n", udata.UUCP,
              myp->gc.MyUUCPName, udata.UUCP, myp->gc.MyUUCPName); /* email */
      if(!udata.PName)   /* priv name check */
      {
         sprintf(buffer, myp->FirstTT->bm[116], 
              udata.RealName, *myp->FirstTT->bm[1141+udata.Sex]); /* name/gender */
         printMCI(buffer);
      }
      if(!udata.PAddress) /* priv address check */
      {
         sprintf(buffer, myp->FirstTT->bm[117], udata.Address); /* address */
         printMCI(buffer);
      }
      sprintf(buffer, myp->FirstTT->bm[118], udata.CityState); /* city state */
      printMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[119], udata.Country, udata.ZipCode); /* country, zip */
      printMCI(buffer);
      if(!udata.PVoice) /* priv voice phone check */
      {
         sprintf(buffer, myp->FirstTT->bm[120], udata.VoiceNo); /* voice phone */
         printMCI(buffer);
      }
      if(!udata.PData) /* priv data phone check */
      {
         sprintf(buffer, myp->FirstTT->bm[126], udata.PhoneNo,
              myp->FirstTT->bm[1812+udata.PhoneVerified]); /* data phone */
         printMCI(buffer);
      }
      if (!udata.PAge) /* priv age check */
      {
         CNetPrintDate(&udata.Birthdate, date, NULL, "www dd-mmm-yyyy");
         sprintf(buffer, myp->FirstTT->bm[122], date, CalcAge(&udata)); /* birthdate */
         printMCI(buffer);
      }
      CNetPrintDate(&udata.FirstCall, date, NULL, NULL);
      sprintf(buffer, myp->FirstTT->bm[123], date); /* first call */
      printMCI(buffer);
      CNetPrintDate(&udata.LastCall, date, NULL, NULL);
      sprintf(buffer, myp->FirstTT->bm[124], date); /* last call */
      printMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[125],
         myp->FirstTT->mm[myp->FirstTT->menustart[30]+udata.CompType]); /* comp type */
      printMCI(buffer);
      sprintf(buffer, myp->FirstTT->bm[145], udata.HighBaud);   /* high baud */
      printMCI(buffer);
      printf("\nThis user is %scurrently online.\n", (udata.z &&
         !((udata.z->IUCBits[udata.z->InPort] & PORTRIGHT_HIDDEN) ||
           (udata.z->HideAll))) ? "":"not ");

      if(new_mail_check(udata.UUCP, date))
      {
         printf("\nNew mail received on %s.\n", date);
      }
      else
      {
         printf("\nNo new mail.\n");
      }
      printf("\n");

      sprintf(buf1, "Mail:Users/%s/_plan", myp->Key[uid-1].UUCP); /* finger file */

      PrintFile(buf1);

      printf("</PRE>\n");

   }
   return;
} /* FDisplay */

void do_finger(char *uids)
{
   long i;
   char *handle;

   if(!*uids)
   {
      return;
   }
   i = atol(uids) - 1;
   if(i >= 0)
   {
      if(!(i < myp->Nums[NUMS_CURRENT_ACCOUNTS]) || (*myp->Key[i].Handle == '!') || (!*myp->Key[i].Handle))
      {
         printf("Invalid account #%s.\n", uids);
         return;
      }
      FDisplay(i, "");
      return;
   }
   else
   {
      for(i = 0; i < myp->Nums[0]; i++)
      {
         handle = myp->Key[i].Handle;
         if(!*handle)
         {
            break;
         }
         if(*handle == '!')
         {
            continue;
         }
         if (!stricmp(handle, uids) || !stricmp(myp->Key[i].UUCP, uids))
         {
            FDisplay(i, "");
            return;
         }
         else if (!stricmp(myp->Key[i].RealName, uids))
         {
            FDisplay(i, uids);
            return;
         }
      }
   }
   printf("Invalid account %s.\n", uids);
} /* do_finger */

void main(int argc, char **argv)
{
   if(CNetBase = OpenLibrary("cnet.library", 4))
   {
      if(CNet4Base = OpenLibrary("cnet4.library", 4))
      {
         printf("Content-type: text/html\n");
         printf("\n");

         Forbid();
         if(myp = (struct MainPort *)FindPort( "cnetport" ))
         {
            Permit();
            PrintFile("AWS:conf/finger.head");
            if (argc == 2)
            {
               plustospace(argv[1]);
               unescape_url(argv[1]);
               do_finger(argv[1]);
            }
            PrintFile("AWS:conf/finger.foot");
         }
         else
         {
            Permit();
            printf("Could not find CONTROL process!\n");
         }
         CloseLibrary(CNet4Base);
      }
      CloseLibrary(CNetBase);
   }
} /* main */

void PrintFile(char *file)
{
   BPTR fp;

   char buff [ BIG_BUFFER ] = "";

   if(fp = Open(file, MODE_OLDFILE))
   {
      while(FGets(fp, buff, sizeof(buff)) && (*buff != 26))
         printMCI(buff);
      Close(fp);
   }
}      
