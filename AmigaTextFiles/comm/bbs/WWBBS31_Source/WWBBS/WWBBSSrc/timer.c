#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <intuition/intuition.h>
#include <libraries/wwbbs.h>
#include <devices/timer.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "wwbbs.h"

#include "wwbbs_settings.h"

#include "wwbbs_lib.h"

#include "libs.h"

#include "Data:Programs/Include/wwbbs_commands.h"

LONG __saveds TimerProc()
	{
		struct ConfirmationMessage *msg;
		struct CommandMessage *qmsg=NULL;
		struct Process *proc;
		proc=(struct Process *) FindTask(NULL);
		WaitPort(&proc->pr_MsgPort);
		if(msg=(struct ConfirmationMessage *) GetMsg(&proc->pr_MsgPort))
			{
				BYTE name[64],id[33];
				struct MsgPort *port;
				BOOL ok=TRUE;
				strcpy(id,msg->cm_ID);
				sprintf(name,"WWBBS Timer.%s",id);
				if(!(port=CreatePort(name,0)))
					ok=FALSE;
				msg->cm_Return=ok;
				ReplyMsg((struct Message *) msg);
				if(ok)
					{
						struct Libraries *libs;
						if(libs=OpenLibraries())
							{
								struct MsgPort *TimerMP=NULL;
								struct timerequest *TimerIO=NULL;
								if(TimerMP=CreateMsgPort())
									{
										if(TimerIO=(struct timerequest *) CreateIORequest(TimerMP,sizeof(struct timerequest)))
											{
												if(!OpenDevice(TIMERNAME,UNIT_VBLANK,(struct IORequest *) TimerIO,0))
													{
														UBYTE node_type=NULL;
														ULONG mask,data;
														BOOL timeron=FALSE,kg=TRUE;
														int interval;
														GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_Type,&node_type,TAG_END);
														while(kg)
															{
																{
																	ULONG waitmask=NULL;
																	waitmask|=1 << port->mp_SigBit;
																	if(timeron)
																		waitmask|=1 << TimerMP->mp_SigBit;
																	mask=Wait(waitmask);
																}
																if(mask & (1 << port->mp_SigBit))
																	{
																		struct CommandMessage *msg;
																		while(kg && (msg=(struct CommandMessage *) GetMsg(port)))
																			{
																				data=msg->cm_Data;
																				msg->cm_Data=NULL;
																				switch(msg->cm_Command)
																					{
																						case TMRCMD_Quit:
																							msg->cm_Data=TRUE;
																							qmsg=msg;
																							kg=FALSE;
																							break;
																						case TMRCMD_Start:
																							if(!timeron)
																								{
																									UWORD timeremaining=0,timeused=0;
																									{
																										struct TagItem *tag;
																										if(tag=FindTagItem(TMRSTARTTAG_TimeRemaining,(struct TagItem *) data))
																											timeremaining=(UWORD) tag->ti_Data;
																										if(tag=FindTagItem(TMRSTARTTAG_TimeUsed,(struct TagItem *) data))
																											timeused=(UWORD) tag->ti_Data;
																									}
																									if(timeremaining)
																										SetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,timeremaining,TAG_END);
																									if(timeused)
																										SetStatusTags(STTAG_Name,id,STTAG_TimeUsed,timeused,TAG_END);
																									TimerIO->tr_node.io_Command=TR_ADDREQUEST;
																									TimerIO->tr_time.tv_secs=5;
																									TimerIO->tr_time.tv_micro=0;
																									SendIO((struct IORequest *) TimerIO);
																									interval=0;
																									timeron=TRUE;
																								}
																							break;
																						case TMRCMD_Stop:
																							if(timeron)
																								{
																									if(!CheckIO((struct IORequest *) TimerIO))
																										AbortIO((struct IORequest *) TimerIO);
																									WaitIO((struct IORequest *) TimerIO);
																									timeron=FALSE;
																								}
																							break;
																					}
																				if(!qmsg)
																					ReplyMsg((struct Message *) msg);
																			}
																	}
																if(timeron)
																	{
																		if(mask & (1 << TimerMP->mp_SigBit))
																			{
																				if(CheckIO((struct IORequest *) TimerIO))
																					{
																						WaitIO((struct IORequest *) TimerIO);
																						interval++;
																						if(interval==60/5)
																							{
																								UWORD timeremaining=0,timeused=0;
																								GetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,&timeremaining,STTAG_TimeUsed,&timeused,TAG_END);
																								if(timeremaining)
																									timeremaining--;
																								timeused++;
																								SetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,timeremaining,STTAG_TimeUsed,timeused,TAG_END);
																								if(!timeremaining)
																									timeron=FALSE;
																								interval=0;
																							}
																						if(timeron)
																							{
																								TimerIO->tr_node.io_Command=TR_ADDREQUEST;
																								TimerIO->tr_time.tv_secs=5;
																								TimerIO->tr_time.tv_micro=0;
																								SendIO((struct IORequest *) TimerIO);
																							}
																						else
																							{
																								while(!NodeCommandTags(id,NDCMD_Panic,TAG_END))
																									;
																							}
																					}
																			}
																	}
															}
														CloseDevice((struct IORequest *) TimerIO);
													}
												DeleteIORequest((struct IORequest *) TimerIO);
											}
										DeleteMsgPort(TimerMP);
									}
								CloseLibraries(libs);
							}
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
