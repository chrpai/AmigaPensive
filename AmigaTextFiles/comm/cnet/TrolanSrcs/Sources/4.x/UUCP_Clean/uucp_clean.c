#ifdef CNET3
 #undef CNET4
#else
 #define CNET4
#endif

#include "rev.h"
#include "uucp_clean.h"

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
void            GetOut( void );
void            LoadError( void );
void            uucp_init( void );

/************************************************************************
 *                           Global Variables                           *
 ************************************************************************/
struct MsgPort         *replyp; /* Some communication details ...       */
struct CPort           *cport;
struct CMessage        cmess;
struct MainPort        *myp;            /* Pointer to CNet port--ALL info!      */
struct PortData        *z;
char                     **bm;
struct Library         *CNetBase = NULL;


/************************************************************************
 *                             Main routine                             *
 ************************************************************************/
void main( int argc, char **argv )
{

/* Definitions */

/* End of definitions */

        Forbid();
        if(argc > 1)
                cport = (struct CPort *)FindPort( argv[1] );
        Permit();

        if( argc<2 || !(cport) )
        {
                Printf("This is a CNet C program.\n");
                exit(0);
        }

        if( !(replyp = CreatePort( 0,0 )))
                exit(0);

        cmess.cn_Message.mn_ReplyPort   = replyp;
        cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
        cmess.cn_Message.mn_Node.ln_Name= "cstuff";

#ifdef CNET4
        // CNet version check
        if( cport->ack != 30 ) {        // change "30" to be compatible with only pre 4.13 systems
                cport->ack = 1;
                LoadError();
                }
#else
        if( cport->ack != 40 )      // Replace the code above with this
        {
           if( cport->ack != 30 )   // to support both 3.x and 4.x systems
           {
              cport->ack = 1;
              LoadError();
           }
        }
#endif

        cport->ack = 0;

        z    = cport->zp;
        myp  = cport->myp;
        bm   = z->bm;

#ifdef CNET3
        if( !(CNetBase = OpenLibrary( "cnet.library", 3 )) ) // <- NOTE version 4 - can be changed to 3 to make compatible with older CNet releases!
                LoadError();
#else
        if( !(CNetBase = OpenLibrary( "cnet.library", 4 )) ) // <- NOTE version 4 - can be changed to 3 to make compatible with older CNet releases!
                LoadError();
#endif

/************************************************************************
 *           End of CNet setup - YOUR CUSTOM CODE STARTS HERE           *
 ************************************************************************/

        DoReturn();
        PutText("This utility will purge Mail:Users/ of orphaned mail directories.");
        DoReturn();

        if(PutQ("Begin scan [Yes]? ?1"))
        {
           DoReturn();
           uucp_init();
        }

        /* exit back to CNet - always call GetOut() to exit back to CNet! */
        GetOut();
}


void uucp_init(void)
{
   struct UDirEnt *ud;           /* pointer to an array of UDirEnt structs */

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
         if((*myp->Key[i].Handle != '!') || (*myp->Key[i].UUCP == 0))
                                    /* ! and NULL are not valid handles  =) */
         {
            strcpy(ud[j].UUCP,myp->Key[i].UUCP);   /* copy key value to array element */
            j++;                                   /* increment array value           */
         }
      }
   }
   else
   {
      PutText("Insufficient memory");
      DoReturn();
      GetOut();
   }

   if(lock = Lock("Mail:Users",ACCESS_READ))    /* Get a lock on the dir   */
   {
      if(Examine(lock,z->fib))                  /* pull a FIB from it      */
      {
         while(ExNext(lock,z->fib))             /* while I can pull a FIB  */
         {
            if(z->fib->fib_DirEntryType > 0)    /* if it's a directory     */
            {
               found = FALSE;                   /* not found yet           */

               sprintf(z->ABuffer,"Checking %s...",z->fib->fib_FileName);
               PutA();

               if(!Stricmp(z->fib->fib_FileName,"SYSOP")) /* don't wanna kill this one */
               {
                  PutText("found.");
                  DoReturn();
                  continue;
               }

               for(i = 0; i < j; i++)              /* loop thru array of ids  */
               {
                  if(ud[i].Found == TRUE)          /* we've already found this one  */
                     continue;

                  if(!stricmp(ud[i].UUCP,z->fib->fib_FileName)) /* we found it! */
                  {
                     ud[i].Found = TRUE;        /* keep from trying this one again  */
                     sprintf(z->ABuffer,"found.",ud[i].UUCP);
                     PutA();
                     DoReturn();
                     found = TRUE;              /* keep from saying we missed it */
                     break;                     /* break from the loop           */
                  }
               }
               if(found != TRUE)                /* didn't find a damn thing      */
               {
                  if(PutQ("not found.  Delete [Yes]? ?1")) /* kill it? */
                  {
                     sprintf(z->ABuffer,"Delete Mail:Users/%s ALL FORCE",z->fib->fib_FileName);
                     Execute(z->ABuffer,NULL,NULL);
                  }
               }
            }
         }
      }
      UnLock(lock);     /* unlock the directory */
   }

   FreeVec(ud);         /* free the array       */
}

/**************************************************************************
 *       Routine called if Load error (wrong CNet version, etc,...        *
 **************************************************************************/
void LoadError( void )
{
        DeletePort( replyp );
        exit(0);
}

/************************************************************************
 *                           Generic EXIT code                          *
 ************************************************************************/
void GetOut( void )
{
   ShutDown( NULL );
   if(CNetBase)
      CloseLibrary(CNetBase);
   DeletePort( replyp );
   exit(0);
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
 *        Sets the "Action" or "Where" field for the user's port          *
 *      ** remember to retain and restore the previous z->DOING **        *
 **************************************************************************/
void SetDoing( char *what )
{
        cmess.arg1 = (ULONG)what;
        CallHost( 7 );
}

/**************************************************************************
 * Print the prompt string passed as "a" and return the user's            *
 * YES or NO equivalent result.  1=YES, 2=NO                              *
 **************************************************************************/
UBYTE PutQ( char *a )
{
        PutText( a );
        return (UBYTE)(z->MCIcreg[0][0]=='1') ;
}

/**************************************************************************
 * Isn't it obvious enough? ;-)
 **************************************************************************/
void DoReturn( void )
{
        PutText("\n");
}
