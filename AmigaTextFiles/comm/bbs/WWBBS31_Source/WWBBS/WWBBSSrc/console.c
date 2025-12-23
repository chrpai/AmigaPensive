#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <libraries/gadtools.h>
#include <libraries/wwbbs.h>
#include <devices/console.h>
#include <devices/conunit.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wwbbs.h"

#include "wwbbs_settings.h"

#include "wwbbs_lib.h"

#include "libs.h"

#include "Data:Programs/Include/wwbbs_commands.h"

LONG __saveds ConsoleProc()
	{
		UWORD console_pens[]={ 0,7,7,7,4,5,0,0,7,(UWORD) ~0 };
		struct NewMenu console_menu[]={
			{NM_TITLE,"Project",0,0,0,0},
			{NM_ITEM,"Quit","Q",0,0,0},
			{NM_END,NULL,0,0,0,0}
		};
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
				sprintf(name,"WWBBS Console.%s",id);
				if(!(port=CreatePort(name,0)))
					ok=FALSE;
				sprintf(command_name,"WWBBS Console Command.%s",id);
				if(!(command_port=CreatePort(command_name,0)))
					ok=FALSE;
				msg->cm_Return=ok;
				ReplyMsg((struct Message *) msg);
				if(ok)
					{
						struct Libraries *libs;
						if(libs=OpenLibraries())
							{
								UBYTE node_type=NULL;
								GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_Type,&node_type,TAG_END);
								{
									struct Screen *Scr=NULL;
									APTR VisualInfo;
									struct Window *Win=NULL;
									struct Menu *MenuStrip;
									struct TextAttr fontta;
									struct TextFont *font=NULL;
									struct MsgPort *ConsoleReadMP=NULL,*ConsoleWriteMP;
									struct IOStdReq *ConsoleReadIO,*ConsoleWriteIO;
									struct CommandMessage *cmsg=NULL;
									ULONG cdata=NULL;
									ULONG mask;
									BOOL kg=TRUE,consoleopen=FALSE;
									BYTE con_read_buff[256],c;
									while(kg)
										{
											{
												ULONG waitmask=NULL;
												waitmask|=1 << port->mp_SigBit;
												if(consoleopen)
													waitmask|=(1 << Win->UserPort->mp_SigBit) | (1 << ConsoleReadMP->mp_SigBit);
												if(cmsg)
													waitmask|=(1 << command_port->mp_SigBit);
												mask=Wait(waitmask);
											}
											if(mask & (1 << port->mp_SigBit))
												{
													struct CommandMessage *msg;
													ULONG data;
													while(kg && (msg=(struct CommandMessage *) GetMsg(port)))
														{
															data=msg->cm_Data;
															msg->cm_Data=NULL;
															switch(msg->cm_Command)
																{
																	case CONCMD_Quit:
																		msg->cm_Data=TRUE;
																		qmsg=msg;
																		kg=FALSE;
																		break;
																	case CONCMD_Open:
																		if(consoleopen)
																			{
																				ScreenToFront(Scr);
																				ActivateWindow(Win);
																			}
																		else
																			{
																				ULONG displaymode=INVALID_ID;
																				Scr=NULL;
																				VisualInfo=NULL;
																				Win=NULL;
																				MenuStrip=NULL;
																				fontta.ta_Name=libs->Settings.font_name;
																				fontta.ta_YSize=libs->Settings.font_ysize;
																				fontta.ta_Style=NULL;
																				fontta.ta_Flags=NULL;
																				font=NULL;
																				ConsoleReadMP=NULL;
																				ConsoleWriteMP=NULL;
																				ConsoleReadIO=NULL;
																				ConsoleWriteIO=NULL;
																				displaymode=libs->Settings.display_id;
																				{
																					if(displaymode==INVALID_ID)
																						{
																							struct Screen *scr;
																							if(scr=LockPubScreen(NULL))
																								{
																									displaymode=GetVPModeID(&scr->ViewPort);
																									UnlockPubScreen(NULL,scr);
																								}
																						}
																				}
																				if(displaymode!=INVALID_ID)
																					{
																						if(strlen(fontta.ta_Name) && fontta.ta_YSize)
																							font=OpenDiskFont(&fontta);
																						if(font)
																							Scr=OpenScreenTags(NULL,SA_Depth,libs->Settings.display_depth,SA_DisplayID,displaymode,SA_Pens,console_pens,SA_DetailPen,0,SA_BlockPen,7,SA_Title,name,SA_PubName,name,SA_Font,&fontta,SA_Type,PUBLICSCREEN,SA_Interleaved,TRUE,TAG_END);
																						else
																							Scr=OpenScreenTags(NULL,SA_Depth,libs->Settings.display_depth,SA_DisplayID,displaymode,SA_Pens,console_pens,SA_DetailPen,0,SA_BlockPen,7,SA_Title,name,SA_PubName,name,SA_SysFont,0,SA_Type,PUBLICSCREEN,SA_Interleaved,TRUE,TAG_END);
																						if(Scr)
																							{
																								SetRGB4(&Scr->ViewPort,0,0,0,0); /* Black */ 
																								SetRGB4(&Scr->ViewPort,1,10,0,0); /* Red */
																								SetRGB4(&Scr->ViewPort,2,0,10,0); /* Green */
																								SetRGB4(&Scr->ViewPort,3,10,6,0); /* Yellow */
																								SetRGB4(&Scr->ViewPort,4,0,0,10); /* Blue */
																								SetRGB4(&Scr->ViewPort,5,10,0,10); /* Magenta */
																								SetRGB4(&Scr->ViewPort,6,0,10,10); /* Cyan */
																								SetRGB4(&Scr->ViewPort,7,10,10,10); /* White */
																								if(libs->Settings.display_depth==4)
																									{
																										SetRGB4(&Scr->ViewPort,8,6,6,6);
																										SetRGB4(&Scr->ViewPort,9,15,0,0);
																										SetRGB4(&Scr->ViewPort,10,0,15,0);
																										SetRGB4(&Scr->ViewPort,11,15,15,0);
																										SetRGB4(&Scr->ViewPort,12,0,0,15);
																										SetRGB4(&Scr->ViewPort,13,15,0,15);
																										SetRGB4(&Scr->ViewPort,14,0,15,15);
																										SetRGB4(&Scr->ViewPort,15,15,15,15);
																									}
																								if(VisualInfo=GetVisualInfo(Scr,TAG_END))
																									{
																										if(Win=OpenWindowTags(NULL,WA_Left,0,WA_Top,Scr->BarHeight+2,WA_Width,Scr->Width,WA_Height,Scr->Height-Scr->BarHeight-2,WA_IDCMP,IDCMP_MENUPICK,WA_PubScreen,Scr,WA_Flags,WFLG_NOCAREREFRESH|WFLG_BORDERLESS|WFLG_BACKDROP|WFLG_ACTIVATE|WFLG_SMART_REFRESH|WFLG_NEWLOOKMENUS,TAG_END))
																											{
																												if( (MenuStrip=CreateMenus(console_menu,TAG_END)) && LayoutMenus(MenuStrip,VisualInfo,GTMN_NewLookMenus,TRUE,TAG_END) && SetMenuStrip(Win,MenuStrip) )
																													{
																														if( (ConsoleReadMP=CreateMsgPort()) && (ConsoleWriteMP=CreateMsgPort()) && (ConsoleReadIO=CreateIORequest(ConsoleReadMP,sizeof(struct IOStdReq))) && (ConsoleWriteIO=CreateIORequest(ConsoleWriteMP,sizeof(struct IOStdReq))) )
																															{
																																ConsoleWriteIO->io_Data=Win;
																																ConsoleWriteIO->io_Length=sizeof(struct Window);
																																if(!OpenDevice("console.device",CONU_STANDARD,(struct IORequest *) ConsoleWriteIO,CONFLAG_DEFAULT))
																																	{
																																		ConsoleReadIO->io_Device=ConsoleWriteIO->io_Device;
																																		ConsoleReadIO->io_Unit=ConsoleWriteIO->io_Unit;
																																		PubScreenStatus(Scr,NULL);
																																		{
																																			strcpy(con_read_buff,"");
																																			ConsoleReadIO->io_Data=&c;
																																			ConsoleReadIO->io_Length=1;
																																			ConsoleReadIO->io_Command=CMD_READ;
																																			SendIO((struct IORequest *) ConsoleReadIO);
																																		}
																																		msg->cm_Data=TRUE;
																																		consoleopen=TRUE;
																																		SetStatusTags(STTAG_Name,id,STTAG_ConsoleOpen,TRUE,TAG_END);
																																	}
																															}
																													}
																											}
																									}
																							}
																					}
																				if(!consoleopen)
																					{
																						if(ConsoleWriteIO) DeleteIORequest(ConsoleWriteIO);
																						if(ConsoleReadIO) DeleteIORequest(ConsoleReadIO);
																						if(ConsoleWriteMP) DeleteMsgPort(ConsoleWriteMP);
																						if(ConsoleReadMP) DeleteMsgPort(ConsoleReadMP);
																						if(MenuStrip) { ClearMenuStrip(Win); FreeMenus(MenuStrip); }
																						if(Win) CloseWindow(Win);
																						if(VisualInfo) FreeVisualInfo(VisualInfo);
																						if(Scr) CloseScreen(Scr);
																						if(font) CloseFont(font);
																					}
																			}
																		break;
																	case CONCMD_Close:
																		if(consoleopen)
																			{
																				if(cmsg)
																					{
																						ReplyMsg((struct Message *) cmsg);
																						cmsg=NULL;
																					}
																				if(!CheckIO((struct IORequest *) ConsoleReadIO))
																					{
																						AbortIO((struct IORequest *) ConsoleReadIO);
																						WaitIO((struct IORequest *) ConsoleReadIO);
																					}
																				CloseDevice((struct IORequest *) ConsoleWriteIO);
																				DeleteIORequest(ConsoleWriteIO);
																				DeleteIORequest(ConsoleReadIO);
																				DeleteMsgPort(ConsoleWriteMP);
																				DeleteMsgPort(ConsoleReadMP);
																				ClearMenuStrip(Win);
																				FreeMenus(MenuStrip);
																				CloseWindow(Win);
																				FreeVisualInfo(VisualInfo);
																				{
																					struct EasyStruct es={ sizeof(struct EasyStruct),NULL,"WWBBS Console Request","Unable to close screen.\nPlease close all visitor windows.","Retry" };
																					while(!CloseScreen(Scr))
																						EasyRequest(NULL,&es,NULL);
																				}
																				if(font) CloseFont(font);
																				consoleopen=FALSE;
																				SetStatusTags(STTAG_Name,id,STTAG_ConsoleOpen,FALSE,TAG_END);
																			}
																		break;
																	case CONCMD_Read:
																		if(consoleopen && !cmsg)
																			{
																				if(con_read_buff[0])
																					{
																						msg->cm_Data=con_read_buff[0];
																						{
																							char buff[256];
																							strcpy(buff,&con_read_buff[1]);
																							strcpy(con_read_buff,buff);
																						}
																					}
																			}
																		break;
																	case CONCMD_Write:
																		if(consoleopen && data)
																			{
																				BYTE *buffer=NULL;
																				ULONG size=0;
																				{
																					struct TagItem *tag;
																					if(tag=FindTagItem(CONWRITETAG_Buffer,(struct TagItem *) data))
																						buffer=(APTR) tag->ti_Data;
																					if(tag=FindTagItem(CONWRITETAG_Size,(struct TagItem *) data))
																						size=(ULONG) tag->ti_Data;
																				}
																				if(buffer && size)
																					{
																						ConsoleWriteIO->io_Data=buffer;
																						ConsoleWriteIO->io_Length=size;
																						ConsoleWriteIO->io_Command=CMD_WRITE;
																						DoIO((struct IORequest *) ConsoleWriteIO);
																						msg->cm_Data=TRUE;
																					}
																			}
																		break;
																	case CONCMD_Wait:
																		if(consoleopen && !cmsg)
																			{
																				if(con_read_buff[0])
																					{
																						msg->cm_Data=con_read_buff[0];
																						{
																							char buff[256];
																							strcpy(buff,&con_read_buff[1]);
																							strcpy(con_read_buff,buff);
																						}
																					}
																				else
																					{
																						cmsg=msg;
																						cdata=data;
																					}
																			}
																		break;
																	case CONCMD_Abort:
																		if(consoleopen && cmsg)
																			{
																				ReplyMsg((struct Message *) cmsg);
																				cmsg=NULL;
																			}
																		break;
																	case CONCMD_Query:
																		if(consoleopen)
																			msg->cm_Data=strlen(con_read_buff);
																		break;
																}
															if(!qmsg && cmsg!=msg)
																ReplyMsg((struct Message *) msg);
														}
												}
											if(consoleopen)
												{
													if(mask & (1 << Win->UserPort->mp_SigBit))
														{
															struct IntuiMessage *msg;
															ULONG class;
															UWORD code;
															BOOL kg2=TRUE;
															while(kg && kg2 && (msg=(struct IntuiMessage *) GetMsg(Win->UserPort)))
																{
																	class=msg->Class;
																	code=msg->Code;
																	ReplyMsg((struct Message *) msg);
																	switch(class)
																		{
																			case IDCMP_MENUPICK:
																				switch(MENUNUM(code))
																					{
																						case 0:
																							switch(ITEMNUM(code))
																								{
																									case 0:
																										switch(node_type)
																											{
																												case NDTYP_Local:
																													SetStatusTags(STTAG_Name,id,STTAG_Panic,TRUE,TAG_END);
																													if(cmsg)
																														{
																															ReplyMsg((struct Message *) cmsg);
																															cmsg=NULL;
																														}
																													break;
																												case NDTYP_Remote:
																													if(consoleopen)
																														{
																															if(cmsg)
																																{
																																	ReplyMsg((struct Message *) cmsg);
																																	cmsg=NULL;
																																}
																															if(!CheckIO((struct IORequest *) ConsoleReadIO))
																																{
																																	AbortIO((struct IORequest *) ConsoleReadIO);
																																	WaitIO((struct IORequest *) ConsoleReadIO);
																																}
																															CloseDevice((struct IORequest *) ConsoleWriteIO);
																															DeleteIORequest(ConsoleWriteIO);
																															DeleteIORequest(ConsoleReadIO);
																															DeleteMsgPort(ConsoleWriteMP);
																															DeleteMsgPort(ConsoleReadMP);
																															ClearMenuStrip(Win);
																															FreeMenus(MenuStrip);
																															CloseWindow(Win);
																															FreeVisualInfo(VisualInfo);
																															{
																																struct EasyStruct es={ sizeof(struct EasyStruct),NULL,"WWBBS Console Request","Unable to close screen.\nPlease close all visitor windows.","Retry" };
																																while(!CloseScreen(Scr))
																																	EasyRequest(NULL,&es,NULL);
																															}
																															if(font) CloseFont(font);
																															consoleopen=FALSE;
																															SetStatusTags(STTAG_Name,id,STTAG_ConsoleOpen,FALSE,TAG_END);
																														}
																													kg2=FALSE;
																													break;
																											}
																										break;
																								}
																							break;
																					}
																				break;
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
																	ReplyMsg((struct Message *) cmsg);
																	cmsg=NULL;
																}
														}
												}
											if(consoleopen)
												{
													if(mask & (1 << ConsoleReadMP->mp_SigBit))
														{
															WaitIO((struct IORequest *) ConsoleReadIO);
															if(cmsg)
																{
																	cmsg->cm_Data=c;
																	ReplyMsg((struct Message *) cmsg);
																	cmsg=NULL;
																}
															else
																{
																	if(strlen(con_read_buff)<=255)
																		strncat(con_read_buff,&c,1);
																}
															{
																ConsoleReadIO->io_Data=&c;
																ConsoleReadIO->io_Length=1;
																ConsoleReadIO->io_Command=CMD_READ;
																SendIO((struct IORequest *) ConsoleReadIO);
															}
														}
												}
										}
									if(consoleopen)
										{
											if(cmsg)
												{
													ReplyMsg((struct Message *) cmsg);
													cmsg=NULL;
												}
											if(!CheckIO((struct IORequest *) ConsoleReadIO))
												{
													AbortIO((struct IORequest *) ConsoleReadIO);
													WaitIO((struct IORequest *) ConsoleReadIO);
												}
											CloseDevice((struct IORequest *) ConsoleWriteIO);
											DeleteIORequest(ConsoleWriteIO);
											DeleteIORequest(ConsoleReadIO);
											DeleteMsgPort(ConsoleWriteMP);
											DeleteMsgPort(ConsoleReadMP);
											ClearMenuStrip(Win);
											FreeMenus(MenuStrip);
											CloseWindow(Win);
											FreeVisualInfo(VisualInfo);
											{
												struct EasyStruct es={ sizeof(struct EasyStruct),NULL,"WWBBS Console Request","Unable to close screen.\nPlease close all visitor windows.","Retry" };
												while(!CloseScreen(Scr))
													EasyRequest(NULL,&es,NULL);
											}
											if(font) CloseFont(font);
										}
								}
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
