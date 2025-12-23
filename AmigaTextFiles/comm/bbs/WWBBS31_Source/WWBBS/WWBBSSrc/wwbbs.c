#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <intuition/intuition.h>
#include <libraries/asl.h>
#include <libraries/commodities.h>
#include <libraries/gadtools.h>
#include <libraries/triton.h>
#include <libraries/wwbbs.h>
#include <devices/console.h>
#include <devices/conunit.h>
#include <devices/serial.h>
#include <devices/timer.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <proto/asl.h>
#include <proto/commodities.h>
#include <proto/diskfont.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/icon.h>
#include <proto/intuition.h>
#include <proto/triton.h>
#include <proto/utility.h>
#include <proto/wwbbs.h>

#include "Data:Programs/Include/wwbbs_update.h"

#include "wwbbs.h"

#include "wwbbs_settings.h"

#include "wwbbs_rev.h"

char *version=VERSTAG;

long __stack=10000;

struct Library *WorldWideBase;

struct MonitorNode {
	struct Node mn_Node;
	BYTE mn_Text[128];
	BYTE mn_Name[33];
};

struct ActionNode {
	struct Node an_Node;
	BYTE an_Name[33];
	BYTE an_Command[256];
};

struct List *CreateList(void);
void DeleteList(struct List *);
struct Node *GetNode(struct List *,ULONG);
ULONG GetNodeNum(struct List *,BYTE *);
void UpdateStatus(struct TR_Project *,struct MonitorNode *);
void close_all(void);

#define StatusText(text,id,minwidth) HorizGroupSA,TextID(text,id),TRAT_MinWidth,minwidth,Space,EndGroup

BYTE sessions_buff[128];
BYTE access_buff[128];
BYTE ratio_buff[128];
BYTE time_buff[128];
BYTE email_buff[128];
BYTE messages_buff[128];
BYTE files_buff[128];
BYTE credits_buff[128];

struct SignalSemaphore settings_semaphore;
struct SettingsStruct settings;

ProjectDefinition(wwbbs_trwintags)
	{
		WindowID(1),WindowPosition(TRWP_CENTERDISPLAY),
		WindowTitle("World Wide BBS"),

		BeginMenu("Project"),
			MenuItem("?_About...",101),
			ItemBarlabel,
			MenuItem("H_Hide",102),
			MenuItem("Q_Quit",103),
		BeginMenu("Settings"),
			MenuItem("M_Screen Mode...",201),
			MenuItem("F_Font...",202),
			ItemBarlabel,
			MenuItem("S_Save settings",203),

		VertGroupA,
			Space,
			HorizGroupA,
				Space,
				ListSS(NULL,1,0,0),TRAT_MinWidth,40,
				Space,
				ListSel(NULL,2,0),TRAT_MinWidth,20,
				Space,
			EndGroup,
			Space,
			HorizGroup,
				Space,
				_TextBox,
				HorizGroup,
					Space,
					VertGroupEA,
						SpaceS,
						TextN("Sessions:"),
						TextN("Access:"),
						TextN("Ratio:"),
						TextN("Time:"),
						TextN("Email:"),
						TextN("Messages:"),
						TextN("Files:"),
						TextN("Credits:"),
						SpaceS,
					EndGroup,
					SpaceS,
					VertGroupEA,
						SpaceS,
						StatusText(NULL,3,10),
						StatusText(NULL,4,3),
						StatusText(NULL,5,30),
						StatusText(NULL,6,25),
						StatusText(NULL,7,40),
						StatusText(NULL,8,40),
						StatusText(NULL,9,60),
						StatusText(NULL,10,40),
						SpaceS,
					EndGroup,
					Space,
				EndGroup,
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

void main(int argc,char **argv)
	{
		if(TR_OpenTriton(TRITON10VERSION,TRCA_Name,"WWBBS",TRCA_LongName,"World Wide BBS",TRCA_Info,"Bulletin Board System",TRCA_Version,"2",TRCA_Release,"1",TRCA_Date,DATE,TAG_END))
			{

		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			close_all();

		{
			BPTR fh;
			if(fh=Open("WWBBS:WWBBS.settings",MODE_OLDFILE))
				{
					FRead(fh,&settings,sizeof(struct SettingsStruct),1);
					Close(fh);
				}
			else
				{
					settings.display_id=INVALID_ID;
					settings.display_depth=3;
					strcpy(settings.font_name,"");
					settings.font_ysize=0;
				}
			InitSemaphore(&settings_semaphore);
		}
		{
			struct MsgPort *BrokerMP;
			CxObj *Broker,*Filter,*Sender,*Translate;
			char **tooltypes;
			tooltypes=ArgArrayInit(argc,argv);
			if(BrokerMP=CreateMsgPort())
				{
					struct NewBroker nb={ NB_VERSION,"World Wide BBS",NULL,"Bulletin Board System",NBU_UNIQUE|NBU_NOTIFY,COF_SHOW_HIDE,0,0,0 };
					nb.nb_Title=&version[6];
					nb.nb_Port=BrokerMP;
					nb.nb_Pri=(BYTE) ArgInt(tooltypes,"CX_PRIORITY",0);
					if(Broker=CxBroker(&nb,NULL))
						{
							if(Filter=CxFilter(ArgString(tooltypes,"CX_POPKEY","control alt w")))
								{
									AttachCxObj(Broker,Filter);
									if(Sender=CxSender(BrokerMP,1))
										{
											AttachCxObj(Filter,Sender);
											if(Translate=CxTranslate(NULL))
												{
													AttachCxObj(Filter,Translate);
													if(!CxObjError(Filter))
														{
															ActivateCxObj(Broker,TRUE);

	/* Set up update port... */
	{
		struct MsgPort *update_port;
		if(update_port=CreateMsgPort())
			{
				SetUpdatePort(update_port);

	/* Start up nodes... */
	{
		ULONG next=NULL;
		BYTE *name;
		UBYTE type=NULL;
		BOOL frontend=FALSE;
		while(next=GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,&name,CFGTAG_Next,next,NDTAG_Type,&type,NDTAG_FrontEnd,&frontend,TAG_END))
			{
				switch(type)
					{
						case NDTYP_Local:
							break;
						case NDTYP_Remote:
							if(!frontend)
								StartProcess("WWBBS Node",NodeProc,20000,name);
							else
								StartProcess("WWBBS Node",NodeFrontEndProc,20000,name);
							break;
					}
			}
	}

	/* Main */
	{
		struct List *node_list=NULL;
		{
			struct List action_list;
			ULONG selected=0;
			BOOL kg=TRUE;
			struct TR_Project *project=NULL;
			NewList(&action_list);
			{
				UBYTE **p;
				for(p=tooltypes;*p;p++)
					{
						if(!strnicmp(*p,"ACTION",6))
							{
								char *q,*r;
								if(q=strchr(*p,'='))
									{
										char name[33],command[256];
										strcpy(name,"");
										strcpy(command,"");
										q++;
										if(r=strchr(q,','))
											{
												r++;
												sprintf(name,"%.*s",r-q-1,q);
												sprintf(command,"%.255s",r);
												{
													struct ActionNode *node;
													if(node=AllocVec(sizeof(struct ActionNode),MEMF_CLEAR))
														{
															strcpy(node->an_Name,name);
															strcpy(node->an_Command,command);
															node->an_Node.ln_Name=node->an_Name;
															AddTail(&action_list,(struct Node *) node);
														}
												}
											}
									}
							}
					}
			}
			if(!stricmp(ArgString(tooltypes,"CX_POPUP","YES"),"YES"))
				{
					if(project=TR_OpenProject(Application,wwbbs_trwintags))
						{
							if(node_list=CreateList())
								{
									TR_SetAttribute(project,1,0,(ULONG) node_list);
									selected=0;
									TR_SetAttribute(project,1,TRAT_Value,selected);
									{
										struct MonitorNode *node;
										if(node=(struct MonitorNode *) GetNode(node_list,selected))
											UpdateStatus(project,node);
									}
								}
							TR_SetAttribute(project,2,0,(ULONG) &action_list);
						}
					else
						TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
				}
			while(kg)
				{
					if(project)
						{
							ULONG mask;
							mask=TR_Wait(Application,(1 << BrokerMP->mp_SigBit) | (1 << update_port->mp_SigBit));
							if(mask & (1 << BrokerMP->mp_SigBit))
								{
									CxMsg *msg;
									ULONG id,type;
									while(kg && project && (msg=(CxMsg *) GetMsg(BrokerMP)))
										{
											id=CxMsgID(msg);
											type=CxMsgType(msg);
											ReplyMsg((struct Message *) msg);
											switch(type)
												{
													case CXM_IEVENT:
														switch(id)
															{
																case 1:
																	WindowToFront(project->trp_Window);
																	ActivateWindow(project->trp_Window);
																	break;
															}
														break;
													case CXM_COMMAND:
														switch(id)
															{
																case CXCMD_DISABLE:
																	ActivateCxObj(Broker,FALSE);
																	break;
																case CXCMD_ENABLE:
																	ActivateCxObj(Broker,TRUE);
																	break;
																case CXCMD_APPEAR:
																	WindowToFront(project->trp_Window);
																	ActivateWindow(project->trp_Window);
																	break;
																case CXCMD_DISAPPEAR:
																	TR_CloseProject(project);
																	project=NULL;
																	break;
																case CXCMD_KILL:
																	TR_CloseProject(project);
																	project=NULL;
																	kg=FALSE;
																	break;
																case CXCMD_UNIQUE:
																	WindowToFront(project->trp_Window);
																	ActivateWindow(project->trp_Window);
																	break;
															}
														break;
												}
										}
								}
							if(mask & (1 << update_port->mp_SigBit))
								{
									struct UpdateMessage *msg;
									while(msg=(struct UpdateMessage *) GetMsg(update_port))
										{
											switch(msg->um_Command)
												{
													case UPDCMD_UpdateStatus:
														{
															BYTE selected_name[33];
															strcpy(selected_name,"");
															{
																struct MonitorNode *node;
																if(node=(struct MonitorNode *) GetNode(node_list,selected))
																	strcpy(selected_name,node->mn_Name);
															}
															TR_SetAttribute(project,1,0,NULL);
															if(node_list) DeleteList(node_list);
															selected=0;
															if(node_list=CreateList())
																{
																	TR_SetAttribute(project,1,0,(ULONG) node_list);
																	selected=GetNodeNum(node_list,selected_name);
																}
															TR_SetAttribute(project,1,TRAT_Value,selected);
															{
																struct MonitorNode *node;
																node=(struct MonitorNode *) GetNode(node_list,selected);
																UpdateStatus(project,node);
															}
														}
														break;
													case UPDCMD_StartupNode:
														{
															UBYTE type=0;
															BOOL frontend=FALSE;
															if(GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,msg->um_ID,NDTAG_Type,&type,NDTAG_FrontEnd,&frontend,TAG_END))
																{
																	switch(type)
																		{
																			case NDTYP_Local:
																				break;
																			case NDTYP_Remote:
																				if(!frontend)
																					StartProcess("WWBBS Node",NodeProc,20000,(BYTE *) msg->um_ID);
																				else
																					StartProcess("WWBBS Node",NodeFrontEndProc,20000,(BYTE *) msg->um_ID);
																				break;
																		}
																}
														}
														break;
													case UPDCMD_KillNode:
														{
															char task[64];
															BOOL istask=TRUE;
															sprintf(task,"WWBBS Local.%s",(BYTE *) msg->um_ID);
															while(istask)
																{
																	Forbid();
																	istask=(FindTask(task)) ? TRUE : FALSE;
																	Permit();
																	if(istask)
																		{
																			while(!NodeCommandTags((BYTE *) msg->um_ID,NDCMD_Panic,TAG_END))
																				Delay(25L);
																		}
																}
														}
														KillProcess("WWBBS Node",(BYTE *) msg->um_ID);
														break;
												}
											FreeVec(msg);
										}
								}

							{
								struct TR_Message *msg;
								while(kg && project && (msg=TR_GetMsg(Application)))
									{
										if(msg->trm_Project==project)
											{
												switch(msg->trm_Class)
													{
														case TRMS_CLOSEWINDOW:
															TR_CloseProject(project);
															project=NULL;
															break;
														case TRMS_NEWVALUE:
															switch(msg->trm_ID)
																{
																	case 1: /* Node List */
																		selected=msg->trm_Data;
																		{
																			struct MonitorNode *node;
																			if(node=(struct MonitorNode *) GetNode(node_list,selected))
																				UpdateStatus(project,node);
																		}
																		if(msg->trm_Qualifier & IEQUALIFIER_REPEAT)
																			{
																				struct MonitorNode *node;
																				if(node=(struct MonitorNode *) GetNode(node_list,selected))
																					{
																						UBYTE type=0;
																						GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,node->mn_Name,NDTAG_Type,&type,TAG_END);
																						switch(type)
																							{
																								case NDTYP_Local:
																									{
																										char task[64];
																										BOOL istask=FALSE;
																										sprintf(task,"WWBBS Local.%s",node->mn_Name);
																										Forbid();
																										istask=(FindTask(task)) ? TRUE : FALSE;
																										Permit();
																										if(istask)
																											ConsoleCommandTags(node->mn_Name,CONCMD_Open,TAG_END);
																										else
																											StartProcess("WWBBS Local",LocalLogonProc,20000,node->mn_Name);
																									}
																									break;
																								case NDTYP_Remote:
																									ConsoleCommandTags(node->mn_Name,CONCMD_Open,TAG_END);
																									break;
																							}
																					}
																			}
																		break;
																	case 2: /* Action List */
																		{
																			struct ActionNode *node;
																			if(node=(struct ActionNode *) GetNode(&action_list,msg->trm_Data))
																				{
																					{
																						BOOL specify_node=FALSE;
																						{
																							char *p;
																							if(p=strchr(node->an_Command,'%'))
																								{
																									p++;
																									if(*p=='s')
																										specify_node=TRUE;
																								}
																						}
																						if(specify_node)
																							{
																								if(GetNode(node_list,selected))
																									{
																										char buff[256+32-2],*p=NULL;
																										{
																											struct MonitorNode *node;
																											if(node=(struct MonitorNode *) GetNode(node_list,selected))
																												p=node->mn_Name;
																										}
																										if(p)
																											{
																												sprintf(buff,node->an_Command,p);
																												SystemTags(buff,TAG_END);
																											}
																									}
																								else
																									TR_EasyRequestTags(Application,"Please select a node first.","Okay",TREZ_LockProject,project,TREZ_Title,"World Wide BBS",TAG_END);
																							}
																						else
																							SystemTags(node->an_Command,TAG_END);
																					}
																				}
																		}
																		break;
																}
															break;
														case TRMS_ACTION:
															switch(msg->trm_ID)
																{
																	case 101: /* About... */
																		{
																			char s[256];
																			sprintf(s,"%%3World Wide BBS 2.5 (%s)\n%%nCopyright © 1995 Arthur Choung\tInternet: choung@seas.ucla.edu or arthur@qedbbs.com\nUS Mail: 10324 Chestnut Street, Bellflower, CA 90706 USA",DATE);
																			TR_EasyRequestTags(Application,s,"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_LockProject,project,TREZ_Title,"About World Wide BBS...",TAG_END);
																		}
																		break;
																	case 102: /* Hide */
																		TR_CloseProject(project);
																		project=NULL;
																		break;
																	case 103: /* Quit */
																		TR_CloseProject(project);
																		project=NULL;
																		kg=FALSE;
																		break;
																	case 201: /* Display Mode... */
																		ObtainSemaphore(&settings_semaphore);
																		TR_LockProject(project);
																		{
																			if(AslBase->lib_Version>=38)
																				{
																					struct ScreenModeRequester *req;
																					if(req=AllocAslRequestTags(ASL_ScreenModeRequest,
																							ASLSM_Window,project->trp_Window,
																							ASLSM_TitleText,"Select Screen Mode...",
																							ASLSM_InitialDisplayID,settings.display_id,
																							ASLSM_InitialDisplayDepth,settings.display_depth,
																							ASLSM_DoDepth,TRUE,
																							ASLSM_MinDepth,3,
																							ASLSM_MaxDepth,4,
																							TAG_END))
																						{
																							if(AslRequestTags(req,TAG_END))
																								{
																									settings.display_id=req->sm_DisplayID;
																									settings.display_depth=req->sm_DisplayDepth;
																								}
																							FreeAslRequest(req);
																						}
																				}
																			else
																				TR_EasyRequestTags(Application,"You need asl.library V38","Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
																		}
																		TR_UnlockProject(project);
																		ReleaseSemaphore(&settings_semaphore);
																		break;
																	case 202: /* Font... */
																		ObtainSemaphore(&settings_semaphore);
																		TR_LockProject(project);
																		{
																			struct FontRequester *req;
																			if(req=AllocAslRequestTags(ASL_FontRequest,
																					ASLFO_Window,project->trp_Window,
																					ASLFO_TitleText,"Select Font...",
																					ASLFO_InitialName,settings.font_name,
																					ASLFO_InitialSize,settings.font_ysize,
																					TAG_END))
																				{
																					if(AslRequestTags(req,TAG_END))
																						{
																							if(strlen(req->fo_Attr.ta_Name)<256)
																								strcpy(settings.font_name,req->fo_Attr.ta_Name);
																							else
																								{
																									strncpy(settings.font_name,req->fo_Attr.ta_Name,255);
																									settings.font_name[255]=NULL;
																								}
																							settings.font_ysize=req->fo_Attr.ta_YSize;
																						}
																					FreeAslRequest(req);
																				}
																		}
																		TR_UnlockProject(project);
																		ReleaseSemaphore(&settings_semaphore);
																		break;
																	case 203: /* Save */
																		ObtainSemaphore(&settings_semaphore);
																		{
																			BPTR fh;
																			if(fh=Open("WWBBS:WWBBS.settings",MODE_NEWFILE))
																				{
																					FWrite(fh,&settings,sizeof(struct SettingsStruct),1);
																					Close(fh);
																				}
																		}
																		ReleaseSemaphore(&settings_semaphore);
																		break;
																}
															break;
													}
											}
										TR_ReplyMsg(msg);
									}
							}
						}
					else
						{
							ULONG mask;
							CxMsg *msg;
							ULONG id,type;
							mask=Wait((1 << BrokerMP->mp_SigBit) | (1 << update_port->mp_SigBit));
							if(mask & (1 << BrokerMP->mp_SigBit))
								{
									while(kg && !project && (msg=(CxMsg *) GetMsg(BrokerMP)))
										{
											id=CxMsgID(msg);
											type=CxMsgType(msg);
											ReplyMsg((struct Message *) msg);
											switch(type)
												{
													case CXM_IEVENT:
														switch(id)
															{
																case 1:
																	if(project=TR_OpenProject(Application,wwbbs_trwintags))
																		{
																			if(node_list=CreateList())
																				{
																					TR_SetAttribute(project,1,0,(ULONG) node_list);
																					selected=0;
																					TR_SetAttribute(project,1,TRAT_Value,selected);
																					{
																						struct MonitorNode *node;
																						if(node=(struct MonitorNode *) GetNode(node_list,selected))
																							UpdateStatus(project,node);
																					}
																				}
																			TR_SetAttribute(project,2,0,(ULONG) &action_list);
																		}
																	else
																		TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
																	break;
															}
														break;
													case CXM_COMMAND:
														switch(id)
															{
																case CXCMD_DISABLE:
																	ActivateCxObj(Broker,FALSE);
																	break;
																case CXCMD_ENABLE:
																	ActivateCxObj(Broker,TRUE);
																	break;
																case CXCMD_APPEAR:
																	if(project=TR_OpenProject(Application,wwbbs_trwintags))
																		{
																			if(node_list=CreateList())
																				{
																					TR_SetAttribute(project,1,0,(ULONG) node_list);
																					selected=0;
																					TR_SetAttribute(project,1,TRAT_Value,selected);
																					{
																						struct MonitorNode *node;
																						if(node=(struct MonitorNode *) GetNode(node_list,selected))
																							UpdateStatus(project,node);
																					}
																				}
																			TR_SetAttribute(project,2,0,(ULONG) &action_list);
																		}
																	else
																		TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
																	break;
																case CXCMD_DISAPPEAR:
																	break;
																case CXCMD_KILL:
																	kg=FALSE;
																	break;
																case CXCMD_UNIQUE:
																	if(project=TR_OpenProject(Application,wwbbs_trwintags))
																		{
																			if(node_list=CreateList())
																				{
																					TR_SetAttribute(project,1,0,(ULONG) node_list);
																					selected=0;
																					TR_SetAttribute(project,1,TRAT_Value,selected);
																					{
																						struct MonitorNode *node;
																						if(node=(struct MonitorNode *) GetNode(node_list,selected))
																							UpdateStatus(project,node);
																					}
																				}
																			TR_SetAttribute(project,2,0,(ULONG) &action_list);
																		}
																	else
																		TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
																	break;
															}
														break;
												}
										}
								}
							if(mask & (1 << update_port->mp_SigBit))
								{
									struct UpdateMessage *msg;
									while(msg=(struct UpdateMessage *) GetMsg(update_port))
										{
											switch(msg->um_Command)
												{
													case UPDCMD_UpdateStatus:
														break;
													case UPDCMD_StartupNode:
														{
															UBYTE type=0;
															BOOL frontend=FALSE;
															if(GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,msg->um_ID,NDTAG_Type,&type,NDTAG_FrontEnd,&frontend,TAG_END))
																{
																	switch(type)
																		{
																			case NDTYP_Local:
																				break;
																			case NDTYP_Remote:
																				if(!frontend)
																					StartProcess("WWBBS Node",NodeProc,20000,(BYTE *) msg->um_ID);
																				else
																					StartProcess("WWBBS Node",NodeFrontEndProc,20000,(BYTE *) msg->um_ID);
																				break;
																		}
																}
														}
														break;
													case UPDCMD_KillNode:
														{
															char task[64];
															BOOL istask=TRUE;
															sprintf(task,"WWBBS Local.%s",(BYTE *) msg->um_ID);
															while(istask)
																{
																	Forbid();
																	istask=(FindTask(task)) ? TRUE : FALSE;
																	Permit();
																	if(istask)
																		{
																			while(!NodeCommandTags((BYTE *) msg->um_ID,NDCMD_Panic,TAG_END))
																				Delay(25L);
																		}
																}
														}
														KillProcess("WWBBS Node",(BYTE *) msg->um_ID);
														break;
												}
											FreeVec(msg);
										}
								}
						}
				}
			{
				struct Node *node;
				while(node=RemHead(&action_list))
					FreeVec(node);
			}
		}
		if(node_list)
			DeleteList(node_list);
	}

	/* Kill nodes */
	{
		ULONG next=NULL;
		BYTE *name=NULL;
		UBYTE type=0;
		while(next=GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,&name,CFGTAG_Next,next,NDTAG_Type,&type,TAG_END))
			{
				switch(type)
					{
						case NDTYP_Local:
							NodeCommandTags(name,NDCMD_Panic,TAG_END);
							{
								char task[64];
								BOOL istask=TRUE;
								sprintf(task,"WWBBS Local.%s",name);
								while(istask)
									{
										Forbid();
										istask=(FindTask(task)) ? TRUE : FALSE;
										Permit();
										if(istask)
											Delay(25L);
									}
							}
							break;
						case NDTYP_Remote:
							KillProcess("WWBBS Node",name);
							break;
					}
			}
	}

	/* Clean up update port */
				SetUpdatePort(NULL);
				DeleteMsgPort(update_port);
			}
	}

														}
												}
										}
								}
							DeleteCxObjAll(Broker);
							{
								struct Message *msg;
								while(msg=GetMsg(BrokerMP))
									ReplyMsg(msg);
							}
						}
					DeleteMsgPort(BrokerMP);
				}
			ArgArrayDone();
		}

				close_all();
			}
		exit(0);
	}

struct List *CreateList()
	{
		struct List *list=NULL;
		if(list=AllocVec(sizeof(struct List),MEMF_CLEAR))
			{
				NewList(list);
				{
					ULONG next=0;
					BYTE *name=NULL,username[33],location[33];
					struct MonitorNode *node;
					while(next=GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,&name,CFGTAG_Next,next,TAG_END))
						{
							if(node=AllocVec(sizeof(struct MonitorNode),MEMF_CLEAR))
								{
									if(GetStatusTags(STTAG_Name,name,STTAG_UserName,username,STTAG_Location,location,TAG_END))
										sprintf(node->mn_Text,"%s: User `%s', Location `%s'",name,(strlen(username)) ? username : "None",(strlen(location)) ? location : "Nowhere");
									else
										sprintf(node->mn_Text,"%s: Idle",name);
									strcpy(node->mn_Name,name);
									node->mn_Node.ln_Name=node->mn_Text;
									AddTail(list,(struct Node *) node);
								}
						}
				}
			}
		return(list);
	}

void DeleteList(struct List *list)
	{
		{
			struct Node *node;
			while(node=RemHead(list))
				FreeVec(node);
		}
		FreeVec(list);
	}

struct Node *GetNode(struct List *list,ULONG num)
	{
		struct Node *node=NULL;
		ULONG i;
		if(list && list->lh_Head->ln_Succ)
			{
				node=list->lh_Head;
				for(i=0;i<num;i++)
					{
						if(node->ln_Succ)
							node=node->ln_Succ;
					}
			}
		return(node);
	}

ULONG GetNodeNum(struct List *list,BYTE *name)
	{
		ULONG ret=0;
		if(list)
			{
				struct MonitorNode *node=NULL;
				for(node=(struct MonitorNode *) list->lh_Head;node->mn_Node.ln_Succ;node=(struct MonitorNode *) node->mn_Node.ln_Succ)
					{
						if(!stricmp(node->mn_Name,name))
							break;
						ret++;
					}
			}
		return(ret);
	}

void UpdateStatus(struct TR_Project *project,struct MonitorNode *node)
	{
		TR_SetAttribute(project,3,TRAT_Text,NULL);
		TR_SetAttribute(project,4,TRAT_Text,NULL);
		TR_SetAttribute(project,5,TRAT_Text,NULL);
		TR_SetAttribute(project,6,TRAT_Text,NULL);
		TR_SetAttribute(project,7,TRAT_Text,NULL);
		TR_SetAttribute(project,8,TRAT_Text,NULL);
		TR_SetAttribute(project,9,TRAT_Text,NULL);
		TR_SetAttribute(project,10,TRAT_Text,NULL);
		if(node)
			{
				BYTE username[33];
				UWORD timeremaining=0,timeused=0;
				strcpy(username,"");
				if(GetStatusTags(STTAG_Name,node->mn_Name,STTAG_UserName,username,STTAG_TimeRemaining,&timeremaining,STTAG_TimeUsed,&timeused,TAG_END))
					{
						sprintf(time_buff,"%d used, %d remaining",timeused,timeremaining);
						TR_SetAttribute(project,6,TRAT_Text,(ULONG) time_buff);
						if(strlen(username))
							{
								ULONG sessions=0;
								UBYTE accesslevel=0,ratiobyte=0,ratiofile=0;
								ULONG emailread=0,emailwritten=0,messagesread=0,messageswritten=0,filesuploaded=0,filesdownloaded=0,kilosuploaded=0,kilosdownloaded=0,creditsfile=0,creditskilo=0;
								if(GetUserTags(USRTAG_Name,username,
										USRTAG_Sessions,&sessions,
										USRTAG_AccessLevel,&accesslevel,
										USRTAG_RatioByte,&ratiobyte,
										USRTAG_RatioFile,&ratiofile,
										USRTAG_EmailRead,&emailread,
										USRTAG_EmailWritten,&emailwritten,
										USRTAG_MessagesRead,&messagesread,
										USRTAG_MessagesWritten,&messageswritten,
										USRTAG_FilesUploaded,&filesuploaded,
										USRTAG_FilesDownloaded,&filesdownloaded,
										USRTAG_KilosUploaded,&kilosuploaded,
										USRTAG_KilosDownloaded,&kilosdownloaded,
										USRTAG_FilesCredits,&creditsfile,
										USRTAG_KilosCredits,&creditskilo,
										TAG_END))
									{
										sprintf(sessions_buff,"%ld",sessions);
										sprintf(access_buff,"%d",accesslevel);
										strcpy(ratio_buff,"");
										if(ratiobyte)
											sprintf(&ratio_buff[strlen(ratio_buff)],"%d:1 byte",ratiobyte);
										else
											strcat(ratio_buff,"No byte ratio");
										strcat(ratio_buff,", ");
										if(ratiofile)
											sprintf(&ratio_buff[strlen(ratio_buff)],"%d:1 file",ratiofile);
										else
											strcat(ratio_buff,"No file ratio");
										sprintf(email_buff,"%ld read, %ld written",emailread,emailwritten);
										sprintf(messages_buff,"%ld read, %ld written",messagesread,messageswritten);
										sprintf(files_buff,"%ld (%ldK) uploaded, %ld (%ldK) downloaded",filesuploaded,kilosuploaded,filesdownloaded,kilosdownloaded);
										sprintf(credits_buff,"%ld file, %ld kilo",creditsfile,creditskilo);
										TR_SetAttribute(project,3,TRAT_Text,(ULONG) sessions_buff);
										TR_SetAttribute(project,4,TRAT_Text,(ULONG) access_buff);
										TR_SetAttribute(project,5,TRAT_Text,(ULONG) ratio_buff);
										TR_SetAttribute(project,7,TRAT_Text,(ULONG) email_buff);
										TR_SetAttribute(project,8,TRAT_Text,(ULONG) messages_buff);
										TR_SetAttribute(project,9,TRAT_Text,(ULONG) files_buff);
										TR_SetAttribute(project,10,TRAT_Text,(ULONG) credits_buff);
									}
							}
					}
			}
	}

void close_all()
	{
		if(WorldWideBase) CloseLibrary(WorldWideBase);
		TR_CloseTriton();
		exit(0);
	}

/***/

BOOL __saveds StartProcess(BYTE *basename,APTR entry,ULONG stacksize,BYTE *id)
	{
		static BYTE name[64];
		struct MsgPort *replyport;
		struct Process *proc;
		BOOL ret=FALSE;
		sprintf(name,"%s.%s",basename,id);
		if(replyport=CreateMsgPort())
			{
				struct ConfirmationMessage msg={0};
				msg.cm_Message.mn_Node.ln_Type=NT_MESSAGE;
				msg.cm_Message.mn_Length=sizeof(struct ConfirmationMessage);
				msg.cm_Message.mn_ReplyPort=replyport;
				msg.cm_ID=id;
				if(proc=CreateNewProcTags(NP_Entry,entry,NP_StackSize,stacksize,NP_Name,name,TAG_END))
					{
						PutMsg(&proc->pr_MsgPort,(struct Message *) &msg);
						WaitPort(replyport);
						GetMsg(replyport);
						ret=msg.cm_Return;
					}
				DeleteMsgPort(replyport);
			}
		return(ret);
	}

void __saveds KillProcess(BYTE *basename,BYTE *id)
	{
		struct MsgPort *port=NULL;
		BYTE name[64];
		sprintf(name,"%s.%s",basename,id);
		Forbid();
		port=FindPort(name);
		Permit();
		if(port)
			{
				if(!strcmp(basename,"WWBBS Node"))
					{
						while(!NodeCommandTags(id,NDCMD_Quit,TAG_END))
							Delay(25L);
					}
				if(!strcmp(basename,"WWBBS Console"))
					{
						while(!ConsoleCommandTags(id,CONCMD_Quit,TAG_END))
							Delay(25L);
					}
				if(!strcmp(basename,"WWBBS Serial"))
					{
						while(!SerialCommandTags(id,SERCMD_Quit,TAG_END))
							Delay(25L);
					}
				if(!strcmp(basename,"WWBBS Timer"))
					{
						while(!TimerCommandTags(id,TMRCMD_Quit,TAG_END))
							Delay(25L);
					}
			}
	}
