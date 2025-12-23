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

#include "email.h"

void EmailCommand(BYTE *id,UWORD cmd,BYTE *arg)
	{
		BOOL updatevars=TRUE;
		switch(cmd)
			{
				case EMAIL_Next:
					if(EmailNext(id))
						updatevars=FALSE;
					break;
				case EMAIL_Current:
					if(email_current)
						{
							APTR group;
							if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
								{
									EmailDisplay(id,group);
									CloseMessageGroup(group);
								}
							updatevars=FALSE;
						}
					else
						printf("~s\nCurrent email does not exist.\n");
					break;
				case EMAIL_Previous:
					if(EmailPrevious(id))
						updatevars=FALSE;
					break;
				case EMAIL_JumpTo:
					if(EmailJumpTo(id))
						updatevars=FALSE;
					break;
				case EMAIL_List:
					EmailList(id);
					updatevars=FALSE;
					break;
				case EMAIL_Write:
					EmailWrite(id);
					break;
				case EMAIL_ReplyTo:
					EmailReplyTo(id);
					break;
				case EMAIL_Delete:
					EmailDelete(id);
					break;
				case EMAIL_Feedback:
					EmailFeedback(id);
					break;
				case EMAIL_ShowNew:
					EmailShowNew(id);
					break;
				default:
					updatevars=FALSE;
					break;
			}
		if(updatevars)
			EmailUpdateVars(id);
	}

/***/

BOOL EmailNext(BYTE *id)
	{
		BOOL ret=FALSE;
		APTR group;
		if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
			{
				ULONG num=0;
				num=email_current;
				while(num=GetNextMessage(group,num))
					{
						if(EmailIsAddressee(id,group,num))
							{
								email_current=num;
								break;
							}
					}
				if(num)
					{
						EmailDisplay(id,group);
						ret=TRUE;
					}
				else
					printf("~s\nNext email does not exist.\n");
				CloseMessageGroup(group);
			}
		return(ret);
	}

BOOL EmailPrevious(BYTE *id)
	{
		BOOL ret=FALSE;
		APTR group;
		if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
			{
				ULONG num=0;
				num=(email_current) ? email_current : (ULONG) ~0;
				while(num=GetPreviousMessage(group,num))
					{
						if(EmailIsAddressee(id,group,num))
							{
								email_current=num;
								break;
							}
					}
				if(num)
					{
						EmailDisplay(id,group);
						ret=TRUE;
					}
				else
					printf("~s\nPrevious email does not exist.\n");
				CloseMessageGroup(group);
			}
		return(ret);
	}

BOOL EmailJumpTo(BYTE *id)
	{
		BOOL ret=FALSE;
		APTR group;
		if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
			{
				ULONG count=0;
				{
					ULONG next=0;
					while(next=GetNextMessage(group,next))
						{
							if(EmailIsAddressee(id,group,next))
								count++;
						}
				}
				if(count)
					{
						BYTE buff[11];
						strcpy(buff,"");
						printf("~p\nEnter number of email to jump to, 1-%ld.\n: ",count);
						if(GetLine(buff,10,GLFLG_Digits))
							{
								if(atoi(buff)>=1 && atoi(buff)<=count)
									{
										ULONG num=0;
										int i;
										for(i=0;i<atoi(buff);i++)
											{
												while(num=GetNextMessage(group,num))
													{
														if(EmailIsAddressee(id,group,num))
															break;
													}
												if(!num)
													break;
											}
										if(num)
											{
												email_current=num;
												EmailDisplay(id,group);
												ret=TRUE;
											}
										else
											printf("~s\nUnable to jump to email number %d.\n",atoi(buff));
									}
								else
									printf("~s\nOut of range.\n");
							}
					}
				else
					printf("~s\nYou have no email.\n");
				CloseMessageGroup(group);
			}
		return(ret);
	}

void EmailList(BYTE *id)
	{
		BYTE rows[4];
		strcpy(rows,"");
		if(GetVar("ROWS",rows,3,NULL)!=-1)
			{
				APTR group;
				if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
					{
						ULONG count=0;
						{
							ULONG next=0;
							while(next=GetNextMessage(group,next))
								{
									if(EmailIsAddressee(id,group,next))
										count++;
								}
						}
						if(count)
							{
								BYTE buff[21];
								strcpy(buff,"");
								printf("~p\nEnter range of email to list, 1-%ld, or `All' to list all email.\n: ",count);
								if(GetLine(buff,20,NULL))
									{
										if(!stricmp(buff,"ALL"))
											sprintf(buff,"1-%ld",count);
										{
											ULONG next=0;
											BOOL first=FALSE;
											int i=0,line=0;
											int numwidth;
											numwidth=(int) (log10((double) count)+1);
											if(numwidth<3) numwidth=3;
											while(next=GetNextMessage(group,next))
												{
													if(EmailIsAddressee(id,group,next))
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
																			printf("\n~h%*sNum From                 Date      Subject%*s\n~o",numwidth-3,"",37-(numwidth-3),"");
																			line++;
																			first=TRUE;
																		}
																	{
																		BYTE *from=NULL,*subject=NULL;
																		struct DateStamp *date=NULL;
																		char date_buff[16];
																		strcpy(date_buff,"");
																		if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_From,&from,MSGTAG_Subject,&subject,MSGTAG_Date,&date,TAG_END))
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
																				printf("%*d %-20.20s %-9.9s %-*.*s\n",numwidth,i,from,date_buff,47-numwidth,47-numwidth,subject);
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
											if(!first)
												printf("~s\nNo email found in specified range.\n");
										}
									}
							}
						else
							printf("~s\nYou have no email.\n");
						CloseMessageGroup(group);
					}
			}
	}

void EmailWrite(BYTE *id)
	{
		BYTE from[33],to[33];
		strcpy(from,"");
		strcpy(to,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,from,TAG_END);
		if(strlen(from))
			{
				printf("~p\nEnter the name of the user to whom you would like to send email.\n: ");
				if(GetLine(to,32,GLFLG_Format))
					{
						if(GetUserTags(USRTAG_Name,to,USRTAG_Exists,TRUE,TAG_END))
							{
								BYTE subject[65];
								strcpy(subject,"");
								printf("~p\nEnter a subject for the email.\n: ");
								if(GetLine(subject,64,NULL))
									EmailSend(id,from,to,subject);
							}
						else
							printf("~s\nUser `%s' does not exist.\n",to);
					}
			}
	}

void EmailReplyTo(BYTE *id)
	{
		BYTE from[33],to[33],subject[65];
		strcpy(from,"");
		strcpy(to,"");
		strcpy(subject,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,from,TAG_END);
		if(strlen(from))
			{
				if(email_current)
					{
						APTR group;
						if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
							{
								BYTE *from_buff=NULL,*subject_buff=NULL;
								if(GetMessageTags(group,MSGTAG_ID,email_current,MSGTAG_From,&from_buff,MSGTAG_Subject,&subject_buff,TAG_END))
									{
										if(strlen(from_buff)<=32)
											strcpy(to,from_buff);
										if(!strnicmp("Re:",subject_buff,3))
											strcpy(subject,subject_buff);
										else
											sprintf(subject,"Re: %.60s",subject_buff);
									}
								CloseMessageGroup(group);
							}
						if(strlen(to))
							EmailSend(id,from,to,subject);
						else
							printf("~s\nUnable to send email.\n");
					}
				else
					printf("~s\nCurrent email does not exist.\n");
			}
	}

void EmailDelete(BYTE *id)
	{
		if(email_current)
			{
				APTR group;
				if(group=OpenMessageGroup(NULL,NULL,EXCLUSIVE_LOCK))
					{
						if(RemMessageTags(group,MSGTAG_ID,email_current))
							printf("~s\nEmail deleted.\n");
						else
							printf("~s\nUnable to delete email.\n");
						CloseMessageGroup(group);
					}
				else
					printf("~s\nUnable to delete email.\n");
			}
		else
			printf("~s\nCurrent email does not exist.\n");
	}

void EmailFeedback(BYTE *id)
	{
		BYTE from[33];
		strcpy(from,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,from,TAG_END);
		if(strlen(from))
			{
				BYTE subject[65];
				strcpy(subject,"");
				printf("~p\nEnter a subject for the feedback.\n: ");
				if(GetLine(subject,64,NULL))
					EmailSend(id,from,"Sysop",subject);
			}
	}

void EmailShowNew(BYTE *id)
	{
		APTR group;
		if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
			{
				ULONG count=0;
				{
					ULONG next=0;
					while(next=GetNextMessage(group,next))
						{
							if(EmailIsAddressee(id,group,next))
								count++;
						}
				}
				if(count)
					printf("~s\nYou have %ld pieces of email waiting.\n",count);
				else
					printf("~s\nYou have no mail.\n");
				CloseMessageGroup(group);
			}
	}

/***/

BOOL EmailIsAddressee(BYTE *id,APTR group,ULONG num)
	{
		BOOL ret=FALSE;
		BOOL onlyrealnames=FALSE;
		BYTE name[33],username[33],realname[33],uucpname[9];
		strcpy(name,"");
		strcpy(username,"");
		strcpy(realname,"");
		strcpy(uucpname,"");
		GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
		GetStatusTags(STTAG_Name,id,STTAG_UserName,name,TAG_END);
		if(strlen(name))
			{
				BYTE *to=NULL;
				GetUserTags(USRTAG_Name,name,USRTAG_UserName,username,USRTAG_RealName,realname,USRTAG_UUCPName,uucpname,TAG_END);
				if(GetMessageTags(group,MSGTAG_ID,num,MSGTAG_To,&to,TAG_END))
					{
						if(!stricmp(to,"Sysop"))
							{
								BYTE sysop_username[33],sysop_realname[33],sysop_uucpname[9];
								strcpy(sysop_username,"");
								strcpy(sysop_realname,"");
								strcpy(sysop_uucpname,"");
								GetUserTags(USRTAG_Name,"Sysop",USRTAG_UserName,sysop_username,USRTAG_RealName,sysop_realname,USRTAG_UUCPName,sysop_uucpname,TAG_END);
								if((!onlyrealnames && !stricmp(sysop_username,username)) || !stricmp(sysop_realname,realname) || !stricmp(sysop_uucpname,uucpname))
									ret=TRUE;
							}
						else
							{
								if((!onlyrealnames && !stricmp(to,username)) || !stricmp(to,realname) || !stricmp(to,uucpname))
									ret=TRUE;
							}
					}
			}
		return(ret);
	}

void EmailDisplay(BYTE *id,APTR group)
	{
		BYTE *from=NULL,*to=NULL,*subject=NULL,*text=NULL;
		struct DateStamp *date=NULL;
		if(GetMessageTags(group,MSGTAG_ID,email_current,
				MSGTAG_From,&from,
				MSGTAG_To,&to,
				MSGTAG_Subject,&subject,
				MSGTAG_Text,&text,
				MSGTAG_Date,&date,
				TAG_END))
			{
				printf("\n~h");
				{
					char buff[64];
					EmailUpdateVars(id);
					sprintf(buff,"[%ld/%ld]",email_promptcurrent,email_prompthigh);
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
				{
					BYTE user[33];
					strcpy(user,"");
					GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
					if(strlen(user))
						{
							ULONG emailread=0;
							GetUserTags(USRTAG_Name,user,USRTAG_EmailRead,&emailread,TAG_END);
							emailread++;
							SetUserTags(USRTAG_Name,user,USRTAG_EmailRead,emailread,TAG_END);
						}
				}
			}
		else
			printf("~s\nCurrent email does not exist.\n");
	}

void EmailSend(BYTE *id,BYTE *from,BYTE *to,BYTE *subject)
	{
		BYTE filename[64];
		sprintf(filename,"T:wwbbs_email.%s",id);
		if(RunEditor(filename))
			{
				BYTE *text=NULL;
				{
					BPTR fh_signature,fh_msg;
					BYTE signature_file[256];
					strcpy(signature_file,"");
					if(GetVar("HOME",signature_file,255,NULL)!=-1)
						{
							AddPart(signature_file,".signature",255);
							if(fh_signature=Open(signature_file,MODE_OLDFILE))
								{
									if(fh_msg=Open(filename,MODE_READWRITE))
										{
											Seek(fh_msg,0,OFFSET_END);
											{
												BYTE buff[256];
												while(FGets(fh_signature,buff,255))
													FPuts(fh_msg,buff);
											}
											Close(fh_msg);
										}
									Close(fh_signature);
								}
						}
				}
				{
					BPTR fh;
					if(fh=Open(filename,MODE_OLDFILE))
						{
							__aligned struct FileInfoBlock fib;
							if(ExamineFH(fh,&fib))
								{
									if(text=AllocVec(fib.fib_Size+1,MEMF_CLEAR))
										{
											if(FRead(fh,text,sizeof(BYTE),fib.fib_Size)==fib.fib_Size)
												{
													APTR group;
													if(group=OpenMessageGroup(NULL,NULL,EXCLUSIVE_LOCK))
														{
															ULONG num;
															if(num=AddMessageTags(group,MSGTAG_DontSave,TRUE,TAG_END))
																{
																	if(SetMessageTags(group,MSGTAG_ID,num,MSGTAG_From,from,MSGTAG_To,to,MSGTAG_Subject,subject,MSGTAG_Text,text,TAG_END))
																		{
																			{
																				BYTE user[33];
																				strcpy(user,"");
																				GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
																				if(strlen(user))
																					{
																						ULONG emailwritten=0;
																						GetUserTags(USRTAG_Name,user,USRTAG_EmailWritten,&emailwritten,TAG_END);
																						emailwritten++;
																						SetUserTags(USRTAG_Name,user,USRTAG_EmailWritten,emailwritten,TAG_END);
																					}
																			}
																			printf("~s\nEmail sent.\n");
																		}
																	else
																		{
																			RemMessageTags(group,MSGTAG_ID,num,TAG_END);
																			printf("~s\nUnable to send email.\n");
																		}
																}
															else
																printf("~s\nUnable to send email.\n");
															CloseMessageGroup(group);
														}
												}
											else
												printf("~s\nUnable to send email.\n");
											FreeVec(text);
										}
									else
										printf("~s\nUnable to send email.\n");
								}
							else
								printf("~s\nUnable to send email.\n");
							Close(fh);
						}
					else
						printf("~s\nUnable to send email.\n");
				}
				DeleteFile(filename);
			}
	}

void EmailUpdateVars(BYTE *id)
	{
		APTR group;
		if(group=OpenMessageGroup(NULL,NULL,SHARED_LOCK))
			{
				ULONG count=0;
				email_promptcurrent=0;
				{
					ULONG next=0;
					while(next=GetNextMessage(group,next))
						{
							if(EmailIsAddressee(id,group,next))
								{
									count++;
									if(next==email_current)
										email_promptcurrent=count;
								}
						}
				}
				if(!email_promptcurrent)
					email_current=0;
				email_prompthigh=count;
				CloseMessageGroup(group);
			}
	}
