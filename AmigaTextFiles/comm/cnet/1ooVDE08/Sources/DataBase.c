/*****************************************************************************

 Database.c       - sample database using VDE
                  > copy "MyDataBase.vde" to "CNet:Systext/VDE/"

                  bad demo source by Devil/1oo%

                  Part of the C/VDEd distribution!

 *****************************************************************************/

#include "General.H"
#include "CNetFunctions.C"


#define  MDBFILE      "Ram:MyDataBase.mdb"
#define  VDEFILE      "MyDataBase.vde"

struct MyDataBase
{
                                // OFFSET DESC
  char          Handle[ 22 ];   //      0 user's handle
  char        RealName[ 32 ];   //     22 user's real name
  char           Phone[ 26 ];   //     54 user's phone#
  struct IsDate     BirthDay;   //     80 user's birthdate
  UBYTE              Married;   //     86 married?
  UBYTE               Childs;   //     87 number of childs
  UBYTE               Gender;   //     88 user's gender
  UBYTE                dummy;   //     89 <just to keep the struct straight>
  LONG                 Flags;   //     90 some flags
  LONG               HighNum;   //     94 a huge number >= 666
                                // ======
                                //     98 bytes length
} mdb;




int main( int argc, char **argv)
{
  LONG               fh, lock;


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


  PutText( "Loading database from file... ");

  /* check for existing file - otherwise fill in some stuff */

  if( lock = Lock( MDBFILE, ACCESS_READ))
    {
      fh = OpenFromLock( lock);
      Read( fh, &mdb, sizeof( struct MyDataBase));
      Close( fh);

      PutText( "done!\n");
    }
  else
    {
      strcpy( mdb.Handle,      "Devil");
      strcpy( mdb.RealName,    "Chris ...");
      strcpy( mdb.Phone,       "+49-9131-66661");

      mdb.BirthDay.Year   = 78;
      mdb.BirthDay.Month  = 3;
      mdb.BirthDay.Date   = 11;
      mdb.BirthDay.Hour   = 12;
      mdb.BirthDay.Minute = 47;
      mdb.BirthDay.Second = 24;

      mdb.Married         = FALSE;
      mdb.Gender          = 1;
      mdb.Flags           = 0xFFFFFFFF;
      mdb.HighNum         = 666;

      PutText( "failed - using defaults!\n");
    }

  PutText( "\nPress any key: g0");

  /* now edit the "database" */

  VisualDataEditor( VDEFILE, &mdb, sizeof( struct MyDataBase));



  /* save the data */

  fh = Open( MDBFILE, MODE_NEWFILE);
  Write( fh, &mdb, sizeof( struct MyDataBase));
  Close( fh);




  /**** clean up *******/

  ShutDown( NULL);
  DeletePort( replyp);
  CloseLibrary( (struct Library *)CNetBase);
}
