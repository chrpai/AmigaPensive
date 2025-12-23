#include "cnet4lib.h"

/**************************************************************************
 * list the contents of a directory (the user will be prompted for        *
 * directory name).                                                       *
 *                                                                        *
 * options: a = allow selection of files to user's select list            *
 *                                                                        *
 *          b = 1: select and download immediately                        *
 *              2: allow wildcard pattern                                 *
 *                                                                        *
 *          c = list files newer than the date specified by c             *
 *              c is a properly filled out Cnet IsDate structure          *
 **************************************************************************/
long ListDir( UBYTE a, UBYTE b, struct IsDate *c )
{
        cmess.arg1 = (ULONG)a;
        cmess.arg2 = (ULONG)b;
        cmess.arg3 = (ULONG)c;
        CallHost( 22 );
        return( (long)cmess.result );
}
