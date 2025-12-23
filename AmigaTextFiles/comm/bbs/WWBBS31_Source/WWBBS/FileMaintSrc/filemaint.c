#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/wwbbs.h>

#include "filemaint_rev.h"

char *ver=VERSTAG;

struct Library *WorldWideBase;

void close_all(void);
BOOL GetNextArea(BYTE *,BYTE *);
int move_file(char *,char *);

void main(int argc,char **argv)
	{
		if(argc!=2)
			{
				printf("Usage: FileMaint <directory>\n");
				printf("All expired files will be copied to that directory.\n");
				exit(0);
			}
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			close_all();
		{
			struct DateStamp CurrentDate;
			DateStamp(&CurrentDate);
			{
				BYTE path[256],area[33],groupname[21],directory[256];
				ULONG days=0;
				strcpy(path,"FileBases");
				strcpy(area,"");
				strcpy(groupname,"");
				strcpy(directory,"");
				while(GetNextArea(path,area))
					{
						if(GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,area,FBTAG_Days,&days,FBTAG_Group,groupname,FBTAG_Directory,directory,TAG_END))
							{
								if(days)
									{
										APTR group;
										if(group=OpenFileGroup(path,area,EXCLUSIVE_LOCK))
											{
												ULONG count=0;
												ULONG next=0;
												BYTE *name=NULL;
												struct DateStamp *uploaddate=NULL,*lastdownloaddate=NULL;
												while(next=GetNextFile(group,next))
													{
														if(GetFileTags(group,FILTAG_ID,next,FILTAG_Name,&name,FILTAG_Date,&uploaddate,FILTAG_LastDownloadDate,&lastdownloaddate,TAG_END))
															{
																if( (lastdownloaddate->ds_Days && lastdownloaddate->ds_Days+days<CurrentDate.ds_Days) || (!lastdownloaddate->ds_Days && uploaddate->ds_Days+days<CurrentDate.ds_Days) )
																	{
																		BYTE src_file[256],dest_file[256];
																		strcpy(src_file,directory);
																		AddPart(src_file,name,255);
																		strcpy(dest_file,argv[1]);
																		AddPart(dest_file,name,255);
																		if(move_file(src_file,dest_file))
																			{
																				if(RemFileTags(group,FILTAG_ID,next,FILTAG_DontSave,TRUE,TAG_END))
																					count++;
																			}
																	}
															}
													}
												if(count)
													SetFileTags(group,FILTAG_ForceSave,TRUE,TAG_END);
												CloseFileGroup(group);
												if(count)
													{
														char s[64];
														sprintf(s,"`%ld' files moved from %s.",count,groupname);
														LogEntry(NULL,"File Maintenance",s);
													}
											}
									}
							}
					}
			}
		}
		close_all();
	}

void close_all()
	{
		if(WorldWideBase) CloseLibrary(WorldWideBase);
		exit(RETURN_OK);
	}

BOOL GetNextArea(BYTE *path,BYTE *name)
	{
		BOOL ret=FALSE;
		if(strlen(name))
			{
				BYTE next[33];
				strcpy(next,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_GetNext,next,TAG_END);
				if(strlen(next))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,next,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,next,255);
								strcpy(name,"");
								ret=GetNextArea(path,name);
							}
						else
							{
								strcpy(name,next);
								ret=TRUE;
							}
					}
				else
					{
						if(strchr(path,'/'))
							{
								char *p;
								strcpy(name,FilePart(path));
								p=PathPart(path);
								*p=NULL;
								ret=GetNextArea(path,name);
							}
					}
			}
		else
			{
				BYTE first[33];
				strcpy(first,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_GetFirst,first,TAG_END);
				if(strlen(first))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,first,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,first,255);
								ret=GetNextArea(path,name);
							}
						else
							{
								strcpy(name,first);
								ret=TRUE;
							}
					}
			}
		return(ret);
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
