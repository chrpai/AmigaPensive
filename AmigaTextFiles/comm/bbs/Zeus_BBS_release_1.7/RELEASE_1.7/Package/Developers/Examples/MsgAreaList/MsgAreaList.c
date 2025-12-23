
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/lists.h>
#include <exec/nodes.h>

#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/rdargs.h>

#include <rexx/storage.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>

#include <plfile.h>
#include <plmsg.h>
#include <pipeline.h>
#include <pipeutil.h>

#include <nodes.h>

struct	Library		*PipeUtilBase;
struct	MsgGlobals	*globals;

void
main ( int argc, char **argv )
{
	struct	MsgAreaNode	*node;

	if ( PipeUtilBase = OpenLibrary ( "zmf.library", 0L ) )
	{
		if ( globals = GetMsgGlobals ( ) )
		{
			if ( node = (struct MsgAreaNode *) GetSucc ( (struct Node *) globals->mg_Config->AreaList ) )
			{
				do
				{
					Printf ( "%ld %s\n", node->area->number, node->area->name ) ;
				}
				while ( node = (struct MsgAreaNode *) GetSucc ( (struct Node *) node ) )
					;
			}
		}
		CloseLibrary ( PipeUtilBase ) ;
	}
}