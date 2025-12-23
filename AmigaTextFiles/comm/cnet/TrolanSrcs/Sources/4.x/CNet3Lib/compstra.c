#include "cnet3lib.h"

short compstra( char *s, char *t )
{
        for( ; tolower(*s) == tolower(*t); s++, t++)
                if( !*s ) return 0;

        return (short)( tolower(*s)-tolower(*t) );
}
