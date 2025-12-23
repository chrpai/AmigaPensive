/*
** UserList CGI for CNet/4
** by Kelly Cochran
** Last updated: Sun Jan 04 11:31:14 1998
*/

#include <stdio.h>
#include <stdlib.h>

#define UL_CONFIG "AWS:conf/userlist.conf"
#define UL_FORMAT 38

void printMCI(char *buff);
void PrintFile(char *file);
void do_ul(void);
int  setup(void);

struct MainPort *myp       = NULL;
struct Library  *CNetBase  = NULL;
struct Library  *CNet4Base = NULL;

char *tptr = NULL;
char **ulm = NULL;

const char *version = "\0$VER: CNet-UserList-CGI 1.4 "__AMIGADATE__;

void do_ul(void)
{
   int   i,
         z;

   char  buff   [ 256 ] = "",
         hindex [  64 ] = "",
         lcall  [  32 ] = "";

   struct UserData *tudata;
   struct UserData udata;

   for(i = 1; i < myp->Nums[0] + 1; i++)
   {
      if(tudata = LockAccount(i))
      {
         memset(&udata, 0, sizeof(struct UserData));
         CopyMem(tudata, (APTR)&udata, sizeof(struct UserData));
         UnLockAccount(i, FALSE);

         if((*udata.Handle == '!') || !*udata.Handle)
         {
            continue;
         }
         sprintf(buff, ulm[1], udata.Sex ? ulm[2]:ulm[3]);
         printMCI(buff);
         sprintf(buff, ulm[4], udata.UUCP, udata.Handle);
         printMCI(buff);
         if(*ulm[5] == '1')
         {
            for(z = 20-strlen(udata.Handle); z > 0; z--)
            {
               printf(" ");
            }
         }
         sprintf(buff, ulm[6], udata.PName ? ulm[0]:udata.RealName);
         printMCI(buff);
         CNetPrintDate(&udata.LastCall, lcall, NULL, ulm[7]);
         sprintf(buff, ulm[8], udata.CityState);
         printMCI(buff);
         sprintf(buff, ulm[9], lcall);
         printMCI(buff);
         sprintf(hindex, ulm[10], udata.UUCP);
         if(FileExists(hindex))
         {
            sprintf(buff, ulm[11], udata.UUCP);
            printMCI(buff);
         }
         else
         {
            printMCI(ulm[12]);
         }
         sprintf(buff, ulm[13], udata.Organ);
         printMCI(buff);
      }
   }
}

void main(void)
{
   int i = UL_FORMAT;

   if(CNetBase = OpenLibrary("cnet.library", 4))
   {
      if(CNet4Base = OpenLibrary("cnet4.library", 4))
      {
         printf("Content-type: text/html\n");
         printf("\n");

         Forbid();
         if(myp = (struct MainPort *)FindPort("cnetport"))
         {
            Permit();

            if(setup())
            {
               while(ulm[i] && *ulm[i])
               {
                  if(!stricmp(ulm[i], "{UL}"))
                  {
                     do_ul();
                  }
                  else
                  {
                     if(FileExists(ulm[i]))
                     {
                        PrintFile(ulm[i]);
                     }
                  }
                  i++;
               }
               FreeVec(tptr);
               FreeVec(ulm);
            }
         }
         else
         {
            Permit();
            printf("Could not find CONTROL process!\n\n");
         }
         CloseLibrary(CNet4Base);
      }
      CloseLibrary(CNetBase);
   }
} /*main*/

void PrintFile(char *file)
{
   BPTR fp;

   char buff [ 80 ] = "";

   if(fp = Open(file, MODE_OLDFILE))
   {
      while(FGets(fp, buff, sizeof(buff)) && (*buff != 0x10) && *buff)
         printMCI(buff);

      Close(fp);
   }
}

int setup(void)
{
   BPTR  fp;

   int   c,
         i;

   long  filesize,
         lines;

   char  *tmp;

   if(fp = Open(UL_CONFIG, MODE_OLDFILE))
   {
      if(!(filesize = FileSize(UL_CONFIG)))
         return FALSE;

      if((tptr = (char *) AllocVec(filesize, MEMF_CLEAR)))
      {
         i = Read(fp, tptr, filesize);
         Close(fp);
         tmp = tptr;
         for(c = 0, lines = 0; c < i; c++, tmp++)
         {
            if(*tmp == '\n')
            {
               lines++;
               *tmp = '\0';
            }
         }
         ulm = (char **)AllocVec((lines + 1) * sizeof(char *), MEMF_CLEAR);

         tmp = ulm[0] = tptr;

         for(c = 1; c < lines; tmp++)
         {
            if(*tmp == '\0')
            {
               tmp++;
               ulm[c] = tmp;
               c++;
            }
         }
      }
   }
   else
      return FALSE;

   return TRUE;
}

void printMCI(char *text)
{
   short font  = 0,
         numcr,
         num,
         x;

   static char buff [ 256 ] = "",
               buf2 [  80 ] = "";

   char *tmp = buff;

   while(*text)
   {
      x = 0;

      if(*text == 0x19)   // CTRL-Y
      {
         text++;
         switch(toupper(*text))
         {
            case 'N':
               text++;
               numcr = atoi(text);
               text++;
               while(x < numcr)
               {
                  *tmp = '\n';
                  tmp++;
                  x++;
               }
               break;

            case 'C':
               font++;
               text++;
               num = atoi(text);
               text++;
               if (num > 7)
                  num -= 8;
               sprintf(buf2, ulm[17], ulm[20+num]);
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
      else 
      {
         if(*text == 0x11)
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
   }
   *tmp = '\0';
   printf(buff);
   for(x = 0; x < font; x++)
      printf(ulm[18]);
}
