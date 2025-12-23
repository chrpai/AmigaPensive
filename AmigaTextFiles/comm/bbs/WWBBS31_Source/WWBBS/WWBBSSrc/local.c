#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wwbbs.h"

#include "wwbbs_settings.h"

#include "wwbbs_lib.h"

#include "node_lib.h"

#include "libs.h"

#include "Data:Programs/Include/wwbbs_commands.h"

LONG __saveds LocalLogonProc()
	{
		struct ConfirmationMessage *msg;
		struct Process *proc;
		proc=(struct Process *) FindTask(NULL);
		WaitPort(&proc->pr_MsgPort);
		if(msg=(struct ConfirmationMessage *) GetMsg(&proc->pr_MsgPort))
			{
				BYTE name[64],id[33];
				struct MsgPort *port;
				BOOL ok=TRUE;
				strcpy(id,msg->cm_ID);
				sprintf(name,"WWBBS Local.%s",id);
				if(!(port=CreatePort(name,0)))
					ok=FALSE;
				msg->cm_Return=ok;
				ReplyMsg((struct Message *) msg);
				if(ok)
					{
						struct Libraries *libs;
						if(libs=OpenLibraries())
							{
								Session(libs,id,TRUE,0);
								CloseLibraries(libs);
							}
						DeletePort(port);
					}
			}
		return(0);
	}
