#include "cnet3lib.h"

long ConvertAccess( char *s )
{
        cmess.arg1 = (ULONG)s;
        CallHost( 18 );
        return( (long)cmess.result );
}
