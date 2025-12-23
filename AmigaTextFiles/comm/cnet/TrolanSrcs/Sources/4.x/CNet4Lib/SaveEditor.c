#include "cnet4lib.h"

void SaveEditor( BPTR fp, UBYTE eof )
{
        BPTR    kp;
        char    buff[100];

        if( kp = OpenEd( MODE_OLDFILE ) ) {
                while( FGets( kp, buff, 82 ) && buff[0]!=26 )
                       FPuts( fp, buff     ) ;

                Close( kp );

                DeleteEd();
        }

        if( eof ) FPuts( fp, "\032\n" );
}
