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

extern void EmailSend(BYTE *,BYTE *,BYTE *,BYTE *);

void MessageWrite(BYTE *id)
	{
		if(strlen(message_area))
			{
				if(!MessageReadOnlyArea())
					{
						BYTE from[33],to[33],subject[65];
						strcpy(from,"");
						strcpy(to,"");
						strcpy(subject,"");
						GetStatusTags(STTAG_Name,id,STTAG_UserName,from,TAG_END);
						if(strlen(from))
							{
								printf("~p\nEnter the subject of the message.\n: ");
								if(GetLine(subject,64,NULL))
									{
										printf("~p\nEnter the name of the user to whom you would like to address this message.\n: ");
										if(!GetLine(to,32,GLFLG_Format))
											strcpy(to,"");
										MessageSend(id,from,to,subject,0);
									}
							}
					}
				else
					printf("~s\nThis area is read only.\n");
			}
		else
			printf("~s\nPlease enter a message area first.\n");
	}

void MessageReplyTo(BYTE *id,BOOL private)
	{
		if(strlen(message_area))
			{
				if(private || !MessageReadOnlyArea())
					{
						if(message_current)
							{
								BYTE from[33],to[33],subject[65];
								strcpy(from,"");
								strcpy(to,"");
								strcpy(subject,"");
								GetStatusTags(STTAG_Name,id,STTAG_UserName,from,TAG_END);
								if(strlen(from))
									{
										BOOL ok=FALSE;
										BYTE filename[64];
										if(private)
											sprintf(filename,"T:wwbbs_email.%s",id);
										else
											sprintf(filename,"T:wwbbs_message.%s",id);
										{
											APTR group;
											if(group=OpenMessageGroup(message_path,message_area,SHARED_LOCK))
												{
													BYTE *from_buff=NULL,*subject_buff=NULL,*text_buff=NULL;
													struct DateStamp *date=NULL;
													if(GetMessageTags(group,MSGTAG_ID,message_current,
															MSGTAG_From,&from_buff,
															MSGTAG_Subject,&subject_buff,
															MSGTAG_Text,&text_buff,
															MSGTAG_Date,&date,
															TAG_END))
														{
															strcpy(to,from_buff);
															if(!strnicmp("Re:",subject_buff,3))
																strcpy(subject,subject_buff);
															else
																sprintf(subject,"Re: %.60s",subject_buff);
															if(text_buff && strlen(text_buff))
																{
#define RIGHT_MARGIN 79-strlen(quote_header)
#define WORD_LENGTH 10
																	FILE *fp;
																	if(fp=fopen(filename,"w"))
																		{
																			BYTE quote_header[64];
																			{
																				strcpy(quote_header," ");
																				if(strlen(from_buff))
																					{
																						char *p=NULL;
																						while(1)
																							{
																								if(p)
																									{
																										if(p=strchr(p,' '))
																											p++;
																										else
																											break;
																									}
																								else
																									p=from_buff;
																								if(*p)
																									strncat(quote_header,p,1);
																								else
																									break;
																							}
																					}
																				strcat(quote_header,"> ");
																			}
																			{
																				BYTE quote_header_template[256];
																				strcpy(quote_header_template,"");
																				GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,message_area,MBTAG_QuoteHeader,quote_header_template,TAG_END);
																				if(strlen(quote_header_template))
																					{
																						BYTE *p,*q[2],date_buff[32];
																						int i=0;
																						q[0]=q[1]=NULL;
																						p=quote_header_template;
																						while((i<2) && (p=strchr(p,'%')))
																							{
																								p++;
																								if(*p)
																									{
																										switch(*p)
																											{
																												case 'D':
																												case 'd':
																													{
																														struct DateTime dt;
																														char time_buff[16];
																														strcpy(date_buff,"");
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
																													q[i]=date_buff;
																													i++;
																													*p='s';
																													break;
																												case 'N':
																												case 'n':
																													q[i]=from_buff;
																													i++;
																													*p='s';
																													break;
																											}
																										p++;
																									}
																							}
																						fprintf(fp,quote_header_template,q[0],q[1]);
																						fputs("\n\n",fp);
																					}
																			}
																			{
																				char buff[256],outbuff[256],*p;
																				int index=0,blank=FALSE;
																				p=text_buff;
																				while(*p)
																					{
																						strcpy(buff,"");
																						{
																							char *q;
																							if(q=strchr(p,'\n'))
																								{
																									if(q-p<=255)
																										{
																											strncpy(buff,p,q-p);
																											buff[q-p]=NULL;
																											p=q+1;
																										}
																									else
																										{
																											strncpy(buff,p,255);
																											buff[255]=NULL;
																											p+=255;
																										}
																								}
																							else
																								{
																									if(strlen(p))
																										{
																											if(strlen(p)<=255)
																												{
																													strcpy(buff,p);
																													p=NULL;
																												}
																											else
																												{
																													strncpy(buff,p,255);
																													buff[255]=NULL;
																													p+=255;
																												}
																										}
																								}
																						}
																						if(strlen(buff))
																							{
																								if(!strncmp(buff,"---",3))
																									break;
																								{
																									char quote_buff[256];
																									sscanf(buff,"%s",quote_buff);
																									if(quote_buff[strlen(quote_buff)-1]=='>')
																										{
																											if(index)
																												{
																													outbuff[index]=NULL;
																													fprintf(fp,"%s%s\n",quote_header,outbuff);
																													index=0;
																												}
																											fprintf(fp,"%s\n",buff);
																										}
																									else
																										{
																											if(index)
																												{
																													if(isspace(buff[0]))
																														{
																															outbuff[index]=NULL;
																															fprintf(fp,"%s%s\n",quote_header,outbuff);
																															index=0;
																														}
																													else
																														{
																															outbuff[index]=' ';
																															index++;
																														}
																												}
																											{
																												char *q;
																												q=buff;
																												while(*q)
																													{
																														outbuff[index]=*q;
																														index++;
																														q++;
																														if(index>RIGHT_MARGIN)
																															{
																																if(strchr(outbuff,' '))
																																	{
																																		while(1)
																																			{
																																				index--;
																																				if(outbuff[index]==' ')
																																					{
																																						outbuff[index]=NULL;
																																						break;
																																					}
																																				q--;
																																			}
																																	}
																																else
																																	{
																																		q-=index-RIGHT_MARGIN;
																																		index=RIGHT_MARGIN;
																																		outbuff[index]=NULL;
																																	}
																																fprintf(fp,"%s%s\n",quote_header,outbuff);
																																index=0;
																															}
																													}
																											}
																											if(strlen(buff)<=RIGHT_MARGIN-WORD_LENGTH)
																												{
																													outbuff[index]=NULL;
																													fprintf(fp,"%s%s\n",quote_header,outbuff);
																													index=0;
																												}
																										}
																									blank=FALSE;
																								}
																							}
																						else
																							{
																								if(index)
																									{
																										outbuff[index]=NULL;
																										fprintf(fp,"%s%s\n",quote_header,outbuff);
																										index=0;
																									}
																								fprintf(fp,"\n");
																								blank=TRUE;
																							}
																					}
																				if(index)
																					{
																						outbuff[index]=NULL;
																						fprintf(fp,"%s%s\n",quote_header,outbuff);
																						index=0;
																						blank=FALSE;
																					}
																				if(!blank)
																					fprintf(fp,"\n");
																			}
																			fclose(fp);
																		}
#undef WORD_LENGTH
#undef RIGHT_MARGIN
																}
															ok=TRUE;
														}
													CloseMessageGroup(group);
												}
										}
										if(ok)
											{
												if(private)
													EmailSend(id,from,to,subject);
												else
													MessageSend(id,from,to,subject,message_current);
												DeleteFile(filename);
											}
										else
											printf("~s\nUnable to reply to current message.\n");
									}
							}
						else
							printf("~s\nCurrent message does not exist.\n");
					}
				else
					printf("~s\nThis area is read only.\n");
			}
		else
			printf("~s\nPlease enter a message area first.\n");
	}

void MessageSend(BYTE *id,BYTE *from,BYTE *to,BYTE *subject,ULONG replyto)
	{
		if(strlen(message_area))
			{
				if(!MessageReadOnlyArea())
					{
						BYTE filename[64];
						sprintf(filename,"T:wwbbs_message.%s",id);
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
																	if(group=OpenMessageGroup(message_path,message_area,EXCLUSIVE_LOCK))
																		{
																			ULONG num;
																			if(num=AddMessageTags(group,MSGTAG_ReplyTo,replyto,MSGTAG_DontSave,TRUE,TAG_END))
																				{
																					if(SetMessageTags(group,MSGTAG_ID,num,MSGTAG_From,from,MSGTAG_To,to,MSGTAG_Subject,subject,MSGTAG_Text,text,TAG_END))
																						{
																							{
																								BYTE user[33];
																								strcpy(user,"");
																								GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
																								if(strlen(user))
																									{
																										ULONG msgswritten=0;
																										GetUserTags(USRTAG_Name,user,USRTAG_MessagesWritten,&msgswritten,TAG_END);
																										msgswritten++;
																										SetUserTags(USRTAG_Name,user,USRTAG_MessagesWritten,msgswritten,TAG_END);
																									}
																							}
																							printf("~s\nMessage posted.\n");
																						}
																					else
																						{
																							RemMessageTags(group,MSGTAG_ID,num,TAG_END);
																							printf("~s\nUnable to post message.\n");
																						}
																				}
																			else
																				printf("~s\nUnable to post message.\n");
																			CloseMessageGroup(group);
																		}
																}
															else
																printf("~s\nUnable to post message.\n");
															FreeVec(text);
														}
													else
														printf("~s\nUnable to post message.\n");
												}
											else
												printf("~s\nUnable to post message.\n");
											Close(fh);
										}
									else
										printf("~s\nUnable to post message.\n");
								}
								DeleteFile(filename);
							}
					}
			}
	}

BOOL MessageReadOnlyArea()
	{
		BOOL readonly=FALSE;
		if(strlen(message_area))
			GetConfigTags(CFGTAG_Path,message_path,CFGTAG_Name,message_area,MBTAG_ReadOnly,&readonly,TAG_END);
		return(readonly);
	}
