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

void FileBaseCommand(BYTE *id,UWORD cmd,BYTE *arg)
	{
		BOOL updatevars=TRUE;
		switch(cmd)
			{
				case FILE_InitGlobal:
					InitFileGlobal(id);
					break;
				case FILE_EditSearchPatterns:
					EditFileSearchPatterns(id);
					break;
				case FILE_JoinArea:
					JoinFileArea(id);
					break;
				case FILE_ChangeArea:
					ChangeFileArea(id);
					updatevars=FALSE;
					break;
				case FILE_ParentArea:
					ParentFileArea(id);
					updatevars=FALSE;
					break;
				case FILE_NextArea:
					NextFileArea(id);
					updatevars=FALSE;
					break;
				case FILE_PreviousArea:
					PreviousFileArea(id);
					updatevars=FALSE;
					break;
				case FILE_Next:
					NextFile(id);
					break;
				case FILE_Current:
					CurrentFile(id);
					break;
				case FILE_Previous:
					PreviousFile(id);
					break;
				case FILE_Jump:
					JumptoFile(id);
					break;
				case FILE_List:
					ListFiles(id);
					break;
				case FILE_SearchNext:
					SearchNext(id);
					break;
				case FILE_Catchup:
					CatchupFiles(id);
					break;
				case FILE_Upload:
					Upload(id);
					updatevars=FALSE;
					break;
				case FILE_Download:
					Download(id);
					break;
				case FILE_ClearTagList:
					ClearFileTagList(id);
					break;
				case FILE_EditTagList:
					EditFileTagList(id);
					break;
				case FILE_Tag:
					TagFile(id);
					break;
				case FILE_ViewArchive:
					ViewArchive(id);
					break;
				case FILE_ReadTextFile:
					ReadTextFile(id);
					break;
				case FILE_UploadFromDisk:
					UploadFromDisk(id);
					updatevars=FALSE;
					break;
				case FILE_Kill:
					KillFile(id);
					updatevars=FALSE;
					break;
				case FILE_Move:
					MoveFile(id);
					updatevars=FALSE;
					break;
				case FILE_EditDescription:
					EditFileDescription(id);
					break;
				case FILE_EditAreaDescription:
					{
						if(strlen(file_area))
							{
								BYTE group[21],filename[256];
								strcpy(group,"");
								strcpy(filename,"");
								if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Group,group,FBTAG_Directory,filename,TAG_END))
									{
										AddPart(filename,".description",255);
										printf("~p\nEnter a description for the area `%s'.\n",file_fullpath);
										RunEditor(filename);
									}
							}
						else
							printf("~s\nPlease enter a file area first.\n");
					}
					break;
			}
		if(updatevars)
			FileUpdateVars(id);
	}

void InitFileGlobal(BYTE *id)
	{
		if(file_global)
			{
				BYTE path[256],name[33];
				BYTE user[33];
				UBYTE accesslevel=0;
				BYTE accessrange[21];
				BYTE group[41];
				BOOL autojoin=FALSE;
				strcpy(path,"FileBases");
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
								if(GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,FBTAG_AccessRange,accessrange,FBTAG_Group,group,FBTAG_AutoJoin,&autojoin,TAG_END))
									{
										if(IsRange(accessrange,accesslevel) && autojoin)
											AddFileGlobal(file_global,group,0);
									}
							}
					}
			}
	}

void EditFileSearchPatterns(BYTE *id)
	{
		char *template[]={
			"[1] File:        ",
			"[2] Description: ",
			"[3] Uploader:    ",
			"[4] Reset Patterns",
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
								case 0: p=(strlen(file_filepattern)) ? file_filepattern : "#?"; break;
								case 1: p=(strlen(file_descriptionpattern)) ? file_descriptionpattern : "#?"; break;
								case 2: p=(strlen(file_uploaderpattern)) ? file_uploaderpattern : "#?"; break;
								case 3: p=""; break;
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
				printf("~p\nEdit File Search Patterns: ");
				if(GetLine(s,1,GLFLG_Digits))
					{
						index=atoi(s);
						switch(index)
							{
								case 1:
									printf("~p\nEnter an AmigaDOS style pattern for the file names.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(file_filepattern,s);
									else
										strcpy(file_filepattern,"");
									break;
								case 2:
									printf("~p\nEnter an AmigaDOS style pattern for the file descriptions.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(file_descriptionpattern,s);
									else
										strcpy(file_descriptionpattern,"");
									break;
								case 3:
									printf("~p\nEnter an AmigaDOS style pattern for the uploaders' names.\n: ");
									if(GetLine(s,40,NULL))
										strcpy(file_uploaderpattern,s);
									else
										strcpy(file_uploaderpattern,"");
									break;
								case 4:
									strcpy(file_filepattern,"");
									strcpy(file_descriptionpattern,"");
									strcpy(file_uploaderpattern,"");
									printf("~s\nFile search patterns reset.\n");
									break;
							}
					}
				else
					kg=FALSE;
			}
	}

struct List *OpenFileGlobal(BYTE *id)
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
								AddPart(file,".fileglobal",255);
								if(fp=fopen(file,"r"))
									{
										BYTE buff[256];
										struct FileGlobalNode *node;
										while(fgets(buff,255,fp))
											{
												if(node=AllocVec(sizeof(struct FileGlobalNode),MEMF_CLEAR))
													{
														sscanf(buff,"%s %ld",node->fgn_Name,&node->fgn_High);
														node->fgn_Node.ln_Name=node->fgn_Name;
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

void CloseFileGlobal(struct List *list)
	{
		BYTE file[256];
		if(GetVar("HOME",file,255,NULL)!=-1)
			{
				FILE *fp;
				AddPart(file,".fileglobal",255);
				if(fp=fopen(file,"w"))
					{
						struct FileGlobalNode *node;
						for(node=(struct FileGlobalNode *) list->lh_Head;node->fgn_Node.ln_Succ;node=(struct FileGlobalNode *) node->fgn_Node.ln_Succ)
							fprintf(fp,"%s %ld\n",node->fgn_Name,node->fgn_High);
						fclose(fp);
					}
			}
		{
			struct FileGlobalNode *node;
			while(node=(struct FileGlobalNode *) RemHead(list))
				FreeVec(node);
			FreeVec(list);
		}
	}

BOOL AddFileGlobal(struct List *list,BYTE *name,ULONG high)
	{
		BOOL ret=FALSE;
		struct FileGlobalNode *node;
		if(node=AllocVec(sizeof(struct FileGlobalNode),MEMF_CLEAR))
			{
				strcpy(node->fgn_Name,name);
				node->fgn_High=high;
				node->fgn_Node.ln_Name=node->fgn_Name;
				AddTail(list,(struct Node *) node);
				ret=TRUE;
			}
		return(ret);
	}

BOOL DeleteFileGlobal(struct List *list,BYTE *name)
	{
		BOOL ret=FALSE;
		struct FileGlobalNode *node;
		if(node=(struct FileGlobalNode *) FindName(list,name))
			{
				Remove((struct Node *) node);
				FreeVec(node);
				ret=TRUE;
			}
		return(ret);
	}

ULONG GetFileGlobalHigh(struct List *list,BYTE *name)
	{
		ULONG ret=0;
		struct FileGlobalNode *node;
		if(node=(struct FileGlobalNode *) FindName(list,name))
			ret=node->fgn_High;
		return(ret);
	}

BOOL SetFileGlobalHigh(struct List *list,BYTE *name,ULONG high)
	{
		BOOL ret=FALSE;
		struct FileGlobalNode *node;
		if(node=(struct FileGlobalNode *) FindName(list,name))
			{
				node->fgn_High=high;
				ret=TRUE;
			}
		return(ret);
	}

struct List *OpenFileTagList(BYTE *id)
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
								AddPart(file,".filetaglist",255);
								if(fp=fopen(file,"r"))
									{
										struct FileTagData data;
										struct FileTagNode *node;
										while(fread(&data,sizeof(struct FileTagData),1,fp))
											{
												if(node=AllocVec(sizeof(struct FileTagNode),MEMF_CLEAR))
													{
														strcpy(node->ftn_Path,data.ftd_Path);
														strcpy(node->ftn_Area,data.ftd_Area);
														node->ftn_ID=data.ftd_ID;
														strcpy(node->ftn_Name,data.ftd_Name);
														node->ftn_Size=data.ftd_Size;
														node->ftn_Node.ln_Name=node->ftn_Name;
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

void CloseFileTagList(struct List *list)
	{
		BYTE file[256];
		if(GetVar("HOME",file,255,NULL)!=-1)
			{
				FILE *fp;
				AddPart(file,".filetaglist",255);
				if(fp=fopen(file,"w"))
					{
						struct FileTagNode *node;
						for(node=(struct FileTagNode *) list->lh_Head;node->ftn_Node.ln_Succ;node=(struct FileTagNode *) node->ftn_Node.ln_Succ)
							fwrite(node->ftn_Path,sizeof(struct FileTagData),1,fp);
						fclose(fp);
					}
			}
		{
			struct Node *node;
			while(node=RemHead(list))
				FreeVec(node);
			FreeVec(list);
		}
	}

BOOL AddFileTag(struct List *list,BYTE *path,BYTE *area,ULONG id,BYTE *name,ULONG size)
	{
		BOOL ret=FALSE;
		struct FileTagNode *node;
		if(node=AllocVec(sizeof(struct FileTagNode),MEMF_CLEAR))
			{
				strcpy(node->ftn_Path,path);
				strcpy(node->ftn_Area,area);
				node->ftn_ID=id;
				strcpy(node->ftn_Name,name);
				node->ftn_Size=size;
				node->ftn_Node.ln_Name=node->ftn_Name;
				AddTail(list,(struct Node *) node);
				ret=TRUE;
			}
		return(ret);
	}

BOOL DeleteFileTag(struct List *list,BYTE *name)
	{
		BOOL ret=FALSE;
		struct Node *node;
		if(node=FindName(list,name))
			{
				Remove(node);
				FreeVec(node);
				ret=TRUE;
			}
		return(ret);
	}

BYTE *GetFileTagPath(struct List *list,BYTE *name)
	{
		BYTE *ret=NULL;
		struct FileTagNode *node;
		if(node=(struct FileTagNode *) FindName(list,name))
			ret=node->ftn_Path;
		return(ret);
	}

BYTE *GetFileTagArea(struct List *list,BYTE *name)
	{
		BYTE *ret=NULL;
		struct FileTagNode *node;
		if(node=(struct FileTagNode *) FindName(list,name))
			ret=node->ftn_Area;
		return(ret);
	}

ULONG GetFileTagID(struct List *list,BYTE *name)
	{
		ULONG ret=0;
		struct FileTagNode *node;
		if(node=(struct FileTagNode *) FindName(list,name))
			ret=node->ftn_ID;
		return(ret);
	}

ULONG GetFileTagSize(struct List *list,BYTE *name)
	{
		ULONG ret=NULL;
		struct FileTagNode *node;
		if(node=(struct FileTagNode *) FindName(list,name))
			ret=node->ftn_Size;
		return(ret);
	}

int SelectFileArea(BYTE *id,UBYTE accesslevel,BYTE *result)
	{
		int selected=0,count=0;
		{
			ULONG next=0;
			BYTE accessrange[21];
			strcpy(accessrange,"");
			while(next=GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Next,next,FBTAG_AccessRange,accessrange,TAG_END))
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
					while(next=GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,&name,CFGTAG_Next,next,CFGTAG_Child,&child,FBTAG_AccessRange,accessrange,TAG_END))
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
					printf("~p\nSelect File Area: ");
					if(GetLine(s,(UWORD) (log10((double) count)+1),GLFLG_Digits))
						{
							if(atoi(s)>=1 && atoi(s)<=count)
								{
									ULONG next=0;
									int i=0;
									BYTE *name=NULL;
									BYTE accessrange[21];
									strcpy(accessrange,"");
									while(next=GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,&name,CFGTAG_Next,next,FBTAG_AccessRange,accessrange,TAG_END))
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
			printf("~s\nNo file areas exist.\n");
		return(selected);
	}

void GetFileDescriptions(BYTE *id)
	{
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,EXCLUSIVE_LOCK))
					{
						ULONG next=0;
						BYTE *name=NULL;
						BYTE *uploader=NULL;
						BYTE *description=NULL;
						while(next=GetNextFile(group,next))
							{
								if(GetFileTags(group,FILTAG_ID,next,FILTAG_Name,&name,FILTAG_Uploader,&uploader,FILTAG_Description,&description,TAG_END))
									{
										if(!description)
											{
												if(!stricmp(uploader,user))
													{
														BYTE file[128];
														sprintf(file,"T:%s.%s",name,id);
														printf("~s\nPlease enter a description for `%s'.\n",name);
														if(RunEditor(file))
															{
																BYTE *buff=NULL;
																{
																	BPTR fh;
																	if(fh=Open(file,MODE_OLDFILE))
																		{
																			__aligned struct FileInfoBlock fib;
																			if(ExamineFH(fh,&fib))
																				{
																					if(buff=AllocVec(fib.fib_Size+1,MEMF_CLEAR))
																						{
																							if(FRead(fh,buff,sizeof(BYTE),fib.fib_Size)==fib.fib_Size)
																								SetFileTags(group,FILTAG_ID,next,FILTAG_Description,buff,FILTAG_DontSave,TRUE,TAG_END);
																							FreeVec(buff);
																						}
																				}
																			Close(fh);
																		}
																}
																DeleteFile(file);
															}
													}
											}
									}
							}
						SetFileTags(group,FILTAG_ForceSave,TRUE,TAG_END);
						CloseFileGroup(group);
					}
			}
	}

void ProcessLogFile(BYTE *id)
	{
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				UBYTE ratiofile=0,ratiobyte=0;
				ULONG filesupld,filesdnld,kilosupld,kilosdnld,creditsfile,creditskilo;
				if(GetUserTags(USRTAG_Name,user,
						USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,
						USRTAG_FilesUploaded,&filesupld,USRTAG_FilesDownloaded,&filesdnld,
						USRTAG_KilosUploaded,&kilosupld,USRTAG_KilosDownloaded,&kilosdnld,
						USRTAG_FilesCredits,&creditsfile,USRTAG_KilosCredits,&creditskilo,
						TAG_END))
					{
						FILE *fp;
						BYTE file[64];
						sprintf(file,"T:wwbbs_filelog.%s",id);
						if(fp=fopen(file,"r"))
							{
								char result[256],action,filename[256],*p;
								long bytes;
								while(fgets(result,255,fp))
									{
										/* cut off last data item */
										p=&result[strlen(result)-1];
										while(*p!=' ')
											p--;
										while(*p==' ')
											{
												*p=NULL;
												p--;
											}
										/* read line */
										p=result;
										action=*p;
										p=strchr(p,' '); while(*p==' ') p++; /* bytes */
										sscanf(p,"%ld",&bytes);
										p=strchr(p,' '); while(*p==' ') p++; /* bps */
										p=strchr(p,' '); while(*p==' ') p++; /* `bps' */
										p=strchr(p,' '); while(*p==' ') p++; /* cps */
										p=strchr(p,' '); while(*p==' ') p++; /* `cps' */
										p=strchr(p,' '); while(*p==' ') p++; /* errors */
										p=strchr(p,' '); while(*p==' ') p++; /* `errors' */
										p=strchr(p,' '); while(*p==' ') p++; /* flow */
										p=strchr(p,' '); while(*p==' ') p++; /* last */
										p=strchr(p,' '); while(*p==' ') p++; /* fname */
										strcpy(filename,p);
										/* process */
										switch(action)
											{
												case 'H': /* file is sent */
													/* increment download count */
													{
														BYTE *path=NULL,*area=NULL;
														ULONG id=0;
														struct DateStamp ds;
														path=GetFileTagPath(file_taglist,filename);
														area=GetFileTagArea(file_taglist,filename);
														id=GetFileTagID(file_taglist,filename);
														DateStamp(&ds);
														if(path && area && id)
															{
																APTR group;
																if(group=OpenFileGroup(path,area,EXCLUSIVE_LOCK))
																	{
																		UWORD downloads=0;
																		if(GetFileTags(group,FILTAG_ID,id,FILTAG_Downloads,&downloads,TAG_END))
																			{
																				downloads++;
																				SetFileTags(group,FILTAG_ID,id,FILTAG_Downloads,downloads,FILTAG_LastDownloadDate,&ds,TAG_END);
																			}
																		CloseFileGroup(group);
																	}
															}
														else
															{
																if(file_current)
																	{
																		APTR group;
																		if(group=OpenFileGroup(file_path,file_area,EXCLUSIVE_LOCK))
																			{
																				UWORD downloads=0;
																				if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Downloads,&downloads,TAG_END))
																					{
																						downloads++;
																						SetFileTags(group,FILTAG_ID,file_current,FILTAG_Downloads,downloads,FILTAG_LastDownloadDate,&ds,TAG_END);
																					}
																				CloseFileGroup(group);
																			}
																	}
															}
													}
													/* remove from tag list */
													{
														struct Node *node;
														if(node=FindName(file_taglist,filename))
															{
																Remove(node);
																FreeVec(node);
															}
													}
													filesdnld++;
													kilosdnld+=bytes/1024;
													if(ratiofile)
														creditsfile--;
													if(ratiobyte)
														creditskilo-=bytes/1024;
													break;
												case 'R': /* file is received */
													{
														APTR group;
														if(group=OpenFileGroup(file_path,file_area,EXCLUSIVE_LOCK))
															{
																ULONG num=0;
																if(num=AddFileTags(group,FILTAG_DontSave,TRUE,TAG_END))
																	{
																		SetFileTags(group,
																			FILTAG_ID,num,
																			FILTAG_Name,FilePart(filename),
																			FILTAG_Uploader,user,
																			FILTAG_Size,bytes,
																			TAG_END);
																	}
																file_promptlow=GetNextFile(group,0);
																file_prompthigh=GetPreviousFile(group,~0);
																CloseFileGroup(group);
															}
														filesupld++;
														kilosupld+=bytes/1024;
														if(ratiofile)
															creditsfile+=ratiofile;
														if(ratiobyte)
															creditskilo+=(bytes/1024)*ratiobyte;
													}
													break;
											}
									}
								fclose(fp);
							}
						SetUserTags(USRTAG_Name,user,
							USRTAG_FilesUploaded,filesupld,USRTAG_FilesDownloaded,filesdnld,
							USRTAG_KilosUploaded,kilosupld,USRTAG_KilosDownloaded,kilosdnld,
							USRTAG_FilesCredits,creditsfile,USRTAG_KilosCredits,creditskilo,
							TAG_END);
					}
			}
	}

void FileUpdateVars(BYTE *id)
	{
		file_promptlow=0;
		file_prompthigh=0;
		if(strlen(file_area))
			{
				APTR group;
				if(group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
					{
						file_promptlow=GetNextFile(group,0);
						file_prompthigh=GetPreviousFile(group,~0);
						CloseFileGroup(group);
					}
			}
	}
