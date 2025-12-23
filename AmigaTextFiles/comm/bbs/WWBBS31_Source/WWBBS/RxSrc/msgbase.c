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

#include "msgbase.h"

void MessageBaseCommand(BYTE *id,UWORD cmd,BYTE *arg)
	{
		BOOL update_vars=TRUE;
		switch(cmd)
			{
				case MSG_InitGlobal:
					InitMessageGlobal(id);
					break;
				case MSG_EditSearchPatterns:
					EditMessageSearchPatterns(id);
					break;
				case MSG_JoinArea:
					JoinMessageArea(id);
					break;
				case MSG_ChangeArea:
					ChangeMessageArea(id);
					break;
				case MSG_ParentArea:
					ParentMessageArea(id);
					break;
				case MSG_NextArea:
					NextMessageArea(id);
					break;
				case MSG_PreviousArea:
					PreviousMessageArea(id);
					break;
				case MSG_NextUnread:
					if(NextUnreadMessage(id))
						update_vars=FALSE;
					break;
				case MSG_Current:
					if(MessageDisplay(id))
						update_vars=FALSE;
					break;
				case MSG_NextThread:
					if(strlen(message_area))
						{
							ULONG num=0;
							if(num=GetNextThread(id))
								{
									message_currentthread=num;
									message_current=0;
									if(message_current=GetNextResponse(id))
										{
											if(MessageDisplay(id))
												update_vars=FALSE;
										}
									else
										printf("~s\nUnable to get first response in thread.\n");
								}
							else
								printf("~s\nUnable to get next thread.\n");
						}
					else
						printf("~s\nPlease enter a message area first.\n");
					break;
				case MSG_PreviousThread:
					if(strlen(message_area))
						{
							ULONG num=0;
							if(num=GetPreviousThread(id))
								{
									message_currentthread=num;
									message_current=0;
									if(message_current=GetPreviousResponse(id))
										{
											if(MessageDisplay(id))
												update_vars=FALSE;
										}
									else
										printf("~s\nUnable to get last response in thread.\n");
								}
							else
								printf("~s\nUnable to get previous thread.\n");
						}
					else
						printf("~s\nPlease enter a message area first.\n");
					break;
				case MSG_JumpToThread:
					{
						ULONG num=0;
						if(num=JumpToThread(id))
							{
								message_currentthread=num;
								message_current=0;
								if(message_current=GetNextResponse(id))
									{
										if(MessageDisplay(id))
											update_vars=FALSE;
									}
							}
					}
					break;
				case MSG_ListThreads:
					ListThreads(id);
					update_vars=FALSE;
					break;
				case MSG_NextResponse:
					if(strlen(message_area))
						{
							ULONG num=0;
							if(num=GetNextResponse(id))
								{
									message_current=num;
									if(MessageDisplay(id))
										update_vars=FALSE;
								}
							else
								printf("~s\nUnable to get next response.\n");
						}
					else
						printf("~s\nPlease enter a message area first.\n");
					break;
				case MSG_PreviousResponse:
					if(strlen(message_area))
						{
							ULONG num=0;
							if(num=GetPreviousResponse(id))
								{
									message_current=num;
									if(MessageDisplay(id))
										update_vars=FALSE;
								}
							else
								printf("~s\nUnable to get previous response.\n");
						}
					else
						printf("~s\nPlease enter a message area first.\n");
					break;
				case MSG_JumpToResponse:
					{
						ULONG num=0;
						if(num=JumpToResponse(id))
							{
								message_current=num;
								if(MessageDisplay(id))
									update_vars=FALSE;
							}
					}
					break;
				case MSG_ListResponses:
					ListResponses(id);
					update_vars=FALSE;
					break;
				case MSG_SearchNext:
					if(SearchNextMessage(id))
						update_vars=FALSE;
					break;
				case MSG_Catchup:
					{
						if(strlen(message_area))
							{
								if(Ask("Catchup messages",FALSE))
									{
										APTR group;
										if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
											{
												ULONG low=0,high=0;
												low=GetNextMessage(group,0);
												high=GetPreviousMessage(group,~0);
												if(low && high)
													{
														if(low==high)
															sprintf(message_read,"%ld",low);
														else
															sprintf(message_read,"%ld-%ld",low,high);
														SetMessageGlobalRead(message_global,message_group,message_read);
													}
												CloseMessageGroup(group);
											}
									}
							}
						else
							printf("~s\nPlease enter a message area first.\n");
					}
					break;
				case MSG_Write:
					MessageWrite(id);
					break;
				case MSG_ReplyTo:
					MessageReplyTo(id,FALSE);
					break;
				case MSG_MailReplyTo:
					MessageReplyTo(id,TRUE);
					break;
				case MSG_Kill:
					{
						if(strlen(message_area))
							{
								if(message_current)
									{
										if(Ask("Kill message",FALSE))
											{
												APTR group;
												if(group=OpenMessageGroup(message_path,message_area,EXCLUSIVE_LOCK))
													{
														if(RemMessageTags(group,MSGTAG_ID,message_current,TAG_END))
															{
																message_current=0;
																{
																	BOOL responses=FALSE;
																	ULONG next=0,thread=0;
																	while(next=GetNextMessage(group,next))
																		{
																			if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
																				{
																					if(thread==message_currentthread)
																						responses=TRUE;
																				}
																		}
																	if(!responses) message_currentthread=0;
																}
																printf("~s\nMessage killed.\n");
																update_vars=TRUE;
															}
														else
															printf("~s\nUnable to kill message.\n");
														CloseMessageGroup(group);
													}
											}
									}
								else
									printf("~s\nCurrent message does not exist.\n");
							}
						else
							printf("~s\nPlease enter a message area first.\n");
					}
					break;
				case MSG_EditAreaDescription:
					{
						if(strlen(message_area))
							{
								BYTE group[21];
								strcpy(group,"");
								if(GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,message_area,MBTAG_Group,group,TAG_END))
									{
										BYTE filename[256];
										sprintf(filename,"WWBBS:Messages/%s.desc",group);
										printf("~p\nEnter a description for the area `%s'.\n",message_fullpath);
										RunEditor(filename);
									}
							}
						else
							printf("~s\nPlease enter a message area first.\n");
					}
					break;
			}
		if(update_vars)
			MessageUpdateVars(id);
	}

void InitMessageGlobal(BYTE *id)
	{
		if(message_global)
			{
				BYTE path[256],name[33];
				BYTE user[33];
				UBYTE accesslevel=0;
				BYTE accessrange[21];
				BYTE group[21];
				BOOL autojoin=FALSE;
				strcpy(path,"MessageBases");
				strcpy(name,"");
				strcpy(accessrange,"");
				strcpy(group,"");
				strcpy(user,"");
				GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
				if(strlen(user))
					{
						GetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,&accesslevel,TAG_END);
						while(GetNextArea(path,name))
							{
								if(GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,MBTAG_AccessRange,accessrange,MBTAG_Group,group,MBTAG_AutoJoin,&autojoin,TAG_END))
									{
										if(IsRange(accessrange,accesslevel) && autojoin)
											AddMessageGlobal(message_global,group,0);
									}
							}
					}
			}
	}

void EditMessageSearchPatterns(BYTE *id)
	{
		char *template[]={
			"[1] From:    ",
			"[2] To:      ",
			"[3] Subject: ",
			"[4] Text:    ",
			"[5] Reset Patterns",
			NULL
		};
		char s[256],*p;
		int index;
		BOOL kg=TRUE;
		while(!panic && kg)
			{
				printf("~o\n");
				index=0;
				strcpy(s,"");
				while(template[index])
					{
						if(WaitForChar(Input(),0))
							{
								getchar();
								break;
							}
						switch(index)
							{
								case 0: p=(strlen(message_frompattern)) ? message_frompattern : "#?"; break;
								case 1: p=(strlen(message_topattern)) ? message_topattern : "#?"; break;
								case 2: p=(strlen(message_subjectpattern)) ? message_subjectpattern : "#?"; break;
								case 3: p=(strlen(message_textpattern)) ? message_textpattern : "#?"; break;
								case 4: p=""; break;
								default: p=NULL; break;
							}
						if(p)
							{
								if(strlen(s))
									{
										if(strlen(template[index])+strlen(p)>39)
											sprintf(&s[strlen(s)]," %s%-*s...",template[index],36-strlen(template[index]),p);
										else
											sprintf(&s[strlen(s)]," %s%-*s",template[index],39-strlen(template[index]),p);
										printf("%s\n",s);
										strcpy(s,"");
									}
								else
									{
										if(strlen(template[index])+strlen(p)>39)
											sprintf(s,"%s%-*s...",template[index],36-strlen(template[index]),p);
										else
											sprintf(s,"%s%-*s",template[index],39-strlen(template[index]),p);
									}
							}
						index++;
					}
				if(strlen(s))
					printf("%s\n",s);
				printf("~p\nEdit Message Search Patterns: ");
				if(GetLine(s,1,GLFLG_Digits))
					{
						index=atoi(s);
						switch(index)
							{
								case 1:
									printf("~p\nEnter an AmigaDOS style pattern for the from field.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(message_frompattern,s);
									else
										strcpy(message_frompattern,"");
									break;
								case 2:
									printf("~p\nEnter an AmigaDOS style pattern for the to field.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(message_topattern,s);
									else
										strcpy(message_topattern,"");
									break;
								case 3:
									printf("~p\nEnter an AmigaDOS style pattern for the subject field.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(message_subjectpattern,s);
									else
										strcpy(message_subjectpattern,"");
									break;
								case 4:
									printf("~p\nEnter an AmigaDOS style pattern for the message text.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(message_textpattern,s);
									else
										strcpy(message_textpattern,"");
									break;
								case 5:
									strcpy(message_frompattern,"");
									strcpy(message_topattern,"");
									strcpy(message_subjectpattern,"");
									strcpy(message_textpattern,"");
									printf("~s\nMessage search patterns reset.\n");
									break;
							}
					}
				else
					kg=FALSE;
			}
	}

struct List *OpenMessageGlobal(BYTE *id)
	{
		struct List *ret=NULL;
		BYTE file[256],user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				if(ret=AllocVec(sizeof(struct List),MEMF_CLEAR))
					{
						NewList(ret);
						if(GetVar("HOME",file,255,NULL)!=-1)
							{
								FILE *fp;
								AddPart(file,".messageglobal",255);
								if(fp=fopen(file,"r"))
									{
										BYTE buff[256];
										struct MessageGlobalNode *node;
										while(fgets(buff,255,fp))
											{
												if(node=AllocVec(sizeof(struct MessageGlobalNode),MEMF_CLEAR))
													{
														char *p;
														if(p=strchr(buff,'\n'))
															*p=NULL;
														if(p=strchr(buff,' '))
															{
																*p=NULL;
																p++;
																strcpy(node->mgn_Name,buff);
																strcpy(node->mgn_Read,p);
															}
														node->mgn_Node.ln_Name=node->mgn_Name;
														AddTail(ret,(struct Node *) node);
													}
											}
										fclose(fp);
									}
							}
					}
			}
		return(ret);
	}

void CloseMessageGlobal(struct List *list)
	{
		BYTE file[256];
		if(GetVar("HOME",file,255,NULL)!=-1)
			{
				FILE *fp;
				AddPart(file,".messageglobal",255);
				if(fp=fopen(file,"w"))
					{
						struct MessageGlobalNode *node;
						for(node=(struct MessageGlobalNode *) list->lh_Head;node->mgn_Node.ln_Succ;node=(struct MessageGlobalNode *) node->mgn_Node.ln_Succ)
							fprintf(fp,"%s %s\n",node->mgn_Name,node->mgn_Read);
						fclose(fp);
					}
			}
		{
			struct MessageGlobalNode *node;
			while(node=(struct MessageGlobalNode *) RemHead(list))
				FreeVec(node);
			FreeVec(list);
		}
	}

BOOL AddMessageGlobal(struct List *list,BYTE *name,BYTE *read)
	{
		BOOL ret=FALSE;
		struct MessageGlobalNode *node;
		if(node=AllocVec(sizeof(struct MessageGlobalNode),MEMF_CLEAR))
			{
				strcpy(node->mgn_Name,name);
				strcpy(node->mgn_Read,read);
				node->mgn_Node.ln_Name=node->mgn_Name;
				AddTail(list,(struct Node *) node);
				ret=TRUE;
			}
		return(ret);
	}

BOOL DeleteMessageGlobal(struct List *list,BYTE *name)
	{
		BOOL ret=FALSE;
		struct MessageGlobalNode *node;
		if(node=(struct MessageGlobalNode *) FindName(list,name))
			{
				Remove((struct Node *) node);
				FreeVec(node);
				ret=TRUE;
			}
		return(ret);
	}

BYTE *GetMessageGlobalRead(struct List *list,BYTE *name)
	{
		BYTE *ret=NULL;
		struct MessageGlobalNode *node;
		if(node=(struct MessageGlobalNode *) FindName(list,name))
			ret=node->mgn_Read;
		return(ret);
	}

BOOL SetMessageGlobalRead(struct List *list,BYTE *name,BYTE *read)
	{
		BOOL ret=FALSE;
		struct MessageGlobalNode *node;
		if(node=(struct MessageGlobalNode *) FindName(list,name))
			{
				strcpy(node->mgn_Read,read);
				ret=TRUE;
			}
		return(ret);
	}

int SelectMessageArea(BYTE *id,UBYTE accesslevel,BYTE *result)
	{
		int selected=0,count=0;
		{
			ULONG next=0;
			BYTE accessrange[21];
			strcpy(accessrange,"");
			while(next=GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Next,next,MBTAG_AccessRange,accessrange,TAG_END))
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
					BOOL child=FALSE;
					BYTE accessrange[21];
					strcpy(s,"");
					strcpy(accessrange,"");
					printf("~o\n");
					while(next=GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,&name,CFGTAG_Next,next,CFGTAG_Child,&child,MBTAG_AccessRange,accessrange,TAG_END))
						{
							if(IsRange(accessrange,accesslevel))
								{
									index++;
									if(strlen(s))
										{
											sprintf(&s[strlen(s)]," [%*d] %s%c%*s",(int) (log10((double) count)+1),index,name,(child) ? '/' : ' ',32-strlen(name),"");
											printf("%s\n",s);
											strcpy(s,"");
										}
									else
										sprintf(s,"[%*d] %s%c%*s",(int) (log10((double) count)+1),index,name,(child) ? '/' : ' ',32-strlen(name),"");
								}
						}
					if(strlen(s))
						printf("%s\n",s);
				}
				{
					BYTE s[256];
					printf("~p\nSelect Message Area: ");
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *name=NULL;
									BYTE accessrange[21];
									strcpy(accessrange,"");
									while(next=GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,&name,CFGTAG_Next,next,MBTAG_AccessRange,accessrange,TAG_END))
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
			printf("~s\nNo message areas exist.\n");
		return(selected);
	}

void MessageUpdateVars(BYTE *id)
	{
		message_promptlowthread=0;
		message_prompthighthread=0;
		message_promptcurrentresponse=0;
		message_prompthighresponse=0;
		if(strlen(message_area))
			{
				APTR group;
				if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
					{
						{
							ULONG next=0;
							ULONG thread=0;
							while(next=GetNextMessage(group,next))
								{
									if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
										{
											if(!message_promptlowthread || message_promptlowthread>thread)
												message_promptlowthread=thread;
											if(!message_prompthighthread || message_prompthighthread<thread)
												message_prompthighthread=thread;
										}
								}
						}
						if(message_currentthread)
							{
								ULONG next=0;
								ULONG thread=0;
								while(next=GetNextMessage(group,next))
									{
										if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
											{
												if(thread==message_currentthread)
													{
														message_prompthighresponse++;
														if(message_current==next)
															message_promptcurrentresponse=message_prompthighresponse;
													}
											}
									}
							}
						CloseMessageGroup(group);
					}
			}
		if(!message_promptcurrentresponse)
			message_current=0;
		if(!message_currentthread)
			message_current=0;
	}
