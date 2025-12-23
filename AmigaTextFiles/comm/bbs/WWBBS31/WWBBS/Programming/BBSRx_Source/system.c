#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/rexxsyslib.h>
#include <proto/wwbbs.h>

#include "rx.h"

extern BOOL FindCommand(struct Command *,BYTE *,UWORD *,UWORD *);
extern struct List *OpenMessageGlobal(BYTE *);
extern void CloseMessageGlobal(struct List *);
extern BYTE *GetMessageGlobalRead(struct List *,BYTE *);
extern void MessageUpdateVars(BYTE *);
extern void FileUpdateVars(BYTE *);
extern struct List *OpenFileGlobal(BYTE *);
extern void CloseFileGlobal(struct List *);
extern struct List *OpenFileTagList(BYTE *);
extern void CloseFileTagList(struct List *);
extern ULONG GetFileGlobalHigh(struct List *,BYTE *);
extern void SetMessageDoorVariables(BYTE *);
extern void SetFileDoorVariables(BYTE *);

void ANSIDetect(BYTE *);
void DoLogoff(BYTE *);
long DoLogon(BYTE *);
void EditPrefs(BYTE *);
void EditUser(BYTE *);
void Logon(BYTE *,BYTE *,char *,char **);
void ReadNews(BYTE *,BYTE *);
long Register(BYTE *);
void RunMenu(BYTE *,BYTE *);
void ShowStats(BYTE *);
void UserEditor(BYTE *);
void UserList(BYTE *);
void ValidateNewUsers(BYTE *);
void WhoIsOnline(BYTE *);

void SetDoorVariables(BYTE *);
int SelectConfig(BYTE *,BYTE *,BYTE *,BYTE *,BYTE *);
int SelectEditor(BYTE *,BYTE *);
int SelectNews(BYTE *,UBYTE,BYTE *);
int SelectNewUser(BYTE *,BYTE *);
int SelectOnlineUser(BYTE *,BYTE *);
void edit_user(BYTE *,BYTE *);
void PrintPrompt(BYTE *,BYTE *);
BOOL IsOnline(BYTE *);

void SystemCommand(BYTE *id,UWORD cmd,BYTE *arg,char *result_buff,char **result,char **error,long *errorlevel)
	{
		switch(cmd)
			{
				case SYS_ANSIDetect:
					ANSIDetect(id);
					break;
				case SYS_ChatRequest:
					break;
				case SYS_Delay:
					if(arg)
						Delay(atol(arg));
					break;
				case SYS_DoLogoff:
					DoLogoff(id);
					break;
				case SYS_DoLogon:
					*errorlevel=DoLogon(id);
					break;
				case SYS_DosCommand:
					if(arg)
						{
							printf("~r");
							SetMode(Input(),0);
							SystemTags(arg,TAG_END);
							SetMode(Input(),1);
						}
					break;
				case SYS_EditPrefs:
					EditPrefs(id);
					break;
				case SYS_EditUser:
					EditUser(id);
					break;
				case SYS_Logon:
					Logon(id,arg,result_buff,result);
					break;
				case SYS_PageUser:
					{
						BYTE node[33],text[78];
						strcpy(node,"");
						if(SelectOnlineUser(id,node))
							{
								printf("~p\nEnter the message that you want to send.\n: ");
								if(GetLine(text,77,NULL))
									{
										if(NodeCommandTags(node,NDCMD_Page,NDPAGETAG_FromNode,id,NDPAGETAG_Text,text,TAG_END))
											printf("~s\nPage sent.\n");
										else
											printf("~s\nUnable to send page.\n");
									}
							}
					}
					break;
				case SYS_ReadNews:
					if(arg)
						ReadNews(id,arg);
					break;
				case SYS_Register:
					*errorlevel=Register(id);
					break;
				case SYS_RunMenu:
					if(arg)
						RunMenu(id,arg);
					break;
				case SYS_ShowStats:
					ShowStats(id);
					break;
				case SYS_StartTimer:
					TimerCommandTags(id,TMRCMD_Start,TAG_END);
					break;
				case SYS_StopTimer:
					TimerCommandTags(id,TMRCMD_Stop,TAG_END);
					break;
				case SYS_UserEditor:
					UserEditor(id);
					break;
				case SYS_UserList:
					UserList(id);
					break;
				case SYS_ValidateNewUsers:
					ValidateNewUsers(id);
					break;
				case SYS_WhoIsOnline:
					WhoIsOnline(id);
					break;
			}
	}

void ANSIDetect(BYTE *id)
	{
		BOOL local=FALSE;
		GetStatusTags(STTAG_Name,id,STTAG_Local,&local,TAG_END);
		if(local)
			SetStatusTags(STTAG_Name,id,STTAG_ANSI,TRUE,TAG_END);
		else
			{
				BOOL ansi=FALSE;
				SerialCommandTags(id,SERCMD_Write,SERWRITETAG_Buffer,"\033[6n",SERWRITETAG_Size,-1,TAG_END);
				Delay(25L);
				{
					BYTE c;
					while(SerialCommandTags(id,SERCMD_Read,SERREADTAG_Buffer,&c,SERREADTAG_Size,1,TAG_END))
						ansi=TRUE;
				}
				SetStatusTags(STTAG_Name,id,STTAG_ANSI,ansi,TAG_END);
			}
	}

void DoLogoff(BYTE *id)
	{
		BYTE username[33];
		UWORD timeused=0;
		struct DateStamp ds={0};
		DateStamp(&ds);
		{
			BYTE home_dir[256];
			BPTR fh;
			strcpy(home_dir,"");
			if(GetVar("HOME",home_dir,255,NULL)!=-1)
				{
					AddPart(home_dir,".lastareas",255);
					if(fh=Open(home_dir,MODE_NEWFILE))
						{
							FPuts(fh,message_path);FPutC(fh,'\n');
							FPuts(fh,message_area);FPutC(fh,'\n');
							FPuts(fh,file_path);FPutC(fh,'\n');
							FPuts(fh,file_area);FPutC(fh,'\n');
							Close(fh);
						}
				}
		}
		if(message_global) CloseMessageGlobal(message_global);
		if(file_taglist) CloseFileTagList(file_taglist);
		if(file_global) CloseFileGlobal(file_global);
		GetStatusTags(STTAG_Name,id,STTAG_UserName,username,STTAG_TimeUsed,&timeused,TAG_END);
		SetUserTags(USRTAG_Name,username,USRTAG_TimeUsed,timeused,USRTAG_LastOn,&ds,TAG_END);
		UnLoadUserTags(USRTAG_Name,username,TAG_END);
		{
			BYTE *nodename=NULL,nodeusername[33];
			BYTE text[78];
			BOOL usermonitor=FALSE;
			ULONG next=0;
			sprintf(text,"`%s' logged off.",username);
			while(next=GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,&nodename,CFGTAG_Next,next,TAG_END))
				{
					strcpy(nodeusername,"");
					GetStatusTags(STTAG_Name,nodename,STTAG_UserName,nodeusername,STTAG_UserMonitor,&usermonitor,TAG_END);
					if(strlen(nodeusername))
						{
							if(strcmp(nodename,id) && usermonitor)
								NodeCommandTags(nodename,NDCMD_Page,NDPAGETAG_FromNode,id,NDPAGETAG_Text,text,TAG_END);
						}
				}
		}
		{
			char s[256];
			sprintf(s,"`%s' logged off.",username);
			LogEntry(id,"Callers",s);
		}
	}

long DoLogon(BYTE *id)
	{
		long ret=0;
		{
			BYTE username[33];
			strcpy(username,"");
			GetStatusTags(STTAG_Name,id,STTAG_UserName,username,TAG_END);
			if(strlen(username))
				{
					BYTE nodeaccessrange[21];
					UBYTE useraccesslevel=0;
					strcpy(nodeaccessrange,"");
					GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_AccessRange,nodeaccessrange,TAG_END);
					GetUserTags(USRTAG_Name,username,USRTAG_AccessLevel,&useraccesslevel,TAG_END);
					if(IsRange(nodeaccessrange,useraccesslevel))
						{
							LoadUserTags(USRTAG_Name,username,TAG_END);
							{
								BYTE logonstack[21];
								ULONG sessions=0;
								UWORD sessiontimelimit=0,inactivitytimelimit=0;
								struct DateStamp laston={0};
								UWORD timeremaining=0,timeused=0;
								struct DateStamp ds={0};
								DateStamp(&ds);
								GetUserTags(USRTAG_Name,username,USRTAG_LogonStack,logonstack,USRTAG_SessionTimeLimit,&sessiontimelimit,USRTAG_InactivityTimeLimit,&inactivitytimelimit,USRTAG_Sessions,&sessions,USRTAG_LastOn,&laston,USRTAG_TimeUsed,&timeused,TAG_END);
								if(ds.ds_Days!=laston.ds_Days)
									timeused=0;
								timeremaining=(sessiontimelimit-timeused<0) ? 0 : sessiontimelimit-timeused;
								SetStatusTags(STTAG_Name,id,STTAG_InactivityTimeLimit,inactivitytimelimit,STTAG_Stack,logonstack,TAG_END);
								sessions++;
								SetUserTags(USRTAG_Name,username,USRTAG_Sessions,sessions,USRTAG_TimeUsed,timeused,TAG_END);
								if(timeremaining)
									{
										TimerCommandTags(id,TMRCMD_Stop,TAG_END);
										TimerCommandTags(id,TMRCMD_Start,TMRSTARTTAG_TimeRemaining,timeremaining,TMRSTARTTAG_TimeUsed,timeused,TAG_END);
									}
								else
									SetStatusTags(STTAG_Name,id,STTAG_Panic,TRUE,TAG_END);
							}
							SetDoorVariables(id);
							{
								BOOL usermonitor=FALSE;
								GetUserTags(USRTAG_Name,username,USRTAG_UserMonitor,&usermonitor,TAG_END);
								SetStatusTags(STTAG_Name,id,STTAG_UserMonitor,usermonitor,TAG_END);
							}
							message_global=OpenMessageGlobal(id);
							file_global=OpenFileGlobal(id);
							file_taglist=OpenFileTagList(id);
							{
								BYTE temp_message_path[256],temp_message_area[33];
								BYTE temp_file_path[256],temp_file_area[33];
								{
									BYTE home_dir[256];
									BPTR fh;
									char *p;
									strcpy(home_dir,"");
									if(GetVar("HOME",home_dir,255,NULL)!=-1)
										{
											AddPart(home_dir,".lastareas",255);
											if(fh=Open(home_dir,MODE_OLDFILE))
												{
													FGets(fh,temp_message_path,255); if(p=strchr(temp_message_path,'\n')) *p=NULL;
													FGets(fh,temp_message_area,255); if(p=strchr(temp_message_area,'\n')) *p=NULL;
													FGets(fh,temp_file_path,255); if(p=strchr(temp_file_path,'\n')) *p=NULL;
													FGets(fh,temp_file_area,255); if(p=strchr(temp_file_area,'\n')) *p=NULL;
													Close(fh);
												}
										}
								}
								{
									if(strlen(temp_message_path) || strlen(temp_message_area))
										{
											if(strlen(temp_message_area))
												{
													BYTE accessrange[21],group[21];
													strcpy(accessrange,"");
													strcpy(group,"");
													if(GetConfigTags(CFGTAG_Path,temp_message_path,CFGTAG_Name,temp_message_area,MBTAG_AccessRange,accessrange,MBTAG_Group,group,TAG_END))
														{
															if(IsRange(accessrange,useraccesslevel))
																{
																	strcpy(message_path,temp_message_path);
																	strcpy(message_area,temp_message_area);
																	strcpy(message_group,group);
																	message_current=0;
																	if(FindName(message_global,group))
																		strcpy(message_read,GetMessageGlobalRead(message_global,group));
																	else
																		strcpy(message_read,"");
																	message_currentthread=0;
																	{
																		char *p;
																		if(p=strchr(message_path,'/'))
																			{
																				p++;
																				sprintf(message_fullpath,"%s/%s",p,message_area);
																			}
																		else
																			strcpy(message_fullpath,message_area);
																	}
																}
														}
													MessageUpdateVars(id);
												}	
											else
												{
													strcpy(message_path,temp_message_path);
													{
														char *p;
														if(p=strchr(message_path,'/'))
															{
																p++;
																sprintf(message_fullpath,"%s/None",p);
															}
														else
															strcpy(message_fullpath,"None");
													}
												}
										}
									if(strlen(temp_file_path) || strlen(temp_file_area))
										{
											if(strlen(temp_file_area))
												{
													BYTE accessrange[21],group[21];
													strcpy(accessrange,"");
													strcpy(group,"");
													if(GetConfigTags(CFGTAG_Path,temp_file_path,CFGTAG_Name,temp_file_area,FBTAG_AccessRange,accessrange,FBTAG_Group,group,TAG_END))
														{
															if(IsRange(accessrange,useraccesslevel))
																{
																	strcpy(file_path,temp_file_path);
																	strcpy(file_area,temp_file_area);
																	strcpy(file_group,group);
																	file_current=file_high=GetFileGlobalHigh(file_global,group);
																	{
																		char *p;
																		if(p=strchr(file_path,'/'))
																			{
																				p++;
																				sprintf(file_fullpath,"%s/%s",p,file_area);
																			}
																		else
																			strcpy(file_fullpath,file_area);
																	}
																}
														}
													FileUpdateVars(id);
												}
											else
												{
													strcpy(file_path,temp_file_path);
													{
														char *p;
														if(p=strchr(file_path,'/'))
															{
																p++;
																sprintf(file_fullpath,"%s/None",p);
															}
														else
															strcpy(file_fullpath,"None");
													}
												}
										}
								}
							}
							SetMessageDoorVariables(id);
							SetFileDoorVariables(id);
							{
								BYTE *nodename=NULL,nodeusername[33];
								BYTE text[78];
								BOOL usermonitor=FALSE;
								ULONG next=0;
								sprintf(text,"`%s' logged on.",username);
								while(next=GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,&nodename,CFGTAG_Next,next,TAG_END))
									{
										strcpy(nodeusername,"");
										GetStatusTags(STTAG_Name,nodename,STTAG_UserName,nodeusername,STTAG_UserMonitor,&usermonitor,TAG_END);
										if(strlen(nodeusername))
											{
												if(strcmp(nodename,id) && usermonitor)
													NodeCommandTags(nodename,NDCMD_Page,NDPAGETAG_FromNode,id,NDPAGETAG_Text,text,TAG_END);
											}
									}
							}
							{
								char s[256];
								sprintf(s,"`%s' logged on.",username);
								LogEntry(id,"Callers",s);
							}
							ret=1;
						}
				}
		}
		return(ret);
	}

void EditPrefs(BYTE *id)
	{
		BYTE name[33];
		UBYTE screenwidth=0,screenheight=0;
		BYTE logonstack[21],editor[33],protocol[33],archiver[33];
		BOOL hotkeys=FALSE,menus=FALSE,usermonitor=FALSE;
		strcpy(name,"");
		strcpy(logonstack,"");
		strcpy(editor,"");
		strcpy(protocol,"");
		strcpy(archiver,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
		if(strlen(name))
			{
				GetUserTags(USRTAG_Name,name,
					USRTAG_ScreenWidth,&screenwidth,
					USRTAG_ScreenHeight,&screenheight,
					USRTAG_LogonStack,logonstack,
					USRTAG_Editor,editor,
					USRTAG_Protocol,protocol,
					USRTAG_Archiver,archiver,
					USRTAG_HotKeys,&hotkeys,
					USRTAG_Menus,&menus,
					USRTAG_UserMonitor,&usermonitor,
					TAG_END);
				{
					char *template[]={
						"[1] Screen Size:   ",
						"[2] Logon Stack:   ",
						"[3] Editor:        ",
						"[4] Protocol:      ",
						"[5] Archiver:      ",
						"[6] Menu Input:    ",
						"[7] Help Level:    ",
						"[8] User Monitor:  ",
						"[9] Edit Signature",
						NULL };
					char s[256],buff[64],*p;
					int index;
					BOOL even,kg=TRUE;
					while(!panic && kg)
						{
							printf("~o\n");
							index=0;
							even=FALSE;
							while(template[index])
								{
									if(WaitForChar(Input(),0))
										{
											getchar();
											break;
										}
									switch(index)
										{
											case 0: sprintf(buff,"%dx%d",screenwidth,screenheight);p=buff; break;
											case 1: p=logonstack; break;
											case 2: p=(strlen(editor)) ? editor : "None"; break;
											case 3: p=(strlen(protocol)) ? protocol : "None"; break;
											case 4: p=(strlen(archiver)) ? archiver : "None"; break;
											case 5: p=(hotkeys) ? "Hot Keys" : "Command Stacking"; break;
											case 6: p=(menus) ? "Novice" : "Expert"; break;
											case 7: p=(usermonitor) ? "On" : "Off"; break;
											case 8: p=""; break;
											default: p=NULL; break;
										}
									if(p)
										{
											if(even) { sprintf(&s[strlen(s)]," %s",template[index]); } else strcpy(s,template[index]);
											if(strlen(template[index])+strlen(p)>39) { sprintf(&s[strlen(s)],"%.*s...",36-strlen(template[index]),p); }
											else { if(even) strcat(s,p); else sprintf(&s[strlen(s)],"%-*s",39-strlen(template[index]),p); }
											if(even) { printf("%s\n",s); even=FALSE; } else even=TRUE;
										}
									index++;
								}
							if(even) printf("%s\n",s);
							printf("~p\nEdit Preferences: ");
							if(GetLine(s,2,GLFLG_Digits))
								{
									index=atoi(s);
									switch(index)
										{
											case 1:
												printf("~p\nEnter your screen width in columns.\n: ");
												if(GetLine(s,3,GLFLG_Digits))
													{
														screenwidth=atoi(s);
														printf("~p\nEnter your screen height in rows.\n: ");
														if(GetLine(s,3,GLFLG_Digits))
															screenheight=atoi(s);
													}
												break;
											case 2:
												printf("~p\nEnter your logon stack.\n: ");
												if(GetLine(s,20,GLFLG_ToUpper))
													strcpy(logonstack,s);
												break;
											case 3:
												if(SelectEditor(id,s))
													strcpy(editor,s);
												break;
											case 4:
												if(SelectConfig(id,"Protocols","Protocol","protocols",s))
													strcpy(protocol,s);
												break;
											case 5:
												if(SelectConfig(id,"Archivers","Archiver","archivers",s))
													strcpy(archiver,s);
												break;
											case 6:
												hotkeys^=1;
												break;
											case 7:
												menus^=1;
												break;
											case 8:
												usermonitor^=1;
												SetStatusTags(STTAG_Name,id,STTAG_UserMonitor,usermonitor,TAG_END);
												break;
											case 9:
												{
													BYTE filename[256];
													strcpy(filename,"");
													if(GetVar("HOME",filename,255,NULL)!=-1)
														{
															AddPart(filename,".signature",255);
															printf("~p\nEnter a signature for yourself.\n");
															RunEditor(filename);
														}
												}
												break;
										}
								}
							else
								kg=FALSE;
						}
				}
				SetUserTags(USRTAG_Name,name,
					USRTAG_ScreenWidth,screenwidth,
					USRTAG_ScreenHeight,screenheight,
					USRTAG_LogonStack,logonstack,
					USRTAG_Editor,editor,
					USRTAG_Protocol,protocol,
					USRTAG_Archiver,archiver,
					USRTAG_HotKeys,hotkeys,
					USRTAG_Menus,menus,
					USRTAG_UserMonitor,usermonitor,
					TAG_END);
				SetDoorVariables(id);
			}
	}

void EditUser(BYTE *id)
	{
		BOOL onlyrealnames=FALSE;
		BYTE name[33],username[33],realname[31],uucpname[9],password[11],address[31],city[21],state[21],zip[11],country[4],phonecode[7],phonenumber[9],phone[16];
		UBYTE birthday=0,birthmonth=0;
		UWORD birthyear=0;
		BYTE birthdate[11];
		strcpy(name,"");
		strcpy(username,"");
		strcpy(realname,"");
		strcpy(uucpname,"");
		strcpy(password,"");
		strcpy(address,"");
		strcpy(city,"");
		strcpy(state,"");
		strcpy(zip,"");
		strcpy(country,"");
		strcpy(phonecode,"");
		strcpy(phonenumber,"");
		strcpy(phone,"");
		strcpy(birthdate,"");
		GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
		GetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
		if(strlen(name))
			{
				GetUserTags(USRTAG_Name,name,
					USRTAG_UserName,username,USRTAG_RealName,realname,USRTAG_UUCPName,uucpname,
					USRTAG_Password,password,
					USRTAG_Address,address,USRTAG_City,city,USRTAG_State,state,USRTAG_Zip,zip,USRTAG_Country,country,
					USRTAG_PhoneCode,phonecode,USRTAG_PhoneNumber,phonenumber,
					USRTAG_BirthDay,&birthday,USRTAG_BirthMonth,&birthmonth,USRTAG_BirthYear,&birthyear,
					TAG_END);
				if(strlen(phonecode) && strlen(phonenumber))
					sprintf(phone,"%s-%s",phonecode,phonenumber);
				if(birthday && birthmonth && birthyear)
					sprintf(birthdate,"%d/%d/%d",birthmonth,birthday,birthyear);
				{
					char *template[]={
						"[-] User Name:  ",
						"[-] Real Name:  ",
						"[-] UUCP Name:  ",
						"[1] Password:   ",
						"[2] Address:    ",
						"[3] City:       ",
						"[4] State:      ",
						"[5] Zip:        ",
						"[6] Country:    ",
						"[7] Phone:      ",
						"[8] Birth Date: ",
						NULL };
					char s[256],*p;
					int index;
					BOOL even,kg=TRUE;
					while(!panic && kg)
						{
							printf("~o\n");
							index=0;
							even=FALSE;
							while(template[index])
								{
									if(WaitForChar(Input(),0))
										{
											getchar();
											break;
										}
									switch(index)
										{
											case 0: p=(onlyrealnames) ? "Not applicable" : username; break;
											case 1: p=realname; break;
											case 2: p=uucpname; break;
											case 3: p=password; break;
											case 4: p=address; break;
											case 5: p=city; break;
											case 6: p=state; break;
											case 7: p=zip; break;
											case 8: p=country; break;
											case 9: p=phone; break;
											case 10: p=birthdate; break;
											default: p=NULL; break;
										}
									if(p)
										{
											if(even) { sprintf(&s[strlen(s)]," %s",template[index]); } else strcpy(s,template[index]);
											if(strlen(template[index])+strlen(p)>39) { sprintf(&s[strlen(s)],"%.*s...",36-strlen(template[index]),p); }
											else { if(even) strcat(s,p); else sprintf(&s[strlen(s)],"%-*s",39-strlen(template[index]),p); }
											if(even) { printf("%s\n",s); even=FALSE; } else even=TRUE;
										}
									index++;
								}
							if(even) printf("%s\n",s);
							printf("~p\nEdit User: ");
							if(GetLine(s,1,GLFLG_Digits))
								{
									index=atoi(s);
									switch(index)
										{
											case 1:
												printf("~p\nEnter the password that you wish to use.\n: ");
												if(GetLine(s,10,GLFLG_NoEcho))
													strcpy(password,s);
												break;
											case 2:
												printf("~p\nEnter your street address.\n: ");
												if(GetLine(s,30,GLFLG_Format))
													strcpy(address,s);
												break;
											case 3:
												printf("~p\nEnter the city that you live in.\n: ");
												if(GetLine(s,20,GLFLG_Format))
													strcpy(city,s);
												break;
											case 4:
												printf("~p\nEnter the state/province that you live in, if applicable.\n: ");
												if(GetLine(s,20,GLFLG_Format))
													strcpy(state,s);
												break;
											case 5:
												printf("~p\nEnter your zip code.\n: ");
												if(GetLine(s,10,GLFLG_ToUpper))
													strcpy(zip,s);
												break;
											case 6:
												printf("~p\nEnter the country that you live in.\n: ");
												if(GetLine(s,3,GLFLG_ToUpper|GLFLG_Chars))
													strcpy(country,s);
												break;
											case 7:
												printf("~p\nEnter your area/country code.\n: ");
												if(GetLine(s,6,GLFLG_Digits|GLFLG_NoSpaces))
													{
														strcpy(phonecode,s);
														printf("~p\nEnter your phone number.\n: ");
														if(GetLine(s,8,GLFLG_Digits|GLFLG_NoSpaces))
															strcpy(phonenumber,s);
														sprintf(phone,"%s-%s",phonecode,phonenumber);
													}
												break;
											case 8:
												printf("~p\nEnter the year in which you were born.\n: ");
												if(GetLine(s,4,GLFLG_Digits|GLFLG_NoSpaces))
													{
														birthyear=atoi(s);
														printf("~p\nEnter the month in which you were born.\n: ");
														if(GetLine(s,2,GLFLG_Digits|GLFLG_NoSpaces))
															{
																birthmonth=atoi(s);
																printf("~p\nEnter the day on which you were born.\n: ");
																if(GetLine(s,2,GLFLG_Digits|GLFLG_NoSpaces))
																	birthday=atoi(s);
															}
														sprintf(birthdate,"%d/%d/%d",birthmonth,birthday,birthyear);
													}
												break;
										}
								}
							else
								kg=FALSE;
						}
				}
				SetUserTags(USRTAG_Name,name,
					USRTAG_Password,password,
					USRTAG_Address,address,USRTAG_City,city,USRTAG_State,state,USRTAG_Zip,zip,USRTAG_Country,country,
					USRTAG_PhoneCode,phonecode,USRTAG_PhoneNumber,phonenumber,
					USRTAG_BirthDay,birthday,USRTAG_BirthMonth,birthmonth,USRTAG_BirthYear,birthyear,
					TAG_END);
			}
	}

void Logon(BYTE *id,BYTE *arg,char *result_buff,char **result)
	{
		BOOL onlyrealnames=FALSE;
		GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
		if(arg && strlen(arg) && GetUserTags(USRTAG_Name,arg,USRTAG_Exists,TRUE,TAG_END))
			{
				BYTE name[33];
				strcpy(name,"");
				{
					BYTE username[33],realname[31];
					strcpy(username,"");
					strcpy(realname,"");
					GetUserTags(USRTAG_Name,arg,USRTAG_UserName,username,USRTAG_RealName,realname,TAG_END);
					strcpy(name,(onlyrealnames) ? realname : username);
				}
				if(IsOnline(name))
					{
						printf("~s\nUser `%s' is already online.\n",name);
						{
							char s[256];
							sprintf(s,"Unsuccessful automatic logon attempt, user `%s' already online.",name);
							LogEntry(id,"Callers",s);
						}
					}
				else
					SetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
			}
		else
		{
		/* Remote logon */
		{
			BOOL kg=TRUE;
			BYTE buff[33];
			int attempt=0;
			while(!panic && kg)
				{
					attempt++;
					if(attempt>5)
						break;
					if(onlyrealnames)
						printf("~p\nEnter your real name, or `New' if you are a new user.\n: ");
					else
						printf("~p\nEnter your user name, or `New' if you are a new user.\n: ");
					if(GetLine(buff,32,GLFLG_Format))
						{
							if(!stricmp(buff,"New"))
								{
									strcpy(result_buff,"NEWUSER");
									*result=result_buff;
									kg=FALSE;
								}
							else
								{
									BYTE name[33],password[11];
									strcpy(name,buff);
									strcpy(password,"");
									if(GetUserTags(USRTAG_Name,name,USRTAG_Password,password,TAG_END))
										{
											{
												BYTE username[33],realname[31];
												strcpy(username,"");
												strcpy(realname,"");
												GetUserTags(USRTAG_Name,name,USRTAG_UserName,username,USRTAG_RealName,realname,TAG_END);
												strcpy(name,(onlyrealnames) ? realname : username);
											}
											printf("~p\nEnter your password.\n: ");
											if(GetLine(buff,10,GLFLG_NoEcho))
												{
													if(!strcmp(buff,password))
														{
															if(IsOnline(name))
																{
																	printf("~s\nUser `%s' is already online.\n",name);
																	{
																		char s[256];
																		sprintf(s,"Unsuccessful logon attempt %d, user `%s' already online.",attempt,name);
																		LogEntry(id,"Callers",s);
																	}
																}
															else
																{
																	SetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
																	kg=FALSE;
																}
														}
													else
														{
															printf("~s\nIncorrect password.\n");
															{
																char s[256];
																sprintf(s,"Unsuccessful logon attempt %d, user `%s' password `%s'.",attempt,name,buff);
																LogEntry(id,"Callers",s);
															}
														}
												}

										}
									else
										printf("~s\nUnknown user `%s'.\n",buff);
								}
						}
				}
		}
		}
	}

void ReadNews(BYTE *id,BYTE *arg)
	{
		BYTE name[33];
		strcpy(name,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
		if(strlen(name))
			{
				UBYTE accesslevel=0;
				GetUserTags(USRTAG_Name,name,USRTAG_AccessLevel,&accesslevel,TAG_END);
				if(!stricmp(arg,"NEW"))
					{
						struct DateStamp laston={0};
						GetUserTags(USRTAG_Name,name,USRTAG_LastOn,&laston,TAG_END);
						{
							ULONG next=0;
							BYTE *name=NULL;
							BYTE accessrange[21],file[256];
							strcpy(accessrange,"");
							strcpy(file,"");
							while(next=GetConfigTags(CFGTAG_Path,"News",CFGTAG_Name,&name,CFGTAG_Next,next,NWSTAG_AccessRange,accessrange,NWSTAG_File,file,TAG_END))
								{
									if(IsRange(accessrange,accesslevel))
										{
											struct DateStamp ds={0};
											{
												BPTR fh;
												if(fh=Open(file,MODE_OLDFILE))
													{
														__aligned struct FileInfoBlock fib={0};
														if(ExamineFH(fh,&fib))
															ds=fib.fib_Date;
														Close(fh);
													}
											}
											if(CompareDates(&laston,&ds)>=0)
												{
													BYTE date[32],time[32];
													strcpy(date,"");
													strcpy(time,"");
													{
														struct DateTime dt;
														dt.dat_Stamp=ds;
														dt.dat_Format=FORMAT_USA;
														dt.dat_Flags=DTF_SUBST;
														dt.dat_StrDay=NULL;
														dt.dat_StrDate=date;
														dt.dat_StrTime=time;
														DateToStr(&dt);
													}
													printf("~h\nNews File: %s (%s %s)\n",name,date,time);
													ShowText(file);
												}
										}
								}
						}
					}
				if(!stricmp(arg,"OLD"))
					{
						BOOL kg=TRUE;
						BYTE news[33];
						strcpy(news,"");
						while(!panic && kg)
							{
								if(SelectNews(id,accesslevel,news))
									{
										BYTE file[256];
										strcpy(file,"");
										GetConfigTags(CFGTAG_Path,"News",CFGTAG_Name,news,NWSTAG_File,file,TAG_END);
										if(strlen(file))
											ShowText(file);
									}
								else
									kg=FALSE;
							}
					}
			}
	}

long Register(BYTE *id)
	{
		long ret=0;
		BOOL onlyrealnames=FALSE;
		GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
		{
			BOOL ok=TRUE;
			BYTE username[33],realname[31],uucpname[9];
			strcpy(username,"");
			strcpy(realname,"");
			strcpy(uucpname,"");
			if(ok)
				{
					if(!onlyrealnames)
						{
							while(1)
								{
									printf("~p\nEnter the user name that you wish to use.\n: ");
									if(ok=GetLine(username,32,GLFLG_Format|GLFLG_NoEmpty))
										{
											if(!stricmp(username,"New") || GetUserTags(USRTAG_Name,username,USRTAG_Exists,TRUE,TAG_END))
												printf("~s\n`%s' is already in use.\n",username);
											else
												break;
										}
									else
										break;
								}
						}
				}
			if(ok)
				{
					while(1)
						{
							printf("~p\nEnter your real name.\n: ");
							if(ok=GetLine(realname,30,GLFLG_Format|GLFLG_NoEmpty))
								{
									if(!stricmp(username,"New") || GetUserTags(USRTAG_Name,realname,USRTAG_Exists,TRUE,TAG_END))
										printf("~s\n`%s' conflicts with another user.\n",realname);
									else
										break;
								}
							else
								break;
						}
				}
			if(ok)
				{
					while(1)
						{
							printf("~p\nEnter the UUCP-style name that you wish to use.\nIf this system supports UUCP then this will also be your UUCP name.\n: ");
							if(ok=GetLine(uucpname,8,GLFLG_ToLower|GLFLG_NoSpaces|GLFLG_BeginChar|GLFLG_NoEmpty))
								{
									if(!stricmp(username,"New") || GetUserTags(USRTAG_Name,uucpname,USRTAG_Exists,TRUE,TAG_END))
										printf("~s\n`%s' is already in use.\n",uucpname);
									else
										break;
								}
							else
								break;
						}
				}
			if(ok)
				{
					BYTE *name=NULL;
					name=(onlyrealnames) ? realname : username;
					{
						BYTE password[11],address[31],city[21],state[21],zip[11],country[4],phonecode[7],phonenumber[9];
						UBYTE birthday=0,birthmonth=0;
						UWORD birthyear=0;
						strcpy(password,"");
						strcpy(address,"");
						strcpy(city,"");
						strcpy(state,"");
						strcpy(zip,"");
						strcpy(country,"");
						strcpy(phonecode,"");
						strcpy(phonenumber,"");
						if(ok)
							{
								printf("~p\nEnter the password that you would like to use.\n: ");
								ok=GetLine(password,10,GLFLG_NoEcho|GLFLG_NoEmpty);
							}
						if(ok)
							{
								printf("~p\nEnter your street address.\n: ");
								ok=GetLine(address,30,GLFLG_Format|GLFLG_NoEmpty);
							}
						if(ok)
							{
								printf("~p\nEnter the city that you live in.\n: ");
								ok=GetLine(city,20,GLFLG_Format|GLFLG_NoEmpty);
							}
						if(ok)
							{
								printf("~p\nEnter the state/province that you live in, if applicable.\n: ");
								ok=GetLine(state,20,GLFLG_Format);
							}
						if(ok)
							{
								printf("~p\nEnter your zip/postal code.\n: ");
								ok=GetLine(zip,10,GLFLG_ToUpper);
							}
						if(ok)
							{
								printf("~p\nEnter the country you live in.\n: ");
								ok=GetLine(country,3,GLFLG_ToUpper|GLFLG_Chars|GLFLG_NoEmpty);
							}
						if(ok)
							{
								printf("~p\nEnter your area/country code.\n: ");
								ok=GetLine(phonecode,6,GLFLG_Digits|GLFLG_NoSpaces|GLFLG_NoEmpty);
							}
						if(ok)
							{
								printf("~p\nEnter your phone number.\n: ");
								ok=GetLine(phonenumber,8,GLFLG_Digits|GLFLG_NoSpaces|GLFLG_NoEmpty);
							}
						if(ok)
							{
								BYTE buff[6];
								printf("~p\nEnter the year in which you were born.\n: ");
								if(ok=GetLine(buff,5,GLFLG_Digits|GLFLG_NoSpaces|GLFLG_NoEmpty))
									birthyear=atoi(buff);
							}
						if(ok)
							{
								BYTE buff[4];
								printf("~p\nEnter the month in which you were born.\n: ");
								if(ok=GetLine(buff,2,GLFLG_Digits|GLFLG_NoSpaces|GLFLG_NoEmpty))
									birthmonth=atoi(buff);
							}
						if(ok)
							{
								BYTE buff[4];
								printf("~p\nEnter the day on which you were born.\n: ");
								if(ok=GetLine(buff,2,GLFLG_Digits|GLFLG_NoSpaces|GLFLG_NoEmpty))
									birthday=atoi(buff);
							}
						if(ok)
							{
								if(AddUserTags(USRTAG_UserName,username,USRTAG_RealName,realname,USRTAG_UUCPName,uucpname,TAG_END))
									{
										SetUserTags(USRTAG_Name,name,
											USRTAG_Password,password,
											USRTAG_Address,address,
											USRTAG_City,city,
											USRTAG_State,state,
											USRTAG_Zip,zip,
											USRTAG_Country,country,
											USRTAG_PhoneCode,phonecode,
											USRTAG_PhoneNumber,phonenumber,
											USRTAG_BirthDay,birthday,
											USRTAG_BirthMonth,birthmonth,
											USRTAG_BirthYear,birthyear,
											USRTAG_AccessLevel,1,
											USRTAG_SessionTimeLimit,15,
											USRTAG_InactivityTimeLimit,3,
											USRTAG_RatioByte,1,
											USRTAG_RatioFile,1,
											USRTAG_Days,30,
											USRTAG_ScreenWidth,80,
											USRTAG_ScreenHeight,24,
											TAG_END);
										SetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
										ret=1;
									}
							}
					}
				}
		}
		return(ret);
	}

void RunMenu(BYTE *id,BYTE *arg)
	{
		BYTE name[33];
		BOOL ansi=FALSE;
		strcpy(name,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,name,STTAG_ANSI,&ansi,TAG_END);
		if(strlen(name))
			{
		BOOL child=FALSE;
		if(GetConfigTags(CFGTAG_Path,"Menus",CFGTAG_Name,arg,CFGTAG_Exists,TRUE,CFGTAG_Child,&child,TAG_END))
			{
		if(child)
			{
				BYTE menu_path[256];
				sprintf(menu_path,"Menus/%s",arg);
				{
					BYTE logonstack[21],stack[256];
					strcpy(logonstack,"");
					strcpy(stack,"");
					GetUserTags(USRTAG_Name,name,USRTAG_LogonStack,logonstack,TAG_END);
					GetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
					if(strlen(stack)+strlen(logonstack)<=255)
						{
							strcat(stack,logonstack);
							SetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
						}
				}
				{
					UBYTE accesslevel=0;
					BOOL menus=FALSE,hotkeys=FALSE;
					BYTE cmdlist[256],stack[256];
					BOOL showmenu=FALSE,kg=TRUE;
					BYTE character,def,number,prompt[64];
					while(!panic && kg)
						{
							strcpy(stack,"");
							GetUserTags(USRTAG_Name,name,USRTAG_AccessLevel,&accesslevel,USRTAG_Menus,&menus,USRTAG_HotKeys,&hotkeys,TAG_END);
							GetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
							SetStatusTags(STTAG_Name,id,STTAG_Location,FilePart(menu_path),TAG_END);
							{
								ULONG next=0;
								BYTE accessrange[21];
								BYTE character=NULL;
								strcpy(cmdlist,"");
								strcpy(accessrange,"");
								while(next=GetConfigTags(CFGTAG_Path,menu_path,CFGTAG_Next,next,MNTAG_AccessRange,accessrange,MNTAG_Character,&character,TAG_END))
									{
										if(IsRange(accessrange,accesslevel))
											strncat(cmdlist,&character,1);
									}
							}
							{
								BYTE path[256];
								strcpy(path,menu_path);
								{
									char *p;
									if(p=PathPart(path))
										*p=NULL;
								}
								character=0;
								def=0;
								number=0;
								strcpy(prompt,"");
								GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,FilePart(menu_path),MNTAG_Character,&character,MNTAG_Default,&def,MNTAG_Number,&number,MNTAG_Prompt,prompt,TAG_END);
							}
							if(!strlen(stack))
								{
									if(menus || showmenu)
										{
											FILE *fp;
											BYTE menu_file[256];
											sprintf(menu_file,"WWBBS:Menus/%s.%s",FilePart(menu_path),(ansi) ? "ansi" : "ascii");
											if(fp=fopen(menu_file,"r"))
												{
													BYTE buff[256];
													printf("~o\n");
													while(fgets(buff,255,fp))
														{
															if(WaitForChar(Input(),0))
																{
																	getchar();
																	break;
																}
															if(buff[0]=='@')
																{
																	char *p;
																	if(p=strchr(&buff[1],'@'))
																		{
																			*p=NULL;
																			p++;
																			if(IsRange(&buff[1],accesslevel))
																				puts(p);
																		}
																}
															else
																puts(buff);
														}
													fclose(fp);
												}
											else
												{
													BYTE s[256];
													strcpy(s,"");
													printf("~o\n");
													{
														ULONG next=0;
														BYTE *name=NULL;
														BYTE accessrange[21],character;
														while(next=GetConfigTags(CFGTAG_Path,menu_path,CFGTAG_Name,&name,CFGTAG_Next,next,
																MNTAG_AccessRange,accessrange,MNTAG_Character,&character,TAG_END))
															{
																if(IsRange(accessrange,accesslevel))
																	{
																		if(strlen(s))
																			{
																				sprintf(&s[strlen(s)]," [%c] %s",character,name);
																				printf("%s\n",s);
																				strcpy(s,"");
																			}
																		else
																			sprintf(s,"[%c] %-32s",character,name);
																	}
															}
													}
													if(strlen(s))
														printf("%s\n",s);
												}
											if(showmenu)
												showmenu=FALSE;
										}
									PrintPrompt(id,prompt);
									if(hotkeys)
										{
											int c;
											printf("~i");
											while(1)
												{
													c=getchar();
													if(c==EOF)
														{
															kg=FALSE;
															break;
														}
													if(isalpha(c))
														c=toupper(c);
													if(isdigit(c) && number)
														{
															sprintf(stack,"%c%c",number,c);
															printf("%c\n",number);
															break;
														}
													if(c=='\r' && def)
														{
															sprintf(stack,"%c",def);
															printf("%c\n",def);
															break;
														}
													if(strchr(cmdlist,c))
														{
															sprintf(stack,"%c",c);
															printf("%c\n",c);
															break;
														}
												}
											SetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
										}
									else
										{
											if(GetLine(stack,255,GLFLG_ToUpper))
												SetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
											else
												{
													if(def)
														{
															sprintf(stack,"%c",def);
															SetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
														}
												}
										}
								}
							if(strlen(stack))
								{
									if(number && isdigit(stack[0]))
										{
											BYTE s[256];
											sprintf(s,"%c%.254s",number,stack);
											strcpy(stack,s);
											SetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
										}
									{
										BYTE c;
										while(kg && strlen(stack))
											{
												c=stack[0];
												{
													BYTE s[256];
													strcpy(s,&stack[1]);
													strcpy(stack,s);
													SetStatusTags(STTAG_Name,id,STTAG_Stack,stack,TAG_END);
												}
												if(strchr(cmdlist,c))
													{
														BYTE menu_name[33];
														ULONG next=0;
														BYTE *name=NULL;
														BOOL child=FALSE;
														BYTE character=0;
														BYTE command[64];
														strcpy(menu_name,"");
														while(next=GetConfigTags(CFGTAG_Path,menu_path,CFGTAG_Name,&name,CFGTAG_Child,&child,CFGTAG_Next,next,MNTAG_Character,&character,TAG_END))
															{
																if(character==c)
																	strcpy(menu_name,name);
															}
														if(strlen(menu_name))
															{
																GetConfigTags(CFGTAG_Path,menu_path,CFGTAG_Name,menu_name,CFGTAG_Child,&child,MNTAG_Character,&character,MNTAG_Command,command,TAG_END);
																if(child)
																	{
																		GetConfigTags(CFGTAG_Path,menu_path,CFGTAG_Name,menu_name,MNTAG_Character,&character,MNTAG_Default,&def,MNTAG_Number,&number,MNTAG_Prompt,prompt,TAG_END);
																		AddPart(menu_path,menu_name,255);
																	}
																else
																	{
																		char *error,*result,result_buff[256];
																		long errorlevel;
																		UWORD group=0,cmd=0;
																		BYTE buff[32],*arg=NULL;
																		strcpy(buff,"");
																		sscanf(command,"%31s",buff);
																		if(arg=strchr(command,' '))
																			arg++;
																		if(FindCommand(cmds,buff,&group,&cmd))
																			{
																				SetStatusTags(STTAG_Name,id,STTAG_Location,menu_name,TAG_END);
																				switch(group)
																					{
																						case GROUP_ABBEREXX:
																							ABBEREXXCommand(id,cmd,arg,result_buff,&result,&error,&errorlevel);
																							break;
																						case GROUP_System:
																							SystemCommand(id,cmd,arg,result_buff,&result,&error,&errorlevel);
																							break;
																						case GROUP_Email:
																							EmailCommand(id,cmd,arg);
																							break;
																						case GROUP_MessageBase:
																							MessageBaseCommand(id,cmd,arg);
																							break;
																						case GROUP_FileBase:
																							FileBaseCommand(id,cmd,arg);
																							break;
																					}
																				SetStatusTags(STTAG_Name,id,STTAG_Location,FilePart(menu_path),TAG_END);
																			}
																		else
																			{
																				if(!stricmp(command,"LOGOFF"))
																					{
																						if(Ask("Log off",FALSE))
																							kg=FALSE;
																					}
																				if(!stricmp(command,"SHOWMENU"))
																					showmenu=TRUE;
																				if(!stricmp(command,"QUIT"))
																					{
																						{
																							char *p;
																							if(p=PathPart(menu_path))
																								*p=NULL;
																						}
																						if(!stricmp(menu_path,"Menus"))
																							kg=FALSE;
																					}
																			}
																	}
															}
													}
											}
									}
								}
						}
				}
			}
		else
			printf("~s\nMenu `%s' has no child items.\n",arg);
			}
		else
			printf("~s\nMenu `%s' does not exist.\n",arg);
			}
	}

void ShowStats(BYTE *id)
	{
		BYTE name[33];
		strcpy(name,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
		if(strlen(name))
			{
				UWORD timeremaining=0,timeused=0;
				UBYTE accesslevel=0,ratiobyte=0,ratiofile=0;
				ULONG sessions=0;
				ULONG emailread=0,emailwritten=0;
				ULONG messagesread=0,messageswritten=0;
				ULONG filesuploaded=0,filesdownloaded=0,filescredits=0;
				ULONG kilosuploaded=0,kilosdownloaded=0,kiloscredits=0;
				struct DateStamp laston={0};
				GetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,&timeremaining,STTAG_TimeUsed,&timeused,TAG_END);
				GetUserTags(USRTAG_Name,name,
					USRTAG_AccessLevel,&accesslevel,
					USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,
					USRTAG_Sessions,&sessions,
					USRTAG_EmailRead,&emailread,USRTAG_EmailWritten,&emailwritten,
					USRTAG_MessagesRead,&messagesread,USRTAG_MessagesWritten,&messageswritten,
					USRTAG_FilesUploaded,&filesuploaded,USRTAG_FilesDownloaded,&filesdownloaded,USRTAG_FilesCredits,&filescredits,
					USRTAG_KilosUploaded,&kilosuploaded,USRTAG_KilosDownloaded,&kilosdownloaded,USRTAG_KilosCredits,&kiloscredits,
					USRTAG_LastOn,&laston,
					TAG_END);
				{
					char *template[]={
						"User:             ",
						"Time:             ",
						"Access:           ",
						"Ratio:            ",
						"Sessions:         ",
						"Last On:          ",
						"Email Read:       ",
						"Email Written:    ",
						"Messages Read:    ",
						"Messages Written: ",
						"Files Uploaded:   ",
						"Kilos Uploaded:   ",
						"Files Downloaded: ",
						"Kilos Downloaded: ",
						"Files Credits:    ",
						"Kilos Credits:    ",
						NULL
					};
					char s[256],buff[256],*p;
					int index=0;
					strcpy(s,"");
					printf("~o\n");
					while(template[index])
						{
							if(WaitForChar(Input(),0))
								{
									getchar();
									break;
								}
							switch(index)
								{
									case 0:
										p=name;
										break;
									case 1:
										sprintf(buff,"%dm used, %dm remaining",timeused,timeremaining);
										p=buff;
										break;
									case 2:
										sprintf(buff,"%d",accesslevel);
										p=buff;
										break;
									case 3:
										if(ratiobyte)
											sprintf(buff,"%d:1 byte",ratiobyte);
										else
											strcpy(buff,"No byte ratio");
										strcat(buff,", ");
										if(ratiofile)
											sprintf(&buff[strlen(buff)],"%d:1 file",ratiofile);
										else
											strcpy(&buff[strlen(buff)],"No file ratio");
										p=buff;
										break;
									case 4:
										sprintf(buff,"%ld",sessions);
										p=buff;
										break;
									case 5:
										if(!laston.ds_Days && !laston.ds_Minute && !laston.ds_Tick)
											p="Never";
										else
											{
												struct DateTime dt={0};
												BYTE date[16],time[16];
												dt.dat_Stamp=laston;
												dt.dat_Format=FORMAT_USA;
												dt.dat_Flags=DTF_SUBST;
												dt.dat_StrDay=NULL;
												dt.dat_StrDate=date;
												dt.dat_StrTime=time;
												DateToStr(&dt);
												sprintf(buff,"%s %s",date,time);
												p=buff;
											}
										break;
									case 6:
										sprintf(buff,"%ld",emailread);
										p=buff;
										break;
									case 7:
										sprintf(buff,"%ld",emailwritten);
										p=buff;
										break;
									case 8:
										sprintf(buff,"%ld",messagesread);
										p=buff;
										break;
									case 9:
										sprintf(buff,"%ld",messageswritten);
										p=buff;
										break;
									case 10:
										sprintf(buff,"%ld",filesuploaded);
										p=buff;
										break;
									case 11:
										sprintf(buff,"%ld",kilosuploaded);
										p=buff;
										break;
									case 12:
										sprintf(buff,"%ld",filesdownloaded);
										p=buff;
										break;
									case 13:
										sprintf(buff,"%ld",kilosdownloaded);
										p=buff;
										break;
									case 14:
										sprintf(buff,"%ld",filescredits);
										p=buff;
										break;
									case 15:
										sprintf(buff,"%ld",kiloscredits);
										p=buff;
										break;
									default:
										p=NULL;
										break;
								}
							if(p)
								{
									if(strlen(s))
										{
											if(strlen(template[index])+strlen(p)>39)
												sprintf(&s[strlen(s)]," %s%.*s...",template[index],36-strlen(template[index]),p);
											else
												sprintf(&s[strlen(s)]," %s%s",template[index],p);
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										{
											if(strlen(template[index])+strlen(p)>39)
												sprintf(&s[strlen(s)],"%s%.*s...",template[index],36-strlen(template[index]),p);
											else
												sprintf(&s[strlen(s)],"%s%-*s",template[index],39-strlen(template[index]),p);
										}
								}
							index++;
						}
					if(strlen(s))
						printf("%s\n",s);
				}
			}
	}

void UserEditor(BYTE *id)
	{
		BYTE user[33];
		BOOL kg=TRUE;
		while(!panic && kg)
			{
				printf("~p\nEnter name of user to edit.\n: ");
				if(GetLine(user,32,GLFLG_Format))
					edit_user(id,user);
				else
					kg=FALSE;
			}
	}

void UserList(BYTE *id)
	{
		BYTE buff[65],pattern[129];
		printf("~p\nEnter an AmigaDOS search pattern for the users' names.\n: ");
		if(GetLine(buff,64,NULL))
			{
				if(ParsePatternNoCase(buff,pattern,128)!=-1)
					{
						printf("~h\nUser                             From                                          \n~o");
						{
							BOOL onlyrealnames=FALSE;
							GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
							{
								ULONG next=0;
								BYTE *username=NULL,*realname=NULL;
								BYTE city[21],state[21],country[4];
								strcpy(city,"");
								strcpy(state,"");
								strcpy(country,"");
								while(next=GetUserTags(USRTAG_Next,next,USRTAG_UserName,&username,USRTAG_RealName,&realname,USRTAG_City,city,USRTAG_State,state,USRTAG_Country,country,TAG_END))
									{
										if( (onlyrealnames && MatchPatternNoCase(pattern,realname)) || (!onlyrealnames && MatchPatternNoCase(pattern,username)) )
											printf("%-32s %s%s%s %s\n",(onlyrealnames) ? realname : username,city,(strlen(state)) ? ", " : ",",state,country);
									}
							}
						}
					}
			}
	}

void ValidateNewUsers(BYTE *id)
	{
		BOOL kg=TRUE;
		BYTE user[33];
		strcpy(user,"");
		while(!panic && kg)
			{
				if(SelectNewUser(id,user))
					edit_user(id,user);
				else
					kg=FALSE;
			}
	}

void WhoIsOnline(BYTE *id)
	{
		printf("~h\nNode  User                             Location                        \n~o");
		{
			ULONG next=0;
			BYTE *name=NULL,username[33],location[33];
			strcpy(username,"");
			strcpy(location,"");
			while(next=GetStatusTags(STTAG_Name,&name,STTAG_UserName,username,STTAG_Location,location,STTAG_Next,next,TAG_END))
				{
					if(strlen(name))
						printf("%4.4s  %-32s %-32s\n",name,username,location);
				}
		}
	}

/***/

void SetDoorVariables(BYTE *id)
	{
		BYTE username[33],realname[31],uucpname[9];
		UBYTE accesslevel=0,ratiobyte=0,ratiofile=0,screenwidth=0,screenheight=0;
		BYTE editor[33],editorcmd[256],protocol[33],protocolsendcmd[256],protocolreceivecmd[256],archiver[33],archiveraddcmd[256],archiverextractcmd[256],archiverviewcmd[256];
		strcpy(username,"");
		strcpy(realname,"");
		strcpy(uucpname,"");
		strcpy(editor,"");
		strcpy(editorcmd,"");
		strcpy(protocol,"");
		strcpy(protocolsendcmd,"");
		strcpy(protocolreceivecmd,"");
		strcpy(archiver,"");
		strcpy(archiveraddcmd,"");
		strcpy(archiverextractcmd,"");
		strcpy(archiverviewcmd,"");
		if(id)
			{
				GetStatusTags(STTAG_Name,id,STTAG_UserName,username,TAG_END);
				if(strlen(username))
					{
						GetUserTags(USRTAG_Name,username,USRTAG_RealName,realname,USRTAG_UUCPName,uucpname,USRTAG_AccessLevel,&accesslevel,USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,USRTAG_ScreenWidth,&screenwidth,USRTAG_ScreenHeight,&screenheight,USRTAG_Editor,editor,USRTAG_Protocol,protocol,USRTAG_Archiver,archiver,TAG_END);
						if(strlen(editor)) GetConfigTags(CFGTAG_Path,"Editors",CFGTAG_Name,editor,EDTAG_Command,editorcmd,TAG_END);
						if(strlen(protocol)) GetConfigTags(CFGTAG_Path,"Protocols",CFGTAG_Name,protocol,PRTAG_SendCommand,protocolsendcmd,PRTAG_ReceiveCommand,protocolreceivecmd,TAG_END);
						if(strlen(archiver)) GetConfigTags(CFGTAG_Path,"Archivers",CFGTAG_Name,archiver,ARCTAG_AddCommand,archiveraddcmd,ARCTAG_ExtractCommand,archiverextractcmd,ARCTAG_ViewCommand,archiverviewcmd,TAG_END);
					}
			}
		{
			char buff[256];
			SetVar("USERNAME",username,-1,GVF_LOCAL_ONLY);
			SetVar("REALNAME",realname,-1,GVF_LOCAL_ONLY);
			SetVar("UUCPNAME",uucpname,-1,GVF_LOCAL_ONLY);
			sprintf(buff,"WWBBS:Users/%s",uucpname);
			SetVar("HOME",buff,-1,GVF_LOCAL_ONLY);
			sprintf(buff,"%d",accesslevel);
			SetVar("ACCESSLEVEL",buff,-1,GVF_LOCAL_ONLY);
			sprintf(buff,"%d",ratiobyte);
			SetVar("BYTERATIO",buff,-1,GVF_LOCAL_ONLY);
			sprintf(buff,"%d",ratiofile);
			SetVar("FILERATIO",buff,-1,GVF_LOCAL_ONLY);
			sprintf(buff,"%d",screenwidth,-1,GVF_LOCAL_ONLY);
			SetVar("COLUMNS",buff,-1,GVF_LOCAL_ONLY);
			sprintf(buff,"%d",screenheight,-1,GVF_LOCAL_ONLY);
			SetVar("ROWS",buff,-1,GVF_LOCAL_ONLY);
			SetVar("EDITOR",editorcmd,-1,GVF_LOCAL_ONLY);
			SetVar("VISUAL",editorcmd,-1,GVF_LOCAL_ONLY);
			SetVar("PROTOCOLSEND",protocolsendcmd,-1,GVF_LOCAL_ONLY);
			SetVar("PROTOCOLRECEIVE",protocolreceivecmd,-1,GVF_LOCAL_ONLY);
			SetVar("ARCHIVERADD",archiveraddcmd,-1,GVF_LOCAL_ONLY);
			SetVar("ARCHIVEREXTRACT",archiverextractcmd,-1,GVF_LOCAL_ONLY);
			SetVar("ARCHIVERVIEW",archiverviewcmd,-1,GVF_LOCAL_ONLY);
		}
	}

int SelectConfig(BYTE *id,BYTE *path,BYTE *prompt,BYTE *error,BYTE *result)
	{
		int selected=0,count=0;
		{
			ULONG next=0;
			while(next=GetConfigTags(CFGTAG_Path,path,CFGTAG_Next,next,TAG_END))
				count++;
		}
		if(count)
			{
				{
					char s[256];
					int index=0;
					ULONG next=0;
					BYTE *name=NULL;
					strcpy(s,"");
					printf("~o\n");
					while(next=GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,&name,CFGTAG_Next,next,TAG_END))
						{
							index++;
							if(strlen(s))
								{
									sprintf(&s[strlen(s)]," [%*d] %-32s",(int) (log10((double) count)+1),index,name);
									printf("%s\n",s);
									strcpy(s,"");
								}
							else
								sprintf(s,"[%*d] %-32s",(int) (log10((double) count)+1),index,name);
						}
					if(strlen(s))
						printf("%s\n",s);
				}
				{
					BYTE s[256];
					printf("~p\nSelect %s: ",prompt);
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *name=NULL;
									while(next=GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,&name,CFGTAG_Next,next,TAG_END))
										{
											i++;
											if(atoi(s)==i)
												{
													selected=i;
													if(result)
														strcpy(result,name);
												}
										}
								}
						}
				}
			}
		else
			printf("~s\nNo %s exist.\n",error);
		return(selected);
	}

int SelectEditor(BYTE *id,BYTE *result)
	{
		int selected=0,count=0;
		BOOL ansi=FALSE,local=FALSE;
		GetStatusTags(STTAG_Name,id,STTAG_ANSI,&ansi,STTAG_Local,&local,TAG_END);
		{
			ULONG next=0;
			UBYTE type=0;
			while(next=GetConfigTags(CFGTAG_Path,"Editors",CFGTAG_Next,next,EDTAG_Type,&type,TAG_END))
				{
					switch(type)
						{
							case EDTYP_Standard:
								count++;
								break;
							case EDTYP_Visual:
								if(ansi)
									count++;
								break;
							case EDTYP_Graphical:
								if(local)
									count++;
								break;
						}
				}
		}
		if(count)
			{
				{
					char s[256];
					int index=0;
					ULONG next=0;
					BYTE *name=NULL;
					UBYTE type=NULL;
					strcpy(s,"");
					printf("~o\n");
					while(next=GetConfigTags(CFGTAG_Path,"Editors",CFGTAG_Name,&name,CFGTAG_Next,next,EDTAG_Type,&type,TAG_END))
						{
							index++;
							if( type==EDTYP_Standard || (type==EDTYP_Visual && ansi) || (type==EDTYP_Graphical && local) )
								{
									if(strlen(s))
										{
											sprintf(&s[strlen(s)]," [%*d] %-32s",(int) (log10((double) count)+1),index,name);
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										sprintf(s,"[%*d] %-32s",(int) (log10((double) count)+1),index,name);
								}
						}
					if(strlen(s))
						printf("%s\n",s);
				}
				{
					BYTE s[256];
					printf("~p\nSelect Editor: ");
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *name=NULL;
									UBYTE type=NULL;
									while(next=GetConfigTags(CFGTAG_Path,"Editors",CFGTAG_Name,&name,CFGTAG_Next,next,EDTAG_Type,&type,TAG_END))
										{
											if( type==EDTYP_Standard || (type==EDTYP_Visual && ansi) || (type==EDTYP_Graphical && local) )
												{
													i++;
													if(atoi(s)==i)
														{
															selected=i;
															if(result)
																strcpy(result,name);
														}
												}
										}
								}
						}
				}
			}
		else
			printf("~s\nNo editors exist.\n");
		return(selected);
	}

int SelectNews(BYTE *id,UBYTE accesslevel,BYTE *result)
	{
		int selected=0,count=0;
		{
			ULONG next=0;
			BYTE accessrange[21];
			strcpy(accessrange,"");
			while(next=GetConfigTags(CFGTAG_Path,"News",CFGTAG_Next,next,NWSTAG_AccessRange,accessrange,TAG_END))
				{
					if(IsRange(accessrange,accesslevel))
						count++;
				}
		}
		if(count)
			{
				{
					char s[256];
					int index=0;
					ULONG next=0;
					BYTE *name=NULL;
					BYTE accessrange[21];
					strcpy(s,"");
					strcpy(accessrange,"");
					printf("~o\n");
					while(next=GetConfigTags(CFGTAG_Path,"News",CFGTAG_Name,&name,CFGTAG_Next,next,NWSTAG_AccessRange,accessrange,TAG_END))
						{
							if(IsRange(accessrange,accesslevel))
								{
									index++;
									if(strlen(s))
										{
											sprintf(&s[strlen(s)]," [%*d] %-32s",(int) (log10((double) count)+1),index,name);
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										sprintf(s,"[%*d] %-32s",(int) (log10((double) count)+1),index,name);
								}
						}
					if(strlen(s))
						printf("%s\n",s);
				}
				{
					BYTE s[256];
					printf("~p\nSelect News: ");
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *name=NULL;
									BYTE accessrange[21];
									strcpy(accessrange,"");
									while(next=GetConfigTags(CFGTAG_Path,"News",CFGTAG_Name,&name,CFGTAG_Next,next,NWSTAG_AccessRange,accessrange,TAG_END))
										{
											if(IsRange(accessrange,accesslevel))
												{
													i++;
													if(atoi(s)==i)
														{
															selected=i;
															if(result)
																strcpy(result,name);
														}
												}
										}
								}
						}
				}
			}
		else
			printf("~s\nNo news exist.\n");
		return(selected);
	}

int SelectNewUser(BYTE *id,BYTE *result)
	{
		int selected=0,count=0;
		{
			ULONG next=0;
			UBYTE accesslevel=0;
			while(next=GetUserTags(USRTAG_Next,next,USRTAG_AccessLevel,&accesslevel,TAG_END))
				{
					if(accesslevel==1)
						count++;
				}
		}
		if(count)
			{
				BOOL onlyrealnames=FALSE;
				GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
				{
					char s[256];
					int index=0;
					ULONG next=0;
					BYTE *username=NULL,*realname=NULL;
					UBYTE accesslevel=0;
					strcpy(s,"");
					printf("~o\n");
					while(next=GetUserTags(USRTAG_UserName,&username,USRTAG_RealName,&realname,USRTAG_Next,next,USRTAG_AccessLevel,&accesslevel,TAG_END))
						{
							if(accesslevel==1)
								{
									index++;
									if(strlen(s))
										{
											sprintf(&s[strlen(s)]," [%*d] %-32s",(int) (log10((double) count)+1),index,(onlyrealnames) ? realname : username);
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										sprintf(s,"[%*d] %-32s",(int) (log10((double) count)+1),index,(onlyrealnames) ? realname : username);
								}
						}
					if(strlen(s))
						printf("%s\n",s);
				}
				{
					BYTE s[256];
					printf("~p\nSelect New User: ");
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *username=NULL,*realname=NULL;
									UBYTE accesslevel=0;
									while(next=GetUserTags(USRTAG_UserName,&username,USRTAG_RealName,&realname,USRTAG_Next,next,USRTAG_AccessLevel,&accesslevel,TAG_END))
										{
											if(accesslevel==1)
												{
													i++;
													if(atoi(s)==i)
														{
															selected=i;
															if(result)
																strcpy(result,(onlyrealnames) ? realname : username);
														}
												}
										}
								}
						}
				}
			}
		else
			printf("~s\nNo new users exist.\n");
		return(selected);
	}

int SelectOnlineUser(BYTE *id,BYTE *result)
	{
		int selected=0,count=0;
		{
			ULONG next=0;
			BYTE *name=NULL,username[33];
			strcpy(name,"");
			strcpy(username,"");
			while(next=GetStatusTags(STTAG_Name,&name,STTAG_Next,next,STTAG_UserName,username,TAG_END))
				{
					if(stricmp(name,id) && strlen(username))
						count++;
				}
		}
		if(count)
			{
				{
					char s[256];
					int index=0;
					ULONG next=0;
					BYTE *name=NULL,username[33];
					strcpy(s,"");
					strcpy(username,"");
					printf("~o\n");
					while(next=GetStatusTags(STTAG_Name,&name,STTAG_Next,next,STTAG_UserName,username,TAG_END))
						{
							if(stricmp(name,id) && strlen(username))
								{
									index++;
									if(strlen(s))
										{
											sprintf(&s[strlen(s)]," [%*d] %-32s",(int) (log10((double) count)+1),index,username);
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										sprintf(s,"[%*d] %-32s",(int) (log10((double) count)+1),index,username);
								}
						}
					if(strlen(s))
						printf("%s\n",s);
				}
				{
					BYTE s[256];
					printf("~p\nSelect Online User: ");
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *name=NULL,username[33];
									strcpy(username,"");
									while(next=GetStatusTags(STTAG_Name,&name,STTAG_Next,next,STTAG_UserName,username,TAG_END))
										{
											if(stricmp(name,id) && strlen(username))
												{
													i++;
													if(atoi(s)==i)
														{
															selected=i;
															if(result)
																strcpy(result,name);
														}
												}
										}
								}
						}
				}
			}
		else
			printf("~s\nThere are no other online users.\n");
		return(selected);
	}
					
void edit_user(BYTE *id,BYTE *user)
	{
		if(GetUserTags(USRTAG_Name,user,USRTAG_Exists,TRUE,TAG_END))
			{
				BOOL deleteuser=FALSE;
				BOOL onlyrealnames=FALSE;
				BYTE username[33],realname[31],uucpname[9];
				BYTE password[11],address[31],city[21],state[21],zip[11],country[4],phonecode[7],phonenumber[9];
				UBYTE birthday=0,birthmonth=0;
				UWORD birthyear=0;
				UBYTE accesslevel=0;
				UWORD sessiontimelimit=0,inactivitytimelimit=0;
				UBYTE ratiobyte=0,ratiofile=0;
				ULONG days=0;
				strcpy(username,"");
				strcpy(realname,"");
				strcpy(uucpname,"");
				strcpy(password,"");
				strcpy(address,"");
				strcpy(city,"");
				strcpy(state,"");
				strcpy(zip,"");
				strcpy(country,"");
				strcpy(phonecode,"");
				strcpy(phonenumber,"");
				GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
				GetUserTags(USRTAG_Name,user,
					USRTAG_UserName,username,USRTAG_RealName,realname,USRTAG_UUCPName,uucpname,
					USRTAG_Password,password,USRTAG_Address,address,USRTAG_City,city,USRTAG_State,state,USRTAG_Zip,zip,USRTAG_Country,country,USRTAG_PhoneCode,phonecode,USRTAG_PhoneNumber,phonenumber,USRTAG_BirthDay,&birthday,USRTAG_BirthMonth,&birthmonth,USRTAG_BirthYear,&birthyear,
					USRTAG_AccessLevel,&accesslevel,USRTAG_SessionTimeLimit,&sessiontimelimit,USRTAG_InactivityTimeLimit,&inactivitytimelimit,USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,USRTAG_Days,&days,
					TAG_END);
				{
					char *template[]={
						"[--] User Name:       ",
						"[--] Real Name:       ",
						"[--] UUCP Name:       ",
						"[ 1] Password:        ",
						"[ 2] Address:         ",
						"[ 3] City:            ",
						"[ 4] State:           ",
						"[ 5] Zip:             ",
						"[ 6] Country:         ",
						"[ 7] Phone:           ",
						"[ 8] Birth Date:      ",
						"[ 9] Access Level:    ",
						"[10] Session Time:    ",
						"[11] Inactivity Time: ",
						"[12] Byte Ratio:      ",
						"[13] File Ratio:      ",
						"[14] Days:            ",
						"[15] Select Access Group",
						"[16] Delete User",
						NULL };
					char s[256],buff[256],*p;
					int index;
					BOOL kg=TRUE;
					while(!panic && kg)
						{
							printf("~o\n");
							index=0;
							strcpy(s,"");
							while(template[index])
								{
									p=NULL;
									if(WaitForChar(Input(),0))
										{
											getchar();
											break;
										}
									switch(index)
										{
											case 0:p=(onlyrealnames) ? "Not applicable" : username;break;
											case 1:p=realname;break;
											case 2:p=uucpname;break;
											case 3:p=password;break;
											case 4:p=address;break;
											case 5:p=city;break;
											case 6:p=state;break;
											case 7:p=zip;break;
											case 8:p=country;break;
											case 9:
												if(strlen(phonecode) && strlen(phonenumber))
													{
														sprintf(buff,"%s-%s",phonecode,phonenumber);
														p=buff;
													}
												break;
											case 10:
												if(birthday && birthmonth && birthyear)
													{
														sprintf(buff,"%d/%d/%d",birthmonth,birthday,birthyear);
														p=buff;
													}
												break;
											case 11:
												sprintf(buff,"%d",accesslevel);
												p=buff;
												break;
											case 12:
												sprintf(buff,"%d",sessiontimelimit);
												p=buff;
												break;
											case 13:
												sprintf(buff,"%d",inactivitytimelimit);
												p=buff;
												break;
											case 14:
												if(ratiobyte)
													sprintf(buff,"%d:1",ratiobyte);
												else
													strcpy(buff,"No byte ratio");
												p=buff;
												break;
											case 15:
												if(ratiofile)
													sprintf(buff,"%d:1",ratiofile);
												else
													strcpy(buff,"No file ratio");
												p=buff;
												break;
											case 16:
												sprintf(buff,"%ld",days);
												p=buff;
												break;
										}
									if(strlen(s))
										{
											sprintf(&s[strlen(s)]," %s",template[index]);
											if(p)
												{
													if(strlen(template[index])+strlen(p)>39)
														sprintf(&s[strlen(s)],"%-.*s...",36-strlen(template[index]),p);
													else
														strcat(s,p);
												}
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										{
											if(p)
												{
													if(strlen(template[index])+strlen(p)>39)
														sprintf(s,"%s%-.*s...",template[index],36-strlen(template[index]),p);
													else
														sprintf(s,"%s%-*s",template[index],39-strlen(template[index]),p);
												}
											else
												sprintf(s,"%-39s",template[index]);
										}
									index++;
								}
							if(strlen(s))
								printf("%s\n",s);
							printf("~p\nEdit User: ");
							if(GetLine(s,2,GLFLG_Digits))
								{
									switch(atoi(s))
										{
											case 1:
												printf("~p\nEnter this user's new password.\n: ");
												if(GetLine(s,10,GLFLG_NoEcho))
													strcpy(password,s);
												break;
											case 2:
												printf("~p\nEnter this user's address.\n: ");
												if(GetLine(s,30,GLFLG_Format))
													strcpy(address,s);
												break;
											case 3:
												printf("~p\nEnter this user's city.\n: ");
												if(GetLine(s,20,GLFLG_Format))
													strcpy(city,s);
												break;
											case 4:
												printf("~p\nEnter this user's state.\n: ");
												if(GetLine(s,20,GLFLG_Format))
													strcpy(state,s);
												break;
											case 5:
												printf("~p\nEnter this user's zip code.\n: ");
												if(GetLine(s,10,GLFLG_ToUpper))
													strcpy(zip,s);
												break;
											case 6:
												printf("~p\nEnter the country that this user lives in.\n: ");
												if(GetLine(s,3,GLFLG_ToUpper|GLFLG_Chars))
													strcpy(country,s);
												break;
											case 7:
												printf("~p\nEnter this user's area/country code.\n: ");
												if(GetLine(s,6,GLFLG_Digits|GLFLG_NoSpaces))
													{
														strcpy(phonecode,s);
														printf("\nEnter this user's phone number.\n: ");
														if(GetLine(s,8,GLFLG_Digits|GLFLG_NoSpaces))
															strcpy(phonenumber,s);
													}
												break;
											case 8:
												printf("~p\nEnter the year in which this user was born.\n: ");
												if(GetLine(s,4,GLFLG_Digits|GLFLG_NoSpaces))
													{
														birthyear=atoi(s);
														printf("~p\nEnter the month in which this user was born.\n: ");
														if(GetLine(s,2,GLFLG_Digits|GLFLG_NoSpaces))
															{
																birthmonth=atoi(s);
																printf("~p\nEnter the day on which you were born.\n: ");
																if(GetLine(s,2,GLFLG_Digits|GLFLG_NoSpaces))
																	birthday=atoi(s);
															}
													}
												break;
											case 9:
												printf("~p\nEnter a new access level for this user.\n: ");
												if(GetLine(s,3,GLFLG_Digits))
													accesslevel=atoi(s);
												break;
											case 10:
												printf("~p\nEnter a new session time limit for this user.\n: ");
												if(GetLine(s,5,GLFLG_Digits))
													sessiontimelimit=atoi(s);
												break;
											case 11:
												printf("~p\nEnter a new inactivity time limit for this user.\n: ");
												if(GetLine(s,5,GLFLG_Digits))
													inactivitytimelimit=atoi(s);
												break;
											case 12:
												printf("~p\nEnter a new byte ratio for this user.\n: ");
												if(GetLine(s,3,GLFLG_Digits))
													ratiobyte=atoi(s);
												break;
											case 13:
												printf("~p\nEnter a new file ratio for this user.\n: ");
												if(GetLine(s,3,GLFLG_Digits))
													ratiofile=atoi(s);
												break;
											case 14:
												printf("~p\nEnter a new number of days of inactivity before deletion for this user.\n: ");
												if(GetLine(s,10,GLFLG_Digits))
													days=atoi(s);
												break;
											case 15:
												if(SelectConfig(id,"AccessGroups","Access Group","access groups",s))
													{
														GetConfigTags(CFGTAG_Path,"AccessGroups",CFGTAG_Name,s,
															AGTAG_AccessLevel,&accesslevel,
															AGTAG_SessionTimeLimit,&sessiontimelimit,AGTAG_InactivityTimeLimit,&inactivitytimelimit,
															AGTAG_RatioByte,&ratiobyte,AGTAG_RatioFile,&ratiofile,
															AGTAG_Days,&days,
															TAG_END);
													}
												break;
											case 16:
												{
													char s[256];
													sprintf(s,"Delete `%s'",user);
													if(Ask(s,FALSE))
														{
															deleteuser=TRUE;
															kg=FALSE;
														}
												}
												break;
										}
								}
							else
								kg=FALSE;
						}
				}
				if(deleteuser)
					{
						if(!RemUserTags(USRTAG_Name,user,TAG_END))
							printf("~s\nUnable to delete user `%s'.\n",user);
					}
				else
					{
						SetUserTags(USRTAG_Name,user,
							USRTAG_Password,password,USRTAG_Address,address,USRTAG_City,city,USRTAG_State,state,USRTAG_Zip,zip,USRTAG_Country,country,USRTAG_PhoneCode,phonecode,USRTAG_PhoneNumber,phonenumber,USRTAG_BirthDay,birthday,USRTAG_BirthMonth,birthmonth,USRTAG_BirthYear,birthyear,
							USRTAG_AccessLevel,accesslevel,USRTAG_SessionTimeLimit,sessiontimelimit,USRTAG_InactivityTimeLimit,inactivitytimelimit,USRTAG_RatioByte,ratiobyte,USRTAG_RatioFile,ratiofile,USRTAG_Days,days,
							TAG_END);
					}
			}
		else
			printf("~s\nUser `%s' does not exist.\n",user);
	}

void PrintPrompt(BYTE *id,BYTE *s)
	{
		UWORD timeremaining=0;
		GetStatusTags(STTAG_Name,id,STTAG_TimeRemaining,&timeremaining,TAG_END);
		{
			BYTE buff[512],*p=NULL;
			BOOL format=FALSE,var=FALSE;
			strcpy(buff,"");
			p=s;
			while(*p)
				{
					if(format || var)
						{
							if(format)
								{
									switch(*p)
										{
											case 'e':strcat(buff,"\033");break;
											case 't':strcat(buff,"\t");break;
											case 'b':strcat(buff,"\b");break;
											case 'n':strcat(buff,"\r\n");break;
											case 'f':strcat(buff,"\f");break;
										}
									format=FALSE;
								}
							if(var)
								{
									switch(*p)
										{
											case 'T':
											case 't':
												sprintf(&buff[strlen(buff)],"%d",timeremaining);
												break;
											case 'E':
											case 'e':
												p++;
												switch(*p)
													{
														case 'C':
														case 'c':
															sprintf(&buff[strlen(buff)],"%ld",email_promptcurrent);
															break;
														case 'H':
														case 'h':
															sprintf(&buff[strlen(buff)],"%ld",email_prompthigh);
															break;
													}
												break;
											case 'M':
											case 'm':
												p++;
												switch(*p)
													{
														case 'A':
														case 'a':
															sprintf(&buff[strlen(buff)],"%s",message_fullpath);
															break;
														case 'C':
															sprintf(&buff[strlen(buff)],"%ld",(message_currentthread) ? message_currentthread-message_promptlowthread+1 : 0);
															break;
														case 'H':
															sprintf(&buff[strlen(buff)],"%ld",(message_prompthighthread) ? message_prompthighthread-message_promptlowthread+1 : 0);
															break;
														case 'c':
															sprintf(&buff[strlen(buff)],"%ld",message_promptcurrentresponse);
															break;
														case 'h':
															sprintf(&buff[strlen(buff)],"%ld",message_prompthighresponse);
															break;
													}
												break;
											case 'F':
											case 'f':
												p++;
												switch(*p)
													{
														case 'A':
														case 'a':
															sprintf(&buff[strlen(buff)],"%s",file_fullpath);
															break;
														case 'C':
														case 'c':
															sprintf(&buff[strlen(buff)],"%ld",file_current);
															break;
														case 'L':
														case 'l':
															sprintf(&buff[strlen(buff)],"%ld",file_promptlow);
															break;
														case 'H':
														case 'h':
															sprintf(&buff[strlen(buff)],"%ld",file_prompthigh);
															break;
														default:
															p--;
															break;
													}
												break;
										}
									var=FALSE;
								}
						}
					else
						{
							switch(*p)
								{
									case '%':var=TRUE;break;
									case '\\':format=TRUE;break;
									default:strncat(buff,p,1);break;
								}
						}
					p++;
				}
			printf("~p%s",buff);
		}
	}

BOOL IsOnline(BYTE *user)
	{
		BOOL ret=FALSE;
		{
			ULONG next=0;
			BYTE *name,username[33];
			strcpy(username,"");
			while(next=GetStatusTags(STTAG_Name,&name,STTAG_Next,next,STTAG_UserName,username,TAG_END))
				{
					if(strlen(username) && !strcmp(username,user))
						ret=TRUE;
				}
		}
		return(ret);
	}
