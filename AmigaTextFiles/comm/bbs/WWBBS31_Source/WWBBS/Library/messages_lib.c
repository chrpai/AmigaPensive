#include "wwbbs.h"
#include "messages.h"

struct MessageNode *GetMessageNode(struct MessageGroupNode *gnode,ULONG id)
	{
		struct MessageNode *ret=NULL;
		{
			struct MessageNode *node;
			for(node=(struct MessageNode *) gnode->mgn_Messages.lh_Head;node->mn_Node.ln_Succ;node=(struct MessageNode *) node->mn_Node.ln_Succ)
				{
					if(node->mn_ID==id)
						{
							ret=node;
							break;
						}
				}
		}
		return(ret);
	}

ULONG GetMessageFields(struct MessageNode *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,msg_tags,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								data=(APTR) tag->ti_Data;
								switch(tag->ti_Tag)
									{
										case MSGTAG_Thread:
											*((ULONG *) data)=node->mn_Thread;
											break;
										case MSGTAG_From:
											*((STRPTR *) data)=node->mn_From;
											break;
										case MSGTAG_To:
											*((STRPTR *) data)=node->mn_To;
											break;
										case MSGTAG_Subject:
											*((STRPTR *) data)=node->mn_Subject;
											break;
										case MSGTAG_Date:
											*((struct DateStamp **) data)=&node->mn_Date;
											break;
										case MSGTAG_Text:
											*((STRPTR *) data)=node->mn_Text;
											break;
									}
								ret++;
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetMessageFields(struct MessageNode *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,msg_tags,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								data=(APTR) tag->ti_Data;
								switch(tag->ti_Tag)
									{
										case MSGTAG_Thread:
											node->mn_Thread=(ULONG) data;
											break;
										case MSGTAG_From:
											if(data && strlen((STRPTR) data)<=32)
												strcpy(node->mn_From,(STRPTR) data);
											else
												retminus++;
											break;
										case MSGTAG_To:
											if(data && strlen((STRPTR) data)<=32)
												strcpy(node->mn_To,(STRPTR) data);
											else
												retminus++;
											break;
										case MSGTAG_Subject:
											if(data && strlen((STRPTR) data)<=64)
												strcpy(node->mn_Subject,(STRPTR) data);
											else
												retminus++;
											break;
										case MSGTAG_Date:
											if(data)
												node->mn_Date=*((struct DateStamp *) data);
											else
												retminus++;
											break;
										case MSGTAG_Text:
											if(node->mn_Text)
												{
													FreeVec(node->mn_Text);
													node->mn_Text=NULL;
												}
											if(data && (node->mn_Text=AllocVec(strlen((STRPTR) data)+1,MEMF_CLEAR)))
												strcpy(node->mn_Text,(STRPTR) data);
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

BOOL LoadMessageGroup(BYTE *groupname)
	{
		BOOL ret=FALSE;
		ObtainSemaphore(&MessageGroupSemaphore);
		FlushMessageGroups();
		{
			struct MessageGroupNode *gnode;
			if(gnode=AllocVec(sizeof(struct MessageGroupNode),MEMF_CLEAR))
				{
					InitSemaphore(&gnode->mgn_Semaphore);
					NewList(&gnode->mgn_Messages);
					InitSemaphore(&gnode->mgn_UsageCountSemaphore);
					strcpy(gnode->mgn_Name,groupname);
					gnode->mgn_Node.ln_Name=gnode->mgn_Name;
					{
						BYTE count_filename[256];
						BPTR count_fh;
						sprintf(count_filename,"WWBBS:Messages/%s.count",groupname);
						if(count_fh=Open(count_filename,MODE_OLDFILE))
							{
								ULONG message_count,thread_count;
								if(FRead(count_fh,&message_count,sizeof(ULONG),1) && FRead(count_fh,&thread_count,sizeof(ULONG),1))
									{
										gnode->mgn_MessageCount=message_count;
										gnode->mgn_ThreadCount=thread_count;
									}
								Close(count_fh);
							}
					}
					{
						BYTE filename[256];
						BPTR fh;
						sprintf(filename,"WWBBS:Messages/%s.messages",groupname);
						if(fh=Open(filename,MODE_OLDFILE))
							{
								BOOL kg=TRUE;
								LONG cmd;
								struct MessageNode *node=NULL;
								while(kg)
									{
										cmd=FGetC(fh);
										switch(cmd)
											{
												case -1: /* EOF */
													kg=FALSE;
													break;
												case MSGCMD_Begin:
													if(!(node=AllocVec(sizeof(struct MessageNode),MEMF_CLEAR)))
														kg=FALSE;
													break;
												case MSGCMD_End:
													if(node)
														{
															AddTail(&gnode->mgn_Messages,(struct Node *) node);
															node=NULL;
														}
													break;
												case MSGCMD_Field:
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
																							case MSGFLD_ID:
																								node->mn_ID=*((ULONG *) data);
																								break;
																							case MSGFLD_Thread:
																								node->mn_Thread=*((ULONG *) data);
																								break;
																							case MSGFLD_From:
																								strcpy(node->mn_From,(STRPTR) data);
																								break;
																							case MSGFLD_To:
																								strcpy(node->mn_To,(STRPTR) data);
																								break;
																							case MSGFLD_Subject:
																								strcpy(node->mn_Subject,(STRPTR) data);
																								break;
																							case MSGFLD_Date:
																								node->mn_Date=*((struct DateStamp *) data);
																								break;
																							case MSGFLD_Text:
																								if(node->mn_Text=AllocVec(size,MEMF_CLEAR))
																									strcpy(node->mn_Text,(STRPTR) data);
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
					AddTail(&MessageGroupList,(struct Node *) gnode);
					ret=TRUE;
				}
		}
		ReleaseSemaphore(&MessageGroupSemaphore);
		return(ret);
	}

BOOL SaveMessageGroup(struct MessageGroupNode *gnode)
	{
		BOOL ret=FALSE;
		{
			BYTE filename[256];
			BPTR fh;
			sprintf(filename,"WWBBS:Messages/%s.messages",gnode->mgn_Name);
			if(fh=Open(filename,MODE_NEWFILE))
				{
					struct MessageNode *node;
					for(node=(struct MessageNode *) gnode->mgn_Messages.lh_Head;node->mn_Node.ln_Succ;node=(struct MessageNode *) node->mn_Node.ln_Succ)
						{
							FPutC(fh,MSGCMD_Begin);
							SaveMessageField(fh,MSGFLD_ID,&node->mn_ID,sizeof(ULONG));
							SaveMessageField(fh,MSGFLD_Thread,&node->mn_Thread,sizeof(ULONG));
							SaveMessageString(fh,MSGFLD_From,node->mn_From);
							SaveMessageString(fh,MSGFLD_To,node->mn_To);
							SaveMessageString(fh,MSGFLD_Subject,node->mn_Subject);
							SaveMessageField(fh,MSGFLD_Date,&node->mn_Date,sizeof(struct DateStamp));
							if(node->mn_Text)
								SaveMessageString(fh,MSGFLD_Text,node->mn_Text);
							FPutC(fh,MSGCMD_End);
						}
					{
						BYTE count_filename[256];
						BPTR count_fh;
						sprintf(count_filename,"WWBBS:Messages/%s.count",gnode->mgn_Name);
						if(count_fh=Open(count_filename,MODE_NEWFILE))
							{
								FWrite(count_fh,&gnode->mgn_MessageCount,sizeof(ULONG),1);
								FWrite(count_fh,&gnode->mgn_ThreadCount,sizeof(ULONG),1);
								Close(count_fh);
								ret=TRUE;
							}
					}
					Close(fh);
				}
		}
		return(ret);
	}

void FlushMessageGroups()
	{
		struct MessageGroupNode *wn,*nn;
		wn=(struct MessageGroupNode *) MessageGroupList.lh_Head;
		while(nn=(struct MessageGroupNode *) wn->mgn_Node.ln_Succ)
			{
				if(!wn->mgn_UsageCount)
					{
						{
							struct MessageNode *node;
							while(node=(struct MessageNode *) RemHead(&wn->mgn_Messages))
								{
									if(node->mn_Text)
										FreeVec(node->mn_Text);
									FreeVec(node);
								}
						}
						Remove((struct Node *) wn);
						FreeVec(wn);
					}
				wn=nn;
			}
	}

void SaveMessageField(BPTR fh,LONG field,APTR data,UWORD size)
	{
		FPutC(fh,MSGCMD_Field);
		FPutC(fh,field);
		FWrite(fh,&size,sizeof(UWORD),1);
		FWrite(fh,data,(ULONG) size,1);
	}

void SaveMessageString(BPTR fh,LONG field,STRPTR data)
	{
		SaveMessageField(fh,field,data,strlen(data)+1);
	}
