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

void JoinFileArea(BYTE *id)
	{
		if(file_global)
			{
				if(strlen(file_area))
					{
						if(strlen(file_group))
							{
								if(FindName(file_global,file_group))
									{
										if(DeleteFileGlobal(file_global,file_group))
											printf("~s\nFile area `%s' removed from global list.\n",file_fullpath);
										else
											printf("~s\nUnable to remove file area `%s' from global list.\n",file_fullpath);
									}
								else
									{
										if(AddFileGlobal(file_global,file_group,0))
											printf("~s\nFile area `%s' added to global list.\n",file_fullpath);
										else
											printf("~s\nUnable to add file area `%s' from global list.\n",file_fullpath);
									}
							}
					}
				else
					printf("~s\nPlease enter a file area first.\n");
			}
	}

void ChangeFileArea(BYTE *id)
	{
		if(file_global)
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
								if(SelectFileArea(id,accesslevel,area))
									{
										BYTE group[41];
										BOOL child=FALSE;
										strcpy(group,"");
										if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,area,CFGTAG_Child,&child,FBTAG_Group,group,TAG_END))
											{
												if(child)
													{
														AddPart(file_path,area,255);
														strcpy(file_area,"");
														strcpy(file_group,"");
														file_current=0;
														file_high=0;
														{
															char *p;
															if(p=strchr(file_path,'/'))
																{
																	p++;
																	sprintf(file_fullpath,"%s/None",p);
																}
															else
																strcpy(file_fullpath,"None");
															printf("~s\nCurrent directory is `%s'.\n",(p) ? p : "None");
														}
													}
												else
													{
														strcpy(file_area,area);
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
														printf("\n");
														{
															BOOL desc=FALSE;
															APTR _group;
															if(_group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
																{
																	desc=DisplayArea(id,_group);
																	CloseFileGroup(_group);
																}
															if(desc)
																GetFileDescriptions(id);
														}
													}
											}
									}
							}
					}
			}
	}

void ParentFileArea(BYTE *id)
	{
		BOOL newarea=FALSE;
		if(strlen(file_area))
			{
				strcpy(file_area,"");
				newarea=TRUE;
			}
		else
			{
				if(strchr(file_path,'/'))
					{
						{
							char *p;
							p=PathPart(file_path);
							*p=NULL;
						}
						newarea=TRUE;
					}
			}
		if(newarea)
			{
				file_current=0;
				file_high=0;
				{
					char *p;
					if(p=strchr(file_path,'/'))
						{
							p++;
							sprintf(file_fullpath,"%s/None",p);
						}
					else
						strcpy(file_fullpath,"None");
					printf("~s\nCurrent directory is `%s'.\n",(p) ? p : "None");
				}
			}
		else
			printf("~s\nParent area does not exist.\n");
	}

void NextFileArea(BYTE *id)
	{
		if(file_global)
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
								BYTE group[41];
								strcpy(temp_path,file_path);
								strcpy(temp_area,file_area);
								strcpy(accessrange,"");
								strcpy(group,"");
								while(kg && GetNextArea(temp_path,temp_area))
									{
										if(GetConfigTags(CFGTAG_Path,temp_path,CFGTAG_Name,temp_area,FBTAG_AccessRange,accessrange,FBTAG_Group,group,TAG_END))
											{
												if(IsRange(accessrange,accesslevel))
													{
														if(FindName(file_global,group))
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
																printf("\n");
																{
																	BOOL desc=FALSE;
																	APTR _group;
																	if(_group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
																		{
																			desc=DisplayArea(id,_group);
																			CloseFileGroup(_group);
																		}
																	if(desc)
																		GetFileDescriptions(id);
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

void PreviousFileArea(BYTE *id)
	{
		if(file_global)
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
								BYTE group[41];
								strcpy(temp_path,file_path);
								strcpy(temp_area,file_area);
								strcpy(accessrange,"");
								strcpy(group,"");
								while(kg && GetPreviousArea(temp_path,temp_area))
									{
										if(GetConfigTags(CFGTAG_Path,temp_path,CFGTAG_Name,temp_area,FBTAG_AccessRange,accessrange,FBTAG_Group,group,TAG_END))
											{
												if(IsRange(accessrange,accesslevel))
													{
														if(FindName(file_global,group))
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
																printf("\n");
																{
																	BOOL desc=FALSE;
																	APTR _group;
																	if(_group=OpenFileGroup(file_path,file_area,SHARED_LOCK))
																		{
																			desc=DisplayArea(id,_group);
																			CloseFileGroup(_group);
																		}
																	if(desc)
																		GetFileDescriptions(id);
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

BOOL DisplayArea(BYTE *id,APTR group)
	{
		BOOL ret=FALSE;
		BYTE user[33];
		strcpy(user,"");
		GetStatusTags(STTAG_Name,id,STTAG_UserName,user,TAG_END);
		if(strlen(user))
			{
				ULONG numfiles=0,newfiles=0;
				file_promptlow=GetNextFile(group,0);
				file_prompthigh=GetPreviousFile(group,~0);
				{
					ULONG next=0;
					BYTE *uploader=NULL;
					BYTE *description=NULL;
					while(next=GetNextFile(group,next))
						{
							if(GetFileTags(group,FILTAG_ID,next,FILTAG_Uploader,&uploader,FILTAG_Description,&description,TAG_END))
								{
									if(!description)
										{
											if(!stricmp(uploader,user))
												ret=TRUE;
										}
									numfiles++;
									if(next>file_high)
										newfiles++;
								}
						}
				}
				printf("~hFile Area: %s\n\n",file_fullpath);
				{
					BYTE filename[256];
					strcpy(filename,"");
					if(GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,FBTAG_Directory,filename,TAG_END))
						{
							AddPart(filename,".description",255);
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
				}
				printf("~o");
				printf("Files Numbered: %ld-%ld\n",file_promptlow,file_prompthigh);
				printf("New Files:      %ld\n",newfiles);
				printf("Total Files:    %ld\n",numfiles);
				SetFileDoorVariables(id);
			}
		return(ret);
	}

void SetFileDoorVariables(BYTE *id)
	{
		BYTE buff[11];
		BYTE accessrange[21],directory[256];
		ULONG diskspacerequired=0,days=0;
		BOOL readonly=FALSE,autojoin=FALSE;
		strcpy(accessrange,"");
		strcpy(directory,"");
		GetConfigTags(CFGTAG_Path,file_path,CFGTAG_Name,file_area,
										FBTAG_AccessRange,accessrange,
										FBTAG_Directory,directory,
										FBTAG_DiskSpaceRequired,&diskspacerequired,
										FBTAG_Days,&days,
										FBTAG_ReadOnly,&readonly,
										FBTAG_AutoJoin,&autojoin,
										TAG_END);
		SetVar("FILEBASE_PATH",file_path,-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_AREA",file_area,-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_ACCESSRANGE",accessrange,-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_GROUP",file_group,-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_DIRECTORY",directory,-1,GVF_LOCAL_ONLY);
		sprintf(buff,"%ld",diskspacerequired);
		SetVar("FILEBASE_DISKSPACEREQUIRED",buff,-1,GVF_LOCAL_ONLY);
		sprintf(buff,"%ld",days);
		SetVar("FILEBASE_DAYS",buff,-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_READONLY",(readonly) ? "1" : "0",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_AUTOJOIN",(autojoin) ? "1" : "0",-1,GVF_LOCAL_ONLY);
	}
