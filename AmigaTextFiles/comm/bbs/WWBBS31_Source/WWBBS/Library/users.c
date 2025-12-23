#include "wwbbs.h"
#include "users.h"
#include <dos/exall.h>

BOOL create_dir(BYTE *);
void delete_dir(BYTE *);

__asm
ULONG GetUser(register __a0 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		STRPTR name=NULL;
		STRPTR username=NULL;
		STRPTR realname=NULL;
		STRPTR uucpname=NULL;
		BOOL exists=FALSE;
		BOOL isnext=FALSE;struct Node *next=NULL;
		ULONG *nextreturn=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,usr_get_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case USRTAG_Name:
												name=data;
												break;
											case USRTAG_UserName:
												username=data;
												break;
											case USRTAG_RealName:
												realname=data;
												break;
											case USRTAG_UUCPName:
												uucpname=data;
												break;
											case USRTAG_Exists:
												exists=(BOOL) data;
												break;
											case USRTAG_Next:
												isnext=TRUE;
												next=data;
												break;
											case USRTAG_NextReturn:
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
							BYTE tmp_realname[31];
							strcpy(tmp_realname,"");
							tmp=next->ln_Succ;
							if(tmp->ln_Succ)
								{
									ObtainSemaphoreShared(&(((struct UserNode *) tmp)->un_Semaphore));
									strcpy(tmp_realname,((struct UserNode *) tmp)->un_RealName);
									ReleaseSemaphore(&(((struct UserNode *) tmp)->un_Semaphore));
								}
							FreeUserNode(next);
							if(strlen(tmp_realname))
								node=GetUserNode(tmp_realname,EXCLUSIVE_LOCK);
						}
					else
						{
							/* begin */
							BYTE begin_realname[31];
							strcpy(begin_realname,"");
							ObtainSemaphoreShared(&UserSemaphore);
							if(UserList.lh_Head->ln_Succ)
								{
									ObtainSemaphoreShared(&(((struct UserNode *) UserList.lh_Head)->un_Semaphore));
									strcpy(begin_realname,((struct UserNode *) UserList.lh_Head)->un_RealName);
									ReleaseSemaphore(&(((struct UserNode *) UserList.lh_Head)->un_Semaphore));
								}
							ReleaseSemaphore(&UserSemaphore);
							if(strlen(begin_realname))
								node=GetUserNode(begin_realname,EXCLUSIVE_LOCK);
						}
					if(node)
						{
							ULONG count=0;
							if(username) { *((STRPTR *) username)=((struct UserNode *) node)->un_UserName; count++; }
							if(realname) { *((STRPTR *) realname)=((struct UserNode *) node)->un_RealName; count++; }
							if(uucpname) { *((STRPTR *) uucpname)=((struct UserNode *) node)->un_UUCPName; count++; }
							if(exists) count++;
							{
								if(IsTag(usrinfo_tags,tags_orig))
									{
										BOOL buffered;
										buffered=LoadUserData(UserInfoFile,node,UNTAG_UserInfo);
										count+=GetUserInfoFields(node,tags_orig);
										if(buffered) UnLoadUserData(node,UNTAG_UserInfo);
									}
								if(IsTag(usraccess_tags,tags_orig))
									{
										BOOL buffered;
										buffered=LoadUserData(UserAccessFile,node,UNTAG_UserAccess);
										count+=GetUserAccessFields(node,tags_orig);
										if(buffered) UnLoadUserData(node,UNTAG_UserAccess);
									}
								if(IsTag(usrprefs_tags,tags_orig))
									{
										BOOL buffered;
										buffered=LoadUserData(UserPrefsFile,node,UNTAG_UserPrefs);
										count+=GetUserPrefsFields(node,tags_orig);
										if(buffered) UnLoadUserData(node,UNTAG_UserPrefs);
									}
								if(IsTag(usrstats_tags,tags_orig))
									{
										BOOL buffered;
										buffered=LoadUserData(UserStatsFile,node,UNTAG_UserStats);
										count+=GetUserStatsFields(node,tags_orig);
										if(buffered) UnLoadUserData(node,UNTAG_UserStats);
									}
							}
							if(nextreturn) *nextreturn=count;
							ret=(ULONG) node;
						}
				}
			else
				{
					if(name && strlen(name))
						{
							struct Node *node;
							if(node=GetUserNode(name,EXCLUSIVE_LOCK))
								{
									if(username) { strcpy(username,((struct UserNode *) node)->un_UserName); ret++; }
									if(realname) { strcpy(realname,((struct UserNode *) node)->un_RealName); ret++; }
									if(uucpname) { strcpy(uucpname,((struct UserNode *) node)->un_UUCPName); ret++; }
									if(exists) ret++;
									{
										if(IsTag(usrinfo_tags,tags_orig))
											{
												BOOL buffered;
												buffered=LoadUserData(UserInfoFile,node,UNTAG_UserInfo);
												ret+=GetUserInfoFields(node,tags_orig);
												if(buffered) UnLoadUserData(node,UNTAG_UserInfo);
											}
										if(IsTag(usraccess_tags,tags_orig))
											{
												BOOL buffered;
												buffered=LoadUserData(UserAccessFile,node,UNTAG_UserAccess);
												ret+=GetUserAccessFields(node,tags_orig);
												if(buffered) UnLoadUserData(node,UNTAG_UserAccess);
											}
										if(IsTag(usrprefs_tags,tags_orig))
											{
												BOOL buffered;
												buffered=LoadUserData(UserPrefsFile,node,UNTAG_UserPrefs);
												ret+=GetUserPrefsFields(node,tags_orig);
												if(buffered) UnLoadUserData(node,UNTAG_UserPrefs);
											}
										if(IsTag(usrstats_tags,tags_orig))
											{
												BOOL buffered;
												buffered=LoadUserData(UserStatsFile,node,UNTAG_UserStats);
												ret+=GetUserStatsFields(node,tags_orig);
												if(buffered) UnLoadUserData(node,UNTAG_UserStats);
											}
									}
									FreeUserNode(node);
								}
						}
				}
		}
		return(ret);
	}

ULONG GetUserTags(Tag tag,...)
	{
		return(GetUser((struct TagItem *) &tag));
	}

__asm
ULONG SetUser(register __a0 struct TagItem *tags_orig)
	{
		ULONG ret=0;
		STRPTR name=NULL;
		STRPTR username=NULL;
		STRPTR realname=NULL;
		STRPTR uucpname=NULL;
		BOOL forcesave=FALSE;
		BOOL dontsave=FALSE;

		BOOL infobuffered=FALSE,accessbuffered=FALSE,prefsbuffered=FALSE,statsbuffered=FALSE;
		BOOL saveindex=FALSE,saveinfo=FALSE,saveaccess=FALSE,saveprefs=FALSE,savestats=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,usr_set_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case USRTAG_Name:
												name=data;
												break;
											case USRTAG_UserName:
												username=data;
												break;
											case USRTAG_RealName:
												realname=data;
												break;
											case USRTAG_UUCPName:
												uucpname=data;
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
						if(node=GetUserNode(name,EXCLUSIVE_LOCK))
							{
								if(IsTag(usrinfo_tags,tags_orig))
									{
										ULONG num;
										infobuffered=LoadUserData(UserInfoFile,node,UNTAG_UserInfo);
										num=SetUserInfoFields(node,tags_orig);
										ret+=num;
										if(num && infobuffered)
											{
												saveinfo=TRUE;
												saveindex=TRUE;
											}
									}
								if(IsTag(usraccess_tags,tags_orig))
									{
										ULONG num;
										accessbuffered=LoadUserData(UserAccessFile,node,UNTAG_UserAccess);
										num=SetUserAccessFields(node,tags_orig);
										ret+=num;
										if(num && accessbuffered)
											{
												saveaccess=TRUE;
												saveindex=TRUE;
											}
									}
								if(IsTag(usrprefs_tags,tags_orig))
									{
										ULONG num;
										prefsbuffered=LoadUserData(UserPrefsFile,node,UNTAG_UserPrefs);
										num=SetUserPrefsFields(node,tags_orig);
										ret+=num;
										if(num && prefsbuffered)
											{
												saveprefs=TRUE;
												saveindex=TRUE;
											}
									}
								if(IsTag(usrstats_tags,tags_orig))
									{
										ULONG num;
										statsbuffered=LoadUserData(UserStatsFile,node,UNTAG_UserStats);
										num=SetUserStatsFields(node,tags_orig);
										ret+=num;
										if(num && statsbuffered)
											{
												savestats=TRUE;
												saveindex=TRUE;
											}
									}
								FreeUserNode(node);
							}
					}
					if( (username && strlen(username) && strlen(username)<=32) || (realname && strlen(realname) && strlen(realname)<=30) || (uucpname && strlen(uucpname) && strlen(uucpname)<=32) )
						{
							struct Node *node;
							ObtainSemaphore(&UserSemaphore);
							if(node=FindUser(name))
								{
									if(username && strlen(username) && strlen(username)<=32)
										{
											strcpy(((struct UserNode *) node)->un_UserName,username);
											saveindex=TRUE;
											ret++;
										}
									if(realname && strlen(realname) && strlen(realname)<=30)
										{
											strcpy(((struct UserNode *) node)->un_RealName,realname);
											saveindex=TRUE;
											ret++;
										}
									if(uucpname && strlen(uucpname) && strlen(uucpname)<=8)
										{
											strcpy(((struct UserNode *) node)->un_UUCPName,uucpname);
											saveindex=TRUE;
											ret++;
										}
								}
							ReleaseSemaphore(&UserSemaphore);
						}
				}
		}
		{
			if(forcesave || (!dontsave && saveinfo))
				SaveUserData(UserInfoFile,UNTAG_UserInfo);
			if(forcesave || (!dontsave && saveaccess))
				SaveUserData(UserAccessFile,UNTAG_UserAccess);
			if(forcesave || (!dontsave && saveprefs))
				SaveUserData(UserPrefsFile,UNTAG_UserPrefs);
			if(forcesave || (!dontsave && savestats))
				SaveUserData(UserStatsFile,UNTAG_UserStats);
			if(forcesave || (!dontsave && saveindex))
				SaveUserIndex(UserIndexFile);
		}
		{
			if(infobuffered || accessbuffered || prefsbuffered || statsbuffered)
				{
					if(name && strlen(name))
						{
							struct Node *node;
							if(node=GetUserNode(name,EXCLUSIVE_LOCK))
								{
									if(infobuffered) UnLoadUserData(node,UNTAG_UserInfo);
									if(accessbuffered) UnLoadUserData(node,UNTAG_UserAccess);
									if(prefsbuffered) UnLoadUserData(node,UNTAG_UserPrefs);
									if(statsbuffered) UnLoadUserData(node,UNTAG_UserStats);
									FreeUserNode(node);
								}
						}
				}
		}
		return(ret);
	}

ULONG SetUserTags(Tag tag,...)
	{
		return(SetUser((struct TagItem *) &tag));
	}

__asm
BOOL AddUser(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR username=NULL;
		STRPTR realname=NULL;
		STRPTR uucpname=NULL;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,usr_add_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case USRTAG_UserName:
												username=data;
												break;
											case USRTAG_RealName:
												realname=data;
												break;
											case USRTAG_UUCPName:
												uucpname=data;
												break;
											case USRTAG_DontSave:
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
			if( (username && strlen(username) && strlen(username)<=32) || (realname && strlen(realname) && strlen(realname)<=30) || (uucpname && strlen(uucpname) && strlen(uucpname)<=8) )
				{
					BOOL ok=TRUE;
					ObtainSemaphore(&UserSemaphore);
					if(username && strlen(username))
						{
							if(FindUser(username))
								ok=FALSE;
						}
					if(realname && strlen(realname))
						{
							if(FindUser(realname))
								ok=FALSE;
						}
					if(uucpname && strlen(uucpname))
						{
							if(FindUser(uucpname))
								ok=FALSE;
						}
					if(ok)
						{
							struct Node *wn;
							if(wn=AllocVec(sizeof(struct UserNode),MEMF_CLEAR))
								{
									InitSemaphore(&(((struct UserNode *) wn)->un_Semaphore));
									{
										ULONG count=1;
										int i=0;
										while(usernode_tags[i]!=TAG_END)
											{
												count++;
												i++;
											}
										((struct UserNode *) wn)->un_Offsets=AllocateTagItems(count);
										((struct UserNode *) wn)->un_Pointers=AllocateTagItems(count);
										if(((struct UserNode *) wn)->un_Offsets && ((struct UserNode *) wn)->un_Pointers)
											{
												for(i=0;i<count;i++)
													{
														((struct UserNode *) wn)->un_Offsets[i].ti_Tag=usernode_tags[i];
														((struct UserNode *) wn)->un_Offsets[i].ti_Data=(usernode_tags[i]==TAG_END) ? (ULONG) 0 : (ULONG) ~0;
														((struct UserNode *) wn)->un_Pointers[i].ti_Tag=usernode_tags[i];
														((struct UserNode *) wn)->un_Pointers[i].ti_Data=(ULONG) 0;
													}
												if(username) strcpy(((struct UserNode *) wn)->un_UserName,username);
												if(realname) strcpy(((struct UserNode *) wn)->un_RealName,realname);
												if(uucpname) strcpy(((struct UserNode *) wn)->un_UUCPName,uucpname);
												{
													BYTE dir[256];
													sprintf(dir,"WWBBS:Users/%s",uucpname);
													if(create_dir(dir))
														{
															AddTail(&UserList,wn);
															ret=TRUE;
														}
												}
											}
									}
									if(!ret)
										{
											if(((struct UserNode *) wn)->un_Offsets) FreeTagItems(((struct UserNode *) wn)->un_Offsets);
											if(((struct UserNode *) wn)->un_Pointers) FreeTagItems(((struct UserNode *) wn)->un_Pointers);
											FreeVec(wn);
										}
								}
						}
					ReleaseSemaphore(&UserSemaphore);
				}
		}
		if(ret && !dontsave)
			SaveUserIndex(UserIndexFile);
		return(ret);
	}

BOOL AddUserTags(Tag tag,...)
	{
		return(AddUser((struct TagItem *) &tag));
	}

__asm
BOOL RemUser(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR name=NULL;
		BOOL dontsave=FALSE;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,usr_rem_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case USRTAG_Name:
												name=data;
												break;
											case USRTAG_DontSave:
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
					ObtainSemaphore(&UserSemaphore);
					{
						struct Node *node;
						if(node=FindUser(name))
							{
								BYTE dir[256];
								sprintf(dir,"WWBBS:Users/%s",((struct UserNode *) node)->un_UUCPName);
								delete_dir(dir);
								if(DeleteFile(dir))
									{
										Remove(node);
										FreeTagItems(((struct UserNode *) node)->un_Offsets);
										{
											struct TagItem *tstate,*tag;
											tstate=((struct UserNode *) node)->un_Pointers;
											while(tag=NextTagItem(&tstate))
												{
													if(tag->ti_Data)
														FreeVec((APTR) tag->ti_Data);
												}
										}
										FreeTagItems(((struct UserNode *) node)->un_Pointers);
										FreeVec(node);
										ret=TRUE;
									}
							}

					}
					ReleaseSemaphore(&UserSemaphore);
				}
		}
		if(ret && !dontsave)
			{
				SaveUserData(UserInfoFile,UNTAG_UserInfo);
				SaveUserData(UserAccessFile,UNTAG_UserAccess);
				SaveUserData(UserPrefsFile,UNTAG_UserPrefs);
				SaveUserData(UserStatsFile,UNTAG_UserStats);
				SaveUserIndex(UserIndexFile);
			}
		return(ret);
	}

BOOL RemUserTags(Tag tag,...)
	{
		return(RemUser((struct TagItem *) &tag));
	}

__asm
BOOL LoadUser(register __a0 struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		STRPTR name=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,usr_load_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case USRTAG_Name:
												name=data;
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
					struct Node *node;
					if(node=GetUserNode(name,EXCLUSIVE_LOCK))
						{
							ret=TRUE;
							if(!LoadUserData(UserInfoFile,node,UNTAG_UserInfo))
								ret=FALSE;
							if(!LoadUserData(UserAccessFile,node,UNTAG_UserAccess))
								ret=FALSE;
							if(!LoadUserData(UserPrefsFile,node,UNTAG_UserPrefs))
								ret=FALSE;
							if(!LoadUserData(UserStatsFile,node,UNTAG_UserStats))
								ret=FALSE;
							FreeUserNode(node);
						}
				}
		}
		return(ret);
	}

BOOL LoadUserTags(Tag tag,...)
	{
		return(LoadUser((struct TagItem *) &tag));
	}

__asm
VOID UnLoadUser(register __a0 struct TagItem *tags_orig)
	{
		STRPTR name=NULL;
		{
			struct TagItem *tags;
			if(tags=CloneTagItems(tags_orig))
				{
					if(FilterTagItems(tags,usr_unload_tags,TAGFILTER_AND))
						{
							struct TagItem *tstate,*tag;
							APTR data;
							tstate=tags;
							while(tag=NextTagItem(&tstate))
								{
									data=(APTR) tag->ti_Data;
									switch(tag->ti_Tag)
										{
											case USRTAG_Name:
												name=data;
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
						SaveUserData(UserInfoFile,UNTAG_UserInfo);
						SaveUserData(UserAccessFile,UNTAG_UserAccess);
						SaveUserData(UserPrefsFile,UNTAG_UserPrefs);
						SaveUserData(UserStatsFile,UNTAG_UserStats);
						SaveUserIndex(UserIndexFile);
					}
					{
						struct Node *node;
						if(node=GetUserNode(name,EXCLUSIVE_LOCK))
							{
								UnLoadUserData(node,UNTAG_UserInfo);
								UnLoadUserData(node,UNTAG_UserAccess);
								UnLoadUserData(node,UNTAG_UserPrefs);
								UnLoadUserData(node,UNTAG_UserStats);
								FreeUserNode(node);
							}
					}
				}
		}
	}

VOID UnLoadUserTags(Tag tag,...)
	{
		UnLoadUser((struct TagItem *) &tag);
	}

BOOL create_dir(BYTE *dir)
	{
		BPTR fh;
		BOOL ret=FALSE;
		if(fh=CreateDir(dir))
			{
				UnLock(fh);
				ret=TRUE;
			}
		return(ret);
	}

void delete_dir(BYTE *dir)
	{
		BPTR lock,old_dir;
		if(lock=Lock(dir,SHARED_LOCK))
			{
				old_dir=CurrentDir(lock);
				{
					struct ExAllControl *eac;
					if(eac=AllocDosObject(DOS_EXALLCONTROL,NULL))
						{
							APTR data;
							if(data=AllocVec(sizeof(BYTE)*16384,MEMF_CLEAR))
								{
									BOOL more;
									eac->eac_LastKey=0;
									do
										{
											more=ExAll(lock,data,sizeof(BYTE)*16384,ED_TYPE,eac);
											if((!more) && (IoErr()!=ERROR_NO_MORE_ENTRIES))
												break;
											if(eac->eac_Entries==0)
												continue;
											{
												struct ExAllData *ead;
												ead=(struct ExAllData *) data;
												do
													{
														if(ead->ed_Type<0) /* plain file */
															DeleteFile(ead->ed_Name);
														if(ead->ed_Type>=0) /* directory */
															delete_dir(ead->ed_Name);
														ead=ead->ed_Next;
													}
												while(ead);
											}
										}
									while(more);
									FreeVec(data);
								}
							FreeDosObject(DOS_EXALLCONTROL,eac);
						}
				}
				CurrentDir(old_dir);
				UnLock(lock);
			}
	}
