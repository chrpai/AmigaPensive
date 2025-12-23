#include "cnet4lib.h"

void ShutDown( char *spawn )
{
        /* spawn = full path/file to run */
        if( spawn )
                strcpy( z->CSpawn, spawn );

        CallHost( 0 );
}
