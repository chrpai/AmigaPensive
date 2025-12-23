#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/exall.h>
#include <libraries/asl.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/asl.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/rexxsyslib.h>
#include <proto/wwbbs.h>

#include "rx.h"

#include "filebase.h"

int move_file(char *,char *);
void GetFileDescriptionsFromCommentField(BYTE *);

void UploadFromDisk(BYTE *id)
	{
		BOOL local=FALSE;
		GetStatusTags(STTAG_Name,id,STTAG_Local,&local,TAG_END);
		if(strlen(file_area))
			{
				ULONG dsr=0,dfs=0;
				BYTE directory[256];
				BOOL readonly=FALSE;
				if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,directory,FBTAG_DiskSpaceRequired,&dsr,FBTAG_ReadOnly,&readonly,TAG_END))
					{
						if(!readonly)
							{
								{
									BPTR lock;
									if(lock=Lock(directory,SHARED_LOCK))
										{
											struct InfoData id;
											if(Info(lock,&id))
												dfs=((id.id_NumBlocks-id.id_NumBlocksUsed)*id.id_BytesPerBlock)/1024;
											UnLock(lock);
										}
								}
								if(!dsr || dfs>=dsr)
									{
										FILE *fp;
										BYTE file[64];
										BOOL ok=FALSE;
										sprintf(file,"T:wwbbs_filelog.%s",id);
										if(fp=fopen(file,"w"))
											{
												if(local)
													{
														struct FileRequester *fr;
														BYTE pubscreen_buff[64];
														sprintf(pubscreen_buff,"WWBBS Console.%s",id);
														if(fr=(struct FileRequester *) AllocAslRequestTags(ASL_FileRequest,ASLFR_PubScreenName,pubscreen_buff,ASLFR_TitleText,"Select File(s) for Upload",ASLFR_InitialDrawer,directory,ASLFR_InitialPattern,"~(#?.info)",ASLFR_DoMultiSelect,TRUE,ASLFR_DoPatterns,TRUE,TAG_END))
															{
																if(AslRequestTags(fr,TAG_END))
																	{
																		if(fr->fr_NumArgs)
																			{
																				struct WBArg *frargs;
																				int i;
																				frargs=fr->fr_ArgList;
																				for(i=0;i<fr->fr_NumArgs;i++)
																					{
																						char src[256],dest[256];
																						strcpy(src,fr->fr_Drawer);
																						AddPart(src,frargs[i].wa_Name,255);
																						strcpy(dest,directory);
																						AddPart(dest,frargs[i].wa_Name,255);
																						if(move_file(src,dest))
																							{
																								BPTR fh;
																								if(fh=Open(dest,MODE_OLDFILE))
																									{
																										__aligned struct FileInfoBlock fib;
																										if(ExamineFH(fh,&fib))
																											{
																												fprintf(fp,"R %ld 0 bps 0 cps 0 errors 0 0 %s 0\n",fib.fib_Size,dest);
																												ok=TRUE;
																											}
																										Close(fh);
																									}
																								else
																									move_file(dest,src);
																							}
																					}
																			}
																		else
																			{
																				if(strlen(fr->fr_File))
																					{
																						char src[256],dest[256];
																						strcpy(src,fr->fr_Drawer);
																						AddPart(src,fr->fr_File,255);
																						strcpy(dest,directory);
																						AddPart(dest,fr->fr_File,255);
																						if(move_file(src,dest))
																							{
																								BPTR fh;
																								if(fh=Open(dest,MODE_OLDFILE))
																									{
																										__aligned struct FileInfoBlock fib;
																										if(ExamineFH(fh,&fib))
																											{
																												fprintf(fp,"R %ld 0 bps 0 cps 0 errors 0 0 %s 0\n",fib.fib_Size,dest);
																												ok=TRUE;
																											}
																										Close(fh);
																									}
																								else
																									move_file(dest,src);
																							}
																					}
																			}
																	}
																FreeAslRequest(fr);
															}
													}
												else
													{
														char patternbuff[128],pattern[256],path[256],file[256];
														printf("~p\nEnter an AmigaDOS style file pattern to upload from disk.\nFile names are also acceptable.\n: ");
														if(GetLine(patternbuff,127,NULL))
															{
																strcpy(path,patternbuff);
																{
																	char *p;
																	p=PathPart(path);
																	*p=NULL;
																}
																strcpy(file,FilePart(patternbuff));
																ParsePatternNoCase(file,pattern,255);
																{
																	BPTR lock;
																	if(lock=Lock(path,SHARED_LOCK))
																		{
																			struct ExAllControl *eac;
																			struct ExAllData *ead,*eadata;
																			int more;
																			if(eac=AllocDosObject(DOS_EXALLCONTROL,NULL))
																				{
																					eac->eac_LastKey=0;
																					if(eadata=AllocVec(sizeof(struct ExAllData)*16,MEMF_CLEAR))
																						{
																							do
																								{
																									more=ExAll(lock,eadata,sizeof(struct ExAllData)*16,ED_SIZE,eac);
																									if((!more) && (IoErr()!=ERROR_NO_MORE_ENTRIES))
																										break;
																									if(eac->eac_Entries==0)
																										continue;
																									{
																										ead=eadata;
																										do
																											{
																												{
																													if(MatchPatternNoCase(pattern,ead->ed_Name))
																														{
																															char src[256],dest[256];
																															strcpy(src,path);
																															AddPart(src,ead->ed_Name,255);
																															strcpy(dest,directory);
																															AddPart(dest,ead->ed_Name,255);
																															if(move_file(src,dest))
																																fprintf(fp,"R %ld 0 bps 0 cps 0 errors 0 0 %s 0\n",ead->ed_Size,dest);
																														}
																												}
																												ead=ead->ed_Next;
																											} while(ead);
																									}
																								} while(more);
																							FreeVec(eadata);
																						}
																					FreeDosObject(DOS_EXALLCONTROL,eac);
																				}
																			UnLock(lock);
																		}
																}
																ok=TRUE;
															}
													}
												fclose(fp);
												if(ok)
													{
														ProcessLogFile(id);
														GetFileDescriptionsFromCommentField(id);
														GetFileDescriptions(id);
													}
												DeleteFile(file);
											}
									}
								else
									printf("~s\nThere is not enough disk space for uploads.\n");
							}
						else
							printf("~s\nThis file area is read only.\n");
					}
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void KillFile(BYTE *id)
	{
		if(strlen(file_area))
			{
				if(file_current)
					{
						BYTE file[256];
						strcpy(file,"");
						if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,file,TAG_END))
							{
								APTR group;
								if(group=OpenFileGroup(file_path,file_area,EXCLUSIVE_LOCK))
									{
										BYTE *name=NULL,*uploader=NULL;
										ULONG size=0;
										if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Name,&name,FILTAG_Size,&size,FILTAG_Uploader,&uploader,TAG_END))
											{
												AddPart(file,name,255);
												printf("~p\nKill `%s' (Y,[N])? ",name);
												if(Ask(NULL,FALSE))
													{
														printf("~p\nRemove credits from `%s' ([Y],N)? ",uploader);
														if(Ask(NULL,TRUE))
															{
																UBYTE ratiobyte=0,ratiofile=0;
																ULONG filesupld=0,filescredits=0,kilosupld,kiloscredits=0;
																if(GetUserTags(USRTAG_Name,uploader,USRTAG_RatioByte,&ratiobyte,USRTAG_RatioFile,&ratiofile,USRTAG_FilesUploaded,&filesupld,USRTAG_FilesCredits,&filescredits,USRTAG_KilosUploaded,&kilosupld,USRTAG_KilosCredits,&kiloscredits,TAG_END))
																	{
																		if(filesupld)
																			filesupld--;
																		if(ratiofile && filescredits)
																			filescredits--;
																		kilosupld=(kilosupld>size/1024) ? kilosupld-(size/1024) : 0;
																		if(ratiobyte)
																			kiloscredits=(kiloscredits>size/1024) ? kiloscredits-(size/1024) : 0;
																		SetUserTags(USRTAG_Name,uploader,USRTAG_FilesUploaded,filesupld,USRTAG_FilesCredits,filescredits,USRTAG_KilosUploaded,kilosupld,USRTAG_KilosCredits,kiloscredits,TAG_END);
																	}
															}
														printf("~p\nDelete `%s' from disk ([Y],N)? ",name);
														if(Ask(NULL,TRUE))
															DeleteFile(file);
														if(RemFileTags(group,FILTAG_ID,file_current,TAG_END))
															{
																file_current=GetPreviousFile(group,file_current);
																file_promptlow=GetNextFile(group,0);
																file_prompthigh=GetPreviousFile(group,~0);
																if(file_current>file_prompthigh)
																	file_current=file_prompthigh;
																if(file_high>file_prompthigh)
																	file_high=file_prompthigh;
																{
																	BYTE file_group[41];
																	GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Group,file_group,TAG_END);
																	SetFileGlobalHigh(file_global,file_group,file_high);
																}
															}
														else
															printf("~s\nUnable to kill `%s'.\n",name);
													}
											}
										CloseFileGroup(group);
									}
							}
					}
				else
					printf("~s\nCurrent file does not exist.\n");
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void MoveFile(BYTE *id)
	{
		if(strlen(file_area))
			{
				if(file_current)
					{
						BOOL isgetfile=FALSE;
						BYTE file_path_dest[256],*file_area_dest=NULL;
						BYTE file_src[256],file_dest[256];
						BYTE name[33],uploader[33],*description=NULL;
						ULONG size=0;
						struct DateStamp date,lastdownloaddate;
						UWORD downloads=0;
						strcpy(file_path_dest,"FileBases/");
						strcpy(file_src,"");
						strcpy(file_dest,"");
						{
							APTR group_src;
							if(group_src=OpenFileGroup(file_path,file_area,EXCLUSIVE_LOCK))
								{
									if(GetFileGroupDirectory(group_src,file_src,255))
										{
											BYTE *name_buff=NULL,*uploader_buff=NULL,*description_buff=NULL;
											struct DateStamp *date_buff=NULL,*lastdownloaddate_buff=NULL;
											if(GetFileTags(group_src,FILTAG_ID,file_current,FILTAG_Name,&name_buff,FILTAG_Uploader,&uploader_buff,FILTAG_Size,&size,FILTAG_Date,&date_buff,FILTAG_Downloads,&downloads,FILTAG_LastDownloadDate,&lastdownloaddate_buff,FILTAG_Description,&description_buff,TAG_END))
												{
													strcpy(name,name_buff);
													strcpy(uploader,uploader_buff);
													date=*date_buff;
													lastdownloaddate=*lastdownloaddate_buff;
													if(description_buff && strlen(description_buff))
														{
															if(description=AllocVec(sizeof(BYTE)*(strlen(description_buff)+1),MEMF_CLEAR))
																strcpy(description,description_buff);
														}
													AddPart(file_src,name,255);
													printf("~p\nEnter the full path of the file base to move `%s' to.\n: ",name);
													if(GetLine(&file_path_dest[strlen(file_path_dest)],255-strlen(file_path_dest),NULL))
														{
															if(file_area_dest=FilePart(file_path_dest))
																{
																	{
																		char *p;
																		p=file_area_dest;
																		p--;
																		*p=NULL;
																	}
																	if(!(!stricmp(file_path_dest,file_path) && !stricmp(file_area_dest,file_area)))
																		{
																			if(RemFileTags(group_src,FILTAG_ID,file_current,TAG_END))
																				{
																					file_current=GetPreviousFile(group_src,file_current);
																					file_promptlow=GetNextFile(group_src,0);
																					file_prompthigh=GetPreviousFile(group_src,~0);
																					if(file_current>file_prompthigh)
																						file_current=file_prompthigh;
																					if(file_high>file_prompthigh)
																						file_high=file_prompthigh;
																					{
																						BYTE file_group[41];
																						strcpy(file_group,"");
																						GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Group,file_group,TAG_END);
																						SetFileGlobalHigh(file_global,file_group,file_high);
																					}
																					isgetfile=TRUE;
																				}
																			else
																				printf("~s\nUnable to remove source file.\n");
																		}
																	else
																		printf("~s\n`%s' is already in `%s/%s'.\n",name,file_path_dest,file_area_dest);
																}
														}
												}
											else
												printf("~s\nUnable to get file.\n");
										}
									else
										printf("~s\nUnable to get directory.\n");
									CloseFileGroup(group_src);
								}
							else
								printf("~s\nUnable to open `%s/%s'.\n",file_path,file_area);
						}
						if(isgetfile)
							{
								APTR group_dest;
								if(group_dest=OpenFileGroup(file_path_dest,file_area_dest,EXCLUSIVE_LOCK))
									{
										if(GetFileGroupDirectory(group_dest,file_dest,255))
											{
												AddPart(file_dest,name,255);
												if(move_file(file_src,file_dest))
													{
														ULONG dest_id;
														if(dest_id=AddFileTags(group_dest,FILTAG_DontSave,TRUE,TAG_END))
															{
																if(!SetFileTags(group_dest,FILTAG_ID,dest_id,FILTAG_Name,name,FILTAG_Uploader,uploader,FILTAG_Size,size,FILTAG_Date,&date,FILTAG_Downloads,downloads,FILTAG_LastDownloadDate,&lastdownloaddate,FILTAG_Description,description,TAG_END))
																	{
																		RemFileTags(group_dest,FILTAG_ID,dest_id,TAG_END);
																		move_file(file_dest,file_src);
																		printf("~s\nUnable to set destination file attributes.\n");
																	}
															}
														else
															{
																move_file(file_dest,file_src);
																printf("~s\nUnable to add file to destination group.\n");
															}
													}
												else
													printf("~s\nUnable to move file.\n");
											}
										else
											printf("~s\nUnable to get directory.\n");
										CloseFileGroup(group_dest);
									}
								else
									printf("~s\nUnable to open `%s/%s'.\n",file_path_dest,file_area_dest);
							}
						if(description)
							FreeVec(description);
					}
				else
					printf("~s\nCurrent file does not exist.\n");
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

int move_file(char *src,char *dest)
	{
		int exists=FALSE,samelock=FALSE,ret=FALSE;
		{
			BPTR fh;
			if(fh=Open(dest,MODE_OLDFILE))
				{
					exists=TRUE;
					Close(fh);
				}
		}
		if(!exists)
			{
				if(Rename(src,dest))
					ret=TRUE;
				else
					{
						BPTR src_fh,dest_fh;
						int ok=FALSE;
						if(src_fh=Open(src,MODE_OLDFILE))
							{
								if(dest_fh=Open(dest,MODE_NEWFILE))
									{
										char *p;
										if(p=AllocMem(sizeof(char)*32768,MEMF_CLEAR))
											{
												LONG count;
												while((count=Read(src_fh,p,sizeof(char)*32768))>0)
													Write(dest_fh,p,count);
												ok=TRUE;
												FreeMem(p,sizeof(char)*32768);
											}
										Close(dest_fh);
									}
								Close(src_fh);
							}
						if(ok)
							{
								DeleteFile(src);
								ret=TRUE;
							}
					}
				if(!ret)
					printf("~s\nUnable to move `%s'.\n",src);
			}
		else
			{
				{
					BPTR src_lock,dest_lock;
					if(src_lock=Lock(src,SHARED_LOCK))
						{
							if(dest_lock=Lock(dest,SHARED_LOCK))
								{
									if(SameLock(src_lock,dest_lock)==LOCK_SAME)
										samelock=TRUE;
									UnLock(dest_lock);
								}
							UnLock(src_lock);
						}
				}
				if(samelock)
					ret=TRUE;
				else
					printf("~s\n`%s' already exists.\n",dest);
			}
		return(ret);
	}

void EditFileDescription(BYTE *id)
	{
		if(strlen(file_area))
			{
				if(file_current)
					{
						APTR group;
						if(group=OpenFileGroup(file_path,file_area,EXCLUSIVE_LOCK))
							{
								BYTE *name=NULL,*description=NULL;
								if(GetFileTags(group,FILTAG_ID,file_current,FILTAG_Name,&name,FILTAG_Description,&description,TAG_END))
									{
										BYTE file[128];
										sprintf(file,"T:%s.%s",name,id);
										if(description)
											{
												FILE *fp;
												if(fp=fopen(file,"w"))
													{
														fputs(description,fp);
														fclose(fp);
													}
											}
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
																				SetFileTags(group,FILTAG_ID,file_current,FILTAG_Description,buff,TAG_END);
																			FreeVec(buff);
																		}
																}
															Close(fh);
														}
												}
											}
										DeleteFile(file);
									}
								CloseFileGroup(group);
							}
					}
				else
					printf("~s\nCurrent file does not exist.\n");
			}
		else
			printf("~s\nPlease enter a file area first.\n");
	}

void GetFileDescriptionsFromCommentField(BYTE *id)
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
										if(!description && !stricmp(uploader,user))
											{
												BYTE filename[256];
												strcpy(filename,"");
												if(GetFileGroupDirectory(group,filename,255))
													{
														if(AddPart(filename,name,255))
															{
																BPTR fh;
																if(fh=Open(filename,MODE_OLDFILE))
																	{
																		__aligned struct FileInfoBlock fib;
																		if(ExamineFH(fh,&fib))
																			{
																				if(strlen(fib.fib_Comment))
																					{
																						printf("~o\n`%s' has the following file comment:\n%s\n",name,fib.fib_Comment);
																						if(Ask("Would you like to use it",TRUE))
																							{
																								char description[81];
																								sprintf(description,"%s\n",fib.fib_Comment);
																								SetFileTags(group,FILTAG_ID,next,FILTAG_Description,description,FILTAG_DontSave,TRUE,TAG_END);
																							}
																					}
																			}
																		Close(fh);
																	}
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
