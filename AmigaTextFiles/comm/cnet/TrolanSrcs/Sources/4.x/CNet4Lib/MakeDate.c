#include "cnet4lib.h"

void MakeDate( struct IsDate *date, char *output )
{
        cmess.arg1 = (ULONG)date;
        cmess.arg2 = (ULONG)output;
        CallHost( 10 );
}
