#include "wwbbs.h"

#include "Data:Programs/Include/wwbbs_commands.h"

ULONG DoCommandMessage(BYTE *basename,BYTE *id,UBYTE cmd,struct TagItem *tags)
	{
		ULONG ret=NULL;
		BYTE name[64];
		struct MsgPort *port=NULL,*replyport;
		struct CommandMessage msg={0};
		if(id)
			sprintf(name,"%s.%s",basename,id);
		else
			strcpy(name,basename);
		if(replyport=CreateMsgPort())
			{
				msg.cm_Message.mn_Node.ln_Type=NT_MESSAGE;
				msg.cm_Message.mn_Length=sizeof(struct CommandMessage);
				msg.cm_Message.mn_ReplyPort=replyport;
				msg.cm_Command=cmd;
				msg.cm_Data=(ULONG) tags;
				Forbid();
				if(port=FindPort(name))
					PutMsg(port,(struct Message *) &msg);
				Permit();
				if(port)
					{
						WaitPort(replyport);
						if(GetMsg(replyport))
							ret=msg.cm_Data;
					}
				DeleteMsgPort(replyport);
			}
		return(ret);
	}

ULONG SendCommandMessage(BYTE *basename,BYTE *id,UBYTE cmd,struct TagItem *tags,ULONG mask,ULONG *retmask,UBYTE abortcmd,BOOL justsignal)
	{
		ULONG ret=NULL;
		BYTE name[64];
		struct MsgPort *port=NULL,*replyport=NULL;
		struct CommandMessage msg={0};
		if(id)
			sprintf(name,"%s.%s",basename,id);
		else
			strcpy(name,basename);
		if(justsignal)
			{
				struct Task *task=NULL;
				UBYTE sigbit;
				Forbid();
				if(port=FindPort(name))
					{
						task=port->mp_SigTask;
						sigbit=port->mp_SigBit;
					}
				Permit();
				if(port)
					{
						Signal(task,sigbit);
						ret=TRUE;
					}
			}
		else
			{
				if(replyport=CreateMsgPort())
					{
						msg.cm_Message.mn_Node.ln_Type=NT_MESSAGE;
						msg.cm_Message.mn_Length=sizeof(struct CommandMessage);
						msg.cm_Message.mn_ReplyPort=replyport;
						msg.cm_Command=cmd;
						msg.cm_Data=(ULONG) tags;
						Forbid();
						if(port=FindPort(name))
							PutMsg(port,(struct Message *) &msg);
						Permit();
						if(port)
							{
								ULONG maskbuff;
								maskbuff=Wait( (1 << replyport->mp_SigBit) | mask);
								if(retmask)
									*retmask=maskbuff;
								if(maskbuff & (1 << replyport->mp_SigBit))
									{
										if(GetMsg(replyport))
											ret=msg.cm_Data;
									}
								else
									{
										DoCommandMessage(basename,id,abortcmd,NULL);
										GetMsg(replyport);
									}
							}
						DeleteMsgPort(replyport);
					}
			}
		return(ret);
	}
