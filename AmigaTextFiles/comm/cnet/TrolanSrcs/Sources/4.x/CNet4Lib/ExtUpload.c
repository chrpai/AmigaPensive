#include "cnet4lib.h"

char *ExtUpload( char *args )
{
        cmess.arg1 = (ULONG)args;
        CallHost( 45 );
        return( (char *)cmess.result );
}
