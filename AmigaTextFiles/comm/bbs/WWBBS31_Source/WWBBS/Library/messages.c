#include "wwbbs.h"
#include "messages.h"

__asm
APTR OpenMessageGroup(register __a0 BYTE *path,register __a1 BYTE *name,register __d0 LONG type)
	{
		APTR ret=NULL;
		BYTE group[41];
		strcpy(group,"");
		if(!path && !name)
			strcpy(group,"email");
		else
			GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,MBTAG_Group,group,TAG_END);
		if(strlen(group))
			{
				struct MessageGroupNode *gnode;
				{
					BOOL exists=FALSE;
					ObtainSemaphoreShared(&MessageGroupSemaphore);
					if(FindName(&MessageGroupList,group))
						exists=TRUE;
					ReleaseSemaphore(&MessageGroupSemaphore);
					if(!exists)
						LoadMessageGroup(group);
				}
				ObtainSemaphoreShared(&MessageGroupSemaphore);
				if(gnode=(struct MessageGroupNode *) FindName(&MessageGroupList,group))
					{
						if(type==EXCLUSIVE_LOCK)
							ObtainSemaphore(&gnode->mgn_Semaphore);
						else
							ObtainSemaphoreShared(&gnode->mgn_Semaphore);
						ObtainSemaphore(&gnode->mgn_UsageCountSemaphore);
						gnode->mgn_UsageCount++;
						ReleaseSemaphore(&gnode->mgn_UsageCountSemaphore);
						ret=gnode;
					}
				else
					ReleaseSemaphore(&MessageGroupSemaphore);
			}
		return(ret);
	}

__asm
void CloseMessageGroup(register __a0 struct MessageGroupNode *gnode)
	{
		ObtainSemaphore(&gnode->mgn_UsageCountSemaphore);
		gnode->mgn_UsageCount--;
		ReleaseSemaphore(&gnode->mgn_UsageCountSemaphore);
		ReleaseSemaphore(&gnode->mgn_Semaphore);
		ReleaseSemaphore(&MessageGroupSemaphore);
	}

__asm
ULONG GetMessage(register __a0 struct MessageGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		ULONG id=0;
		BOOL exists=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,msg_get_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case MSGTAG_ID:
												id=(ULONG) data;
												break;
											case MSGTAG_Exists:
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
					struct MessageNode *node=NULL;
					if(node=GetMessageNode(gnode,id))
						{
							if(exists) ret++;
							ret+=GetMessageFields(node,tags_orig);
						}
				}
		}
		return(ret);
	}

ULONG GetMessageTags(struct MessageGroupNode *gnode,Tag tag,...)
	{
		return(GetMessage(gnode,(struct TagItem *) &tag));
	}

__asm
ULONG SetMessage(register __a0 struct MessageGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		ULONG id=0;
		BOOL forcesave=FALSE,dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,msg_set_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case MSGTAG_ID:
												id=(ULONG) data;
												break;
											case MSGTAG_ForceSave:
												forcesave=(BOOL) data;
												break;
											case MSGTAG_DontSave:
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
					struct MessageNode *node=NULL;
					if(node=GetMessageNode(gnode,id))
						ret+=SetMessageFields(node,tags_orig);
				}
		}
		if((ret && !dontsave) || forcesave)
			SaveMessageGroup(gnode);
		return(ret);
	}

ULONG SetMessageTags(struct MessageGroupNode *gnode,Tag tag,...)
	{
		return(SetMessage(gnode,(struct TagItem *) &tag));
	}

__asm
ULONG AddMessage(register __a0 struct MessageGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		ULONG replyto=0;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,msg_add_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case MSGTAG_ReplyTo:
												replyto=(ULONG) data;
												break;
											case MSGTAG_DontSave:
												dontsave=(BOOL) data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			struct MessageNode *node;
			if(node=AllocVec(sizeof(struct MessageNode),MEMF_CLEAR))
				{
					node->mn_ID=gnode->mgn_MessageCount+1;
					if(replyto)
						{
							ULONG thread=0;
							if(GetMessageTags(gnode,MSGTAG_ID,replyto,MSGTAG_Thread,&thread,TAG_END))
								node->mn_Thread=thread;
						}
					else
						node->mn_Thread=gnode->mgn_ThreadCount+1;
					DateStamp(&node->mn_Date);
					AddTail(&gnode->mgn_Messages,(struct Node *) node);
					gnode->mgn_MessageCount++;
					if(!replyto)
						gnode->mgn_ThreadCount++;
					ret=node->mn_ID;
				}
		}
		if(ret && !dontsave)
			SaveMessageGroup(gnode);
		return(ret);
	}

ULONG AddMessageTags(struct MessageGroupNode *gnode,Tag tag,...)
	{
		return(AddMessage(gnode,(struct TagItem *) &tag));
	}

__asm
BOOL RemMessage(register __a0 struct MessageGroupNode *gnode,register __a1 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		ULONG id=0;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,msg_rem_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case MSGTAG_ID:
												id=(ULONG) data;
												break;
											case MSGTAG_DontSave:
												dontsave=(BOOL) data;
												break;
										}
								}
						}
					FreeTagItems(tags);
				}
		}
		{
			struct MessageNode *node;
			if(node=GetMessageNode(gnode,id))
				{
					Remove((struct Node *) node);
					FreeVec(node);
					ret=TRUE;
				}
		}
		if(ret && !dontsave)
			SaveMessageGroup(gnode);
		return(ret);
	}

BOOL RemMessageTags(struct MessageGroupNode *gnode,Tag tag,...)
	{
		return(RemMessage(gnode,(struct TagItem *) &tag));
	}

__asm
ULONG GetNextMessage(register __a0 struct MessageGroupNode *gnode,register __d0 ULONG id)
	{
		ULONG ret=0;
		struct MessageNode *node;
		if(node=GetMessageNode(gnode,id))
			{
				node=(struct MessageNode *) node->mn_Node.ln_Succ;
				if(node->mn_Node.ln_Succ)
					ret=node->mn_ID;
			}
		else
			{
				for(node=(struct MessageNode *) gnode->mgn_Messages.lh_Head;node->mn_Node.ln_Succ;node=(struct MessageNode *) node->mn_Node.ln_Succ)
					{
						if(node->mn_ID>id)
							{
								ret=node->mn_ID;
								break;
							}
					}
			}
		return(ret);
	}

__asm
ULONG GetPreviousMessage(register __a0 struct MessageGroupNode *gnode,register __d0 ULONG id)
	{
		ULONG ret=0;
		struct MessageNode *node;
		if(node=GetMessageNode(gnode,id))
			{
				node=(struct MessageNode *) node->mn_Node.ln_Pred;
				if(node->mn_Node.ln_Pred)
					ret=node->mn_ID;
			}
		else
			{
				for(node=(struct MessageNode *) gnode->mgn_Messages.lh_TailPred;node->mn_Node.ln_Pred;node=(struct MessageNode *) node->mn_Node.ln_Pred)
					{
						if(node->mn_ID<id)
							{
								ret=node->mn_ID;
								break;
							}
					}
			}
		return(ret);
	}
