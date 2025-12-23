#include "wwbbs.h"
#include "status.h"

__asm
ULONG GetStatus(register __a0 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		APTR name=NULL;
		BOOL exists=FALSE;
		BOOL isnext=FALSE;struct Node *next=NULL;
		ULONG *nextreturn=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,st_get_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case STTAG_Name:
												name=data;
												break;
											case STTAG_Exists:
												exists=(BOOL) data;
												break;
											case STTAG_Next:
												isnext=TRUE;
												next=data;
												break;
											case STTAG_NextReturn:
												nextreturn=data;
												break;
											default:
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			if(isnext)
				{
					struct Node *node=NULL;
					if(next)
						{
							/* next */
							struct Node *tmp;
							tmp=next->ln_Succ;
							if(tmp->ln_Succ)
								node=GetStatusNode(tmp->ln_Name,SHARED_LOCK);
							FreeStatusNode(next);
						}
					else
						{
							/* begin */
							ObtainSemaphoreShared(&StatusSemaphore);
							if(StatusList.lh_Head->ln_Succ)
								node=GetStatusNode(StatusList.lh_Head->ln_Name,SHARED_LOCK);
							ReleaseSemaphore(&StatusSemaphore);
						}
					if(node)
						{
							ULONG count=0;
							if(name) { *((STRPTR *) name)=node->ln_Name; count++; }
							if(exists) count++;
							count+=GetStatusFields(node,tags_orig);
							if(nextreturn) *nextreturn=count;
							ret=(ULONG) node;
						}
				}
			else
				{
					if(name && strlen((STRPTR) name)) /* Required */
						{
							struct Node *node;
							if(node=GetStatusNode((STRPTR) name,SHARED_LOCK))
								{
									if(exists) ret++;
									ret+=GetStatusFields(node,tags_orig);
									FreeStatusNode(node);
								}
						}
				}
		}
		return(ret);
	}

ULONG GetStatusTags(Tag tag,...)
	{
		return(GetStatus((struct TagItem *) &tag));
	}

__asm
ULONG SetStatus(register __a0 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		STRPTR name=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,st_set_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case STTAG_Name:
												name=data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			if(name && strlen(name))
				{
					struct Node *node;
					if(node=GetStatusNode(name,EXCLUSIVE_LOCK))
						{
							ret+=SetStatusFields(node,tags_orig);
							FreeStatusNode(node);
						}
				}
		}
		if(ret)
			SendUpdate(UPDCMD_UpdateStatus,NULL);
		return(ret);
	}

ULONG SetStatusTags(Tag tag,...)
	{
		return(SetStatus((struct TagItem *) &tag));
	}

__asm
BOOL AddStatus(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR name=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,st_add_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case STTAG_Name:
												name=data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			if(name && strlen(name) && strlen(name)<=32)
				{
					ObtainSemaphore(&StatusSemaphore);
					if(!FindName(&StatusList,name))
						{
							struct Node *wn=NULL;
							if(wn=AllocVec(sizeof(struct StatusNode),MEMF_CLEAR))
								{
									wn->ln_Name=((struct StatusNode *) wn)->sn_NodeName;
									InitSemaphore(&(((struct StatusNode *) wn)->sn_Semaphore));
									strcpy(((struct StatusNode *) wn)->sn_NodeName,name);
									AddTail(&StatusList,wn);
									SendUpdate(UPDCMD_UpdateStatus,NULL);
									ret=TRUE;
								}
						}
					ReleaseSemaphore(&StatusSemaphore);
				}
		}
		return(ret);
	}

BOOL AddStatusTags(Tag tag,...)
	{
		return(AddStatus((struct TagItem *) &tag));
	}

__asm
BOOL RemStatus(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR name=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,st_rem_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case STTAG_Name:
												name=data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			if(name && strlen(name))
				{
					ObtainSemaphore(&StatusSemaphore);
					{
						struct Node *node=NULL;
						if(node=FindName(&StatusList,name))
							{
								Remove(node);
								FreeVec(node);
								SendUpdate(UPDCMD_UpdateStatus,NULL);
								ret=TRUE;
							}
					}
					ReleaseSemaphore(&StatusSemaphore);
				}
		}
		return(ret);
	}

BOOL RemStatusTags(Tag tag,...)
	{
		return(RemStatus((struct TagItem *) &tag));
	}
