#include "cnet4lib.h"

void PrepEditor( BPTR fp )
{
        BPTR    kp;
        char    buff[100];

        if( fp ) {
                if( kp = OpenEd( MODE_NEWFILE ) ) {
                        while( FGets( fp, buff, 82 ) && buff[0]!=26 )
                               FPuts( kp, buff     ) ;

                        Close( kp );
                }
        }
        else    DeleteEd();
}
