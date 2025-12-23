#include <exec/types.h>
#include <exec/exec.h>
#include <graphics/displayinfo.h>
#include <intuition/gadgetclass.h>
#include <intuition/intuition.h>
#include <libraries/asl.h>
#include <libraries/triton.h>
#include <libraries/wwbbs.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/asl.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/icon.h>
#include <proto/intuition.h>
#include <proto/triton.h>
#include <proto/wwbbs.h>

#include "bbsprefs.h"

#include "bbsprefs_rev.h"

char *version=VERSTAG;

long __stack=20000;

struct Library *AslBase;
struct Library *WorldWideBase;

void close_all(void);
void Prefs(UWORD);
void error_request(struct TR_Project *,BYTE *);
struct List *CreateList(BYTE *);
void DeleteList(struct List *);
struct Node *GetNode(struct List *,ULONG);
ULONG GetNodeNum(struct List *,BYTE *);
void update_gadgets(struct TR_Project *,UWORD,BYTE *,struct List *,ULONG);
BOOL process_prefs_gadgets(struct TR_Project *,BYTE *,BYTE *,UWORD,ULONG,ULONG);
BOOL asl_file_requester(struct TR_Project *,BYTE *,BYTE *,BYTE *,BYTE *);
BOOL asl_drawer_requester(struct TR_Project *,BYTE *,BYTE *,BYTE *);
void set_defaults(BYTE *,BYTE *,UWORD);

#define update_gadget(project,id,data,node) TR_SetAttribute(project,id,0,(ULONG)data);TR_SetAttribute(project,id,TRAT_Disabled,(node) ? FALSE : TRUE)
#define update_integer_gadget(project,id,buff,data,node) sprintf(buff,"%ld",(ULONG)data);TR_SetAttribute(project,id,0,(ULONG)buff);TR_SetAttribute(project,id,TRAT_Disabled,(node) ? FALSE : TRUE)
#define update_cycle_gadget(project,id,data,node) TR_SetAttribute(project,id,TRAT_Value,(ULONG)data);TR_SetAttribute(project,id,TRAT_Disabled,(node) ? FALSE : TRUE)
#define update_checkbox_gadget(project,id,data,node) TR_SetAttribute(project,id,TRAT_Value,(ULONG)data);TR_SetAttribute(project,id,TRAT_Disabled,(node) ? FALSE : TRUE)
#define update_text_gadget(project,id,data,node) TR_SetAttribute(project,id,TRAT_Text,(ULONG)data);TR_SetAttribute(project,id,TRAT_Disabled,(node) ? FALSE : TRUE)

#define set_config(tag,data) SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,tag,(APTR) data,TAG_END)

void main(int argc,char **argv)
	{
		if(TR_OpenTriton(TRITON10VERSION,TRCA_Name,"BBSPrefs",TRCA_LongName,"BBS Preferences",TRCA_Info,"WWBBS Preferences",TRCA_Version,"2",TRCA_Release,"1",TRCA_Date,DATE,TAG_END))
			{

		if(!(AslBase=OpenLibrary("asl.library",0)))
			close_all();
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			close_all();

		{
			UBYTE **tooltypes,*p;
			tooltypes=ArgArrayInit(argc,argv);
			p=ArgString(tooltypes,"FILEBASEDIRECTORY","");
			if(strlen(p)>255)
				p="";
			strcpy(default_directory,p);
			p=ArgString(tooltypes,"MESSAGEBASEQUOTEHEADER","In a message dated %d, %n wrote:");
			if(strlen(p)>80)
				p="In a message dated %d, %n wrote:";
			strcpy(default_quote_header,p);
			p=ArgString(tooltypes,"NODECOMMAND","WWBBS:BBSRx WWBBS:Rexx/Session.rexx");
			if(strlen(p)>255)
				p="WWBBS:BBSRx WWBBS:Rexx/Session.rexx";
			strcpy(default_node_command,p);
			ArgArrayDone();
		}

		{
			struct TR_Project *project;
			if(project=TR_OpenProject(Application,bbsprefs_trwintags))
				{
					BOOL kg=TRUE;
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
																	case 1: /* List */
																		TR_LockProject(project);
																		Prefs(msg->trm_Data);
																		TR_UnlockProject(project);
																		break;
																}
															break;
														case TRMS_ACTION:
															switch(msg->trm_ID)
																{
																	case 101: /* About... */
																		{
																			char s[256];
																			sprintf(s,"%%3BBS Preferences 2.5 (%s)\n%%nCopyright © 1995 Arthur Choung\tInternet: choung@seas.ucla.edu or arthur@qedbbs.com\nUS Mail: 10324 Chestnut Street, Bellflower, CA 90706 USA",DATE);
																			TR_EasyRequestTags(Application,s,"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_LockProject,project,TREZ_Title,"About World Wide BBS...",TAG_END);
																		}
																		break;
																	case 102: /* Quit */
																		kg=FALSE;
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

		SetConfigTags(CFGTAG_ForceSave,TRUE,TAG_END);
		close_all();

			}
		exit(0);
	}

void close_all()
	{
		if(AslBase) CloseLibrary(AslBase);
		if(WorldWideBase) CloseLibrary(WorldWideBase);
		TR_CloseTriton();
		exit(0);
	}

void Prefs(UWORD prefsid)
	{
		struct TR_Project *project;
		if(project=TR_OpenProject(Application,prefs_trwintags[prefsid]))
			{
				BYTE path[512];
				struct List *list=NULL;
				strcpy(path,prefs_path[prefsid]);
				if(prefs_flags[prefsid] & PREFS_ListMode)
					{
						if(list=CreateList(path))
							{
								TR_SetAttribute(project,51,0,(ULONG) list);
								TR_SetAttribute(project,51,TRAT_Value,0);
								update_gadgets(project,prefsid,path,list,0);
							}
						else
							{
								char s[256];
								sprintf(s,"Unable to create list for `%s'.",path);
								error_request(project,s);
							}
					}
				else
					update_gadgets(project,prefsid,path,NULL,0);
				{
					BOOL kg=TRUE;
					ULONG selected=0;
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
																	case 51: /* List */
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				if( (msg->trm_Qualifier & IEQUALIFIER_REPEAT) && (prefs_flags[prefsid] & PREFS_TreeMode) )
																					{
																						struct Node *node;
																						if(node=GetNode(list,selected))
																							{
																								TR_SetAttribute(project,51,0,NULL);
																								DeleteList(list);
																								selected=0;
																								if(!AddPart(path,node->ln_Name,511))
																									{
																										char s[256];
																										sprintf(s,"Unable to enter `%s/%s'.",path,node->ln_Name);
																										error_request(project,s);
																									}
																								list=CreateList(path);
																								TR_SetAttribute(project,51,0,(ULONG) list);
																								TR_SetAttribute(project,51,TRAT_Value,selected);
																								update_gadgets(project,prefsid,path,list,selected);
																							}
																					}
																				else
																					{
																						selected=msg->trm_Data;
																						update_gadgets(project,prefsid,path,list,selected);
																					}
																			}
																		break;
																	case 52: /* Edit */
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						STRPTR newname=NULL;
																						newname=(STRPTR) TR_GetAttribute(project,52,0);
																						if(strcmp(newname,node->ln_Name))
																							{
																								if(SetConfigTags(
																										CFGTAG_Path,path,
																										CFGTAG_Name,node->ln_Name,
																										CFGTAG_NewName,newname,
																										CFGTAG_DontSave,TRUE,
																										TAG_END))
																									{
																										TR_SetAttribute(project,51,0,0);
																										DeleteList(list);
																										selected=0;
																										if(list=CreateList(path))
																											selected=GetNodeNum(list,newname);
																										TR_SetAttribute(project,51,0,(ULONG) list);
																										TR_SetAttribute(project,51,TRAT_Value,selected);
																									}
																								else
																									error_request(project,"Unable to set new name.");
																							}
																					}
																			}
																		break;
																	default:
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						if(process_prefs_gadgets(project,path,node->ln_Name,prefsid,msg->trm_ID,msg->trm_Data))
																							update_gadgets(project,prefsid,path,list,selected);
																					}
																			}
																		else
																			{
																				if(process_prefs_gadgets(project,NULL,"System",prefsid,msg->trm_ID,msg->trm_Data))
																					update_gadgets(project,prefsid,path,list,selected);
																			}
																		break;
																}
															break;
														case TRMS_ACTION:
															switch(msg->trm_ID)
																{
																	case 53: /* Parent */
																		if(prefs_flags[prefsid] & PREFS_TreeMode)
																			{
																				if(strchr(path,'/'))
																					{
																						char *p;
																						if(p=PathPart(path))
																							{
																								TR_SetAttribute(project,51,0,0);
																								DeleteList(list);
																								selected=0;
																								*p=NULL;
																								list=CreateList(path);
																								TR_SetAttribute(project,51,0,(ULONG) list);
																								TR_SetAttribute(project,51,TRAT_Value,selected);
																								update_gadgets(project,prefsid,path,list,selected);
																							}
																					}
																			}
																		break;
																	case 54: /* Child */
																		if(prefs_flags[prefsid] & PREFS_TreeMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						TR_SetAttribute(project,51,0,0);
																						DeleteList(list);
																						selected=0;
																						if(!AddPart(path,node->ln_Name,511))
																							{
																								char s[256];
																								sprintf(s,"Unable to enter `%s/%s'.",path,node->ln_Name);
																								error_request(project,s);
																							}
																						list=CreateList(path);
																						TR_SetAttribute(project,51,0,(ULONG) list);
																						TR_SetAttribute(project,51,TRAT_Value,selected);
																						update_gadgets(project,prefsid,path,list,selected);
																					}
																			}
																		break;
																	case 55: /* Add */
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				if(AddConfigTags(
																						CFGTAG_Path,path,
																						CFGTAG_Name,"Unnamed",
																						CFGTAG_Type,prefs_type[prefsid],
																						CFGTAG_DontSave,TRUE,
																						TAG_END))
																					{
																						set_defaults(path,"Unnamed",prefsid);
																						TR_SetAttribute(project,51,0,0);
																						DeleteList(list);
																						selected=0;
																						if(list=CreateList(path))
																							selected=GetNodeNum(list,"Unnamed");
																						TR_SetAttribute(project,51,0,(ULONG) list);
																						TR_SetAttribute(project,51,TRAT_Value,selected);
																						update_gadgets(project,prefsid,path,list,selected);
																						/* activate gadget */
																					}
																				else
																					{
																						char s[256];
																						sprintf(s,"Unable to add to `%s'.",path);
																						error_request(project,s);
																					}
																			}
																		break;
																	case 56: /* Delete */
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						if(RemConfigTags(
																								CFGTAG_Path,path,
																								CFGTAG_Name,node->ln_Name,
																								CFGTAG_DontSave,TRUE,
																								TAG_END))
																							{
																								TR_SetAttribute(project,51,0,0);
																								DeleteList(list);
																								if(selected)
																									selected--;
																								list=CreateList(path);
																								TR_SetAttribute(project,51,0,(ULONG) list);
																								TR_SetAttribute(project,51,TRAT_Value,selected);
																								update_gadgets(project,prefsid,path,list,selected);
																							}
																						else
																							{
																								char s[256];
																								sprintf(s,"Unable to delete `%s/%s'.",path,node->ln_Name);
																								error_request(project,s);
																							}
																					}
																			}
																		break;
																	case 57: /* Move Up */
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						if(SetConfigTags(
																								CFGTAG_Path,path,
																								CFGTAG_Name,node->ln_Name,
																								CFGTAG_MoveUp,TRUE,
																								CFGTAG_DontSave,TRUE,
																								TAG_END))
																							{
																								BYTE name[33];
																								strcpy(name,node->ln_Name);
																								TR_SetAttribute(project,51,0,0);
																								DeleteList(list);
																								selected=0;
																								if(list=CreateList(path))
																									selected=GetNodeNum(list,name);
																								TR_SetAttribute(project,51,0,(ULONG) list);
																								TR_SetAttribute(project,51,TRAT_Value,selected);
																								update_gadgets(project,prefsid,path,list,selected);
																							}
																						else
																							{
																								char s[256];
																								sprintf(s,"Unable to move `%s' up.",node->ln_Name);
																								error_request(project,s);
																							}
																					}
																			}
																		break;
																	case 58: /* Move Down */
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						if(SetConfigTags(
																								CFGTAG_Path,path,
																								CFGTAG_Name,node->ln_Name,
																								CFGTAG_MoveDown,TRUE,
																								CFGTAG_DontSave,TRUE,
																								TAG_END))
																							{
																								BYTE name[33];
																								strcpy(name,node->ln_Name);
																								TR_SetAttribute(project,51,0,0);
																								DeleteList(list);
																								selected=0;
																								if(list=CreateList(path))
																									selected=GetNodeNum(list,name);
																								TR_SetAttribute(project,51,0,(ULONG) list);
																								TR_SetAttribute(project,51,TRAT_Value,selected);
																								update_gadgets(project,prefsid,path,list,selected);
																							}
																						else
																							{
																								char s[256];
																								sprintf(s,"Unable to move `%s' down.",node->ln_Name);
																								error_request(project,s);
																							}
																					}
																			}
																		break;
																	case 101: /* Quit */
																		kg=FALSE;
																		break;
																	default:
																		if(prefs_flags[prefsid] & PREFS_ListMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						if(process_prefs_gadgets(project,path,node->ln_Name,prefsid,msg->trm_ID,msg->trm_Data))
																							update_gadgets(project,prefsid,path,list,selected);
																					}
																			}
																		else
																			{
																				if(process_prefs_gadgets(project,NULL,"System",prefsid,msg->trm_ID,msg->trm_Data))
																					update_gadgets(project,prefsid,path,list,selected);
																			}
																		break;
																}
															break;
														case TRMS_KEYPRESSED:
															switch(msg->trm_Code)
																{
																	case 0x4f: /* left */
																		if(prefs_flags[prefsid] & PREFS_TreeMode)
																			{
																				if(strchr(path,'/'))
																					{
																						char *p;
																						if(p=PathPart(path))
																							{
																								TR_SetAttribute(project,51,0,0);
																								DeleteList(list);
																								selected=0;
																								*p=NULL;
																								list=CreateList(path);
																								TR_SetAttribute(project,51,0,(ULONG) list);
																								TR_SetAttribute(project,51,TRAT_Value,selected);
																								update_gadgets(project,prefsid,path,list,selected);
																							}
																					}
																			}
																		break;
																	case 0x4e: /* right */
																		if(prefs_flags[prefsid] & PREFS_TreeMode)
																			{
																				struct Node *node;
																				if(node=GetNode(list,selected))
																					{
																						TR_SetAttribute(project,51,0,0);
																						DeleteList(list);
																						selected=0;
																						if(!AddPart(path,node->ln_Name,511))
																							{
																								char s[256];
																								sprintf(s,"Unable to enter `%s/%s'.",path,node->ln_Name);
																								error_request(project,s);
																							}
																						list=CreateList(path);
																						TR_SetAttribute(project,51,0,(ULONG) list);
																						TR_SetAttribute(project,51,TRAT_Value,selected);
																						update_gadgets(project,prefsid,path,list,selected);
																					}
																			}
																		break;
																}
															break;
													}
											}
										TR_ReplyMsg(msg);
									}
							}
						}
				}
				if(list)
					{
						TR_SetAttribute(project,51,0,NULL);
						DeleteList(list);
					}
				TR_CloseProject(project);
			}
		else
			TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
	}

void error_request(struct TR_Project *project,BYTE *s)
	{
		TR_EasyRequestTags(Application,s,"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_LockProject,project,TREZ_Title,"World Wide BBS",TAG_END);
	}

/***/

struct List *CreateList(BYTE *path)
	{
		struct List *list=NULL;
		if(list=AllocVec(sizeof(struct List),MEMF_CLEAR))
			{
				NewList(list);
				{
					ULONG next=0;
					STRPTR name;
					struct BufferNode *node;
					while(next=GetConfigTags(
							CFGTAG_Path,path,
							CFGTAG_Name,&name,
							CFGTAG_Next,next,
							TAG_END))
						{
							if(node=AllocVec(sizeof(struct BufferNode),MEMF_CLEAR))
								{
									strcpy(node->bn_Buffer,name);
									node->bn_Node.ln_Name=node->bn_Buffer;
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
				struct Node *node=NULL,*searchnode=NULL;
				if(searchnode=FindName(list,name))
					{
						for(node=list->lh_Head;node->ln_Succ;node=node->ln_Succ)
							{
								if(node==searchnode)
									break;
								ret++;
							}
					}
			}
		return(ret);
	}

/***/

void update_gadgets(struct TR_Project *project,UWORD prefsid,BYTE *path,struct List *list,ULONG selected)
	{
		if(prefs_flags[prefsid] & PREFS_ListMode)
			{
				struct Node *node;
				if(node=GetNode(list,selected))
					{
						update_gadget(project,52,node->ln_Name,node);
						TR_SetAttribute(project,56,TRAT_Disabled,FALSE);
						TR_SetAttribute(project,57,TRAT_Disabled,(selected==0) ? TRUE : FALSE);
						{
							struct Node *wn;
							wn=node->ln_Succ;
							TR_SetAttribute(project,58,TRAT_Disabled,(wn->ln_Succ) ? FALSE : TRUE);
						}
					}
				else
					{
						TR_SetAttribute(project,56,TRAT_Disabled,TRUE);
						TR_SetAttribute(project,57,TRAT_Disabled,TRUE);
						TR_SetAttribute(project,58,TRAT_Disabled,TRUE);
					}
			}
		if(prefs_flags[prefsid] & PREFS_TreeMode)
			{
				TR_SetAttribute(project,59,TRAT_Text,NULL);
				{
					char *p;
					if(p=strchr(path,'/'))
						{
							p++;
							if(strlen(p)>32)
								sprintf(path_buff,"...%.29s",&p[strlen(p)-29]);
							else
								strcpy(path_buff,p);
							TR_SetAttribute(project,59,TRAT_Text,(ULONG) path_buff);
						}
				}
				TR_SetAttribute(project,53,TRAT_Disabled,(strchr(path,'/')) ? FALSE : TRUE);
				TR_SetAttribute(project,54,TRAT_Disabled,(list->lh_Head->ln_Succ) ? FALSE : TRUE);
			}
		{
			struct Node *node=NULL;
			node=GetNode(list,selected);
			switch(prefsid)
				{
					case 0: /* Access Groups */
						{
							ULONG days=0;
							UWORD sessiontimelimit=0,inactivitytimelimit=0;
							UBYTE accesslevel=0,ratiobyte=0,ratiofile=0;
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										AGTAG_AccessLevel,&accesslevel,
										AGTAG_SessionTimeLimit,&sessiontimelimit,
										AGTAG_InactivityTimeLimit,&inactivitytimelimit,
										AGTAG_RatioByte,&ratiobyte,
										AGTAG_RatioFile,&ratiofile,
										AGTAG_Days,&days,
										TAG_END);
								}
							update_integer_gadget(project,1,ag_accesslevel_buff,accesslevel,node);
							update_integer_gadget(project,2,ag_sessiontimelimit_buff,sessiontimelimit,node);
							update_integer_gadget(project,3,ag_inactivitytimelimit_buff,inactivitytimelimit,node);
							update_integer_gadget(project,4,ag_ratiobyte_buff,ratiobyte,node);
							update_integer_gadget(project,5,ag_ratiofile_buff,ratiofile,node);
							update_integer_gadget(project,6,ag_days_buff,days,node);
						}
						break;
					case 1: /* Archivers */
						{
							strcpy(arc_extension,"");
							strcpy(arc_addcommand,"");
							strcpy(arc_extractcommand,"");
							strcpy(arc_viewcommand,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										ARCTAG_Extension,arc_extension,
										ARCTAG_AddCommand,arc_addcommand,
										ARCTAG_ExtractCommand,arc_extractcommand,
										ARCTAG_ViewCommand,arc_viewcommand,
										TAG_END);
								}
							update_gadget(project,1,arc_extension,node);
							update_gadget(project,2,arc_addcommand,node);
							update_gadget(project,3,arc_extractcommand,node);
							update_gadget(project,4,arc_viewcommand,node);
						}
						break;
					case 2: /* Editors */
						{
							UBYTE type=0;
							ULONG stack=0;
							strcpy(ed_command,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										EDTAG_Type,&type,
										EDTAG_Command,ed_command,
										EDTAG_Stack,&stack,
										TAG_END);
								}
							update_cycle_gadget(project,1,type,node);
							update_gadget(project,2,ed_command,node);
							TR_SetAttribute(project,3,TRAT_Disabled,(node) ? FALSE : TRUE);
							update_integer_gadget(project,4,ed_stack_buff,stack,node);
						}
						break;
					case 3: /* File Bases */
						{
							ULONG diskspacerequired=0,days=0;
							BOOL readonly=FALSE,autojoin=FALSE;
							strcpy(fb_accessrange,"");
							strcpy(fb_group,"");
							strcpy(fb_directory,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										FBTAG_AccessRange,fb_accessrange,
										FBTAG_Group,fb_group,
										FBTAG_Directory,fb_directory,
										FBTAG_DiskSpaceRequired,&diskspacerequired,
										FBTAG_Days,&days,
										FBTAG_ReadOnly,&readonly,
										FBTAG_AutoJoin,&autojoin,
										TAG_END);
								}
							update_gadget(project,1,fb_accessrange,node);
							update_gadget(project,2,fb_group,node);
							update_gadget(project,3,fb_directory,node);
							TR_SetAttribute(project,4,TRAT_Disabled,(node) ? FALSE : TRUE);
							update_integer_gadget(project,5,fb_diskspacerequired,diskspacerequired,node);
							update_integer_gadget(project,6,fb_days,days,node);
							update_checkbox_gadget(project,7,readonly,node);
							update_checkbox_gadget(project,8,autojoin,node);
						}
						break;
					case 4: /* Menus */
						{
							BOOL child=FALSE;
							BYTE character=0,defaultchar=0,number=0;
							strcpy(mn_accessrange,"");
							strcpy(mn_prompt,"");
							strcpy(mn_command,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										CFGTAG_Child,&child,
										MNTAG_AccessRange,mn_accessrange,
										MNTAG_Character,&character,
										MNTAG_Default,&defaultchar,
										MNTAG_Number,&number,
										MNTAG_Prompt,mn_prompt,
										MNTAG_Command,mn_command,
										TAG_END);
								}
							update_gadget(project,1,mn_accessrange,node);
							sprintf(mn_character,"%c",character);
							update_gadget(project,2,mn_character,node);
							if(child)
								{
									sprintf(mn_defaultchar,"%c",defaultchar);
									update_gadget(project,3,mn_defaultchar,node);
									sprintf(mn_number,"%c",number);
									update_gadget(project,4,mn_number,node);
									update_gadget(project,5,mn_prompt,node);
									TR_SetAttribute(project,6,TRAT_Disabled,TRUE);
								}
							else
								{
									update_gadget(project,3,NULL,FALSE);
									update_gadget(project,4,NULL,FALSE);
									update_gadget(project,5,NULL,FALSE);
									update_gadget(project,6,mn_command,node);
								}
						}
						break;
					case 5: /* Message Bases */
						{
							ULONG days=0;
							BOOL readonly=FALSE,autojoin=FALSE;
							strcpy(mb_accessrange,"");
							strcpy(mb_group,"");
							strcpy(mb_quoteheader,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										MBTAG_AccessRange,mb_accessrange,
										MBTAG_Group,mb_group,
										MBTAG_QuoteHeader,mb_quoteheader,
										MBTAG_Days,&days,
										MBTAG_ReadOnly,&readonly,
										MBTAG_AutoJoin,&autojoin,
										TAG_END);
								}
							update_gadget(project,1,mb_accessrange,node);
							update_gadget(project,2,mb_group,node);
							update_gadget(project,3,mb_quoteheader,node);
							update_checkbox_gadget(project,4,readonly,node);
							update_checkbox_gadget(project,5,autojoin,node);
							update_integer_gadget(project,6,mb_days,days,node);
						}
						break;
					case 6: /* News */
						{
							strcpy(nws_accessrange,"");
							strcpy(nws_file,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										NWSTAG_AccessRange,nws_accessrange,
										NWSTAG_File,nws_file,
										TAG_END);
								}
							update_gadget(project,1,nws_accessrange,node);
							update_gadget(project,2,nws_file,node);
							TR_SetAttribute(project,3,TRAT_Disabled,(node) ? FALSE : TRUE);
							if(node)
								{
									struct DateStamp ds={0};
									if(strlen(nws_file))
										{
											BPTR fh;
											if(fh=Open(nws_file,MODE_OLDFILE))
												{
													__aligned struct FileInfoBlock fib;
													if(ExamineFH(fh,&fib))
														ds=fib.fib_Date;
													Close(fh);
												}
										}
									update_text_gadget(project,4,NULL,TRUE);
									if(ds.ds_Days || ds.ds_Minute  || ds.ds_Tick)
										{
											{
												struct DateTime dt;
												char strdate[32],strtime[32];
												dt.dat_Stamp=ds;
												dt.dat_Format=FORMAT_USA;
												dt.dat_Flags=DTF_SUBST;
												dt.dat_StrDay=NULL;
												dt.dat_StrDate=strdate;
												dt.dat_StrTime=strtime;
												DateToStr(&dt);
												sprintf(date_buff,"%s %s",strdate,strtime);
											}
											update_text_gadget(project,4,date_buff,TRUE);
										}
								}
							else
								update_text_gadget(project,4,0,FALSE);
						}
						break;
					case 7: /* Nodes */
						{
							UBYTE type=0;
							ULONG unit=0,buffersize=0,baud=0;
							BOOL handshaking=FALSE,autobaud=FALSE,frontend=FALSE;
							strcpy(nd_accessrange,"");
							strcpy(nd_device,"");
							strcpy(nd_command,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										NDTAG_AccessRange,nd_accessrange,
										NDTAG_Type,&type,
										NDTAG_Device,nd_device,
										NDTAG_Unit,&unit,
										NDTAG_BufferSize,&buffersize,
										NDTAG_Baud,&baud,
										NDTAG_Handshaking,&handshaking,
										NDTAG_AutoBaud,&autobaud,
										NDTAG_FrontEnd,&frontend,
										NDTAG_Command,nd_command,
										TAG_END);
								}
							update_gadget(project,1,nd_accessrange,node);
							update_cycle_gadget(project,2,type,node);
							update_gadget(project,3,nd_device,node);
							TR_SetAttribute(project,4,TRAT_Disabled,(node) ? FALSE : TRUE);
							update_integer_gadget(project,5,nd_unit,unit,node);
							update_integer_gadget(project,6,nd_buffersize,buffersize,node);
							update_integer_gadget(project,7,nd_baud,baud,node);
							update_checkbox_gadget(project,8,handshaking,node);
							update_checkbox_gadget(project,9,autobaud,node);
							update_checkbox_gadget(project,10,frontend,node);
							update_gadget(project,11,nd_command,node);
						}
						break;
					case 8: /* Protocols */
						{
							BOOL batch=FALSE,bidirectional=FALSE;
							strcpy(pr_sendcommand,"");
							strcpy(pr_receivecommand,"");
							if(node)
								{
									GetConfigTags(
										CFGTAG_Path,path,
										CFGTAG_Name,node->ln_Name,
										PRTAG_SendCommand,pr_sendcommand,
										PRTAG_ReceiveCommand,pr_receivecommand,
										PRTAG_Batch,&batch,
										PRTAG_Bidirectional,&bidirectional,
										TAG_END);
								}
							update_gadget(project,1,pr_sendcommand,node);
							update_gadget(project,2,pr_receivecommand,node);
							update_checkbox_gadget(project,3,batch,node);
							update_checkbox_gadget(project,4,bidirectional,node);
						}
						break;
					case 9: /* System */
						{
							BOOL onlyrealnames=FALSE;
							strcpy(sys_inputcolor,"");
							strcpy(sys_headercolor,"");
							strcpy(sys_outputcolor,"");
							strcpy(sys_promptcolor,"");
							strcpy(sys_systemcolor,"");
							strcpy(sys_fileiddizextractor,"");
							GetConfigTags(
								CFGTAG_Name,"System",
								SYSTAG_InputColor,sys_inputcolor,
								SYSTAG_HeaderColor,sys_headercolor,
								SYSTAG_OutputColor,sys_outputcolor,
								SYSTAG_PromptColor,sys_promptcolor,
								SYSTAG_SystemColor,sys_systemcolor,
								SYSTAG_FileIDDizExtractor,sys_fileiddizextractor,
								SYSTAG_OnlyRealNames,&onlyrealnames,
								TAG_END);
							TR_SetAttribute(project,5,0,(ULONG)sys_headercolor);
							TR_SetAttribute(project,6,0,(ULONG)sys_inputcolor);
							TR_SetAttribute(project,7,0,(ULONG)sys_outputcolor);
							TR_SetAttribute(project,8,0,(ULONG)sys_promptcolor);
							TR_SetAttribute(project,9,0,(ULONG)sys_systemcolor);
							TR_SetAttribute(project,12,0,(ULONG)sys_fileiddizextractor);
							TR_SetAttribute(project,11,TRAT_Value,(ULONG)onlyrealnames);
						}
						break;
				}
		}
	}

BOOL process_prefs_gadgets(struct TR_Project *project,BYTE *path,BYTE *name,UWORD prefsid,ULONG id,ULONG data)
	{
		BOOL ret=FALSE;
		switch(prefsid)
			{
				case 0: /* Access Groups */
					switch(id)
						{
							case 1:
								set_config(AGTAG_AccessLevel,atol((STRPTR) data));
								break;
							case 2:
								set_config(AGTAG_SessionTimeLimit,atol((STRPTR) data));
								break;
							case 3:
								set_config(AGTAG_InactivityTimeLimit,atol((STRPTR) data));
								break;
							case 4:
								set_config(AGTAG_RatioByte,atol((STRPTR) data));
								break;
							case 5:
								set_config(AGTAG_RatioFile,atol((STRPTR) data));
								break;
							case 6:
								set_config(AGTAG_Days,atol((STRPTR) data));
								break;
						}
					break;
				case 1: /* Archivers */
					switch(id)
						{
							case 1:
								set_config(ARCTAG_Extension,data);
								break;
							case 2:
								set_config(ARCTAG_AddCommand,data);
								break;
							case 3:
								set_config(ARCTAG_ExtractCommand,data);
								break;
							case 4:
								set_config(ARCTAG_ViewCommand,data);
								break;
						}
					break;
				case 2: /* Editors */
					switch(id)
						{
							case 1:
								set_config(EDTAG_Type,data);
								break;
							case 2:
								set_config(EDTAG_Command,data);
								break;
							case 3:
								{
									BYTE file[256],*p;
									p=(BYTE *) TR_GetAttribute(project,2,0);
									if(asl_file_requester(project,"Select Editor...",(p) ? p : "",NULL,file))
										{
											set_config(EDTAG_Command,file);
											TR_SetAttribute(project,2,0,(ULONG) file);
										}
								}
								break;
							case 4:
								set_config(EDTAG_Stack,atol((STRPTR) data));
								break;
						}
					break;
				case 3: /* File Bases */
					switch(id)
						{
							case 1:
								ret=(set_config(FBTAG_AccessRange,data)) ? FALSE : TRUE;
								break;
							case 2:
								ret=(set_config(FBTAG_Group,data)) ? FALSE : TRUE;
								break;
							case 3:
								if(set_config(FBTAG_Directory,data))
									{
										if(strlen((STRPTR) data))
											mkdir((STRPTR) data);
									}
								else
									ret=TRUE;
								break;
							case 4:
								{
									BYTE dir[256],*p;
									p=(BYTE *) TR_GetAttribute(project,3,0);
									if(asl_drawer_requester(project,"Select Directory...",(p) ? p : "",dir))
										{
											if(set_config(FBTAG_Directory,dir))
												{
													TR_SetAttribute(project,3,0,(ULONG) dir);
													if(strlen(dir))
														mkdir(dir);
												}
											else
												ret=TRUE;
										}
								}
								break;
							case 5:
								ret=(set_config(FBTAG_DiskSpaceRequired,atol((STRPTR) data))) ? FALSE : TRUE;
								break;
							case 6:
								ret=(set_config(FBTAG_Days,atol((STRPTR) data))) ? FALSE : TRUE;
								break;
							case 7:
								ret=(set_config(FBTAG_ReadOnly,data)) ? FALSE : TRUE;
								break;
							case 8:
								ret=(set_config(FBTAG_AutoJoin,data)) ? FALSE : TRUE;
								break;
						}
					break;
				case 4: /* Menus */
					switch(id)
						{
							case 1:
								set_config(MNTAG_AccessRange,data);
								break;
							case 2:
								{
									BOOL ok=FALSE;
									if(((STRPTR) data)[0])
										{
											BOOL inuse=FALSE;
											BYTE *temp_name=NULL;
											BYTE c=NULL;
											{
												ULONG next=NULL;
												while(next=GetConfigTags(CFGTAG_Path,path,
														CFGTAG_Name,&temp_name,
														CFGTAG_Next,next,
														MNTAG_Character,&c,
														TAG_END))
													{
														if(toupper(((STRPTR) data)[0])==toupper(c))
															{
																if(stricmp(temp_name,name))
																	inuse=TRUE;
															}
													}
											}
											if(!inuse)
												ok=TRUE;
										}
									else
										ok=TRUE;
									if(ok)
										{
											SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
												MNTAG_Character,toupper(((STRPTR) data)[0]),
												TAG_END);
											{
												char s[2];
												sprintf(s,"%c",toupper(((STRPTR) data)[0]));
												TR_SetAttribute(project,id,0,(ULONG) s);
											}
										}
									else
										{
											if(data && ((STRPTR) data)[0])
												{
													char s[256];
													sprintf(s,"Character `%c' is in use.",toupper(((STRPTR) data)[0]));
													error_request(project,s);
												}
											{
												BYTE c=NULL,s[2];
												GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,MNTAG_Character,&c,TAG_END);
												sprintf(s,"%c",toupper(c));
												TR_SetAttribute(project,id,0,(ULONG) s);
											}
										}
								}
								break;
							case 3:
								SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
									MNTAG_Default,toupper(((STRPTR) data)[0]),
									TAG_END);
								{
									char s[2];
									sprintf(s,"%c",toupper(((STRPTR) data)[0]));
									TR_SetAttribute(project,id,0,(ULONG) s);
								}
								break;
							case 4:
								SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
									MNTAG_Number,toupper(((STRPTR) data)[0]),
									TAG_END);
								{
									char s[2];
									sprintf(s,"%c",toupper(((STRPTR) data)[0]));
									TR_SetAttribute(project,id,0,(ULONG) s);
								}
								break;
							case 5:
								set_config(MNTAG_Prompt,data);
								break;
							case 6:
								set_config(MNTAG_Command,data);
								break;
						}
					break;
				case 5: /* Message Bases */
					switch(id)
						{
							case 1:
								set_config(MBTAG_AccessRange,data);
								break;
							case 2:
								if(!strchr((STRPTR) data,' '))
									set_config(MBTAG_Group,data);
								else
									error_request(project,"Spaces are not allowed.");
								break;
							case 3:
								set_config(MBTAG_QuoteHeader,data);
								break;
							case 4:
								set_config(MBTAG_ReadOnly,data);
								break;
							case 5:
								set_config(MBTAG_AutoJoin,data);
								break;
							case 6:
								set_config(MBTAG_Days,atol((STRPTR) data));
								break;
						}
					break;
				case 6: /* News */
					switch(id)
						{
							case 1:
								set_config(NWSTAG_AccessRange,data);
								break;
							case 2:
								set_config(NWSTAG_File,data);
								{
									struct DateStamp ds={0};
									{
										BPTR fh;
										if(fh=Open((STRPTR) data,MODE_OLDFILE))
											{
												__aligned struct FileInfoBlock fib;
												if(ExamineFH(fh,&fib))
													ds=fib.fib_Date;
												Close(fh);
											}
									}
									if(ds.ds_Days || ds.ds_Minute  || ds.ds_Tick)
										{
											{
												TR_SetAttribute(project,4,TRAT_Text,NULL);
												{
													struct DateTime dt;
													char strdate[32],strtime[32];
													dt.dat_Stamp=ds;
													dt.dat_Format=FORMAT_USA;
													dt.dat_Flags=DTF_SUBST;
													dt.dat_StrDay=NULL;
													dt.dat_StrDate=strdate;
													dt.dat_StrTime=strtime;
													DateToStr(&dt);
													sprintf(date_buff,"%s %s",strdate,strtime);
												}
												TR_SetAttribute(project,4,TRAT_Text,(ULONG) date_buff);
												TR_SetAttribute(project,4,TRAT_Disabled,FALSE);
											}
										}
								}
								break;
							case 3:
								{
									BYTE file[256],*p;
									p=(BYTE *) TR_GetAttribute(project,2,0);
									if(asl_file_requester(project,"Select File...",(p) ? p : "",NULL,file))
										{
											set_config(NWSTAG_File,file);
											TR_SetAttribute(project,2,0,(ULONG) file);
											{
												struct DateStamp ds={0};
												{
													BPTR fh;
													if(fh=Open((STRPTR) file,MODE_OLDFILE))
														{
															__aligned struct FileInfoBlock fib;
															if(ExamineFH(fh,&fib))
																ds=fib.fib_Date;
															Close(fh);
														}
												}
												if(ds.ds_Days || ds.ds_Minute  || ds.ds_Tick)
													{
														{
															TR_SetAttribute(project,4,TRAT_Text,NULL);
															{
																struct DateTime dt;
																char strdate[32],strtime[32];
																dt.dat_Stamp=ds;
																dt.dat_Format=FORMAT_USA;
																dt.dat_Flags=DTF_SUBST;
																dt.dat_StrDay=NULL;
																dt.dat_StrDate=strdate;
																dt.dat_StrTime=strtime;
																DateToStr(&dt);
																sprintf(date_buff,"%s %s",strdate,strtime);
															}
															TR_SetAttribute(project,4,TRAT_Text,(ULONG) date_buff);
															TR_SetAttribute(project,4,TRAT_Disabled,FALSE);
														}
													}
											}
										}
								}
								break;
							case 4:
								break;
						}
					break;
				case 7: /* Nodes */
					switch(id)
						{
							case 1:
								set_config(NDTAG_AccessRange,data);
								break;
							case 2:
								set_config(NDTAG_Type,data);
								break;
							case 3:
								set_config(NDTAG_Device,data);
								break;
							case 4:
								{
									BYTE file[256],buff[256],*p;
									p=(BYTE *) TR_GetAttribute(project,3,0);
									strcpy(buff,"DEVS:");
									if(p)
										AddPart(buff,p,255);
									if(asl_file_requester(project,"Select Device...",buff,"#?.device",file))
										{
											set_config(NDTAG_Device,FilePart(file));
											TR_SetAttribute(project,3,0,(ULONG) FilePart(file));
										}
								}
								break;
							case 5:
								set_config(NDTAG_Unit,atol((STRPTR) data));
								break;
							case 6:
								set_config(NDTAG_BufferSize,atol((STRPTR) data));
								break;
							case 7:
								set_config(NDTAG_Baud,atol((STRPTR) data));
								break;
							case 8:
								set_config(NDTAG_Handshaking,data);
								break;
							case 9:
								set_config(NDTAG_AutoBaud,data);
								break;
							case 10:
								set_config(NDTAG_FrontEnd,data);
								break;
							case 11:
								set_config(NDTAG_Command,data);
								break;
						}
					break;
				case 8: /* Protocols */
					switch(id)
						{
							case 1:
								set_config(PRTAG_SendCommand,data);
								break;
							case 2:
								set_config(PRTAG_ReceiveCommand,data);
								break;
							case 3:
								set_config(PRTAG_Batch,data);
								break;
							case 4:
								set_config(PRTAG_Bidirectional,data);
								break;
						}
					break;
				case 9: /* System */
					switch(id)
						{
							case 5:
								set_config(SYSTAG_HeaderColor,data);
								break;
							case 6:
								set_config(SYSTAG_InputColor,data);
								break;
							case 7:
								set_config(SYSTAG_OutputColor,data);
								break;
							case 8:
								set_config(SYSTAG_PromptColor,data);
								break;
							case 9:
								set_config(SYSTAG_SystemColor,data);
								break;
							case 11:
								set_config(SYSTAG_OnlyRealNames,data);
								break;
							case 12:
								set_config(SYSTAG_FileIDDizExtractor,data);
								break;
						}
					break;
			}
		return(ret);
	}

BOOL asl_file_requester(struct TR_Project *project,BYTE *title,BYTE *file,BYTE *pattern,BYTE *result)
	{
		BOOL ret=FALSE;
		{
			struct FileRequester *fr;
			char dir[256];
			{
				strcpy(dir,"");
				{
					char *p;
					if(p=PathPart(file))
						sprintf(dir,"%.*s",p-file,file);
				}
			}
			if(fr=(struct FileRequester *) AllocAslRequestTags(ASL_FileRequest,
					ASLFR_TitleText,title,
					ASLFR_InitialDrawer,dir,
					ASLFR_InitialFile,FilePart(file),
					ASLFR_InitialPattern,(pattern) ? pattern : "#?",
					ASLFR_FilterDrawers,TRUE,
					TAG_END))
				{
					TR_LockProject(project);
					if(AslRequestTags(fr,TAG_END))
						{
							strcpy(result,fr->fr_Drawer);
							AddPart(result,fr->fr_File,255);
							ret=TRUE;
						}
					TR_UnlockProject(project);
					FreeAslRequest(fr);
				}
		}
		return(ret);
	}

BOOL asl_drawer_requester(struct TR_Project *project,BYTE *title,BYTE *drawer,BYTE *result)
	{
		BOOL ret=FALSE;
		{
			struct FileRequester *fr;
			if(fr=(struct FileRequester *) AllocAslRequestTags(ASL_FileRequest,
					ASLFR_TitleText,title,
					ASLFR_InitialDrawer,drawer,
					ASLFR_DrawersOnly,TRUE,
					TAG_END))
				{
					TR_LockProject(project);
					if(AslRequestTags(fr,TAG_END))
						{
							strcpy(result,fr->fr_Drawer);
							ret=TRUE;
						}
					TR_UnlockProject(project);
					FreeAslRequest(fr);
				}
		}
		return(ret);
	}

void set_defaults(BYTE *path,BYTE *name,UWORD prefsid)
	{
		switch(prefsid)
			{
				case 0: /* Access Groups */
					break;
				case 1: /* Archivers */
					break;
				case 2: /* Editors */
					SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
						EDTAG_Stack,4096,TAG_END);
					break;
				case 3: /* File Bases */
					SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
						FBTAG_AccessRange,"1-255",
						FBTAG_Directory,default_directory,
						FBTAG_DiskSpaceRequired,1024,
						FBTAG_AutoJoin,TRUE,
						TAG_END);
					break;
				case 4: /* Menus */
					SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
						MNTAG_AccessRange,"1-255",
						TAG_END);
					break;
				case 5: /* Message Bases */
					SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
						MBTAG_AccessRange,"1-255",
						MBTAG_QuoteHeader,default_quote_header,
						MBTAG_AutoJoin,TRUE,
						TAG_END);
					break;
				case 6: /* News */
					SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
						NWSTAG_AccessRange,"1-255",
						TAG_END);
					break;
				case 7: /* Nodes */
					SetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_DontSave,TRUE,
						NDTAG_AccessRange,"1-255",
						NDTAG_Type,NDTYP_Remote,
						NDTAG_BufferSize,65536,
						NDTAG_Baud,38400,
						NDTAG_Handshaking,TRUE,
						NDTAG_Command,default_node_command,
						TAG_END);
					break;
				case 8: /* Protocols */
					break;
			}
	}
