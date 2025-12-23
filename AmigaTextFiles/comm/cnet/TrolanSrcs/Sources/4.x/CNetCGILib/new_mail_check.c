#include "cnetcgi_lib.h"

BOOL new_mail_check(char *uucp, char *date)
{
   BPTR  fp;

   struct MailHeader4    mh4;
   struct IsDate         mid;

   ULONG  time = NULL;

   char buff [ 256 ] = "";

   sprintf(buff, "Mail:Users/%s/FOLDERS/INBOX/_mhead4", uucp);

   if(fp = Open(buff, MODE_OLDFILE))
   {
      while(Read(fp, &mh4, sizeof(struct MailHeader4)) != NULL)
      {
         if(!(mh4.flags & MAIL_OPENED) && !mh4.Killed)
         {
            if(time < mh4.Date)
               time = mh4.Date;
         }
      }
      Close(fp);
   }

   if(time)
   {
      CNetExplodeTime(time, &mid);
      CNetPrintDate(&mid, date, NULL, NULL);
      return TRUE;
   }

   return FALSE;
}
