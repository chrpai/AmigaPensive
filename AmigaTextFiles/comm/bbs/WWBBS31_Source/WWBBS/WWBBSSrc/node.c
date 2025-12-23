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

BOOL __saveds GetModemResult(struct Libraries *,BYTE *,char **,char *,long);
BOOL __saveds GetModemResultWait(struct Libraries *,BYTE *,char **,char *,long);
int __saveds SerialRead(struct Libraries *,BYTE *,char *,long,long,long);

LONG __saveds NodeProc()
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
				sprintf(name,"WWBBS Node.%s",id);
				if(!(port=CreatePort(name,0)))
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
												if(SerialCommandTags(id,SERCMD_Open,TAG_END))
													{
														/* Check carrier, if present hang up... */
														{
															char *matchstring[]={"OK",NULL};
															if(SerialCommandTags(id,SERCMD_Carrier,TAG_END))
																{
																	SerialCommandTags(id,SERCMD_Write,
																		SERWRITETAG_Buffer,"+++",
																		SERWRITETAG_Size,-1,
																		TAG_END);
																	GetModemResult(libs,id,matchstring,NULL,5000000);
																	SerialCommandTags(id,SERCMD_Write,
																		SERWRITETAG_Buffer,"ATH\r",
																		SERWRITETAG_Size,-1,
																		TAG_END);
																	GetModemResult(libs,id,matchstring,NULL,5000000);
																}
															SerialCommandTags(id,SERCMD_SetBaud,SERSETBAUDTAG_ResetBaud,TRUE,TAG_END);
															SerialCommandTags(id,SERCMD_Write,
																SERWRITETAG_Buffer,"ATZ\r",
																SERWRITETAG_Size,-1,
																TAG_END);
															GetModemResult(libs,id,matchstring,NULL,5000000);
														}

														/* Wait if no carrier present... */
														if(!SerialCommandTags(id,SERCMD_Carrier,TAG_END))
															{
																BOOL kg2=TRUE;
																char *matchstring[]={"RING","CONNECT",NULL};
																char buff[256];
																while(kg && kg2)
																	{
																		if(GetModemResultWait(libs,id,matchstring,buff,1 << port->mp_SigBit))
																			{
																				if(!strncmp(buff,"RING",4))
																					SerialCommandTags(id,SERCMD_Write,
																						SERWRITETAG_Buffer,"ATA\r",
																						SERWRITETAG_Size,-1,
																						TAG_END);
																				if(!strncmp(buff,"CONNECT",7))
																					{
																						ULONG baud=0;
																						{
																							char *p;
																							if(p=strchr(buff,'\r'))
																								*p=NULL;
																							if(p=strchr(buff,'\n'))
																								*p=NULL;
																						}
																						if(!strcmp(buff,"CONNECT"))
																							baud=300;
																						else
																							baud=atol(&buff[8]);
																						if(node_autobaud && baud)
																							SerialCommandTags(id,SERCMD_SetBaud,SERSETBAUDTAG_NewBaud,baud,TAG_END);
																						Session(libs,id,FALSE,baud);
																						kg2=FALSE;
																					}
																			}
																		{
																			struct CommandMessage *msg;
																			while(kg && kg2 && (msg=(struct CommandMessage *) GetMsg(port)))
																				{
																					msg->cm_Data=NULL;
																					switch(msg->cm_Command)
																						{
																							case NDCMD_Quit:
																								msg->cm_Data=TRUE;
																								qmsg=msg;
																								kg2=FALSE;
																								kg=FALSE;
																								break;
																						}
																					if(!qmsg)
																						ReplyMsg((struct Message *) msg);
																				}
																		}
																	}
															}
														SerialCommandTags(id,SERCMD_Close,TAG_END);
														Delay(50L);
													}
												else
													{
														WaitPort(port);
														{
															struct CommandMessage *msg;
															while(kg && (msg=(struct CommandMessage *) GetMsg(port)))
																{
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
													}
											}
										KillProcess("WWBBS Serial",id);
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

BOOL __saveds GetModemResult(struct Libraries *libs,BYTE *id,char **matchstring,char *destbuff,long timeout)
	{
		char buffer[256];
		int len,kg=TRUE;
		BOOL ret=FALSE;
		while(kg)
			{
				if(len=SerialRead(libs,id,buffer,255,timeout,NULL))
					{
						int count;
						while(1)
							{
								if(len==255)
									break;
								count=SerialRead(libs,id,&buffer[len],255-len,500000,NULL);
								if(!count)
									break;
								len+=count;
							}
					}
				if(len)
					{
						int i,j;
						buffer[len]=NULL;
						for(i=0;i<strlen(buffer);i++)
							{
								for(j=0;matchstring[j];j++)
									{
										if(!strncmp(&buffer[i],matchstring[j],strlen(matchstring[j])))
											{
												if(destbuff)
													strcpy(destbuff,&buffer[i]);
												ret=TRUE;
												kg=FALSE;
											}
										if(!kg)
											break;
									}
								if(!kg)
									break;
							}
					}
				else
					kg=FALSE;
			}
		return(ret);
	}

BOOL __saveds GetModemResultWait(struct Libraries *libs,BYTE *id,char **matchstring,char *destbuff,long mask)
	{
		char buffer[256];
		int len,kg=TRUE;
		BOOL ret=FALSE;
		while(kg)
			{
				if(len=SerialRead(libs,id,buffer,255,NULL,mask))
					{
						int count;
						while(1)
							{
								if(len==255)
									break;
								count=SerialRead(libs,id,&buffer[len],255-len,500000,NULL);
								if(!count)
									break;
								len+=count;
							}
					}
				if(len)
					{
						int i,j;
						buffer[len]=NULL;
						for(i=0;i<strlen(buffer);i++)
							{
								for(j=0;matchstring[j];j++)
									{
										if(!strncmp(&buffer[i],matchstring[j],strlen(matchstring[j])))
											{
												if(destbuff)
													strcpy(destbuff,&buffer[i]);
												ret=TRUE;
												kg=FALSE;
											}
										if(!kg)
											break;
									}
								if(!kg)
									break;
							}
					}
				else
					kg=FALSE;
			}
		return(ret);
	}

int __saveds SerialRead(struct Libraries *libs,BYTE *id,char *buffer,long size,long timeout,long waitmask)
	{
		ULONG mask;
		int ret=0;
		ret=SerialCommandTags(id,SERCMD_Read,
					SERREADTAG_Buffer,buffer,
					SERREADTAG_Size,size,
					TAG_END);
		if(!ret)
			{
				if(waitmask)
					ret=SerialCommandTags(id,SERCMD_Wait,
								SERWAITTAG_WaitMask,waitmask,
								SERWAITTAG_WaitMaskRet,&mask,
								SERWAITTAG_Buffer,buffer,
								SERWAITTAG_Size,size,
								TAG_END);
				else
					{
						struct MsgPort *TimerMP;
						struct timerequest *TimerIO;
						if(TimerMP=CreateMsgPort())
							{
								if(TimerIO=(struct timerequest *) CreateIORequest(TimerMP,sizeof(struct timerequest)))
									{
										if(!OpenDevice(TIMERNAME,UNIT_VBLANK,(struct IORequest *) TimerIO,0))
											{
												TimerIO->tr_node.io_Command=TR_ADDREQUEST;
												TimerIO->tr_time.tv_secs=timeout/1000000;
												TimerIO->tr_time.tv_micro=timeout%1000000;
												SendIO((struct IORequest *) TimerIO);
												{
													BOOL kg=TRUE;
													while(kg)
														{
															ret=SerialCommandTags(id,SERCMD_Wait,
																		SERWAITTAG_WaitMask,(1 << TimerMP->mp_SigBit) | waitmask,
																		SERWAITTAG_WaitMaskRet,&mask,
																		SERWAITTAG_Buffer,buffer,
																		SERWAITTAG_Size,size,
																		TAG_END);
															if(ret)
																kg=FALSE;
															if(CheckIO((struct IORequest *) TimerIO))
																kg=FALSE;
															if(mask)
																kg=FALSE;
														}
												}
												if(!CheckIO((struct IORequest *) TimerIO))
													AbortIO((struct IORequest *) TimerIO);
												WaitIO((struct IORequest *) TimerIO);
												CloseDevice((struct IORequest *) TimerIO);
											}
										DeleteIORequest((struct IORequest *) TimerIO);
									}
								DeleteMsgPort(TimerMP);
							}
					}
			}
		return(ret);
	}
