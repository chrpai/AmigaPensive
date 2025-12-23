#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

char version[] = "$VER: CNFinger v1.1 ("__DATE__")";

/************************************************************************
 *    CNet/3 and CNet/4  C language interface routines and examples     *
 *                                                                      *
 *  copyright © 1996 ZenMetal Software ... this code may be freely      *
 *  distributed to and used by registered CNet owners EXCLUSIVELY.      *
 *  Other distribution is in violation of copyright laws.               *
 ************************************************************************/

/************************************************************************
 *                          Function prototypes                         *
 ************************************************************************/
void            CallHost( UBYTE c );
void            ShutDown( char *spawn );
int             intGetOut( void );
void            LoadError( void );
void            check_entry(void);
void            cnet_finger(char *name);

/************************************************************************
 *                           Global Variables                           *
 ************************************************************************/
struct MsgPort         *replyp; /* Some communication details ...       */
struct CPort           *cport;
struct CMessage        cmess;
struct MainPort        *myp;    /* Pointer to CNet port--ALL info!      */
struct PortData        *z;
char                   **bm;
struct Library         *CNetBase    = NULL;
struct Library         *SocketBase2 = NULL;
struct sockaddr_in     *sin;

extern struct h_errno;

/************************************************************************
 *                             Main routine                             *
 ************************************************************************/
void main( int argc, char **argv )
{
   if(CNetBase = OpenLibrary("cnet.library", 4))
   {
      onbreak(intGetOut);

      Forbid();
      if((argc > 1) && (cport = (struct CPort *)FindPort(argv[1])))
      {
         Permit();
         if(replyp = CreatePort(0,0))
         {
            cmess.cn_Message.mn_ReplyPort    = replyp;
            cmess.cn_Message.mn_Length       = sizeof( struct CMessage );
            cmess.cn_Message.mn_Node.ln_Name = "cstuff";

            if(cport->ack != 40)
            {
               if(!(cport->ack != 30))
               {
                  cport->ack = 0;
                  z          = cport->zp;
                  myp        = cport->myp;
                  bm         = z->bm;

                  if(SocketBase2 = OpenLibrary("bsdsocket.library",3))
                  {
                     CloseLibrary(SocketBase2);
                     if(sin = AllocVec(sizeof(struct sockaddr_in),MEMF_CLEAR))
                        check_entry();
                     if(sin)
                        FreeVec(sin);
                  }
                  else
                     PutText(bm[2215]);
               }
            }
         }
         ShutDown(NULL);
      }
      else
         Permit();

      CloseLibrary(CNetBase);
   }
   else
      Printf("Could not open cnet.library.\n");

   exit(0);
}

int intGetOut(void)
{
   if(replyp)
      ShutDown(NULL);
   if(sin)
      FreeVec(sin);
   if(CNetBase)
      CloseLibrary(CNetBase);

   exit(0);

   return 0;
}

void check_entry(void)
{
   struct hostent *he,
                  tmp;

   struct servent *se;

   char  *site;
   char  *alist[1];

   int   args,
         sd,
         i;

   char name [  256 ] = "";
   char buff [ 1024 ] = "";

   ULONG ip;

   if(z->npitems > 1)
   {
      for(args = 1; *z->pitem[args]; args++)
      {
         strcat(name, z->pitem[args]);
         if(args < z->npitems - 1)
            strcat(name, " ");
      }
   }
   else
   {
      EnterLine(30, ELINE_INPUTBOX | ELINE_ALLOWCHAT, bm[171]);
      strcpy(name, z->InBuffer);
   }

   if(site = strchr(name,'@'))
   {
      *site = '\0';
      *site++;
   }
   else
   {
      cnet_finger(name);
      return;
   }

   he = gethostbyname(site);

   if(!he)
   {
      if(!(ip = inet_addr(site)))
      {
         sprintf(z->ABuffer,"Unknown host: %sn1",site);
         PutA();
         return;
      }
      tmp.h_name      = site;
      tmp.h_addr_list = alist;
      tmp.h_addr      = (char *)&ip;
      tmp.h_length    = sizeof(struct in_addr);
      tmp.h_addrtype  = AF_INET;
      tmp.h_aliases   = 0;
      he              = &tmp;
   }

   se = getservbyname("finger", "tcp");

   if(!se)
   {
      PutText("finger/tcp: Unknown servicen1");
      return;
   }

   sprintf(z->ABuffer, "[%s]n1", he->h_name);
   PutA();

   if((sd = socket(AF_INET,SOCK_STREAM,0)) < 0)
      PutText("Error opening socket.n1");
   else
   {
      CopyMem(he->h_addr, &sin->sin_addr, sizeof(struct in_addr));
      sin->sin_port   = se->s_port;
      sin->sin_family = AF_INET;

      if((connect(sd, (struct sockaddr *)sin, sizeof(struct sockaddr_in))) < 0)
         PutText("Couldn't connect.n1");
      else
      {
         i = strlen(name);
         name[i++] = '\n';
         name[i++] = '\0';
         if(send(sd, name, i, 0) != i)
            return;
            
         while((i = recv(sd, buff, sizeof(buff) - 1, 0)) > 0)
         {
            buff[i++] = '\0';
            PutText(buff);
         }
      }

      CloseSocket(sd);
   }

   return;
}

void cnet_finger(char *name)
{
   short age,
         id;

   char date1 [ 32 ] = "";

   if(id = FindAccount(name, &z->user2, FALSE))
   {
      PutText("n1");
      sprintf(z->ABuffer, bm[114], id);
      PutA();
      sprintf(z->ABuffer, bm[115], z->user2.Handle);
      PutA();
      if(z->SysMaint || !z->user2.PName)
      {
         sprintf(z->ABuffer, bm[116], z->user2.RealName, z->user2.Sex ? 'M':'F');
         PutA();
      }
      if(z->SysMaint || !z->user2.PAddress)
      {
         sprintf(z->ABuffer, bm[117], z->user2.Address);
         PutA();
      }
      sprintf(z->ABuffer, bm[118], z->user2.CityState);
      PutA();
      sprintf(z->ABuffer, bm[119], z->user2.Country, z->user2.ZipCode);
      PutA();
      if(z->SysMaint || !z->user2.PVoice)
      {
         sprintf(z->ABuffer, bm[120], z->user2.VoiceNo);
         PutA();
      }
      if(z->SysMaint || !z->user2.PData)
      {
         sprintf(z->ABuffer, bm[126], z->user2.PhoneNo, bm[1812+z->user2.PhoneVerified]);
         PutA();
      }
      if(z->SysMaint || !z->user2.PAge)
      {
         MakeDate(&z->user2.Birthdate, date1);
         date1[15] = '\0';
         age = z->Today.Year - z->user2.Birthdate.Year;
         if(z->Today.Month == z->user2.Birthdate.Month)
         {
            if(z->Today.Date < z->user2.Birthdate.Date)
               age--;
         }
         if(z->Today.Month < z->user2.Birthdate.Month)
            age--;
         sprintf(z->ABuffer, bm[122], date1, age);
         PutA();
      }
      MakeDate(&z->user2.FirstCall, date1);
      sprintf(z->ABuffer, bm[123], date1);
      PutA();
      MakeDate(&z->user2.LastCall, date1);
      sprintf(z->ABuffer, bm[124], date1); /* last call */
      PutA();
      sprintf(z->ABuffer, bm[125], z->mm[z->menustart[30]+z->user2.CompType]);
      PutA();
      sprintf(z->ABuffer, bm[145], z->user2.HighBaud);
      PutA();

      sprintf(z->ABuffer, "Mail:Users/%s/_plan", z->user2.UUCP);
      ReadGraphics(z->ABuffer,FALSE);
      if(z->SysMaint)
      {
         sprintf(z->ABuffer, "Mail:Users/%s/_plan_sq", z->user2.UUCP);
         ReadGraphics(z->ABuffer,FALSE);
         sprintf(z->ABuffer, bm[864], z->user2.PubMessages, z->user2.PriMessages);
         PutA();
         PutText(bm[865]);
         PutText(bm[866]);
         sprintf(z->ABuffer, bm[867], z->user2.UpFiles, z->user2.UpBytes);
         PutA();
         sprintf(z->ABuffer, bm[868], z->user2.DownFiles, z->user2.DownBytes);
         PutA();
#if 0
         if(z->user2.Privs.ABits & FCREDEX_FLAG)
            sprintf(date1, bm[869], 
         if(z->user2.Privs.ABits & BCREDEX_FLAG)
            sprintf(date2, bm[870], 
         sprintf(z->ABuffer, bm[871], 
         PutA();
         sprintf(z->ABuffer, bm[876],
         PutA();
         sprintf(z->ABuffer, bm[877],
         PutA();
         sprintf(z->ABuffer, bm[881],
         PutA();
#endif
      }
   }
   return;
}

/**************************************************************************
 *                         another file to run?                           *
 **************************************************************************/
void ShutDown( char *spawn )
{
        /* spawn = full path/file to run */
        if( spawn )
                strcpy( z->CSpawn, spawn );

        CallHost( 0 );
}


void CallHost( UBYTE c )
{
        cmess.command = c;
        PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
        WaitPort( replyp );
        GetMsg  ( replyp );
}

void PutText( char *text )
{
        cmess.arg1 = (ULONG)text;       /* text to print                */
        CallHost( 1 );
}

void PutA( void )
{
        PutText( z->ABuffer );
}

/**************************************************************************
 *    ENTERLINE FLAGS:                                                    *
 *    Construct by ORing the following bit defines:                       *
 *                                                                        *
 * ie. EnterLine(3, ELINE_ALLCAPS|ELINE_INPOUTBOX, "My Prompt>");         *
 *                                                                        *
 *     FLAGS:                                                             *
 *     ------                                                             *
 *     ELINE_ALLCAPS       = All capitalized                              *
 *     ELINE_FILENAME      = FILENAME.  Don't allow =":; or asterisk      *
 *     ELINE_USEINBUFF     = Begin with existing z.InBuffer               *
 *     ELINE_NOLEADSPACE   = Chop leading spaces                          *
 *     ELINE_CAPWORDSTART  = Force 1st letter of word caps                *
 *     ELINE_RESTWORDLOWER = Force all others lower case                  *
 *     ELINE_NUMBERSONLY   = Numeric input only                           *
 *     ELINE_INPUTBOX      = Print input box (terminated with . )         *
 *     ELINE_ALLOWMCI      = DO allow MCI                                 *
 *     ELINE_HANDLESPECIAL = HANDLES/SPECIAL.  Don't allow ^_`{|}~@       *
 *     ELINE_XSLASHSTART   = Exit for . or / at beginning of line         *
 *     ELINE_XBKSPACESTART = Exit for backspace at beginning of line      *
 *     ELINE_NOOLMS        = Do not allow OLM's to appear while editing   *
 *     ELINE_ALLOWCHAT     = Allow Chat break in at this prompt. COMMAND  *
 *                           PROMPT.                                      *
 *     ELINE_NOSPACES      = Don't allow SPACES                           *
 *     ELINE_NOCURSORMOVE  = DON'T ALLOW MOVEMENT                         *
 *     ELINE_NOSLASHES     = Don't allow forward slash                    *
 *                                                                        *
 *  NOTE: The old "numeric" flag values still work 100% ..                *
 **************************************************************************/
int EnterLine( UBYTE len, ULONG flags, char *prompt )
{
   cmess.arg1 = (ULONG)len;     /* how many chars max to input  */
   cmess.arg2 = (ULONG)flags;   /* flags describing required input handling */
   cmess.arg3 = (ULONG)prompt;  /* text to print before input   */
   CallHost( 2 );               /* result is in z->InBuffer     */

   return( (int)strlen( z->InBuffer ));
}

/**************************************************************************
 *        Sets the "Action" or "Where" field for the user's port          *
 *      ** remember to retain and restore the previous z->DOING **        *
 **************************************************************************/
void SetDoing( char *what )
{
        cmess.arg1 = (ULONG)what;
        CallHost( 7 );
}

/**************************************************************************
 *    Find an account based on account number, Handle or real name        *
 *    This is the same routine that, if the user is not found, pops up    *
 *    the mini-userlist for user choice.                                  *
 **************************************************************************/
short FindAccount( char *a, struct UserData *b, UBYTE quiet )
{
   cmess.arg1 = (ULONG)a;
   cmess.arg2 = (ULONG)b;
   cmess.arg3 = (ULONG)quiet;
   CallHost( 20 );
   return( (short)cmess.result );
}

UBYTE PutQ( char *a )
{
   PutText( a );
   return (UBYTE)(z->MCIcreg[0][0]=='1') ;
}

void MakeDate( struct IsDate *date, char *output )
{
	cmess.arg1 = (ULONG)date;
	cmess.arg2 = (ULONG)output;
	CallHost( 10 );
}

UBYTE ReadGraphics( char *path, char flags )
{
	cmess.arg1 = (ULONG)path;
	cmess.arg2 = (ULONG)flags;	/* 1 = print File Not Found	*/
	CallHost( 9 );
	return( (UBYTE)cmess.result );		/* FALSE if File Not Found	*/
}
