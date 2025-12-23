// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

// note: This code must be compiled using NOSTACKCHECK and NOSTARTUP options
//
//   eg: sc DOS-UpTime nostkchk nostartup link

#include <stdarg.h>

struct Library *CNet4Base=NULL;
struct Library *CNetBase=NULL;

void CleanUp    ( void );
void MCIprintf  ( char *fmt, ...);


__saveds void main( void )
{
   struct MainPort *myp=NULL;
   short i;
   struct PortData *zp;
   char date[24];
   LONG uptime;
   char utstring[128];
   struct RangeContext rc;
   char *argbuff;
   LONG myargs[1] = { 0L };
	struct RDArgs *MyReadArgs=NULL;

   // We open dos.library because the startup code is not available to autoopen
   // dos.library

   if(!(DOSBase=(struct DosLibrary *)OpenLibrary("dos.library", 37L)) || !(CNet4Base=OpenLibrary("cnet4.library", 4L)) || !(CNetBase=OpenLibrary("cnet.library", 4L)) )
   	{
   	CleanUp();
      return;
      }

	// parse stdin using ReadArgs so that we don't have to link with startup code.

	Forbid();
	myp=(struct MainPort *)FindPort(CNETPORT);
	Permit();

   if(myp)
      {
		MyReadArgs=ReadArgs("PORTS", myargs, NULL);
		if(myargs[0])
			{
	      argbuff = (char *)myargs[0];
	      if(argbuff[0] == '*')
	      	argbuff = "-";
	      }
	   else
	      argbuff = "-";

		// Now do the CNet stuff to list port uptimes.
      memset(&rc, 0, sizeof(struct RangeContext));

      if(CNetFindRange( argbuff, 0, myp->HiPort, &rc))
      	{
			MCIprintf(myp->FirstTT->bm[2425]); // uptime header
			MCIprintf(myp->FirstTT->bm[2426]); // header underline

			// control uptime

			MakeDateZone( &myp->SAMDate[1], date, &myp->FirstTT->bm[1069], NULL, NULL );
			CNetUpTime(utstring, CNetTimeNowDiff(CNetImplodeTime(&myp->SAMDate[1])));
			MCIprintf(myp->FirstTT->bm[2427], date, (*utstring) ? utstring : (char *)myp->FirstTT->bm[2421]);

	      // port uptimes..

			while( (i=CNetNextRange( &rc )) != -1)
				{
				if((zp=myp->PortZ[i]) != myp->z0 ) 
					{
					MakeDateZone( &zp->BootDate,date, &myp->FirstTT->bm[1069], NULL, NULL );
					CNetUpTime(utstring, CNetTimeNowDiff(CNetImplodeTime(&zp->BootDate)));
					MCIprintf(myp->FirstTT->bm[2428], i, date, *utstring ? utstring : (char *)myp->FirstTT->bm[2421]);
					}
				}
			MCIprintf(myp->FirstTT->bm[2429]); // footer
			}

		FreeArgs(MyReadArgs);
		}
   else
      {
  	   Printf("\nCNet:Control is not running\n\n");
      CleanUp();
      return;
      }
   Printf("\n");
   CleanUp();
   return;
}



// called before exiting so that resources used may be closed/freed
void CleanUp( void )
{
   if(CNet4Base)
      CloseLibrary(CNet4Base);
   if(CNetBase)
      CloseLibrary(CNetBase);
   if(DOSBase)
      CloseLibrary((struct Library *)DOSBase);
}



// MCI filtering varargs print routine
void MCIprintf(char *fmt, ...)
{
   va_list ap;
   char sprbuff[256], outbuff[256], *outp=outbuff, *inp;
   UBYTE CQ;
   int num, x;

   va_start(ap, fmt);
   vsprintf(sprbuff, fmt, ap);
   va_end(ap);

   inp=sprbuff;

   while(*inp)
      {
      CQ=FALSE;
      switch(*inp)
         {
         case 17: // ^Q
            CQ=TRUE;
         case 25: // ^Y
            inp++;
            switch(*inp)
               {
               case 'n':
                  inp++;
                  while(*inp == ' ')
                     inp++;
                  num=atoi(inp);
                  for(x=0;x<num;x++)
                     {
                     *outp='\n';
                     outp++;
                     }
                  break;
               default:
                  inp++;
                  break;
               }

            if(CQ)
               {
               char *s;

               if(s=strchr(inp, '}'))
                  inp=s;
               CQ=FALSE;
               }
            break;
         default:
            *outp=*inp;
            outp++;
            break;
         }
      inp++;
      }
   *outp='\0';
   
   PutStr(outbuff);
}
   

