#include "wwbbs.h"

#include "Data:Programs/Include/wwbbs_update.h"

__asm
void SetUpdatePort(register __a0 struct MsgPort *port)
	{
		ObtainSemaphore(&UpdateSemaphore);
		UpdatePort=port;
		ReleaseSemaphore(&UpdateSemaphore);
	}

__asm
void SendUpdate(register __d0 UBYTE cmd,register __a0 BYTE *id)
	{
		ObtainSemaphoreShared(&UpdateSemaphore);
		if(UpdatePort)
			{
				struct UpdateMessage *msg;
				if(msg=AllocVec(sizeof(struct UpdateMessage),MEMF_CLEAR))
					{
						msg->um_Message.mn_Node.ln_Type=NT_MESSAGE;
						msg->um_Message.mn_Length=sizeof(struct UpdateMessage);
						msg->um_Command=cmd;
						if(id)
							strcpy(msg->um_ID,id);
						PutMsg(UpdatePort,(struct Message *) msg);
					}
			}
		ReleaseSemaphore(&UpdateSemaphore);
	}
