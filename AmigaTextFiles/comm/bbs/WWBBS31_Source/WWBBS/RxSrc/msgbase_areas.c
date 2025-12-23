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

void JoinMessageArea(BYTE *id)
	{
		if(message_global)
			{
				if(strlen(message_area))
					{
						if(strlen(message_group))
							{
								if(FindName(message_global,message_group))
									{
										if(DeleteMessageGlobal(message_global,message_group))
											printf("~s\nMessage area `%s' removed from global list.\n",message_fullpath);
										else
											printf("~s\nUnable to remove message area `%s' from global list.\n",message_fullpath);
									}
								else
									{
										if(AddMessageGlobal(message_global,message_group,0))
											printf("~s\nMessage area `%s' added to global list.\n",message_fullpath);
										else
											printf("~s\nUnable to add message area `%s' from global list.\n",message_fullpath);
									}
							}
					}
				else
					printf("~s\nPlease enter a message area first.\n");
			}
	}

void ChangeMessageArea(BYTE *id)
	{
		if(message_global)
			{
				BYTE user[33];
				strcpy(user,"");
				GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
				if(strlen(user))
					{
						UBYTE accesslevel=0;
						if(GetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,&accesslevel,TAG_END))
							{
								BYTE area[33];
								strcpy(area,"");
								if(SelectMessageArea(id,accesslevel,area))
									{
										BYTE group[21];
										BOOL child=FALSE;
										strcpy(group,"");
										if(GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,area,CFGTAG_Child,&child,MBTAG_Group,group,TAG_END))
											{
												if(child)
													{
														AddPart(message_path,area,255);
														strcpy(message_area,"");
														strcpy(message_group,"");
														message_current=0;
														strcpy(message_read,"");
														message_currentthread=0;
														{
															char *p;
															if(p=strchr(message_path,'/'))
																{
																	p++;
																	sprintf(message_fullpath,"%s/None",p);
																}
															else
																strcpy(message_fullpath,"None");
															printf("~s\nCurrent directory is `%s'.\n",(p) ? p : "None");
														}
													}
												else
													{
														strcpy(message_area,area);
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
														printf("\n");
														{
															APTR _group;
															if(_group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
																{
																	DisplayMessageArea(id,_group);
																	CloseMessageGroup(_group);
																}
														}
													}
											}
									}
							}
					}
			}
	}

void ParentMessageArea(BYTE *id)
	{
		BOOL newarea=FALSE;
		if(strlen(message_area))
			{
				strcpy(message_area,"");
				newarea=TRUE;
			}
		else
			{
				if(strchr(message_path,'/'))
					{
						{
							char *p;
							p=PathPart(message_path);
							*p=NULL;
						}
						newarea=TRUE;
					}
			}
		if(newarea)
			{
				message_current=0;
				strcpy(message_read,"");
				message_currentthread=0;
				{
					char *p;
					if(p=strchr(message_path,'/'))
						{
							p++;
							sprintf(message_fullpath,"%s/None",p);
						}
					else
						strcpy(message_fullpath,"None");
					printf("~s\nCurrent directory is `%s'.\n",(p) ? p : "None");
				}
			}
		else
			printf("~s\nParent area does not exist.\n");
	}

void NextMessageArea(BYTE *id)
	{
		if(message_global)
			{
				BYTE user[33];
				GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
				if(strlen(user))
					{
						UBYTE accesslevel=0;
						if(GetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,&accesslevel,TAG_END))
							{
								BOOL kg=TRUE;
								BYTE temp_path[256],temp_area[33];
								BYTE accessrange[21];
								BYTE group[21];
								strcpy(temp_path,message_path);
								strcpy(temp_area,message_area);
								strcpy(accessrange,"");
								strcpy(group,"");
								while(kg && GetNextArea(temp_path,temp_area))
									{
										if(GetConfigTags(CFGTAG_Path,temp_path,CFGTAG_Name,temp_area,MBTAG_AccessRange,accessrange,MBTAG_Group,group,TAG_END))
											{
												if(IsRange(accessrange,accesslevel))
													{
														if(FindName(message_global,group))
															{
																strcpy(message_path,temp_path);
																strcpy(message_area,temp_area);
																strcpy(message_group,group);
																message_current=0;
																strcpy(message_read,GetMessageGlobalRead(message_global,group));
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
																printf("\n");
																{
																	APTR _group;
																	if(_group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
																		{
																			DisplayMessageArea(id,_group);
																			CloseMessageGroup(_group);
																		}
																}
																kg=FALSE;
															}
													}
											}
									}
								if(kg)
									printf("~s\nNext area does not exist.\n");
							}
					}
			}
	}

void PreviousMessageArea(BYTE *id)
	{
		if(message_global)
			{
				BYTE user[33];
				GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
				if(strlen(user))
					{
						UBYTE accesslevel=0;
						if(GetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,&accesslevel,TAG_END))
							{
								BOOL kg=TRUE;
								BYTE temp_path[256],temp_area[33];
								BYTE accessrange[21];
								BYTE group[21];
								strcpy(temp_path,message_path);
								strcpy(temp_area,message_area);
								strcpy(accessrange,"");
								strcpy(group,"");
								while(kg && GetPreviousArea(temp_path,temp_area))
									{
										if(GetConfigTags(CFGTAG_Path,temp_path,CFGTAG_Name,temp_area,MBTAG_AccessRange,accessrange,MBTAG_Group,group,TAG_END))
											{
												if(IsRange(accessrange,accesslevel))
													{
														if(FindName(message_global,group))
															{
																strcpy(message_path,temp_path);
																strcpy(message_area,temp_area);
																strcpy(message_group,group);
																message_current=0;
																strcpy(message_read,GetMessageGlobalRead(message_global,group));
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
																printf("\n");
																{
																	APTR _group;
																	if(_group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
																		{
																			DisplayMessageArea(id,_group);
																			CloseMessageGroup(_group);
																		}
																}
																kg=FALSE;
															}
													}
											}
									}
								if(kg)
									printf("~s\nPrevious area does not exist.\n");
							}
					}
			}
	}

void DisplayMessageArea(BYTE *id,APTR group)
	{
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				ULONG nummsgs=0,newmsgs=0;
				{
					ULONG next=0;
					while(next=GetNextMessage(group,next))
						{
							if(GetMessageTags(group,MSGTAG_ID,next,TAG_END))
								{
									nummsgs++;
									if(!IsRange(message_read,next))
										newmsgs++;
								}
						}
				}
				printf("~hMessage Area: %s\n\n",message_fullpath);
				{
					BYTE filename[256];
					sprintf(filename,"WWBBS:Messages/%s.desc",message_group);
					{
						BPTR fh;
						if(fh=Open(filename,MODE_OLDFILE))
							{
								Close(fh);
								ShowText(filename);
								printf("\n");
							}
					}
				}
				printf("~o");
				printf("Messages Numbered: %ld-%ld\n",GetNextMessage(group,0),GetPreviousMessage(group,~0));
				printf("New Messages:      %ld\n",newmsgs);
				printf("Total Messages:    %ld\n",nummsgs);
				SetMessageDoorVariables(id);
			}
	}

void SetMessageDoorVariables(BYTE *id)
	{
		BYTE buff[11];
		BYTE accessrange[21],quoteheader[81];
		ULONG days=0;
		BOOL readonly=FALSE,autojoin=FALSE;
		strcpy(accessrange,"");
		strcpy(quoteheader,"");
		GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,message_area,
										MBTAG_AccessRange,accessrange,
										MBTAG_QuoteHeader,quoteheader,
										MBTAG_Days,&days,
										MBTAG_ReadOnly,&readonly,
										MBTAG_AutoJoin,&autojoin,
										TAG_END);
		SetVar("MESSAGEBASE_PATH",message_path,-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_AREA",message_area,-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_ACCESSRANGE",accessrange,-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_GROUP",message_group,-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_QUOTEHEADER",quoteheader,-1,GVF_LOCAL_ONLY);
		sprintf(buff,"%ld",days);
		SetVar("MESSAGEBASE_DAYS",buff,-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_READONLY",(readonly) ? "1" : "0",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_AUTOJOIN",(autojoin) ? "1" : "0",-1,GVF_LOCAL_ONLY);
	}
