#include "cnet3lib.h"

void CallEditor( short max, short inlines )
{
        cmess.arg1 = (ULONG)max;        /* Maximum number of lines (250)*/
        cmess.arg2 = (ULONG)inlines;    /* TRUE/FALSE use existing _edbuff? */
        CallHost( 8 );
}
