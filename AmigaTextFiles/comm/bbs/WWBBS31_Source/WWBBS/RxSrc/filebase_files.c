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

#include "filebase.h"

void NextFile(BYTE *id)
	{
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				UBYTE accesslevel=0;
				if(GetUserTags(USRTAG_Name,user,USRTAG_AccessLevel,&accesslevel,TAG_END))
					{
						BOOL none=TRUE;
						if(strlen(file_area))
							{
								APTR group;
								if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
									{
										ULONG num=0;
										while(num=GetNextFile(group,file_current))
											{
												none=FALSE;
												file_current=num;
												if(file_current>file_high)
													{
														file_high=file_current;
														SetFileGlobalHigh(file_global,file_group,file_high);
													}
												if(!DisplayFile(id,group))
													break;
											}
										CloseFileGroup(group);
									}
							}
						if(none)
							{
								BOOL kg=TRUE;
								BYTE temp_path[256],temp_area[33];
								ULONG temp_high=0;
								BYTE accessrange[21];
								BYTE group[41];
								strcpy(temp_path,file_path);
								strcpy(temp_area,file_area);
								strcpy(accessrange,"");
								strcpy(group,"");
								printf("~s\n");
								while(kg && GetNextArea(temp_path,temp_area))
									{
										if(GetConfigTags(CFGTAG_Path,temp_path,CFGTAG_Name,temp_area,FBTAG_AccessRange,accessrange,FBTAG_Group,group,TAG_END))
											{
												if(IsRange(accessrange,accesslevel))
													{
														if(FindName(file_global,group))
															{
																temp_high=GetFileGlobalHigh(file_global,group);
																{
																	BOOL desc=FALSE;
																	APTR _group;
																	if(_group=OpenFileGroup(temp_path,temp_area,SHARED_LOCK))
																		{
																			if(GetNextFile(_group,temp_high))
																				{
																					strcpy(file_path,temp_path);
																					strcpy(file_area,temp_area);
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
																					desc=DisplayArea(id,_group);
																					kg=FALSE;
																				}
																			CloseFileGroup(_group);
																		}
																	if(desc)
																		GetFileDescriptions(id);
																}
															}
													}
											}
									}
								if(kg)
									printf("End of global search.\n");
							}
					}
			}
	}

void CurrentFile(BYTE *id)
	{
		if(strlen(file_area))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
					{
						if(file_current)
							DisplayFile(id,group);
						else
							printf("~s\nCurrent file does not exist.\n");
						CloseFileGroup(group);
					}
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void PreviousFile(BYTE *id)
	{
		if(strlen(file_area))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
					{
						ULONG num=0;
						if(num=GetPreviousFile(group,file_current))
							{
								file_current=num;
								DisplayFile(id,group);
							}
						else
							printf("~s\nPrevious file does not exist.\n");
						CloseFileGroup(group);
					}
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void JumptoFile(BYTE *id)
	{
		if(strlen(file_area))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
					{
						BYTE s[11];
						strcpy(s,"");
						printf("~p\nEnter number of file to jump to.\n: ");
						if(GetLine(s,10,GLFLG_Digits))
							{
								if(GetFileTags(group,FILTAG_ID,atol(s),FILTAG_Exists,TRUE,TAG_END))
									{
										file_current=atol(s);
										if(file_current>file_high)
											{
												file_high=file_current;
												SetFileGlobalHigh(file_global,file_group,file_high);
											}
										DisplayFile(id,group);
									}
								else
									{
										ULONG num=0;
										if(num=GetNextFile(group,atol(s)))
											{
												file_current=num;
												if(file_current>file_high)
													{
														file_high=file_current;
														SetFileGlobalHigh(file_global,file_group,file_high);
													}
												DisplayFile(id,group);
											}
										else
											printf("~s\nUnable to jump to file number %ld.\n",atol(s));
									}
							}
						CloseFileGroup(group);
					}
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void ListFiles(BYTE *id)
	{
		BYTE rows[4];
		strcpy(rows,"");
		if(GetVar("ROWS",rows,3,NULL)!=-1)
			{
				if(strlen(file_area))
					{
						ULONG lowfile=0,highfile=0;
						{
							APTR group;
							if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
								{
									lowfile=GetNextFile(group,0);
									highfile=GetPreviousFile(group,~0);
									CloseFileGroup(group);
								}
						}
						if(lowfile && highfile)
							{
								BYTE buff[21];
								strcpy(buff,"");
								printf("~p\nEnter range of files to list, %ld-%ld, `All' to list all files,\nor `New' to list new files.\n: ",lowfile,highfile);
								if(GetLine(buff,20,NULL))
									{
										if(!stricmp(buff,"ALL"))
											sprintf(buff,"%ld-%ld",lowfile,highfile);
										if(!stricmp(buff,"NEW"))
											sprintf(buff,"%ld-%ld",file_high+1,highfile);
										{
											APTR group;
											if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
												{
													ULONG next=0;
													BOOL first=FALSE;
													int line=0;
													int numwidth;
													numwidth=(int) (log10((double) highfile)+1);
													if(numwidth<3) numwidth=3;
													while(next=GetNextFile(group,next))
														{
															if(IsRange(buff,next))
																{
																	if(WaitForChar(Input(),0))
																		{
																			getchar();
																			break;
																		}
																	if(!first)
																		{
																			printf("\n~h%*sNum File             Size Description%*s\n~o",numwidth-3,"",42-(numwidth-3),"");
																			line++;
																			first=TRUE;
																		}
																	{
																		BYTE *file=NULL,*description=NULL;
																		ULONG size=0;
																		if(GetFileTags(group,FILTAG_ID,next,FILTAG_Name,&file,FILTAG_Size,&size,FILTAG_Description,&description,TAG_END))
																			{
																				BYTE size_buff[5],description_buff[64];
																				strcpy(size_buff,"");
																				strcpy(description_buff,"");
																				{
																					strcpy(size_buff,"");
																					if(size<1000*1024) /* 1 megabyte */
																						sprintf(size_buff,"%dK",size/1024);
																					else
																						{
																							if(size<10*1024*1024) /* 10 megabytes */
																								sprintf(size_buff,"%d.%dM",size/(1024*1024),((size%(1024*1024))*10)/(1024*1024));
																							else
																								{
																									if(size<1000*1024*1024) /* 1 gigabyte */
																										sprintf(size_buff,"%dM",size/1024/1024);
																									else
																										strcpy(size_buff,"BIG");
																								}
																						}
																				}
																				if(description)
																					{
																						BYTE *p;
																						if(p=strchr(description,'\n'))
																							{
																								if(p-description<64)
																									{
																										strncpy(description_buff,description,p-description);
																										description_buff[p-description]=NULL;
																									}
																								else
																									{
																										strncpy(description_buff,description,63);
																										description_buff[63]=NULL;
																									}
																							}
																						else
																							{
																								if(strlen(description)<64)
																									strcpy(description_buff,description);
																								else
																									{
																										strncpy(description_buff,description,63);
																										description_buff[63]=NULL;
																									}
																							}
																					}
																				else
																					strcpy(description_buff,"");
																				printf("%*d %-16.16s %4.4s %-*.*s\n",numwidth,next,file,size_buff,56-numwidth,56-numwidth,description_buff);
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
														printf("~s\nNo files found in specified range.\n");
													CloseFileGroup(group);
												}
										}
									}
							}
						else
							printf("~s\nNo files exist.\n");
					}
				else
					printf("~s\nPlease enter a file area first.\n");
			}
	}

void SearchNext(BYTE *id)
	{
		if(strlen(file_area))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
					{
						BYTE parsed_filepattern[83],parsed_descriptionpattern[83],parsed_uploaderpattern[83];
						ULONG num=0;
						BYTE *file=NULL,*description=NULL,*uploader=NULL;
						ParsePatternNoCase((strlen(file_filepattern)) ? file_filepattern : "#?",parsed_filepattern,82);
						ParsePatternNoCase((strlen(file_descriptionpattern)) ? file_descriptionpattern : "#?",parsed_descriptionpattern,82);
						ParsePatternNoCase((strlen(file_uploaderpattern)) ? file_uploaderpattern : "#?",parsed_uploaderpattern,82);
						while(num=GetNextFile(group,file_current))
							{
								file_current=num;
								if(file_current>file_high)
									{
										file_high=file_current;
										SetFileGlobalHigh(file_global,file_group,file_high);
									}
								if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Name,&file,FILTAG_Description,&description,FILTAG_Uploader,&uploader,TAG_END))
									{
										if(MatchPatternNoCase(parsed_filepattern,file) && MatchPatternNoCase(parsed_descriptionpattern,description) && MatchPatternNoCase(parsed_uploaderpattern,uploader))
											{
												if(!DisplayFile(id,group))
													break;
											}
									}
							}
						CloseFileGroup(group);
					}
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void CatchupFiles(BYTE *id)
	{
		if(strlen(file_area))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
					{
						if(Ask("Catchup files",FALSE))
							{
								ULONG num=0;
								if(num=GetPreviousFile(group,~0))
									{
										file_current=file_high=num;
										SetFileGlobalHigh(file_global,file_group,file_high);
										printf("~s\nAll files marked as read.\n");
									}
								else
									printf("~s\nUnable to catch up files.\n");
							}
						CloseFileGroup(group);
					}
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

BOOL DisplayFile(BYTE *id,APTR group)
	{
		BOOL ret=FALSE;
		BYTE *name=NULL,*uploader=NULL;
		ULONG size=0;
		struct DateStamp *date=NULL;
		UWORD downloads=0;
		struct DateStamp *lastdownloaddate=NULL;
		BYTE *description=NULL;
		if(GetFileTags(group,FILTAG_ID,file_current,
				FILTAG_Name,&name,
				FILTAG_Uploader,&uploader,
				FILTAG_Size,&size,
				FILTAG_Date,&date,
				FILTAG_Downloads,&downloads,
				FILTAG_LastDownloadDate,&lastdownloaddate,
				FILTAG_Description,&description,
				TAG_END))
			{
				printf("\n~h");
				{
					char date_buff[32];
					strcpy(date_buff,"");
					{
						struct DateTime dt;
						char time_buff[32];
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
					printf("[File     ] %-32s  %33.33s\n",name,date_buff);
				}
				printf("[Size     ] %-67ld\n",size);
				printf("[Uploader ] %-67s\n",uploader);
				{
					char buff[32];
					sprintf(buff,"[%ld/%ld]",file_current,GetPreviousFile(group,~0));
					printf("[Downloads] %-5d  %60.60s\n",downloads,buff);
				}
				if(description)
					{
						printf("\n");
						if(Pager(description,5))
							ret=TRUE;
					}
			}
		return(ret);
	}
