#include "cnet3lib.h"

void DeleteEd( void )
{
        char    filename[80];

        MakeEd    ( filename ) ;
        DeleteFile( filename ) ;
}
