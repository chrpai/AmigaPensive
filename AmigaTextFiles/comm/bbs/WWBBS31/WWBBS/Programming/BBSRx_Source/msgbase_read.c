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

BOOL NextUnreadMessage(BYTE *id)
	{
		BOOL ret=FALSE;
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				UBYTE accesslevel=0;
				if(GetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,&accesslevel,TAG_END))
					{
						BOOL none=TRUE;
						if(strlen(message_area))
							{
								APTR group;
								if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
									{
										ULONG old_message_current,old_message_currentthread;
										old_message_current=message_current;
										old_message_currentthread=message_currentthread;
										while(none && (message_current=GetNextResponse(id)))
											{
												if(!IsRange(message_read,message_current))
													none=FALSE;
											}
										if(none)
											{
												ULONG num=0;
												while(none && (num=GetNextThread(id)))
													{
														message_currentthread=num;
														message_current=0;
														while(none && (message_current=GetNextResponse(id)))
															{
																if(!IsRange(message_read,message_current))
																	none=FALSE;
															}
													}
											}
										if(none)
											{
												message_current=old_message_current;
												message_currentthread=old_message_currentthread;
											}
										CloseMessageGroup(group);
									}
							}
						if(none)
							{
								BOOL kg=TRUE;
								BYTE temp_path[256],temp_area[33],*temp_read=NULL;
								BYTE accessrange[21];
								BYTE group[21];
								strcpy(temp_path,message_path);
								strcpy(temp_area,message_area);
								strcpy(accessrange,"");
								strcpy(group,"");
								printf("~s\n");
								while(kg && GetNextArea(temp_path,temp_area))
									{
										if(GetConfigTags(CFGTAG_Path,temp_path,CFGTAG_Name,temp_area,MBTAG_AccessRange,accessrange,MBTAG_Group,group,TAG_END))
											{
												if(IsRange(accessrange,accesslevel))
													{
														if(FindName(message_global,group))
															{
																temp_read=GetMessageGlobalRead(message_global,group);
																{
																	APTR _group;
																	if(_group=OpenMessageGroup(temp_path,temp_area,SHARED_LOCK))
																		{
																			ULONG next=0;
																			while(kg && (next=GetNextMessage(_group,next)))
																				{
																					if(!IsRange(temp_read,next))
																						{
																							strcpy(message_path,temp_path);
																							strcpy(message_area,temp_area);
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
																							DisplayMessageArea(id,_group);
																							kg=FALSE;
																						}
																				}
																			CloseMessageGroup(_group);
																		}
																}
															}
													}
											}
									}
								if(kg)
									printf("End of global search.\n");
							}
						else
							{
								if(MessageDisplay(id))
									ret=TRUE;
							}
					}
			}
		return(ret);
	}

ULONG GetNextThread(BYTE *id)
	{
		ULONG nextthread=0;
		if(strlen(message_area))
			{
				APTR group;
				if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
					{
						ULONG next=0;
						ULONG thread=0;
						while(next=GetNextMessage(group,next))
							{
								if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
									{
										if(thread>message_currentthread && (!nextthread || thread<nextthread))
											nextthread=thread;
									}
							}
						CloseMessageGroup(group);
					}
			}
		return(nextthread);
	}

ULONG GetPreviousThread(BYTE *id)
	{
		ULONG previousthread=0;
		if(strlen(message_area))
			{
				APTR group;
				if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
					{
						ULONG previous=(ULONG) ~0;
						ULONG thread=0;
						while(previous=GetPreviousMessage(group,previous))
							{
								if(GetMessageTags(group,MSGTAG_ID,previous,MSGTAG_Thread,&thread,TAG_END))
									{
										if((!message_currentthread || thread<message_currentthread) && (!previousthread || thread>previousthread))
											previousthread=thread;
									}
							}
						CloseMessageGroup(group);
					}
			}
		return(previousthread);
	}

ULONG JumpToThread(BYTE *id)
	{
		ULONG num=0;
		if(strlen(message_area))
			{
				ULONG lowthread=0,highthread=0;
				{
					APTR group;
					if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
						{
							ULONG next=0;
							ULONG thread=0;
							while(next=GetNextMessage(group,next))
								{
									if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
										{
											if(!lowthread || thread<lowthread)
												lowthread=thread;
											if(!highthread || thread>highthread)
												highthread=thread;
										}
								}
							CloseMessageGroup(group);
						}
				}
				if(lowthread && highthread)
					{
						BYTE buff[11];
						strcpy(buff,"");
						printf("~p\nEnter the number of the thread to jump to, 1-%ld.\n: ",highthread-lowthread+1);
						if(GetLine(buff,10,GLFLG_Digits))
							{
								if(atol(buff)>=1 && atol(buff)<=highthread-lowthread+1)
									{
										APTR group;
										if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
											{
												ULONG next=0;
												ULONG thread=0;
												while(next=GetNextMessage(group,next))
													{
														if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
															{
																if(thread==atol(buff)+lowthread-1)
																	{
																		num=thread;
																		break;
																	}
															}
													}
												CloseMessageGroup(group);
											}
									}
								else
									printf("~s\nOut of range.\n");
							}
					}
				else
					printf("~s\nNo threads exist.\n");
			}
		else
			printf("~s\nPlease enter a message area first.\n");
		return(num);
	}

void ListThreads(BYTE *id)
	{
		BYTE rows[4];
		strcpy(rows,"");
		if(GetVar("ROWS",rows,3,NULL)!=-1)
			{
				if(strlen(message_area))
					{
						ULONG lowthread=0,highthread=0;
						{
							APTR group;
							if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
								{
									ULONG next=0;
									ULONG thread=0;
									while(next=GetNextMessage(group,next))
										{
											if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
												{
													if(!lowthread || thread<lowthread)
														lowthread=thread;
													if(!highthread || thread>highthread)
														highthread=thread;
												}
										}
									CloseMessageGroup(group);
								}
						}
						if(lowthread && highthread)
							{
								BYTE buff[21];
								strcpy(buff,"");
								printf("~p\nEnter range of threads to list, %ld-%ld, or `All' to list all threads.\n: ",lowthread,highthread);
								if(GetLine(buff,20,NULL))
									{
										if(!stricmp(buff,"ALL"))
											sprintf(buff,"%ld-%ld",lowthread,highthread);
										{
											ULONG *response_count=NULL,*response_id=NULL;
											int numwidth;
											numwidth=(int) (log10((double) highthread)+1);
											if(numwidth<3) numwidth=3;
											if(response_count=AllocVec(sizeof(ULONG)*(highthread-lowthread+1),MEMF_CLEAR))
												{
													if(response_id=AllocVec(sizeof(ULONG)*(highthread-lowthread+1),MEMF_CLEAR))
														{
															APTR group;
															if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
																{
																	{
																		ULONG next=0,thread=0;
																		while(next=GetNextMessage(group,next))
																			{
																				if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
																					{
																						response_count[thread-lowthread]++;
																						if(!response_id[thread-lowthread])
																							response_id[thread-lowthread]=next;
																					}
																			}
																	}
																	{
																		BOOL first=FALSE;
																		int i,line=0;
																		for(i=lowthread;i<=highthread;i++)
																			{
																				if(response_count[i-lowthread] && IsRange(buff,i))
																					{
																						if(WaitForChar(Input(),0))
																							{
																								getchar();
																								break;
																							}
																						if(!first)
																							{
																								printf("\n~h%*sNum  Responses Subject%*s\n~o",numwidth-3,"",57-(numwidth-3),"");
																								line++;
																								first=TRUE;
																							}
																						{
																							BYTE *subject=NULL;
																							if(GetMessageTags(group,MSGTAG_ID,response_id[i-lowthread],MSGTAG_Subject,&subject,TAG_END))
																								{
																									printf("%*d %10ld %-*.*s\n",numwidth,i,response_count[i-lowthread],67-numwidth,67-numwidth,subject);
																									line++;
																									if(line==atoi(rows)-1)
																										{
																											printf("~pMore ([Y],N)? ");
																											if(!Ask(NULL,TRUE))
																												break;
																											printf("~o");
																											line=0;
																										}
																								}
																						}
																					}
																			}
																		if(!first)
																			printf("~s\nNo threads found in specified range.\n");
																	}
																	CloseMessageGroup(group);
																}
															FreeVec(response_id);
														}
													FreeVec(response_count);
												}
										}
									}
							}
						else
							printf("~s\nNo threads exist.\n");
					}
				else
					printf("~s\nPlease enter a message area first.\n");
			}
	}

ULONG GetNextResponse(BYTE *id)
	{
		ULONG nextresponse=0;
		if(strlen(message_area))
			{
				if(message_currentthread)
					{
						APTR group;
						if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
							{
								ULONG next;
								ULONG thread=0;
								next=message_current;
								while(next=GetNextMessage(group,next))
									{
										if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
											{
												if(thread==message_currentthread)
													{
														nextresponse=next;
														break;
													}
											}
									}
								CloseMessageGroup(group);
							}
					}
			}
		return(nextresponse);
	}

ULONG GetPreviousResponse(BYTE *id)
	{
		ULONG previousresponse=0;
		if(strlen(message_area))
			{
				if(message_currentthread)
					{
						APTR group;
						if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
							{
								ULONG previous;
								ULONG thread=0;
								previous=(message_current) ? message_current : (ULONG) ~0;
								while(previous=GetPreviousMessage(group,previous))
									{
										if(GetMessageTags(group,MSGTAG_ID,previous,MSGTAG_Thread,&thread,TAG_END))
											{
												if(thread==message_currentthread)
													{
														previousresponse=previous;
														break;
													}
											}
									}
								CloseMessageGroup(group);
							}
					}
			}
		return(previousresponse);
	}

ULONG JumpToResponse(BYTE *id)
	{
		ULONG num=0;
		if(strlen(message_area))
			{
				if(message_currentthread)
					{
						ULONG count=0;
						{
							APTR group;
							if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
								{
									ULONG next=0;
									ULONG thread=0;
									while(next=GetNextMessage(group,next))
										{
											if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
												{
													if(thread==message_currentthread)
														count++;
												}
										}
									CloseMessageGroup(group);
								}
						}
						if(count)
							{
								BYTE buff[11];
								strcpy(buff,"");
								printf("~p\nEnter the number of the response to jump to, 1-%ld.\n: ",count);
								if(GetLine(buff,10,GLFLG_Digits))
									{
										if(atol(buff)>=1 && atol(buff)<=count)
											{
												APTR group;
												if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
													{
														ULONG next=0;
														ULONG index=0;
														ULONG thread=0;
														while(next=GetNextMessage(group,next))
															{
																if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
																	{
																		if(thread==message_currentthread)
																			{
																				index++;
																				if(index==atol(buff))
																					{
																						num=next;
																						break;
																					}
																			}
																	}
															}
														CloseMessageGroup(group);
													}
											}
										else
											printf("~s\nOut of range.\n");
									}
							}
						else
							printf("~s\nThere are no responses.\n");
					}
				else
					printf("~s\nThere is no current thread.\n");
			}
		else
			printf("~s\nPlease enter a message area first.\n");
		return(num);
	}

void ListResponses(BYTE *id)
	{
		BYTE rows[4];
		strcpy(rows,"");
		if(GetVar("ROWS",rows,3,NULL)!=-1)
			{
				if(strlen(message_area))
					{
						if(message_currentthread && message_current)
							{
								ULONG count=0;
								BYTE subject[65];
								strcpy(subject,"");
								{
									APTR group;
									if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
										{
											BYTE *subject_buff=NULL;
											if(GetMessageTags(group,MSGTAG_ID,message_current,MSGTAG_Subject,&subject_buff,TAG_END))
												{
													ULONG next=0;
													ULONG thread=0;
													while(next=GetNextMessage(group,next))
														{
															if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
																{
																	if(thread==message_currentthread)
																		count++;
																}
														}
													strcpy(subject,subject_buff);
												}
											CloseMessageGroup(group);
										}
								}
								if(count)
									{
										BYTE buff[21];
										strcpy(buff,"");
										printf("~p\nEnter range of responses to list, 1-%ld, or `All' to list all responses.\n: ",count);
										if(GetLine(buff,20,NULL))
											{
												if(!stricmp(buff,"ALL"))
													sprintf(buff,"1-%ld",count);
												{
													APTR group;
													if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
														{
															ULONG next=0,thread=0;
															BOOL first=FALSE;
															int i=0,line=0;
															int numwidth;
															numwidth=(int) (log10((double) count)+1);
															if(numwidth<3) numwidth=3;
															while(next=GetNextMessage(group,next))
																{
																	if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Thread,&thread,TAG_END))
																		{
																			if(thread==message_currentthread)
																				{
																					i++;
																					if(IsRange(buff,i))
																						{
																							if(WaitForChar(Input(),0))
																								{
																									getchar();
																									break;
																								}
																							if(!first)
																								{
																									BYTE *subject=NULL;
																									if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Subject,&subject,TAG_END))
																										{
																											printf("\n~oSubject: %s\n",subject);
																											printf("\n~h%*sNum Date      From%*s To%*s\n~o",numwidth-3,"",(64-(numwidth-3))/2-4,"",(64-(numwidth-3))/2-2,"");
																											line+=3;
																											first=TRUE;
																										}
																								}
																							{
																								BYTE *from=NULL,*to=NULL;
																								struct DateStamp *date=NULL;
																								char date_buff[16];
																								strcpy(date_buff,"");
																								if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_From,&from,MSGTAG_To,&to,MSGTAG_Date,&date,TAG_END))
																									{
																										{
																											struct DateTime dt;
																											dt.dat_Stamp=*date;
																											dt.dat_Format=FORMAT_USA;
																											dt.dat_Flags=DTF_SUBST;
																											dt.dat_StrDay=NULL;
																											dt.dat_StrDate=date_buff;
																											dt.dat_StrTime=NULL;
																											DateToStr(&dt);
																										}
																										printf("%*d %-9.9s %-*.*s %-*.*s\n",numwidth,i,date_buff,(64-(numwidth-3))/2,(64-(numwidth-3))/2,from,(64-(numwidth-3))/2,(64-(numwidth-3))/2,to);
																										line++;
																										if(line==atoi(rows)-1)
																											{
																												printf("~pMore ([Y],N)? ");
																												if(!Ask(NULL,TRUE))
																													break;
																												printf("~o");
																												line=0;
																											}
																									}
																							}
																						}
																				}
																		}
																}
															if(!first)
																printf("~s\nNo responses found in specified range.\n");
															CloseMessageGroup(group);
														}
												}
											}
									}
								else
									printf("~s\nNo responses exist.\n");
							}
						else
							printf("~s\nPlease enter a thread first.\n");
					}
				else
					printf("~s\nPlease enter a message area first.\n");
			}
	}

BOOL SearchNextMessage(BYTE *id)
	{
		BOOL ret=FALSE;
		if(strlen(message_area))
			{
				APTR group;
				if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
					{
						BOOL isdisplayed=FALSE;
						ULONG temp_currentthread=0;
						ULONG temp_current=0;
						temp_currentthread=message_currentthread;
						temp_current=message_current;
						{
							ULONG num=0;
							BYTE *from=NULL,*to=NULL,*subject=NULL,*text=NULL;
							BYTE parsed_frompattern[83],parsed_topattern[83],parsed_subjectpattern[83],parsed_textpattern[83];
							ParsePatternNoCase((strlen(message_frompattern)) ? message_frompattern : "#?",parsed_frompattern,82);
							ParsePatternNoCase((strlen(message_topattern)) ? message_topattern : "#?",parsed_topattern,82);
							ParsePatternNoCase((strlen(message_subjectpattern)) ? message_subjectpattern : "#?",parsed_subjectpattern,82);
							ParsePatternNoCase((strlen(message_textpattern)) ? message_textpattern : "#?",parsed_textpattern,82);
							while(num=GetNextThread(id))
								{
									message_currentthread=num;
									message_current=0;
									while(message_current=GetNextResponse(id))
										{
											if(GetMessageTags(group,MSGTAG_ID,message_current,MSGTAG_From,&from,MSGTAG_To,&to,MSGTAG_Subject,&subject,MSGTAG_Text,&text,TAG_END))
												{
													if(MatchPatternNoCase(parsed_frompattern,from) &&
															MatchPatternNoCase(parsed_topattern,to) &&
															MatchPatternNoCase(parsed_subjectpattern,subject) &&
															MatchPatternNoCase(parsed_textpattern,text))
														{
															isdisplayed=TRUE;
															if(MessageDisplay(id))
																ret=TRUE;
															break;
														}
												}
										}
								}
						}
						if(!isdisplayed)
							{
								message_currentthread=temp_currentthread;
								message_current=temp_current;
							}
						CloseMessageGroup(group);
					}
			}
		else
			printf("~s\nPlease enter a message area first.\n");
		return(ret);
	}

BOOL MessageDisplay(BYTE *id)
	{
		BOOL ret=FALSE;
		if(strlen(message_area))
			{
				if(message_current)
					{
						ULONG lowmsg=0,highmsg=0;
						MessageUpdateVars(id);
						{
							APTR group;
							if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
								{
									BYTE *from=NULL,*to=NULL,*subject=NULL,*text=NULL;
									struct DateStamp *date=NULL;
									ULONG thread=0;
									lowmsg=GetNextMessage(group,0);
									highmsg=GetPreviousMessage(group,~0);
									if(GetMessageTags(group,MSGTAG_ID,message_current,MSGTAG_Thread,&thread,MSGTAG_From,&from,MSGTAG_To,&to,MSGTAG_Subject,&subject,MSGTAG_Text,&text,MSGTAG_Date,&date,TAG_END))
										{
											printf("\n~h");
											{
												char buff[64];
												sprintf(buff,"[%ld/%ld] [%ld/%ld]",message_currentthread-message_promptlowthread+1,message_prompthighthread-message_promptlowthread+1,message_promptcurrentresponse,message_prompthighresponse);
												printf("[From   ] %-32s %36.36s\n",from,buff);
											}
											if(to && strlen(to))
												printf("[To     ] %-69s\n",to);
											{
												char date_buff[32];
												strcpy(date_buff,"");
												{
													struct DateTime dt;
													char time_buff[16];
													strcpy(time_buff,"");
													dt.dat_Stamp=*date;
													dt.dat_Format=FORMAT_USA;
													dt.dat_Flags=DTF_SUBST;
													dt.dat_StrDay=NULL;
													dt.dat_StrDate=date_buff;
													dt.dat_StrTime=time_buff;
													DateToStr(&dt);
													strcat(date_buff," ");
													strcat(date_buff,time_buff);
												}
												printf("[Date   ] %-69s\n",date_buff);
											}
											printf("[Subject] %-69.69s\n",subject);
											if(text)
												{
													printf("\n");
													Pager(text,(to && strlen(to)) ? 5 : 4);
												}
										}
									CloseMessageGroup(group);
								}
						}
						if(!IsRange(message_read,message_current))
							{
								UBYTE *read_flags=NULL;
								if(read_flags=AllocVec(sizeof(BYTE) * (((highmsg-lowmsg+1)/8) + (((highmsg-lowmsg+1)%8) ? 1 : 0)),MEMF_CLEAR))
									{
										{
											int i;
											for(i=0;i<highmsg-lowmsg+1;i++)
												{
													if(IsRange(message_read,i+lowmsg))
														read_flags[i/8]|=(1 << (i%8));
												}
										}
										read_flags[(message_current-lowmsg)/8]|=(1 << ((message_current-lowmsg)%8));
										strcpy(message_read,"");
										{
											ULONG low=(ULONG) ~0;
											int i=0;
											while(i<highmsg-lowmsg+1)
												{
													if(read_flags[i/8] & (1 << (i%8)))
														{
															if(low==~0)
																low=i;
														}
													else
														{
															if(low!=~0)
																{
																	if(low==i-1)
																		{
																			if(strlen(message_read))
																				sprintf(&message_read[strlen(message_read)],",%ld",low+lowmsg);
																			else
																				sprintf(message_read,"%ld",low+lowmsg);
																		}
																	else
																		{
																			if(strlen(message_read))
																				sprintf(&message_read[strlen(message_read)],",%ld-%ld",low+lowmsg,i-1+lowmsg);
																			else
																				sprintf(message_read,"%ld-%ld",low+lowmsg,i-1+lowmsg);
																		}
																	low=(ULONG) ~0;
																}
														}
													i++;
												}
											if(low!=~0)
												{
													if(low==i-1)
														{
															if(strlen(message_read))
																sprintf(&message_read[strlen(message_read)],",%ld",low+lowmsg);
															else
																sprintf(message_read,"%ld",low+lowmsg);
														}
													else
														{
															if(strlen(message_read))
																sprintf(&message_read[strlen(message_read)],",%ld-%ld",low+lowmsg,i-1+lowmsg);
															else
																sprintf(message_read,"%ld-%ld",low+lowmsg,i-1+lowmsg);
														}
												}
										}
										SetMessageGlobalRead(message_global,message_group,message_read);
										FreeVec(read_flags);
									}
								{
									BYTE user[33];
									strcpy(user,"");
									GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
									if(strlen(user))
										{
											ULONG msgsread=0;
											GetUserTags(USRTAG_Name,user,USRTAG_MessagesRead,&msgsread,TAG_END);
											msgsread++;
											SetUserTags(USRTAG_Name,user,USRTAG_MessagesRead,msgsread,TAG_END);
										}
								}
							}
						ret=TRUE;
					}
				else
					printf("~s\nCurrent message does not exist.\n");
			}
		else
			printf("~s\nPlease enter a message area first.\n");
		return(ret);
	}
