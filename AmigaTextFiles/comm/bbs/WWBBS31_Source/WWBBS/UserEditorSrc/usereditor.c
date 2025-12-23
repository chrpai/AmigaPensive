#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/triton.h>
#include <libraries/wwbbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/triton.h>
#include <proto/wwbbs.h>

#include "usereditor_rev.h"

char *ver=VERSTAG;

struct Library *WorldWideBase;

struct UserNode {
	struct Node un_Node;
	BYTE un_Text[41];
	BYTE un_Name[33];
};

struct AccessGroupNode {
	struct Node agn_Node;
	BYTE agn_Name[33];
};

void close_all(void);
struct List *CreateList(void);
void DeleteList(struct List *);
BOOL user_editor(BYTE *);
void view_info(BYTE *);
void view_stats(BYTE *);
BOOL select_access_group(BYTE *);

ProjectDefinition(main_trwintags)
	{
		WindowID(1),WindowPosition(TRWP_CENTERDISPLAY),
		WindowTitle("User Editor"),

		BeginMenu("Project"),
			MenuItem("?_About...",101),
			ItemBarlabel,
			MenuItem("Q_Quit",102),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				ListSel(NULL,1,0),TRAT_MinWidth,32,TRAT_MinHeight,20,
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

void main()
	{
		if(TR_OpenTriton(TRITON10VERSION,TRCA_Name,"UserEditor",TRCA_LongName,"User Editor",TRCA_Info,"Edits WWBBS Users",TRCA_Version,"2",TRCA_Release,"1",TRCA_Date,DATE,TAG_END))
			{

		if(WorldWideBase=OpenLibrary("wwbbs.library",0))
			{
				struct List *list;
				if(list=CreateList())
					{
						struct TR_Project *project;
						if(project=TR_OpenProject(Application,main_trwintags))
							{
								TR_SetAttribute(project,1,0,(ULONG) list);
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
																					case 1:
																						{
																							struct UserNode *node;
																							int i;
																							BOOL changed;
																							node=(struct UserNode *) list->lh_Head;
																							for(i=0;i<msg->trm_Data;i++)
																								{
																									if(node->un_Node.ln_Succ)
																										node=(struct UserNode *) node->un_Node.ln_Succ;
																								}
																							TR_LockProject(project);
																							changed=user_editor(node->un_Name);
																							TR_UnlockProject(project);
																							if(changed)
																								{
																									TR_SetAttribute(project,1,0,NULL);
																									DeleteList(list);
																									if(list=CreateList())
																										TR_SetAttribute(project,1,0,(ULONG) list);
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
																							sprintf(s,"%%3UserEditor 2.5 (%s)\n%%nCopyright © 1995 Arthur Choung\tInternet: choung@seas.ucla.edu or arthur@qedbbs.com\nUS Mail: 10324 Chestnut Street, Bellflower, CA 90706 USA",DATE);
																							TR_EasyRequestTags(Application,s,"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_LockProject,project,TREZ_Title,"About World Wide BBS...",TAG_END);
																						}
																						break;
																					case 102: /* Quit */
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
								}
								TR_CloseProject(project);
							}
						else
							TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
						DeleteList(list);
					}
				CloseLibrary(WorldWideBase);
			}

				TR_CloseTriton();
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
					BOOL onlyrealnames=FALSE;
					GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames);
					{
						ULONG next=0;
						BYTE *name=NULL,*username=NULL,*realname=NULL;
						UBYTE accesslevel=0;
						while(next=GetUserTags(USRTAG_Next,next,USRTAG_UserName,&username,USRTAG_RealName,&realname,USRTAG_AccessLevel,&accesslevel,TAG_END))
							{
								{
									struct UserNode *node;
									if(node=AllocVec(sizeof(struct UserNode),MEMF_CLEAR))
										{
											name=(onlyrealnames) ? realname : username;
											strcpy(node->un_Name,name);
											strcpy(node->un_Text,name);
											switch(accesslevel)
												{
													case 0:
														strcat(node->un_Text," [DEL]");
														break;
													case 1:
														strcat(node->un_Text," [NEW]");
														break;
													case 255:
														strcat(node->un_Text," [SYSOP]");
														break;
												}
											node->un_Node.ln_Name=node->un_Text;
											AddTail(list,(struct Node *) node);
										}
								}
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

/***/

#define string_gadget(text,id,maxchars) HorizGroupEAC,HorizGroupSAC,Space,TextID(text,id),EndGroup,Space,HorizGroupAC,StringGadget(NULL,id),TRAT_Value,maxchars,EndGroup,EndGroup

BYTE usereditor_title[256];

BYTE accesslevel_buff[4];
BYTE sessiontimelimit_buff[6];
BYTE inactivitytimelimit_buff[6];
BYTE ratiobyte_buff[4];
BYTE ratiofile_buff[4];
BYTE days_buff[11];

ProjectDefinition(usereditor_trwintags)
	{
		WindowID(2),WindowPosition(TRWP_CENTERDISPLAY),

		BeginMenu("Project"),
			MenuItem("Q_Quit",101),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				NamedFrameBox("Access"),
				HorizGroupA,
					Space,
					VertGroupA,
						Space,
						string_gadget("_Access Level",1,3),
						Space,
						string_gadget("_Session Time Limit",2,5),
						Space,
						string_gadget("_Inactivity Time Limit",3,5),
						Space,
						string_gadget("Ratio _Byte",4,3),
						Space,
						string_gadget("Ratio _File",5,3),
						Space,
						string_gadget("_Days",6,10),
						Space,
					EndGroup,
					Space,
				EndGroup,
				Space,
				HorizGroupEA,
					Button("Inf_o",11),
					Space,
					Button("S_tats",12),
				EndGroup,
				Space,
				HorizGroupEA,
					Button("Select Access _Group",13),
					Space,
					Button("D_elete User",14),
				EndGroup,
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

BOOL user_editor(BYTE *user)
	{
		BOOL ret=FALSE;
		struct TR_Project *project;
		if(project=TR_OpenProject(Application,usereditor_trwintags))
			{
				sprintf(usereditor_title,"User Editor: %s",user);
				TR_SetAttribute(project,0,TRWI_Title,(ULONG) usereditor_title);
				{
					UBYTE accesslevel=0;
					UWORD sessiontimelimit=0,inactivitytimelimit=0;
					UBYTE ratiobyte=0,ratiofile=0;
					ULONG days=0;
					GetUserTags(USRTAG_Name,user,
						USRTAG_AccessLevel,&accesslevel,
						USRTAG_SessionTimeLimit,&sessiontimelimit,USRTAG_InactivityTimeLimit,&inactivitytimelimit,
						USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,
						USRTAG_Days,&days,
						TAG_END);
					sprintf(accesslevel_buff,"%d",accesslevel);
					TR_SetAttribute(project,1,0,(ULONG) accesslevel_buff);
					sprintf(sessiontimelimit_buff,"%d",sessiontimelimit);
					TR_SetAttribute(project,2,0,(ULONG) sessiontimelimit_buff);
					sprintf(inactivitytimelimit_buff,"%d",inactivitytimelimit);
					TR_SetAttribute(project,3,0,(ULONG) inactivitytimelimit_buff);
					sprintf(ratiobyte_buff,"%d",ratiobyte);
					TR_SetAttribute(project,4,0,(ULONG) ratiobyte_buff);
					sprintf(ratiofile_buff,"%d",ratiofile);
					TR_SetAttribute(project,5,0,(ULONG) ratiofile_buff);
					sprintf(days_buff,"%ld",days);
					TR_SetAttribute(project,6,0,(ULONG) days_buff);
				}
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
																	case 1:
																		SetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,atol((STRPTR) msg->trm_Data),TAG_END);
																		ret=TRUE;
																		break;
																	case 2:
																		SetUserTags(USRTAG_Name,user,USRTAG_SessionTimeLimit,atol((STRPTR) msg->trm_Data),TAG_END);
																		break;
																	case 3:
																		SetUserTags(USRTAG_Name,user,USRTAG_InactivityTimeLimit,atol((STRPTR) msg->trm_Data),TAG_END);
																		break;
																	case 4:
																		SetUserTags(USRTAG_Name,user,USRTAG_RatioByte,atol((STRPTR) msg->trm_Data),TAG_END);
																		break;
																	case 5:
																		SetUserTags(USRTAG_Name,user,USRTAG_RatioFile,atol((STRPTR) msg->trm_Data),TAG_END);
																		break;
																	case 6:
																		SetUserTags(USRTAG_Name,user,USRTAG_Days,atol((STRPTR) msg->trm_Data),TAG_END);
																		break;
																}
															break;
														case TRMS_ACTION:
															switch(msg->trm_ID)
																{
																	case 11: /* Info */
																		view_info(user);
																		break;
																	case 12: /* Stats */
																		view_stats(user);
																		break;
																	case 13: /* Select Access Group */
																		{
																			BYTE accessgroup[33];
																			strcpy(accessgroup,"");
																			if(select_access_group(accessgroup))
																				{
																					UBYTE accesslevel=0;
																					UWORD sessiontimelimit=0,inactivitytimelimit=0;
																					UBYTE ratiobyte=0,ratiofile=0;
																					ULONG days=0;
																					GetConfigTags(CFGTAG_Path,"AccessGroups",CFGTAG_Name,accessgroup,
																						AGTAG_AccessLevel,&accesslevel,
																						AGTAG_SessionTimeLimit,&sessiontimelimit,
																						AGTAG_InactivityTimeLimit,&inactivitytimelimit,
																						AGTAG_RatioByte,&ratiobyte,
																						AGTAG_RatioFile,&ratiofile,
																						AGTAG_Days,&days,
																						TAG_END);
																					SetUserTags(USRTAG_Name,user,
																						USRTAG_AccessLevel,accesslevel,
																						USRTAG_SessionTimeLimit,sessiontimelimit,
																						USRTAG_InactivityTimeLimit,inactivitytimelimit,
																						USRTAG_RatioByte,ratiobyte,
																						USRTAG_RatioFile,ratiofile,
																						USRTAG_Days,days,
																						TAG_END);
																					sprintf(accesslevel_buff,"%d",accesslevel);
																					TR_SetAttribute(project,1,0,(ULONG) accesslevel_buff);
																					sprintf(sessiontimelimit_buff,"%d",sessiontimelimit);
																					TR_SetAttribute(project,2,0,(ULONG) sessiontimelimit_buff);
																					sprintf(inactivitytimelimit_buff,"%d",inactivitytimelimit);
																					TR_SetAttribute(project,3,0,(ULONG) inactivitytimelimit_buff);
																					sprintf(ratiobyte_buff,"%d",ratiobyte);
																					TR_SetAttribute(project,4,0,(ULONG) ratiobyte_buff);
																					sprintf(ratiofile_buff,"%d",ratiofile);
																					TR_SetAttribute(project,5,0,(ULONG) ratiofile_buff);
																					sprintf(days_buff,"%ld",days);
																					TR_SetAttribute(project,6,0,(ULONG) days_buff);
																					ret=TRUE;
																				}
																		}
																		break;
																	case 14: /* Delete User */
																		{
																			BYTE s[64];
																			sprintf(s,"Delete `%s'?",user);
																			if(TR_EasyRequestTags(Application,s,"Delete|Cancel",TREZ_LockProject,project,TREZ_Title,"User Editor",TAG_END)==1)
																				{
																					RemUserTags(USRTAG_Name,user,TAG_END);
																					ret=TRUE;
																					kg=FALSE;
																				}
																		}
																		break;
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
				}
				TR_CloseProject(project);
			}
		else
			TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
		return(ret);
	}

/***/

#define text(text,id,minwidth) HorizGroupSA,TextID(text,id),TRAT_MinWidth,minwidth,Space,EndGroup

BYTE username[33];
BYTE realname[31];
BYTE uucpname[9];
BYTE password[11];
BYTE address[31];
BYTE city[21];
BYTE state[21];
BYTE zip[11];
BYTE country[4];
BYTE phone[16];
BYTE birthdate[14];

ProjectDefinition(viewinfo_trwintags)
	{
		WindowID(3),WindowPosition(TRWP_CENTERDISPLAY),WindowTitle("Viewing User Info..."),

		BeginMenu("Project"),
			MenuItem("Q_Quit",101),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				_TextBox,
				HorizGroup,
					Space,
					VertGroupEA,
						SpaceS,
						TextN("User Name:"),
						TextN("Real Name:"),
						TextN("UUCP Name:"),
						TextN("Password:"),
						TextN("Address:"),
						TextN("City:"),
						TextN("State:"),
						TextN("Zip:"),
						TextN("Country:"),
						TextN("Phone:"),
						TextN("Birth Date:"),
						SpaceS,
					EndGroup,
					SpaceS,
					VertGroupEA,
						SpaceS,
						text(NULL,1,32),
						text(NULL,2,30),
						text(NULL,3,8),
						text(NULL,4,10),
						text(NULL,5,30),
						text(NULL,6,20),
						text(NULL,7,20),
						text(NULL,8,10),
						text(NULL,9,3),
						text(NULL,10,15),
						text(NULL,11,13),
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

void view_info(BYTE *user)
	{
		struct TR_Project *project;
		if(project=TR_OpenProject(Application,viewinfo_trwintags))
			{
				{
					BYTE phonecode[7],phonenumber[9];
					UBYTE birthday=0,birthmonth=0;
					UWORD birthyear=0;
					strcpy(phonecode,"");
					strcpy(phonenumber,"");
					GetUserTags(USRTAG_Name,user,
						USRTAG_UserName,username,USRTAG_RealName,realname,USRTAG_UUCPName,uucpname,
						USRTAG_Password,password,
						USRTAG_Address,address,
						USRTAG_City,city,
						USRTAG_State,state,
						USRTAG_Zip,zip,
						USRTAG_Country,country,
						USRTAG_PhoneCode,phonecode,
						USRTAG_PhoneNumber,phonenumber,
						USRTAG_BirthDay,&birthday,
						USRTAG_BirthMonth,&birthmonth,
						USRTAG_BirthYear,&birthyear,
						TAG_END);
					TR_SetAttribute(project,1,TRAT_Text,(ULONG) ((strlen(username)) ? username : "Not applicable"));
					TR_SetAttribute(project,2,TRAT_Text,(ULONG) realname);
					TR_SetAttribute(project,3,TRAT_Text,(ULONG) uucpname);
					TR_SetAttribute(project,4,TRAT_Text,(ULONG) password);
					TR_SetAttribute(project,5,TRAT_Text,(ULONG) address);
					TR_SetAttribute(project,6,TRAT_Text,(ULONG) city);
					TR_SetAttribute(project,7,TRAT_Text,(ULONG) state);
					TR_SetAttribute(project,8,TRAT_Text,(ULONG) zip);
					TR_SetAttribute(project,9,TRAT_Text,(ULONG) country);
					if(strlen(phonecode) && strlen(phonenumber))
						sprintf(phone,"%s-%s",phonecode,phonenumber);
					else
						strcpy(phone,"");
					TR_SetAttribute(project,10,TRAT_Text,(ULONG) phone);
					if(birthday && birthmonth && birthyear)
						sprintf(birthdate,"%d/%d/%d",birthmonth,birthday,birthyear);
					else
						strcpy(birthdate,"");
					TR_SetAttribute(project,11,TRAT_Text,(ULONG) birthdate);
				}
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
				}
				TR_CloseProject(project);
			}
		else
			TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
	}

/***/

BYTE sessions_buff[128];
BYTE access_buff[128];
BYTE ratio_buff[128];
BYTE email_buff[128];
BYTE messages_buff[128];
BYTE files_buff[128];
BYTE credits_buff[128];
BYTE laston_buff[128];

ProjectDefinition(viewstats_trwintags)
	{
		WindowID(4),WindowPosition(TRWP_CENTERDISPLAY),WindowTitle("Viewing User Stats..."),

		BeginMenu("Project"),
			MenuItem("Q_Quit",101),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				_TextBox,
				HorizGroup,
					Space,
					VertGroupEA,
						SpaceS,
						TextN("Sessions:"),
						TextN("Access:"),
						TextN("Ratio:"),
						TextN("Email:"),
						TextN("Messages:"),
						TextN("Files:"),
						TextN("Credits:"),
						TextN("Last On:"),
						SpaceS,
					EndGroup,
					SpaceS,
					VertGroupEA,
						SpaceS,
						text(NULL,1,10),
						text(NULL,2,3),
						text(NULL,3,30),
						text(NULL,4,40),
						text(NULL,5,40),
						text(NULL,6,60),
						text(NULL,7,40),
						text(NULL,8,20),
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

void view_stats(BYTE *user)
	{
		struct TR_Project *project;
		if(project=TR_OpenProject(Application,viewstats_trwintags))
			{
				{
					ULONG sessions=0;
					UBYTE accesslevel=0,ratiobyte=0,ratiofile=0;
					ULONG emailread=0,emailwritten=0,messagesread=0,messageswritten=0,filesuploaded=0,filesdownloaded=0,kilosuploaded=0,kilosdownloaded=0,creditsfile=0,creditskilo=0;
					struct DateStamp laston={0};
					GetUserTags(USRTAG_Name,user,
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
						USRTAG_LastOn,&laston,
						TAG_END);
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
					TR_SetAttribute(project,1,TRAT_Text,(ULONG) sessions_buff);
					TR_SetAttribute(project,2,TRAT_Text,(ULONG) access_buff);
					TR_SetAttribute(project,3,TRAT_Text,(ULONG) ratio_buff);
					TR_SetAttribute(project,4,TRAT_Text,(ULONG) email_buff);
					TR_SetAttribute(project,5,TRAT_Text,(ULONG) messages_buff);
					TR_SetAttribute(project,6,TRAT_Text,(ULONG) files_buff);
					TR_SetAttribute(project,7,TRAT_Text,(ULONG) credits_buff);
					{
						struct DateTime dt;
						BYTE date[16],time[16];
						dt.dat_Stamp=laston;
						dt.dat_Format=FORMAT_USA;
						dt.dat_Flags=DTF_SUBST;
						dt.dat_StrDay=NULL;
						dt.dat_StrDate=date;
						dt.dat_StrTime=time;
						DateToStr(&dt);
						sprintf(laston_buff,"%s %s",date,time);
						TR_SetAttribute(project,8,TRAT_Text,(ULONG) laston_buff);
					}
				}
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
				}
				TR_CloseProject(project);
			}
		else
			TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
	}

/***/

ProjectDefinition(selectaccessgroup_trwintags)
	{
		WindowID(5),WindowPosition(TRWP_CENTERDISPLAY),WindowTitle("Select Access Group..."),

		BeginMenu("Project"),
			MenuItem("Q_Quit",101),

		HorizGroupA,
			Space,
			VertGroupA,
				Space,
				ListSel(NULL,1,0),
				Space,
			EndGroup,
			Space,
		EndGroup,

		EndProject
	};

BOOL select_access_group(BYTE *result)
	{
		BOOL ret=FALSE;
		struct List list;
		NewList(&list);
		{
			ULONG next=0;
			BYTE *name=0;
			while(next=GetConfigTags(CFGTAG_Path,"AccessGroups",CFGTAG_Name,&name,CFGTAG_Next,next,TAG_END))
				{
					{
						struct AccessGroupNode *node;
						if(node=AllocVec(sizeof(struct AccessGroupNode),MEMF_CLEAR))
							{
								strcpy(node->agn_Name,name);
								node->agn_Node.ln_Name=node->agn_Name;
								AddTail(&list,(struct Node *) node);
							}
					}
				}
		}
		{
			struct TR_Project *project;
			if(project=TR_OpenProject(Application,selectaccessgroup_trwintags))
				{
					TR_SetAttribute(project,1,0,(ULONG) &list);
					{
						BOOL kg=TRUE;
						while(kg)
							{
								TR_Wait(Application,0);
								{
									struct TR_Message *msg;
									if(kg && (msg=TR_GetMsg(Application)))
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
																				struct AccessGroupNode *node;
																				int i;
																				node=(struct AccessGroupNode *) list.lh_Head;
																				for(i=0;i<msg->trm_Data;i++)
																					{
																						if(node->agn_Node.ln_Succ)
																							node=(struct AccessGroupNode *) node->agn_Node.ln_Succ;
																					}
																				strcpy(result,node->agn_Name);
																				ret=TRUE;
																				kg=FALSE;
																			}
																			break;
																	}
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
					}
					TR_CloseProject(project);
				}
			else
				TR_EasyRequestTags(Application,TR_GetErrorString(TR_GetLastError(Application)),"Okay",TREZ_ReqPos,TRWP_CENTERDISPLAY,TREZ_Title,"World Wide BBS Error Request",TAG_END);
		}
		return(ret);
	}
