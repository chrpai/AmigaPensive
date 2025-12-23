#include "cnet3lib.h"

void CallHost( UBYTE c )
{
        cmess.command = c;
        PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
        WaitPort( replyp );
        GetMsg  ( replyp );
}
