#include "wwbbs.h"
#include "config.h"

__asm
ULONG GetConfig(register __a0 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		STRPTR path=NULL;
		APTR name=NULL;
		BOOL exists=FALSE;
		BOOL *parent=NULL;
		BOOL *child=NULL;
		UBYTE *type=NULL;
		BOOL isnext=FALSE;struct Node *next=NULL;
		ULONG *nextreturn=NULL;
		BYTE *getfirst=NULL,*getlast=NULL,*getnext=NULL,*getprevious=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,cfg_get_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case CFGTAG_Path:
												path=data;
												break;
											case CFGTAG_Name:
												name=data;
												break;
											case CFGTAG_Type:
												type=data;
												break;
											case CFGTAG_Exists:
												exists=(BOOL) data;
												break;
											case CFGTAG_Parent:
												parent=data;
												break;
											case CFGTAG_Child:
												child=data;
												break;
											case CFGTAG_GetFirst:
												getfirst=data;
												break;
											case CFGTAG_GetLast:
												getlast=data;
												break;
											case CFGTAG_GetNext:
												getnext=data;
												break;
											case CFGTAG_GetPrevious:
												getprevious=data;
												break;
											case CFGTAG_Next:
												isnext=TRUE;
												next=data;
												break;
											case CFGTAG_NextReturn:
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
								node=GetConfigNode(path,tmp->ln_Name,SHARED_LOCK);
							FreeConfigNode(next);
						}
					else
						{
							/* begin */
							if(path && strlen(path))
								{
									char pathbuff[256];
									if(strlen(path)<=255)
										{
											strcpy(pathbuff,path);
											{
												char *p;
												p=PathPart(pathbuff);
												*p=NULL;
											}
											{
												struct Node *parent;
												if(parent=GetConfigNode(pathbuff,FilePart(path),SHARED_LOCK))
													{
														if(((struct ConfigNode *) parent)->cn_Child && ((struct ConfigNode *) parent)->cn_Child->lh_Head->ln_Succ)
															node=GetConfigNode(path,((struct ConfigNode *) parent)->cn_Child->lh_Head->ln_Name,SHARED_LOCK);
														FreeConfigNode(parent);
													}
											}
										}
								}
							else
								{
									ObtainSemaphoreShared(&ConfigSemaphore);
									if(ConfigList.lh_Head->ln_Succ)
										node=GetConfigNode(NULL,ConfigList.lh_Head->ln_Name,SHARED_LOCK);
									ReleaseSemaphore(&ConfigSemaphore);
								}
						}
					if(node)
						{
							ULONG count=0;
							if(name) { *((STRPTR *) name)=node->ln_Name; count++; }
							if(type) { *type=((struct ConfigNode *) node)->cn_Type; count++; }
							if(exists) count++;
							if(parent) { *parent=(((struct ConfigNode *) node)->cn_Parent) ? TRUE : FALSE; count++; }
							if(child) { *child=(((struct ConfigNode *) node)->cn_Child) ? TRUE : FALSE; count++; }
							count+=GetConfigFields(node,tags_orig);
							if(nextreturn) *nextreturn=count;
							ret=(ULONG) node;
						}
				}
			else
				{
					if(name && strlen((STRPTR) name)) /* Required */
						{
							struct Node *node;
							if(node=GetConfigNode(path,(STRPTR) name,SHARED_LOCK))
								{
									if(type) { *type=((struct ConfigNode *) node)->cn_Type; ret++; }
									if(exists) ret++;
									if(parent) { *parent=(((struct ConfigNode *) node)->cn_Parent) ? TRUE : FALSE; ret++; }
									if(child) { *child=(((struct ConfigNode *) node)->cn_Child) ? TRUE : FALSE; ret++; }
									if(getnext)
										{
											struct Node *nn;
											nn=node->ln_Succ;
											if(nn->ln_Succ)
												{
													strcpy(getnext,nn->ln_Name);
													ret++;
												}
										}
									if(getprevious)
										{
											struct Node *pn;
											pn=node->ln_Pred;
											if(pn->ln_Pred)
												{
													strcpy(getprevious,pn->ln_Name);
													ret++;
												}
										}
									ret+=GetConfigFields(node,tags_orig);
									FreeConfigNode(node);
								}
						}
					if(getfirst || getlast)
						{
							if(path && strlen(path))
								{
									char pathbuff[256];
									if(strlen(path)<=255)
										{
											strcpy(pathbuff,path);
											{
												char *p;
												p=PathPart(pathbuff);
												*p=NULL;
											}
											{
												struct Node *parent;
												if(parent=GetConfigNode(pathbuff,FilePart(path),SHARED_LOCK))
													{
														if(((struct ConfigNode *) parent)->cn_Child)
															{
																if(getfirst)
																	{
																		if(((struct ConfigNode *) parent)->cn_Child->lh_Head->ln_Succ)
																			strcpy(getfirst,((struct ConfigNode *) parent)->cn_Child->lh_Head->ln_Name);
																		else
																			strcpy(getfirst,"");
																		ret++;
																	}
																if(getlast)
																	{
																		if(((struct ConfigNode *) parent)->cn_Child->lh_TailPred->ln_Pred)
																			strcpy(getlast,((struct ConfigNode *) parent)->cn_Child->lh_TailPred->ln_Name);
																		else
																			strcpy(getlast,"");
																		ret++;
																	}
															}
														FreeConfigNode(parent);
													}
											}
										}
								}
							else
								{
									ObtainSemaphoreShared(&ConfigSemaphore);
									if(getfirst)
										{
											if(ConfigList.lh_Head->ln_Succ)
												strcpy(getfirst,ConfigList.lh_Head->ln_Name);
											else
												strcpy(getfirst,"");
											ret++;
										}
									if(getlast)
										{
											if(ConfigList.lh_TailPred->ln_Succ)
												strcpy(getlast,ConfigList.lh_TailPred->ln_Name);
											else
												strcpy(getlast,"");
											ret++;
										}
									ReleaseSemaphore(&ConfigSemaphore);
								}
						}
				}
		}
		return(ret);
	}

ULONG GetConfigTags(Tag tag,...)
	{
		return(GetConfig((struct TagItem *) &tag));
	}

__asm
ULONG SetConfig(register __a0 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		STRPTR path=NULL;
		STRPTR name=NULL;
		STRPTR newname=NULL;
		BOOL moveup=FALSE;
		BOOL movedown=FALSE;
		BOOL forcesave=FALSE;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,cfg_set_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case CFGTAG_Path:
												path=data;
												break;
											case CFGTAG_Name:
												name=data;
												break;
											case CFGTAG_NewName:
												newname=data;
												break;
											case CFGTAG_MoveUp:
												moveup=(BOOL) data;
												break;
											case CFGTAG_MoveDown:
												movedown=(BOOL) data;
												break;
											case CFGTAG_ForceSave:
												forcesave=(BOOL) data;
												break;
											case CFGTAG_DontSave:
												dontsave=(BOOL) data;
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
			if(name && strlen(name))
				{
					{
						struct Node *node;
						if(node=GetConfigNode(path,name,EXCLUSIVE_LOCK))
							{
								ret+=SetConfigFields(node,tags_orig);
								FreeConfigNode(node);
							}
					}
					if(moveup)
						{
							struct Node *parent=NULL;
							if(path && strlen(path))
								{
									char pathbuff[256],*p;
									if(strlen(path)<=255)
										{
											strcpy(pathbuff,path);
											p=PathPart(pathbuff);
											*p=NULL;
											parent=GetConfigNode(pathbuff,FilePart(path),EXCLUSIVE_LOCK);
										}
								}
							else
								ObtainSemaphore(&ConfigSemaphore);
							if(!path || parent)
								{
									struct List *list=NULL;
									list=(parent) ? ((struct ConfigNode *) parent)->cn_Child : &ConfigList;
									if(list)
										{
											struct Node *node=NULL;
											if(node=FindName(list,name))
												{
													struct Node *pred;
													pred=node->ln_Pred;
													if(pred->ln_Pred)
														{
															pred=pred->ln_Pred;
															Remove(node);
															Insert(list,node,pred);
															ret++;
														}
												}
										}
									if(parent)
										FreeConfigNode(parent);
									else
										ReleaseSemaphore(&ConfigSemaphore);
								}							
						}
					if(movedown)
						{
							struct Node *parent=NULL;
							if(path && strlen(path))
								{
									char pathbuff[256],*p;
									if(strlen(path)<=255)
										{
											strcpy(pathbuff,path);
											p=PathPart(pathbuff);
											*p=NULL;
											parent=GetConfigNode(pathbuff,FilePart(path),EXCLUSIVE_LOCK);
										}
								}
							else
								ObtainSemaphore(&ConfigSemaphore);
							if(!path || parent)
								{
									struct List *list=NULL;
									list=(parent) ? ((struct ConfigNode *) parent)->cn_Child : &ConfigList;
									if(list)
										{
											struct Node *node=NULL;
											if(node=FindName(list,name))
												{
													struct Node *succ;
													succ=node->ln_Succ;
													if(succ->ln_Succ)
														{
															Remove(node);
															Insert(list,node,succ);
															ret++;
														}
												}
										}
									if(parent)
										FreeConfigNode(parent);
									else
										ReleaseSemaphore(&ConfigSemaphore);
								}
						}
					if(newname && strlen(newname) && strlen(newname)<=32)
						{
							struct Node *parent=NULL;
							if(path && strlen(path))
								{
									char pathbuff[256],*p;
									if(strlen(path)<=255)
										{
											strcpy(pathbuff,path);
											p=PathPart(pathbuff);
											*p=NULL;
											parent=GetConfigNode(pathbuff,FilePart(path),EXCLUSIVE_LOCK);
										}
								}
							else
								ObtainSemaphore(&ConfigSemaphore);
							if(!path || parent)
								{
									struct List *list=NULL;
									list=(parent) ? ((struct ConfigNode *) parent)->cn_Child : &ConfigList;
									if(list)
										{
											struct Node *node=NULL;
											if(node=FindName(list,name))
												{
													if(!strchr(newname,'/') && !FindName(list,newname))
														{
															strcpy(((struct ConfigNode *) node)->cn_Name,newname);
															ret++;
														}
												}
										}
									if(parent)
										FreeConfigNode(parent);
									else
										ReleaseSemaphore(&ConfigSemaphore);
								}
						}
				}
		}
		if(ret)
			{
				if(path && !stricmp(path,"Nodes") && name && strlen(name))
					{
						SendUpdate(UPDCMD_KillNode,name);
						SendUpdate(UPDCMD_StartupNode,name);
						SendUpdate(UPDCMD_UpdateStatus,NULL);
					}
			}
		if((ret && !dontsave) || forcesave)
			SaveConfig(ConfigFile);
		return(ret);
	}

ULONG SetConfigTags(Tag tag,...)
	{
		return(SetConfig((struct TagItem *) &tag));
	}

__asm
BOOL AddConfig(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR path=NULL;
		STRPTR name=NULL;
		UBYTE istype=FALSE,type;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,cfg_add_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case CFGTAG_Path:
												path=data;
												break;
											case CFGTAG_Name:
												name=data;
												break;
											case CFGTAG_Type:
												istype=TRUE;
												type=(UBYTE) data;
												break;
											case CFGTAG_DontSave:
												dontsave=(BOOL) data;
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
			if(name && strlen(name) && strlen(name)<=32 && istype)
				{
					struct List *list;
					struct Node *node=NULL;
					list=&ConfigList;
					if(path && strlen(path))
						{
							char pathbuff[256],*p;
							if(strlen(path)<=255)
								{
									strcpy(pathbuff,path);
									p=PathPart(pathbuff);
									*p=NULL;
									if(node=GetConfigNode(pathbuff,FilePart(path),EXCLUSIVE_LOCK))
										{
											if(((struct ConfigNode *) node)->cn_Child)
												list=((struct ConfigNode *) node)->cn_Child;
											else
												{
													if(((struct ConfigNode *) node)->cn_Child=AllocVec(sizeof(struct List),MEMF_CLEAR))
														{
															NewList(((struct ConfigNode *) node)->cn_Child);
															list=((struct ConfigNode *) node)->cn_Child;
														}
													else
														{
															FreeConfigNode(node);
															list=NULL;
														}
												}
										}
									else
										list=NULL;
								}
							else
								list=NULL;
						}
					else
						ObtainSemaphore(&ConfigSemaphore);
					if(list)
						{
							if(!strchr(name,'/') && !FindName(list,name))
								{
									ULONG size=0;
									BOOL update=FALSE;
									switch(type)
										{
											case CFGTYP_Config:
												size=sizeof(struct ConfigNode);
												break;
											case CFGTYP_AccessGroup:
												size=sizeof(struct AccessGroupNode);
												break;
											case CFGTYP_Archiver:
												size=sizeof(struct ArchiverNode);
												break;
											case CFGTYP_Editor:
												size=sizeof(struct EditorNode);
												break;
											case CFGTYP_FileBase:
												if(AttemptSemaphore(&FileGroupSemaphore))
													size=sizeof(struct FileBaseNode);
												break;
											case CFGTYP_Menu:
												size=sizeof(struct MenuNode);
												break;
											case CFGTYP_MessageBase:
												size=sizeof(struct MessageBaseNode);
												break;
											case CFGTYP_News:
												size=sizeof(struct NewsNode);
												break;
											case CFGTYP_Node:
												size=sizeof(struct NodeNode);
												update=UPDCMD_UpdateStatus;
												break;
											case CFGTYP_Protocol:
												size=sizeof(struct ProtocolNode);
												break;
											case CFGTYP_System:
												size=sizeof(struct SystemNode);
												break;
										}
									if(size)
										{
											struct Node *wn=NULL;
											if(wn=AllocVec(size,MEMF_CLEAR))
												{
													wn->ln_Name=((struct ConfigNode *) wn)->cn_Name;
													InitSemaphore(&(((struct ConfigNode *) wn)->cn_Semaphore));
													((struct ConfigNode *) wn)->cn_Type=type;
													((struct ConfigNode *) wn)->cn_Parent=node;
													strcpy(((struct ConfigNode *) wn)->cn_Name,name);
													AddTail(list,wn);
													if(update)
														{
															SendUpdate(UPDCMD_StartupNode,wn->ln_Name);
															SendUpdate(UPDCMD_UpdateStatus,NULL);
														}
													ret=TRUE;
												}
											switch(type)
												{
													case CFGTYP_FileBase:
														ReleaseSemaphore(&FileGroupSemaphore);
														break;
												}
										}
								}
							if(node)
								FreeConfigNode(node);
							else
								ReleaseSemaphore(&ConfigSemaphore);
						}
				}
		}
		if(ret && !dontsave)
			SaveConfig(ConfigFile);
		return(ret);
	}

BOOL AddConfigTags(Tag tag,...)
	{
		return(AddConfig((struct TagItem *) &tag));
	}

__asm
BOOL RemConfig(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR path=NULL;
		STRPTR name=NULL;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,cfg_rem_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case CFGTAG_Path:
												path=data;
												break;
											case CFGTAG_Name:
												name=data;
												break;
											case CFGTAG_DontSave:
												dontsave=(BOOL) data;
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
			if(!strncmp(name,"FileBases",9))
				{
					if(!AttemptSemaphore(&FileGroupSemaphore))
						name=NULL;
				}
			if(name && strlen(name))
				{
					struct Node *parent=NULL;
					if(path && strlen(path))
						{
							char pathbuff[256],*p;
							if(strlen(path)<=255)
								{
									strcpy(pathbuff,path);
									p=PathPart(pathbuff);
									*p=NULL;
									parent=GetConfigNode(pathbuff,FilePart(path),EXCLUSIVE_LOCK);
								}
						}
					else
						ObtainSemaphore(&ConfigSemaphore);
					if(!path || parent)
						{
							struct List *list=NULL;
							list=(parent) ? ((struct ConfigNode *) parent)->cn_Child : &ConfigList;
							if(list)
								{
									struct Node *node=NULL;
									if(node=FindName(list,name))
										{
											if(!(((struct ConfigNode *) node)->cn_Child))
												{
													Remove(node);
													FreeVec(node);
													if(!(list->lh_Head->ln_Succ) && parent)
														{
															FreeVec(((struct ConfigNode *) parent)->cn_Child);
															((struct ConfigNode *) parent)->cn_Child=NULL;
														}
													ret=TRUE;
												}
										}
								}
							if(parent)
								FreeConfigNode(parent);
							else
								ReleaseSemaphore(&ConfigSemaphore);
						}
				}
			if(!strncmp(name,"FileBases",9))
				ReleaseSemaphore(&FileGroupSemaphore);
		}
		if(ret)
			{
				if(path && !stricmp(path,"Nodes") && name && strlen(name))
					{
						SendUpdate(UPDCMD_KillNode,name);
						SendUpdate(UPDCMD_UpdateStatus,NULL);
					}
			}
		if(ret && !dontsave)
			SaveConfig(ConfigFile);
		return(ret);
	}

BOOL RemConfigTags(Tag tag,...)
	{
		return(RemConfig((struct TagItem *) &tag));
	}
