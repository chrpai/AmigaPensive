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

LONG __saveds NodeFrontEndProc()
	{
		struct ConfirmationMessage *msg;
		struct CommandMessage *qmsg=NULL;
		struct Process *proc;
		proc=(struct Process *) FindTask(NULL);
		WaitPort(&proc->pr_MsgPort);
		if(msg=(struct ConfirmationMessage *) GetMsg(&proc->pr_MsgPort))
			{
				BYTE name[64],frontend_name[64],id[33];
				struct MsgPort *port,*frontend_port;
				BOOL ok=TRUE;
				strcpy(id,msg->cm_ID);
				sprintf(name,"WWBBS Node.%s",id);
				sprintf(frontend_name,"WWBBS Front End.%s",id);
				if(!(port=CreatePort(name,0)))
					ok=FALSE;
				if(!(frontend_port=CreatePort(frontend_name,0)))
					ok=FALSE;
				msg->cm_Return=ok;
				ReplyMsg((struct Message *) msg);
				if(ok)
					{
						struct Libraries *libs;
						if(libs=OpenLibraries())
							{
								BOOL node_autobaud=FALSE;
								GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_AutoBaud,&node_autobaud,TAG_END);
								if(StartProcess("WWBBS Serial",SerialProc,20000,id))
									{
										BOOL kg=TRUE;
										while(kg)
											{
												Wait( (1 << port->mp_SigBit) | (1 << frontend_port->mp_SigBit) );
												{
													struct CommandMessage *msg;
													ULONG data;
													while(kg && (msg=(struct CommandMessage *) GetMsg(port)))
														{
															data=msg->cm_Data;
															msg->cm_Data=NULL;
															switch(msg->cm_Command)
																{
																	case NDCMD_Quit:
																		msg->cm_Data=TRUE;
																		qmsg=msg;
																		kg=FALSE;
																		break;
																}
															if(!qmsg)
																ReplyMsg((struct Message *) msg);
														}
												}
												{
													struct CommandMessage *msg;
													ULONG data;
													while(kg && (msg=(struct CommandMessage *) GetMsg(frontend_port)))
														{
															data=msg->cm_Data;
															msg->cm_Data=NULL;
															switch(msg->cm_Command)
																{
																	case NDCMD_Session:
																		{
																			ULONG baud=0;
																			{
																				struct TagItem *tag;
																				if(tag=FindTagItem(NDSESSIONTAG_Baud,(struct TagItem *) data))
																					baud=(ULONG) tag->ti_Data;
																			}
																			if(SerialCommand(id,SERCMD_Open,TAG_END))
																				{
																					if(node_autobaud && baud)
																						SerialCommandTags(id,SERCMD_SetBaud,SERSETBAUDTAG_NewBaud,baud,TAG_END);
																					Session(libs,id,FALSE,baud);
																					SerialCommand(id,SERCMD_Close,TAG_END);
																				}
																		}
																		msg->cm_Data=TRUE;
																		break;
																}
															if(!qmsg)
																ReplyMsg((struct Message *) msg);
														}
												}
											}
										KillProcess("WWBBS Serial",id);
									}
								CloseLibraries(libs);
							}
						DeletePort(frontend_port);
						DeletePort(port);
					}
			}
		if(qmsg)
			{
				Forbid();
				ReplyMsg((struct Message *) qmsg);
			}
		return(0);
	}
