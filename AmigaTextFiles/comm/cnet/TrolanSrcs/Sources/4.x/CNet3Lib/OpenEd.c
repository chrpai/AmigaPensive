#include "cnet3lib.h"

BPTR OpenEd( long mode )
{
        char    filename[80];

        MakeEd( filename );

        return Open( filename, mode );
}
