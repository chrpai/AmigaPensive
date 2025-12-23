#include "wwbbs.h"
#include "files.h"

__asm
APTR OpenFileGroup(register __a0 BYTE *path,register __a1 BYTE *name,register __d0 LONG type)
	{
		APTR ret=NULL;
		BYTE group[41],directory[256];
		strcpy(group,"");
		strcpy(directory,"");
		if(GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,FBTAG_Group,group,FBTAG_Directory,directory,TAG_END))
			{
				if(strlen(group) && strlen(directory))
					{
						struct FileGroupNode *gnode;
						{
							BOOL exists=FALSE;
							ObtainSemaphoreShared(&FileGroupSemaphore);
							if(FindName(&FileGroupList,group))
								exists=TRUE;
							ReleaseSemaphore(&FileGroupSemaphore);
							if(!exists)
								LoadFileGroup(group,directory);
						}
						ObtainSemaphoreShared(&FileGroupSemaphore);
						if(gnode=(struct FileGroupNode *) FindName(&FileGroupList,group))
							{
								if(type==EXCLUSIVE_LOCK)
									ObtainSemaphore(&gnode->fgn_Semaphore);
								else
									ObtainSemaphoreShared(&gnode->fgn_Semaphore);
								ObtainSemaphore(&gnode->fgn_UsageCountSemaphore);
								gnode->fgn_UsageCount++;
								ReleaseSemaphore(&gnode->fgn_UsageCountSemaphore);
								ret=gnode;
							}
						else
							ReleaseSemaphore(&FileGroupSemaphore);
					}
			}
		return(ret);
	}

__asm
void CloseFileGroup(register __a0 struct FileGroupNode *gnode)
	{
		ObtainSemaphore(&gnode->fgn_UsageCountSemaphore);
		gnode->fgn_UsageCount--;
		ReleaseSemaphore(&gnode->fgn_UsageCountSemaphore);
		ReleaseSemaphore(&gnode->fgn_Semaphore);
		ReleaseSemaphore(&FileGroupSemaphore);
	}

__asm
BOOL GetFileGroupDirectory(register __a0 struct FileGroupNode *gnode,register __a1 BYTE *buffer,register __d0 LONG len)
	{
		return((BOOL) ((NameFromLock(gnode->fgn_Directory,buffer,len)) ? TRUE : FALSE));
	}

__asm
ULONG GetFile(register __a0 struct FileGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		ULONG id=0;
		BOOL exists=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,file_get_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case FILTAG_ID:
												id=(ULONG) data;
												break;
											case FILTAG_Exists:
												exists=(BOOL) data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			if(id)
				{
					struct FileNode *node=NULL;
					if(node=GetFileNode(gnode,id))
						{
							if(exists) ret++;
							ret+=GetFileFields(node,tags_orig);
						}
				}
		}
		return(ret);
	}

ULONG GetFileTags(struct FileGroupNode *gnode,Tag tag,...)
	{
		return(GetFile(gnode,(struct TagItem *) &tag));
	}

__asm
ULONG SetFile(register __a0 struct FileGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		ULONG id=0;
		BOOL forcesave=FALSE,dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,file_set_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case FILTAG_ID:
												id=(ULONG) data;
												break;
											case FILTAG_ForceSave:
												forcesave=(BOOL) data;
												break;
											case FILTAG_DontSave:
												dontsave=(BOOL) data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			if(id)
				{
					struct FileNode *node=NULL;
					if(node=GetFileNode(gnode,id))
						ret+=SetFileFields(node,tags_orig);
				}
		}
		if((ret && !dontsave) || forcesave)
			SaveFileGroup(gnode);
		return(ret);
	}

ULONG SetFileTags(struct FileGroupNode *gnode,Tag tag,...)
	{
		return(SetFile(gnode,(struct TagItem *) &tag));
	}

__asm
ULONG AddFile(register __a0 struct FileGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,file_add_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case FILTAG_DontSave:
												dontsave=(BOOL) data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			struct FileNode *node;
			if(node=AllocVec(sizeof(struct FileNode),MEMF_CLEAR))
				{
					node->fn_ID=gnode->fgn_FileCount+1;
					DateStamp(&node->fn_Date);
					AddTail(&gnode->fgn_Files,(struct Node *) node);
					gnode->fgn_FileCount++;
					ret=node->fn_ID;
				}
		}
		if(ret && !dontsave)
			SaveFileGroup(gnode);
		return(ret);
	}

ULONG AddFileTags(struct FileGroupNode *gnode,Tag tag,...)
	{
		return(AddFile(gnode,(struct TagItem *) &tag));
	}

__asm
BOOL RemFile(register __a0 struct FileGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		ULONG id=0;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,file_rem_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case FILTAG_ID:
												id=(ULONG) data;
												break;
											case FILTAG_DontSave:
												dontsave=(BOOL) data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			struct FileNode *node;
			if(node=GetFileNode(gnode,id))
				{
					Remove((struct Node *) node);
					FreeVec(node);
					ret=TRUE;
				}
		}
		if(ret && !dontsave)
			SaveFileGroup(gnode);
		return(ret);
	}

BOOL RemFileTags(struct FileGroupNode *gnode,Tag tag,...)
	{
		return(RemFile(gnode,(struct TagItem *) &tag));
	}

__asm
ULONG GetNextFile(register __a0 struct FileGroupNode *gnode,register __d0 ULONG id)
	{
		ULONG ret=0;
		struct FileNode *node;
		if(node=GetFileNode(gnode,id))
			{
				node=(struct FileNode *) node->fn_Node.ln_Succ;
				if(node->fn_Node.ln_Succ)
					ret=node->fn_ID;
			}
		else
			{
				for(node=(struct FileNode *) gnode->fgn_Files.lh_Head;node->fn_Node.ln_Succ;node=(struct FileNode *) node->fn_Node.ln_Succ)
					{
						if(node->fn_ID>id)
							{
								ret=node->fn_ID;
								break;
							}
					}
			}
		return(ret);
	}

__asm
ULONG GetPreviousFile(register __a0 struct FileGroupNode *gnode,register __d0 ULONG id)
	{
		ULONG ret=0;
		struct FileNode *node;
		if(node=GetFileNode(gnode,id))
			{
				node=(struct FileNode *) node->fn_Node.ln_Pred;
				if(node->fn_Node.ln_Pred)
					ret=node->fn_ID;
			}
		else
			{
				for(node=(struct FileNode *) gnode->fgn_Files.lh_TailPred;node->fn_Node.ln_Pred;node=(struct FileNode *) node->fn_Node.ln_Pred)
					{
						if(node->fn_ID<id)
							{
								ret=node->fn_ID;
								break;
							}
					}
			}
		return(ret);
	}
