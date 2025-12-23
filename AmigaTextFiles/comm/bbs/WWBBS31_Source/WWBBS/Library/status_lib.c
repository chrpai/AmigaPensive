#include "wwbbs.h"
#include "status.h"

struct Node *GetStatusNode(BYTE *name,LONG accessmode)
	{
		struct Node *ret=NULL;
		if(name && strlen(name))
			{
				ObtainSemaphoreShared(&StatusSemaphore);
				{
					struct Node *node=NULL;
					if(node=FindName(&StatusList,name))
						{
							switch(accessmode)
								{
									case EXCLUSIVE_LOCK:
										ObtainSemaphore(&(((struct StatusNode *) node)->sn_Semaphore));
										break;
									case SHARED_LOCK:
									default:
										ObtainSemaphoreShared(&(((struct StatusNode *) node)->sn_Semaphore));
										break;
								}
							ret=node;
						}
					else
						ReleaseSemaphore(&StatusSemaphore);
				}
			}
		return(ret);
	}

void FreeStatusNode(struct Node *node)
	{
		ReleaseSemaphore(&(((struct StatusNode *) node)->sn_Semaphore));
		ReleaseSemaphore(&StatusSemaphore);
	}

ULONG GetStatusFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		Tag *tags_filter;
		if(tags=CloneTagItems(tags_orig))
			{
				tags_filter=st_tags;
				if(FilterTagItems(tags,tags_filter,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								data=(APTR) tag->ti_Data;
								switch(tag->ti_Tag)
									{
										case STTAG_UserName:
											strcpy((STRPTR) data,((struct StatusNode *) node)->sn_UserName);
											break;
										case STTAG_Location:
											strcpy((STRPTR) data,((struct StatusNode *) node)->sn_Location);
											break;
										case STTAG_Prompt:
											strcpy((STRPTR) data,((struct StatusNode *) node)->sn_Prompt);
											break;
										case STTAG_Stack:
											strcpy((STRPTR) data,((struct StatusNode *) node)->sn_Stack);
											break;
										case STTAG_TimeRemaining:
											*((UWORD *) data)=((struct StatusNode *) node)->sn_TimeRemaining;
											break;
										case STTAG_TimeUsed:
											*((UWORD *) data)=((struct StatusNode *) node)->sn_TimeUsed;
											break;
										case STTAG_InactivityTimeLimit:
											*((UWORD *) data)=((struct StatusNode *) node)->sn_InactivityTimeLimit;
											break;
										case STTAG_Panic:
											*((BOOL *) data)=(((struct StatusNode *) node)->sn_Flags & STFLG_Panic) ? TRUE : FALSE;
											break;
										case STTAG_Local:
											*((BOOL *) data)=(((struct StatusNode *) node)->sn_Flags & STFLG_Local) ? TRUE : FALSE;
											break;
										case STTAG_ConsoleOpen:
											*((BOOL *) data)=(((struct StatusNode *) node)->sn_Flags & STFLG_ConsoleOpen) ? TRUE : FALSE;
											break;
										case STTAG_ANSI:
											*((BOOL *) data)=(((struct StatusNode *) node)->sn_Flags & STFLG_ANSI) ? TRUE : FALSE;
											break;
										case STTAG_PageReceived:
											*((BOOL *) data)=(((struct StatusNode *) node)->sn_Flags & STFLG_PageReceived) ? TRUE : FALSE;
											break;
										case STTAG_UserMonitor:
											*((BOOL *) data)=(((struct StatusNode *) node)->sn_Flags & STFLG_UserMonitor) ? TRUE : FALSE;
											break;
									}
								ret++;
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetStatusFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		Tag *tags_filter=NULL;
		if(tags=CloneTagItems(tags_orig))
			{
				tags_filter=st_tags;
				if(FilterTagItems(tags,tags_filter,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								data=(APTR) tag->ti_Data;
								switch(tag->ti_Tag)
									{
										case STTAG_UserName:
											if(data && strlen((STRPTR) data)<=32)
												strcpy(((struct StatusNode *) node)->sn_UserName,(STRPTR) data);
											else
												retminus++;
											break;
										case STTAG_Location:
											if(data && strlen((STRPTR) data)<=32)
												strcpy(((struct StatusNode *) node)->sn_Location,(STRPTR) data);
											else
												retminus++;
											break;
										case STTAG_Prompt:
											if(data && strlen((STRPTR) data)<=255)
												strcpy(((struct StatusNode *) node)->sn_Prompt,(STRPTR) data);
											else
												retminus++;
											break;
										case STTAG_Stack:
											if(data && strlen((STRPTR) data)<=255)
												strcpy(((struct StatusNode *) node)->sn_Stack,(STRPTR) data);
											else
												retminus++;
											break;
										case STTAG_TimeRemaining:
											((struct StatusNode *) node)->sn_TimeRemaining=(UWORD) data;
											break;
										case STTAG_TimeUsed:
											((struct StatusNode *) node)->sn_TimeUsed=(UWORD) data;
											break;
										case STTAG_InactivityTimeLimit:
											((struct StatusNode *) node)->sn_InactivityTimeLimit=(UWORD) data;
											break;
										case STTAG_Panic:
											if((BOOL) data)
												((struct StatusNode *) node)->sn_Flags|=STFLG_Panic;
											else
												((struct StatusNode *) node)->sn_Flags&=~STFLG_Panic;
											break;
										case STTAG_Local:
											if((BOOL) data)
												((struct StatusNode *) node)->sn_Flags|=STFLG_Local;
											else
												((struct StatusNode *) node)->sn_Flags&=~STFLG_Local;
											break;
										case STTAG_ConsoleOpen:
											if((BOOL) data)
												((struct StatusNode *) node)->sn_Flags|=STFLG_ConsoleOpen;
											else
												((struct StatusNode *) node)->sn_Flags&=~STFLG_ConsoleOpen;
											break;
										case STTAG_ANSI:
											if((BOOL) data)
												((struct StatusNode *) node)->sn_Flags|=STFLG_ANSI;
											else
												((struct StatusNode *) node)->sn_Flags&=~STFLG_ANSI;
											break;
										case STTAG_PageReceived:
											if((BOOL) data)
												((struct StatusNode *) node)->sn_Flags|=STFLG_PageReceived;
											else
												((struct StatusNode *) node)->sn_Flags&=~STFLG_PageReceived;
											break;
										case STTAG_UserMonitor:
											if((BOOL) data)
												((struct StatusNode *) node)->sn_Flags|=STFLG_UserMonitor;
											else
												((struct StatusNode *) node)->sn_Flags&=~STFLG_UserMonitor;
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

void UnLoadStatus()
	{
		ObtainSemaphore(&StatusSemaphore);
		{
			struct Node *node;
			while(node=RemHead(&StatusList))
				FreeVec(node);
		}
		ReleaseSemaphore(&StatusSemaphore);
	}
