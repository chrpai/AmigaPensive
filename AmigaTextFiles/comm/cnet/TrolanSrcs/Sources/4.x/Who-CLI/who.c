/*
** CNet/4 Who CLI script
** by Kelly Cochran
** Last updated: Sun Nov 30 19:38:42 1997
*/

#include <stdio.h>
#include <stdlib.h>

#define WHO_FORMAT 35

#define BIG_BUFFER 1024

void do_who(void);
void main(void);
BOOL setup(void);
void printMCI(char *text);
void PrintFile(char *file);

struct MainPort *myp       = NULL;
struct Library  *CNetBase  = NULL;
struct Library  *CNet4Base = NULL;

char **wm  = NULL;
char *wptr = NULL;

const char *version = "$VER: CNet-Who-CLI 4.1 "__AMIGADATE__;

void do_who(void)
{
   int i, z, spacer;
   char *handle;
   char flag;
   char date [ 40 ];

   char buff [ BIG_BUFFER ];

   sprintf(buff, wm[11], myp->gc.MySystemName);
   printMCI(buff);

   sprintf(buff, wm[0]);
   printMCI(buff);
   sprintf(buff, wm[1]);
   printMCI(buff);

   for(i=0;i <= myp->HiPort;i++)
   {
      if(!myp->pc[i].online)
      {
         continue;
      }

      if(!myp->PortZ[i]->id || (myp->PortZ[i]->IUCBits[i] & PORTRIGHT_HIDDEN) || (myp->PortZ[i]->HideAll))
      {
         if(!myp->gc.SkipIdlePorts)
         {
            char *who = myp->FirstTT->bm[902];
            if(*myp->PortZ[i]->PDE->sp.IdleWho)
            {
               who = myp->PortZ[i]->PDE->sp.IdleWho;
            }
            sprintf(buff, wm[2], i, who);
            printMCI(buff);
            sprintf(buff, wm[3], myp->FirstTT->bm[906]);
            printMCI(buff);
         }
         continue;
      }         
      flag = ' ';
      if(myp->PortZ[i]->user1.MyPrivs.ABits2 & SUPERUSER_FLAG)
      {
         flag = *myp->FirstTT->bm[947];
      }
      if(myp->PortZ[i]->user1.MyPrivs.ABits & CONFCON_FLAG)
      {
         flag = *myp->FirstTT->bm[924];
      }
      sprintf(buff, wm[4], i, flag);
      printMCI(buff);
      handle = myp->PortZ[i]->user1.Handle;
      spacer = 20 - strlen(handle);
      sprintf(buff, wm[5], myp->PortZ[i]->user1.UUCP, myp->gc.MyUUCPName);
      printMCI(buff);
      sprintf(buff, wm[6], handle);
      printMCI(buff);
      if(*wm[32] == '1')
      {
         for (z=0; z < spacer;z++)
         {
            printf(" ");
         }
      }
      CNetPrintDate(&myp->PortZ[i]->user1.ConnectDate, date, NULL, NULL);
      sprintf(buff, wm[7], date+16, (myp->PortZ[i]->CurrentCPS)/10);
      printMCI(buff);
      sprintf(buff, wm[8], myp->PortZ[i]->user1.CityState,
                   myp->PortZ[i]->user1.Country);
      printMCI(buff);
      sprintf(buff, wm[9], myp->PortZ[i]->MyDoing);
      printMCI(buff);
      if(*myp->PortZ[i]->user1.Banner)
      {
         sprintf(buff, wm[10], myp->PortZ[i]->user1.Banner);
         printMCI(buff);
      }
   }
   return;
}

void main(void)
{
   int i = WHO_FORMAT;

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
               while(wm[i] && *wm[i])
               {
                  if(!stricmp(wm[i], "{WHO}"))
                  {
                     do_who();
                  }
                  else
                  {
                     if(FileExists(wm[i]))
                     {
                        PrintFile(wm[i]);
                     }
                  }
                  i++;
               }
               FreeVec(wptr);
               FreeVec(wm);
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
}

void PrintFile(char *file)
{
   BPTR fp;

   char buff [ BIG_BUFFER ] = "";

   if(fp = Open(file, MODE_OLDFILE))
   {
      while(FGets(fp, buff, sizeof(buff)) && (*buff != 26))
      {
         printMCI(buff);
      }
      Close(fp);
   }
}

BOOL setup(void)
{
   BPTR  fp;

   int   c,
         i;

   long  filesize,
         lines    = 0;

   char  *tmp;

   if(fp = Open("AWS:conf/who.conf", MODE_OLDFILE))
   {
      if(!(filesize = FileSize("AWS:conf/who.conf")))
      {
         Close(fp);
         return FALSE;
      }

      if((wptr = (char *) AllocVec(filesize, MEMF_CLEAR)))
      {
         i = Read(fp, wptr, filesize);
         Close(fp);
         tmp = wptr;
         for(c = 0; c < i; c++)
         {
            if(*tmp == '\n')
            {
               lines++;
               *tmp = '\0';
            }
            else if(*tmp == 0x18)
            {
               *tmp = ' ';
            }

            tmp++;
         }
         if(wm = (char **)AllocVec((lines + 1) * sizeof(char *), MEMF_CLEAR))
         {
            tmp = wm[0] = wptr;

            for(c = 1;c < lines; tmp++)
            {
               if(*tmp == '\0')
               {
                  tmp++;
                  wm[c] = tmp;
                  c++;
               }
            }
         }
         else
         {
            FreeVec(wptr);
            return FALSE;
         }
      }
   }
   else
   {
      return FALSE;
   }

   return TRUE;
}

void printMCI(char *text)
{
   short font  = 0,
         numcr,
         num,
         x;

   char buff [ BIG_BUFFER ] = "",
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
               {
                  num -= 8;
               }
               sprintf(buf2, wm[12], wm[15+num]);
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
         if(*text == '{')
         {
            while( (*text != '}') && (*text != '\0') ) // curly brace
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
   for(x = 0;x < font;x++)
   {
      printf(wm[13]);
   }
}
