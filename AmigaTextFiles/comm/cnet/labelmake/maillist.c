/****************************************************************************
                                 MailList Maker

 DOX: Run it as a C Pfile.  Doesn't get much harder than that.  It will zoom
 thru your userlist and make a textfile in RAM: (called "maillist") which you
 may then copy to prt: to print out mailing labels.
 
 However - this makes it of ALL ACOUNTS!  If they have no address, then you
 can A: Edit the file and remove them, or B: waste the label.
 
 AGREEMENT OF USE: This file may only be run on legal C-Net AMIGA systems.
 This source code and it's executable are ONLY to be distributed in this
 original form with **NONE** of your modifications.  You may edit this for
 your own use, but not re-distribute those changes.
 
 I will not be held responsible if this erases your hard drive, crashes your
 computer, etc... blah blah blah, there, my butt is covered.  No more legal
 talk, I promise!
 ****************************************************************************
       Copyright © 1991 The Brazilian / Bob Maple      //\ Software
 ****************************************************************************
 I can be reached in the following ways:        USENET: bmaple@isis.cs.du.edu
 						FW ID#: 360/The Brazilian
 						BBS   : 303/697-8156 [14400]
 ****************************************************************************/


struct MsgPort  *replyp;
struct CPort    *cport;
struct CMessage  cmess;

struct   MainPort  *myp;
struct   PortData  *z;

main( int argc, char **argv )
{

   FILE *input;
   FILE *output;
   int i;

   if( argc<2 || !(cport = (struct CPort *)FindPort( argv[1] )) ) {
      printf("Must be ran as a C-Net Pfile!\n");
      exit(0);
   }

   if( !(replyp=CreatePort( 0,0 )) )
      exit(0);

   cmess.cn_Message.mn_ReplyPort   = replyp;
   cmess.cn_Message.mn_Length      = sizeof( struct CMessage );
   cmess.cn_Message.mn_Node.ln_Name= "cstuff";


   if( cport->ack != 97 ) {
       cport->ack = 1;
       goto err;
   }

   cport->ack = 0;

   z    =   cport->zp;
   myp  =   cport->myp;

   /* PLEASE DO NOT REMOVE THIS!  SHOW A LITTLE RESPECT... I WOULDN'T RIP
      YOUR NAME OUT OF YOUR OWN PROGRAM.  */

   PutText( "\\f1\\cdMailList Maker\n\\c5v1.0 (c) 1991 The Brazilian\n" );
   
   EnterLine( 1, 1, "\n\\cfDo you really want to do this?: \\c7" );
   if( *z->InBuffer == 'Y' )
   {
   
     PutText( "\n\\caWorking.. [    ]" );
     
     if( (input = fopen( "system:bbs.udata", "r" )) == NULL )
     {
       PutText( "Error! Couldn't find userfile!!!\n" );
       goto exit;
     }
     
     output = fopen( "ram:maillist", "w" );
     
     for( i=1; i <= *myp->Nums; i++ )
     {
       sprintf( z->ABuffer, "\\<5%4d]", i ); PutA();

       ReadAccount( i, &z->user2 );
       
       fprintf( output, "%s\n%s\n%s  %s\n\n\n\n", z->user2.RealName,  
                        z->user2.Address, z->user2.CityState, z->user2.ZipCode );
     }
     fclose( input );
     fclose( output );
     
   }    

exit:
      ShutDown( NULL );

err:  DeletePort( replyp );
      exit(0);
}

void ShutDown( char *spawn )
{
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

void PutText( UBYTE *text )
{
   cmess.arg1 = (ULONG)text;   /* text to print      */
   CallHost( 1 );
}

void PutA( void )
{
   PutText( z->ABuffer );
}

LONG EnterLine( UBYTE len, SHORT flags, UBYTE *prompt )
{
   cmess.arg1 = (ULONG)len;
   cmess.arg2 = (ULONG)flags;

   cmess.arg3 = (ULONG)prompt;
   CallHost( 2 );
   return( (LONG)strlen( z->InBuffer ));
}

void ReadAccount( SHORT id, struct UserData *user )
{
   cmess.arg1 = (ULONG)id;
   cmess.arg2 = (ULONG)user;
   CallHost( 11 );
}

