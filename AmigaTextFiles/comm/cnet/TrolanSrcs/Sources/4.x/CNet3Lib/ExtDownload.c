#include "cnet3lib.h"

char *ExtDownload( char *args )
{
        cmess.arg1 = (ULONG)args;
        CallHost( 44 );
        return( (char *)cmess.result );
}
