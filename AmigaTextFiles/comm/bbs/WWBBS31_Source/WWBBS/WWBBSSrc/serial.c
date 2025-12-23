#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <intuition/intuition.h>
#include <libraries/wwbbs.h>
#include <devices/serial.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wwbbs.h"

#include "wwbbs_settings.h"

#include "wwbbs_lib.h"

#include "libs.h"

#include "Data:Programs/Include/wwbbs_commands.h"

#include "Data:Programs/Include/OwnDevUnit.h"

LONG __saveds SerialProc()
	{
		struct ConfirmationMessage *msg;
		struct CommandMessage *qmsg=NULL;
		struct Process *proc;
		proc=(struct Process *) FindTask(NULL);
		WaitPort(&proc->pr_MsgPort);
		if(msg=(struct ConfirmationMessage *) GetMsg(&proc->pr_MsgPort))
			{
				BYTE name[64],command_name[64],id[33];
				struct MsgPort *port,*command_port;
				BOOL ok=TRUE;
				strcpy(id,msg->cm_ID);
				sprintf(name,"WWBBS Serial.%s",id);
				if(!(port=CreatePort(name,0)))
					ok=FALSE;
				sprintf(command_name,"WWBBS Serial Command.%s",id);
				if(!(command_port=CreatePort(command_name,0)))
					ok=FALSE;
				msg->cm_Return=ok;
				ReplyMsg((struct Message *) msg);
				if(ok)
					{
						struct Libraries *libs;
						if(libs=OpenLibraries())
							{
								struct Library *OwnDevUnitBase;
								BYTE serial_device[33];
								ULONG serial_unit=0,serial_buffersize=0,serial_baud=0;
								BOOL serial_handshaking=FALSE,frontend=FALSE;
								strcpy(serial_device,"");
								OwnDevUnitBase=OpenLibrary(ODU_NAME,0);
								GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,
									NDTAG_Device,serial_device,
									NDTAG_Unit,&serial_unit,
									NDTAG_BufferSize,&serial_buffersize,
									NDTAG_Baud,&serial_baud,
									NDTAG_Handshaking,&serial_handshaking,
									NDTAG_FrontEnd,&frontend,
									TAG_END);
								{
									struct MsgPort *SerialMP=NULL;
									struct IOExtSer *SerialIO=NULL;
									BOOL seriallocked=FALSE;
									BYTE signal=-1;
									struct CommandMessage *cmsg=NULL;
									ULONG cdata=NULL;
									ULONG mask;
									BOOL serialopen=FALSE,kg=TRUE;
									while(kg)
										{
											{
												ULONG waitmask=NULL;
												waitmask|=1 << port->mp_SigBit;
												if(cmsg)
													waitmask|=(1 << SerialMP->mp_SigBit) | (1 << command_port->mp_SigBit);
												if(seriallocked)
													waitmask|=(1 << signal);
												mask=Wait(waitmask);
											}
											if(mask & (1 << port->mp_SigBit))
												{
													struct CommandMessage *msg;
													while(kg && (msg=(struct CommandMessage *) GetMsg(port)))
														{
															ULONG data;
															data=msg->cm_Data;
															msg->cm_Data=NULL;
															switch(msg->cm_Command)
																{
																	case SERCMD_Quit:
																		msg->cm_Data=TRUE;
																		qmsg=msg;
																		kg=FALSE;
																		break;
																	case SERCMD_Open:
																		{
																			SerialMP=NULL;
																			SerialIO=NULL;
																			if(strlen(serial_device) && serial_buffersize && serial_baud)
																				{
																					if(OwnDevUnitBase && !frontend)
																						{
																							seriallocked=FALSE;
																							signal=-1;
																							if( (signal=AllocSignal(-1)) != -1)
																								{
																									if(!LockDevUnit(serial_device,serial_unit,name,signal))
																										seriallocked=TRUE;
																									else
																										{
																											FreeSignal(signal);
																											signal=-1;
																										}
																								}
																						}
																					if(!OwnDevUnitBase || frontend || seriallocked)
																						{
																							if(SerialMP=CreateMsgPort())
																								{
																									if(SerialIO=(struct IOExtSer *) CreateIORequest(SerialMP,sizeof(struct IOExtSer)))
																										{
																											if(serial_handshaking)
																												SerialIO->io_SerFlags|=SERF_7WIRE;
																											else
																												SerialIO->io_SerFlags&=~SERF_7WIRE;
																											SerialIO->io_SerFlags|=SERF_SHARED;
																											if(!OpenDevice(serial_device,serial_unit,(struct IORequest *) SerialIO,0))
																												{
																													serialopen=TRUE;
																													msg->cm_Data=TRUE;
																												}
																										}
																								}
																							if(!serialopen)
																								{
																									if(SerialIO) DeleteIORequest((struct IORequest *) SerialIO);
																									if(SerialMP) DeleteMsgPort(SerialMP);
																								}
																						}
																				}
																		}
																		break;
																	case SERCMD_Close:
																		if(serialopen)
																			{
																				if(cmsg)
																					{
																						if(!CheckIO((struct IORequest *) SerialIO))
																							AbortIO((struct IORequest *) SerialIO);
																						WaitIO((struct IORequest *) SerialIO);
																						ReplyMsg((struct Message *) cmsg);
																						cmsg=NULL;
																					}
																				CloseDevice((struct IORequest *) SerialIO);
																				DeleteIORequest((struct IORequest *) SerialIO);
																				DeleteMsgPort(SerialMP);
																				serialopen=FALSE;
																				if(OwnDevUnitBase && !frontend)
																					{
																						FreeDevUnit(serial_device,serial_unit);
																						FreeSignal(signal);
																						signal=-1;
																						seriallocked=FALSE;
																					}
																			}
																		break;
																	case SERCMD_Read:
																		if(!cmsg && data)
																			{
																				APTR buffer=NULL;
																				ULONG size=0;
																				{
																					struct TagItem *tag;
																					if(tag=FindTagItem(SERREADTAG_Buffer,(struct TagItem *) data))
																						buffer=(APTR) tag->ti_Data;
																					if(tag=FindTagItem(SERREADTAG_Size,(struct TagItem *) data))
																						size=tag->ti_Data;
																				}
																				if(buffer && size)
																					{
																						ULONG count;
																						SerialIO->IOSer.io_Command=SDCMD_QUERY;
																						DoIO((struct IORequest *) SerialIO);
																						count=SerialIO->IOSer.io_Actual;
																						if(count)
																							{
																								SerialIO->IOSer.io_Length=(count>size) ? size : count;
																								SerialIO->IOSer.io_Data=buffer;
																								SerialIO->IOSer.io_Command=CMD_READ;
																								DoIO((struct IORequest *) SerialIO);
																								msg->cm_Data=SerialIO->IOSer.io_Actual;
																							}
																					}
																			}
																		break;
																	case SERCMD_Write:
																		if(!cmsg && data)
																			{
																				APTR buffer=NULL;
																				ULONG size=0;
																				{
																					struct TagItem *tag;
																					if(tag=FindTagItem(SERWRITETAG_Buffer,(struct TagItem *) data))
																						buffer=(APTR) tag->ti_Data;
																					if(tag=FindTagItem(SERWRITETAG_Size,(struct TagItem *) data))
																						size=tag->ti_Data;
																				}
																				if(buffer && size)
																					{
																						SerialIO->IOSer.io_Data=buffer;
																						SerialIO->IOSer.io_Length=size;
																						SerialIO->IOSer.io_Command=CMD_WRITE;
																						DoIO((struct IORequest *) SerialIO);
																						msg->cm_Data=(ULONG) TRUE;
																					}
																			}
																		break;
																	case SERCMD_Wait:
																		if(!cmsg && data)
																			{
																				APTR buffer=NULL;
																				ULONG size=0;
																				{
																					struct TagItem *tag;
																					if(tag=FindTagItem(SERWAITTAG_Buffer,(struct TagItem *) data))
																						buffer=(APTR) tag->ti_Data;
																					if(tag=FindTagItem(SERWAITTAG_Size,(struct TagItem *) data))
																						size=tag->ti_Data;
																				}
																				if(buffer && size)
																					{
																						cmsg=msg;
																						cdata=data;
																						SerialIO->IOSer.io_Length=1;
																						SerialIO->IOSer.io_Data=buffer;
																						SerialIO->IOSer.io_Command=CMD_READ;
																						SendIO((struct IORequest *) SerialIO);
																					}
																			}
																		break;
																	case SERCMD_Abort:
																		if(cmsg)
																			{
																				if(!CheckIO((struct IORequest *) SerialIO))
																					AbortIO((struct IORequest *) SerialIO);
																				WaitIO((struct IORequest *) SerialIO);
																				ReplyMsg((struct Message *) cmsg);
																				cmsg=NULL;
																			}
																		break;
																	case SERCMD_SetBaud:
																		if(!cmsg && data)
																			{
																				BOOL resetbaud=FALSE;
																				ULONG newbaud=0;
																				{
																					struct TagItem *tag;
																					if(tag=FindTagItem(SERSETBAUDTAG_ResetBaud,(struct TagItem *) data))
																						resetbaud=tag->ti_Data;
																					if(tag=FindTagItem(SERSETBAUDTAG_NewBaud,(struct TagItem *) data))
																						newbaud=tag->ti_Data;
																				}
																				SerialIO->io_RBufLen=serial_buffersize;
																				SerialIO->io_ReadLen=8;
																				SerialIO->io_WriteLen=8;
																				SerialIO->io_StopBits=1;
																				if(resetbaud)
																					{
																						SerialIO->io_Baud=serial_baud;
																						msg->cm_Data=serial_baud;
																					}
																				else
																					{
																						if(newbaud)
																							{
																								SerialIO->io_Baud=newbaud;
																								msg->cm_Data=newbaud;
																							}
																					}
																				SerialIO->io_SerFlags|=SERF_RAD_BOOGIE;
																				SerialIO->IOSer.io_Command=SDCMD_SETPARAMS;
																				DoIO((struct IORequest *) SerialIO);
																			}
																		break;
																	case SERCMD_Carrier:
																		SerialIO->IOSer.io_Command=SDCMD_QUERY;
																		DoIO((struct IORequest *) SerialIO);
																		if(!(SerialIO->io_Status & (1L << 5)))
																			msg->cm_Data=TRUE;
																		break;
																	case SERCMD_Query:
																		if(!cmsg)
																			{
																				SerialIO->IOSer.io_Command=SDCMD_QUERY;
																				DoIO((struct IORequest *) SerialIO);
																				msg->cm_Data=SerialIO->IOSer.io_Actual;
																			}
																		break;
																}
															if(!qmsg && cmsg!=msg)
																ReplyMsg((struct Message *) msg);
														}
												}
											if(seriallocked)
												{
													if(mask & (1 << signal))
														{
															FreeDevUnit(serial_device,serial_unit);
															Delay(50L);
															while(LockDevUnit(serial_device,serial_unit,name,signal))
																;
														}
												}
											if(serialopen)
												{
													if(mask & (1 << SerialMP->mp_SigBit))
														{
															if(CheckIO((struct IORequest *) SerialIO))
																{
																	WaitIO((struct IORequest *) SerialIO);
																	if(cmsg && cdata)
																		{
																			BYTE *buffer=NULL;
																			ULONG size=NULL;
																			{
																				struct TagItem *tag;
																				if(tag=FindTagItem(SERWAITTAG_Buffer,(struct TagItem *) cdata))
																					buffer=(APTR) tag->ti_Data;
																				if(tag=FindTagItem(SERWAITTAG_Size,(struct TagItem *) cdata))
																					size=tag->ti_Data;
																			}
																			if(size>1)
																				{
																					ULONG count;
																					SerialIO->IOSer.io_Command=SDCMD_QUERY;
																					DoIO((struct IORequest *) SerialIO);
																					count=SerialIO->IOSer.io_Actual;
																					if(count)
																						{
																							SerialIO->IOSer.io_Length=(count>size-1) ? size-1 : count;
																							SerialIO->IOSer.io_Data=(APTR) &buffer[1];
																							SerialIO->IOSer.io_Command=CMD_READ;
																							DoIO((struct IORequest *) SerialIO);
																							cmsg->cm_Data=SerialIO->IOSer.io_Actual+1;
																						}
																					else
																						cmsg->cm_Data=1;
																				}
																			else
																				cmsg->cm_Data=1;
																			ReplyMsg((struct Message *) cmsg);
																			cmsg=NULL;
																		}
																}
														}
													if(mask & (1 << command_port->mp_SigBit))
														{
															struct CommandMessage *msg;
															ULONG data;
															while(msg=(struct CommandMessage *) GetMsg(command_port))
																{
																	data=msg->cm_Data;
																	msg->cm_Data=NULL;
																	switch(msg->cm_Command)
																		{
																			case NDCMD_Quit:
																				SetStatusTags(STTAG_Name,id,STTAG_Panic,TRUE,TAG_END);
																				break;
																			case NDCMD_Panic:
																				SetStatusTags(STTAG_Name,id,STTAG_Panic,TRUE,TAG_END);
																				msg->cm_Data=TRUE;
																				break;
																			case NDCMD_Page:
																				break;
																		}
																	ReplyMsg((struct Message *) msg);
																}
															if(cmsg)
																{
																	if(!CheckIO((struct IORequest *) SerialIO))
																		AbortIO((struct IORequest *) SerialIO);
																	WaitIO((struct IORequest *) SerialIO);
																	ReplyMsg((struct Message *) cmsg);
																	cmsg=NULL;
																}
														}
												}
										}
									if(serialopen)
										{
											if(cmsg)
												{
													if(!CheckIO((struct IORequest *) SerialIO))
														AbortIO((struct IORequest *) SerialIO);
													WaitIO((struct IORequest *) SerialIO);
													ReplyMsg((struct Message *) cmsg);
													cmsg=NULL;
												}
											CloseDevice((struct IORequest *) SerialIO);
											DeleteIORequest((struct IORequest *) SerialIO);
											DeleteMsgPort(SerialMP);
											if(OwnDevUnitBase && !frontend)
												{
													FreeDevUnit(serial_device,serial_unit);
													FreeSignal(signal);
													signal=-1;
													seriallocked=FALSE;
												}
										}
								}
								if(OwnDevUnitBase) CloseLibrary(OwnDevUnitBase);
								CloseLibraries(libs);
							}
					}
				if(command_port) DeletePort(command_port);
				if(port) DeletePort(port);
			}
		if(qmsg)
			{
				Forbid();
				ReplyMsg((struct Message *) qmsg);
			}
		return(0);
	}
