#include <exec/types.h>
#include <exec/exec.h>
#include <dos/exall.h>
#include <libraries/triton.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/triton.h>

#include "logviewer_rev.h"

char *ver=VERSTAG;

struct BufferNode {
	struct Node bn_Node;
	char bn_Buffer[256];
};

void display_log(char *);
void close_all(void);

ProjectDefinition(main_trwintags)
	{
		WindowID(1),WindowPosition(TRWP_CENTERDISPLAY),
		WindowTitle("Log Viewer"),

		BeginMenu("Project"),
			MenuItem("?_About...",101),
			ItemBarlabel,
			MenuItem("Q_Quit",102),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				ListSel(NULL,1,0),TRAT_MinWidth,32,TRAT_MinHeight,10,
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

void main()
	{
		if(TR_OpenTriton(TRITON11VERSION,TRCA_Name,"LogViewer",TRCA_LongName,"Log Viewer",TRCA_Info,"Views WWBBS Logs",TRCA_Version,"2",TRCA_Release,"1",TRCA_Date,DATE,TAG_END))
			{
				struct List BufferList;
				struct TR_Project *project;
				NewList(&BufferList);
				{
					BPTR lock;
					if(lock=Lock("WWBBS:Logs",SHARED_LOCK))
						{
							struct ExAllControl *eac;
							struct ExAllData *ead,*eadata;
							int more;
							if(eac=AllocDosObject(DOS_EXALLCONTROL,NULL))
								{
									eac->eac_LastKey=0;
									if(eadata=AllocVec(sizeof(struct ExAllData)*16,MEMF_CLEAR))
										{
											do
												{
													more=ExAll(lock,eadata,sizeof(struct ExAllData)*16,ED_NAME,eac);
													if((!more) && (IoErr()!=ERROR_NO_MORE_ENTRIES))
														break;
													if(eac->eac_Entries==0)
														continue;
													{
														ead=eadata;
														do
															{
																{
																	struct BufferNode *node;
																	if(node=AllocVec(sizeof(struct BufferNode),MEMF_CLEAR))
																		{
																			sprintf(node->bn_Buffer,"%s",ead->ed_Name);
																			node->bn_Node.ln_Name=node->bn_Buffer;
																			AddTail(&BufferList,(struct Node *) node);
																		}
																}
																ead=ead->ed_Next;
															} while(ead);
													}
												} while(more);
											FreeVec(eadata);
										}
									FreeDosObject(DOS_EXALLCONTROL,eac);
								}
							UnLock(lock);
						}
				}
				if(project=TR_OpenProject(Application,main_trwintags))
					{
						BOOL kg=TRUE;
						TR_SetAttribute(project,1,0,(ULONG) &BufferList);
						while(kg)
							{
								TR_Wait(Application,0);
								{
									struct TR_Message *msg;
									while(kg && (msg=TR_GetMsg(Application)))
										{
											if(msg->trm_Project==project)
												{
													switch(msg->trm_Class)
														{
															case TRMS_CLOSEWINDOW:
																kg=FALSE;
																break;
															case TRMS_NEWVALUE:
																switch(msg->trm_ID)
																	{
																		case 1:
																			{
																				struct Node *node;
																				int i;
																				node=BufferList.lh_Head;
																				for(i=0;i<msg->trm_Data;i++)
																					{
																						if(node->ln_Succ)
																							node=node->ln_Succ;
																					}
																				display_log(node->ln_Name);
																			}
																			break;
																	}
																break;
															case TRMS_ACTION:
																switch(msg->trm_ID)
																	{
																		case 101:
																			{
																				char s[256];
																				sprintf(s,"%%3LogViewer 2.5 (%s)\n%%nCopyright © 1995 Arthur Choung\tInternet: choung@seas.ucla.edu or arthur@qedbbs.com\nUS Mail: 10324 Chestnut Street, Bellflower, CA 90706 USA",DATE);
																				TR_EasyRequestTags(Application,s,"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_LockProject,project,TREZ_Title,"About World Wide BBS...",TAG_END);
																			}
																			break;
																		case 102:
																			kg=FALSE;
																			break;
																	}
																break;
														}
												}
											TR_ReplyMsg(msg);
										}
								}
							}
						TR_CloseProject(project);
					}
				else
					TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
				{
					struct Node *node;
					while(node=RemHead(&BufferList))
						FreeVec(node);
				}
				TR_CloseTriton();
			}
		exit(0);
	}

ProjectDefinition(logviewer_trwintags)
	{
		WindowID(2),WindowPosition(TRWP_CENTERDISPLAY),

		BeginMenu("Project"),
			MenuItem("Q_Quit",101),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				ListRO(NULL,1,0),TRAT_MinWidth,60,TRAT_MinHeight,20,
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

void display_log(char *log)
	{
		struct List BufferList;
		NewList(&BufferList);
		{
			BPTR fh;
			struct BufferNode *node;
			char file[256],buff[256];
			sprintf(file,"WWBBS:Logs/%s",log);
			if(fh=Open(file,MODE_OLDFILE))
				{
					while(FGets(fh,buff,255))
						{
							{
								char *p;
								if(p=strchr(buff,'\n'))
									*p=NULL;
							}
							if(node=AllocVec(sizeof(struct BufferNode),MEMF_CLEAR))
								{
									strcpy(node->bn_Buffer,buff);
									node->bn_Node.ln_Name=node->bn_Buffer;
									AddTail(&BufferList,(struct Node *) node);
								}
						}
					Close(fh);
				}
		}
		{
			struct TR_Project *project;
			char title[64];
			sprintf(title,"Viewing %s...",log);
			if(project=TR_OpenProject(Application,logviewer_trwintags))
				{
					BOOL kg=TRUE;
					TR_SetAttribute(project,0,TRWI_Title,(ULONG) title);
					TR_SetAttribute(project,1,0,(ULONG) &BufferList);
					while(kg)
						{
							TR_Wait(Application,0);
							{
								struct TR_Message *msg;
								while(kg && (msg=TR_GetMsg(Application)))
									{
										if(msg->trm_Project==project)
											{
												switch(msg->trm_Class)
													{
														case TRMS_CLOSEWINDOW:
															kg=FALSE;
															break;
														case TRMS_ACTION:
															switch(msg->trm_ID)
																{
																	case 101:
																		kg=FALSE;
																		break;
																}
															break;
													}
											}
										TR_ReplyMsg(msg);
									}
							}
						}
					TR_CloseProject(project);
				}
			else
				TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
		}
		{
			struct Node *node;
			while(node=RemHead(&BufferList))
				FreeVec(node);
		}
	}
