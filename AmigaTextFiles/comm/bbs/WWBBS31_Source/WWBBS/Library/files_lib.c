#include "wwbbs.h"
#include "files.h"

struct FileNode *GetFileNode(struct FileGroupNode *gnode,ULONG id)
	{
		struct FileNode *ret=NULL;
		{
			struct FileNode *node;
			for(node=(struct FileNode *) gnode->fgn_Files.lh_Head;node->fn_Node.ln_Succ;node=(struct FileNode *) node->fn_Node.ln_Succ)
				{
					if(node->fn_ID==id)
						{
							ret=node;
							break;
						}
				}
		}
		return(ret);
	}

ULONG GetFileFields(struct FileNode *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,file_tags,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								data=(APTR) tag->ti_Data;
								switch(tag->ti_Tag)
									{
										case FILTAG_Name:
											*((STRPTR *) data)=node->fn_Name;
											break;
										case FILTAG_Uploader:
											*((STRPTR *) data)=node->fn_Uploader;
											break;
										case FILTAG_Size:
											*((ULONG *) data)=node->fn_Size;
											break;
										case FILTAG_Date:
											*((struct DateStamp **) data)=&node->fn_Date;
											break;
										case FILTAG_Downloads:
											*((UWORD *) data)=node->fn_Downloads;
											break;
										case FILTAG_LastDownloadDate:
											*((struct DateStamp **) data)=&node->fn_LastDownloadDate;
											break;
										case FILTAG_Description:
											*((STRPTR *) data)=node->fn_Description;
											break;
									}
								ret++;
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetFileFields(struct FileNode *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,file_tags,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								data=(APTR) tag->ti_Data;
								switch(tag->ti_Tag)
									{
										case FILTAG_Name:
											if(data && strlen((STRPTR) data)<=32)
												strcpy(node->fn_Name,(STRPTR) data);
											else
												retminus++;
											break;
										case FILTAG_Uploader:
											if(data && strlen((STRPTR) data)<=32)
												strcpy(node->fn_Uploader,(STRPTR) data);
											else
												retminus++;
											break;
										case FILTAG_Size:
											node->fn_Size=(ULONG) data;
											break;
										case FILTAG_Date:
											if(data)
												node->fn_Date=*((struct DateStamp *) data);
											else
												retminus++;
											break;
										case FILTAG_Downloads:
											node->fn_Downloads=(UWORD) data;
											break;
										case FILTAG_LastDownloadDate:
											if(data)
												node->fn_LastDownloadDate=*((struct DateStamp *) data);
											else
												retminus++;
											break;
										case FILTAG_Description:
											if(node->fn_Description)
												{
													FreeVec(node->fn_Description);
													node->fn_Description=NULL;
												}
											if(data && (node->fn_Description=AllocVec(strlen((STRPTR) data)+1,MEMF_CLEAR)))
												strcpy(node->fn_Description,(STRPTR) data);
											else
												retminus++;
											break;
									}
								ret++;
							}
					}
				FreeTagItems(tags);
			}
		ret-=retminus;
		return(ret);
	}

BOOL LoadFileGroup(BYTE *groupname,BYTE *directory)
	{
		BOOL ret=FALSE;
		ObtainSemaphore(&FileGroupSemaphore);
		FlushFileGroups();
		{
			BPTR old_dir,lock;
			if(lock=Lock(directory,SHARED_LOCK))
				{
					old_dir=CurrentDir(lock);
					{
						struct FileGroupNode *gnode;
						if(gnode=AllocVec(sizeof(struct FileGroupNode),MEMF_CLEAR))
							{
								InitSemaphore(&gnode->fgn_Semaphore);
								NewList(&gnode->fgn_Files);
								InitSemaphore(&gnode->fgn_UsageCountSemaphore);
								strcpy(gnode->fgn_Name,groupname);
								gnode->fgn_Directory=DupLock(lock);
								gnode->fgn_Node.ln_Name=gnode->fgn_Name;
								{
									BPTR count_fh;
									if(count_fh=Open(".count",MODE_OLDFILE))
										{
											ULONG count;
											if(FRead(count_fh,&count,sizeof(ULONG),1))
												gnode->fgn_FileCount=count;
											Close(count_fh);
										}
								}
								{
									BPTR fh;
									if(fh=Open(".files",MODE_OLDFILE))
										{
											BOOL kg=TRUE;
											LONG cmd;
											struct FileNode *node=NULL;
											while(kg)
												{
													cmd=FGetC(fh);
													switch(cmd)
														{
															case -1: /* EOF */
																kg=FALSE;
																break;
															case FILCMD_Begin:
																if(!(node=AllocVec(sizeof(struct FileNode),MEMF_CLEAR)))
																	kg=FALSE;
																break;
															case FILCMD_End:
																if(node)
																	{
																		AddTail(&gnode->fgn_Files,(struct Node *) node);
																		node=NULL;
																	}
																break;
															case FILCMD_Field:
																if(node)
																	{
																		LONG field;
																		UWORD size;
																		if( ((field=FGetC(fh))!=-1) && (FRead(fh,&size,sizeof(UWORD),1)) )
																			{
																				APTR data;
																				if(data=AllocVec(size,MEMF_CLEAR))
																					{
																						if(FRead(fh,data,size,1))
																							{
																								switch(field)
																									{
																										case FILFLD_ID:
																											node->fn_ID=*((ULONG *) data);
																											break;
																										case FILFLD_Name:
																											strcpy(node->fn_Name,(STRPTR) data);
																											break;
																										case FILFLD_Uploader:
																											strcpy(node->fn_Uploader,(STRPTR) data);
																											break;
																										case FILFLD_Size:
																											node->fn_Size=*((ULONG *) data);
																											break;
																										case FILFLD_Date:
																											node->fn_Date=*((struct DateStamp *) data);
																											break;
																										case FILFLD_Downloads:
																											node->fn_Downloads=*((UWORD *) data);
																											break;
																										case FILFLD_LastDownloadDate:
																											node->fn_LastDownloadDate=*((struct DateStamp *) data);
																											break;
																										case FILFLD_Description:
																											if(node->fn_Description=AllocVec(size,MEMF_CLEAR))
																												strcpy(node->fn_Description,(STRPTR) data);
																											break;
																									}
																							}
																						else
																							kg=FALSE;
																						FreeVec(data);
																					}
																				else
																					kg=FALSE;
																			}
																		else
																			kg=FALSE;
																	}
																break;
														}
												}
											Close(fh);
										}
								}
								AddTail(&FileGroupList,(struct Node *) gnode);
								ret=TRUE;
							}
					}
					CurrentDir(old_dir);
					UnLock(lock);
				}
		}
		ReleaseSemaphore(&FileGroupSemaphore);
		return(ret);
	}

BOOL SaveFileGroup(struct FileGroupNode *gnode)
	{
		BOOL ret=FALSE;
		{
			BPTR old_dir,fh;
			old_dir=CurrentDir(gnode->fgn_Directory);
			if(fh=Open(".files",MODE_NEWFILE))
				{
					struct FileNode *node;
					for(node=(struct FileNode *) gnode->fgn_Files.lh_Head;node->fn_Node.ln_Succ;node=(struct FileNode *) node->fn_Node.ln_Succ)
						{
							FPutC(fh,FILCMD_Begin);
							SaveFileField(fh,FILFLD_ID,&node->fn_ID,sizeof(ULONG));
							SaveFileString(fh,FILFLD_Name,node->fn_Name);
							SaveFileString(fh,FILFLD_Uploader,node->fn_Uploader);
							SaveFileField(fh,FILFLD_Size,&node->fn_Size,sizeof(ULONG));
							SaveFileField(fh,FILFLD_Date,&node->fn_Date,sizeof(struct DateStamp));
							SaveFileField(fh,FILFLD_Downloads,&node->fn_Downloads,sizeof(UWORD));
							SaveFileField(fh,FILFLD_LastDownloadDate,&node->fn_LastDownloadDate,sizeof(struct DateStamp));
							if(node->fn_Description)
								SaveFileString(fh,FILFLD_Description,node->fn_Description);
							FPutC(fh,FILCMD_End);
						}
					{
						BPTR count_fh;
						if(count_fh=Open(".count",MODE_NEWFILE))
							{
								FWrite(count_fh,&gnode->fgn_FileCount,sizeof(ULONG),1);
								Close(count_fh);
								ret=TRUE;
							}
					}
					Close(fh);
				}
			CurrentDir(old_dir);
		}
		return(ret);
	}

void FlushFileGroups()
	{
		struct FileGroupNode *wn,*nn;
		wn=(struct FileGroupNode *) FileGroupList.lh_Head;
		while(nn=(struct FileGroupNode *) wn->fgn_Node.ln_Succ)
			{
				if(!wn->fgn_UsageCount)
					{
						{
							struct FileNode *node;
							while(node=(struct FileNode *) RemHead(&wn->fgn_Files))
								{
									if(node->fn_Description)
										FreeVec(node->fn_Description);
									FreeVec(node);
								}
						}
						UnLock(wn->fgn_Directory);
						Remove((struct Node *) wn);
						FreeVec(wn);
					}
				wn=nn;
			}
	}

void SaveFileField(BPTR fh,LONG field,APTR data,UWORD size)
	{
		FPutC(fh,FILCMD_Field);
		FPutC(fh,field);
		FWrite(fh,&size,sizeof(UWORD),1);
		FWrite(fh,data,(ULONG) size,1);
	}

void SaveFileString(BPTR fh,LONG field,STRPTR data)
	{
		SaveFileField(fh,field,data,strlen(data)+1);
	}
