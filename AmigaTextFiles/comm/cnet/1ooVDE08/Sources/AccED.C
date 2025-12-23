/*****************************************************************************

 AccED.c        - simple replacement for CNet's "Edit Account" Command!

                  bad demo source by Devil/1oo%

                  Part of the C/VDEd distribution!

 *****************************************************************************/

#include "General.H"
#include "CNetFunctions.C"



int main( int argc, char **argv)
{
  short              accnr;


  /******** init stuff *****/

  Forbid();
  if( argc > 1)
    {
      cport = (struct CPort *)FindPort( argv[ 1 ]);
    }
  Permit();

  if( argc < 2 || !(cport))
    {
      printf( "This a CNetC program!\n");
      return( 20);
    }

  if( !(replyp = CreatePort( NULL, NULL)))
    {
      printf( "Couldn't create replyport!\n");
      return( 20);
    }

  cmess.cn_Message.mn_ReplyPort      = replyp;
  cmess.cn_Message.mn_Length         = sizeof( struct CMessage);
  cmess.cn_Message.mn_Node.ln_Name   = "cstuff";

  if( cport -> ack == 30)
    {
      cport -> ack = FALSE;
      z            = cport -> zp;
      myp          = cport -> myp;

      if( !(CNetBase = OpenLibrary( "cnet.library", NULL)))
        {
          printf( "Couldn't open library!\n");
          return( 20);
        }
    }
  else
    {
      return( 20);
    }


  /**** main code *******/


  if( EnterLine( 21, 8+16+128+512, "Enter handle: "))
    {
      accnr = FindAccount( z -> InBuffer, &z -> user2);

      if( accnr != NULL)
        {
          /* ok, we got an account. Call VDE! */

          VisualDataEditor( "udata", &z -> user2, sizeof( struct UserData));

          /*                ^^
               CNet's VDE file for "struct UserData"
          */

        }
    }
  




  /**** clean up *******/

  ShutDown( NULL);
  DeletePort( replyp);
  CloseLibrary( (struct Library *)CNetBase);
}
