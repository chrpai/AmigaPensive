#include "cnet4lib.h"

short VisualDataEditor( char *file, void *data, long size )
{
        cmess.arg1 = (ULONG)file;
        cmess.arg2 = (ULONG)data;
        cmess.arg3 = (ULONG)size;
        CallHost( 40 );
        return( (short)cmess.result );
}
