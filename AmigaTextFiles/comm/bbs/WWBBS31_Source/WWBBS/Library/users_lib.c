#include "wwbbs.h"
#include "users.h"

void checkpoint(BYTE *);

struct Node *GetUserNode(BYTE *name,LONG accessmode)
	{
		struct Node *ret=NULL;
		if(name && strlen(name))
			{
				ObtainSemaphoreShared(&UserSemaphore);
				{
					struct Node *node=NULL;
					if(node=FindUser(name))
						{
							switch(accessmode)
								{
									case EXCLUSIVE_LOCK:
										ObtainSemaphore(&(((struct UserNode *) node)->un_Semaphore));
										break;
									case SHARED_LOCK:
									default:
										ObtainSemaphoreShared(&(((struct UserNode *) node)->un_Semaphore));
										break;
								}
							ret=node;
						}
					else
						ReleaseSemaphore(&UserSemaphore);
				}
			}
		return(ret);
	}

void FreeUserNode(struct Node *node)
	{
		ReleaseSemaphore(&(((struct UserNode *) node)->un_Semaphore));
		ReleaseSemaphore(&UserSemaphore);
	}

ULONG GetUserInfoFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usrinfo_tags,TAGFILTER_AND))
					{
						struct UserInfo *ui=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserInfo,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										ui=(struct UserInfo *) tag->ti_Data;
								}
						}
						if(ui)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_Password:
													strcpy((STRPTR) data,ui->ui_Password);
													break;
												case USRTAG_Address:
													strcpy((STRPTR) data,ui->ui_Address);
													break;
												case USRTAG_City:
													strcpy((STRPTR) data,ui->ui_City);
													break;
												case USRTAG_State:
													strcpy((STRPTR) data,ui->ui_State);
													break;
												case USRTAG_Zip:
													strcpy((STRPTR) data,ui->ui_Zip);
													break;
												case USRTAG_Country:
													strcpy((STRPTR) data,ui->ui_Country);
													break;
												case USRTAG_PhoneCode:
													strcpy((STRPTR) data,ui->ui_PhoneCode);
													break;
												case USRTAG_PhoneNumber:
													strcpy((STRPTR) data,ui->ui_PhoneNumber);
													break;
												case USRTAG_BirthDay:
													*((UBYTE *) data)=ui->ui_BirthDay;
													break;
												case USRTAG_BirthMonth:
													*((UBYTE *) data)=ui->ui_BirthMonth;
													break;
												case USRTAG_BirthYear:
													*((UWORD *) data)=ui->ui_BirthYear;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetUserInfoFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usrinfo_tags,TAGFILTER_AND))
					{
						struct UserInfo *ui=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserInfo,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										ui=(struct UserInfo *) tag->ti_Data;
								}
						}
						if(ui)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_Password:
													if(data && strlen((STRPTR) data)<=10)
														strcpy(ui->ui_Password,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_Address:
													if(data && strlen((STRPTR) data)<=30)
														strcpy(ui->ui_Address,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_City:
													if(data && strlen((STRPTR) data)<=20)
														strcpy(ui->ui_City,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_State:
													if(data && strlen((STRPTR) data)<=20)
														strcpy(ui->ui_State,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_Zip:
													if(data && strlen((STRPTR) data)<=10)
														strcpy(ui->ui_Zip,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_Country:
													if(data && strlen((STRPTR) data)<=3)
														strcpy(ui->ui_Country,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_PhoneCode:
													if(data && strlen((STRPTR) data)<=6)
														strcpy(ui->ui_PhoneCode,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_PhoneNumber:
													if(data && strlen((STRPTR) data)<=8)
														strcpy(ui->ui_PhoneNumber,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_BirthDay:
													ui->ui_BirthDay=(UBYTE) data;
													break;
												case USRTAG_BirthMonth:
													ui->ui_BirthMonth=(UBYTE) data;
													break;
												case USRTAG_BirthYear:
													ui->ui_BirthYear=(UWORD) data;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		ret-=retminus;
		return(ret);
	}

ULONG GetUserAccessFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usraccess_tags,TAGFILTER_AND))
					{
						struct UserAccess *ua=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						tstate=tags;
						{
							if(tag=FindTagItem(UNTAG_UserAccess,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										ua=(struct UserAccess *) tag->ti_Data;
								}
						}
						if(ua)
							{
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_AccessLevel:
													*((UBYTE *) data)=ua->ua_AccessLevel;
													break;
												case USRTAG_SessionTimeLimit:
													*((UWORD *) data)=ua->ua_SessionTimeLimit;
													break;
												case USRTAG_InactivityTimeLimit:
													*((UWORD *) data)=ua->ua_InactivityTimeLimit;
													break;
												case USRTAG_RatioByte:
													*((UBYTE *) data)=ua->ua_RatioByte;
													break;
												case USRTAG_RatioFile:
													*((UBYTE *) data)=ua->ua_RatioFile;
													break;
												case USRTAG_Days:
													*((ULONG *) data)=ua->ua_Days;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetUserAccessFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usraccess_tags,TAGFILTER_AND))
					{
						struct UserAccess *ua=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserAccess,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										ua=(struct UserAccess *) tag->ti_Data;
								}
						}
						if(ua)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_AccessLevel:
													ua->ua_AccessLevel=(UBYTE) data;
													break;
												case USRTAG_SessionTimeLimit:
													ua->ua_SessionTimeLimit=(UWORD) data;
													break;
												case USRTAG_InactivityTimeLimit:
													ua->ua_InactivityTimeLimit=(UWORD) data;
													break;
												case USRTAG_RatioByte:
													ua->ua_RatioByte=(UBYTE) data;
													break;
												case USRTAG_RatioFile:
													ua->ua_RatioFile=(UBYTE) data;
													break;
												case USRTAG_Days:
													ua->ua_Days=(ULONG) data;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		ret-=retminus;
		return(ret);
	}

ULONG GetUserPrefsFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usrprefs_tags,TAGFILTER_AND))
					{
						struct UserPrefs *up=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserPrefs,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										up=(struct UserPrefs *) tag->ti_Data;
								}
						}
						if(up)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_ScreenWidth:
													*((UBYTE *) data)=up->up_ScreenWidth;
													break;
												case USRTAG_ScreenHeight:
													*((UBYTE *) data)=up->up_ScreenHeight;
													break;
												case USRTAG_LogonStack:
													strcpy((STRPTR) data,up->up_LogonStack);
													break;
												case USRTAG_Editor:
													strcpy((STRPTR) data,up->up_Editor);
													break;
												case USRTAG_Protocol:
													strcpy((STRPTR) data,up->up_Protocol);
													break;
												case USRTAG_Archiver:
													strcpy((STRPTR) data,up->up_Archiver);
													break;
												case USRTAG_HotKeys:
													*((BOOL *) data)=(up->up_Flags & UPFLG_HotKeys) ? TRUE : FALSE;
													break;
												case USRTAG_Menus:
													*((BOOL *) data)=(up->up_Flags & UPFLG_NoMenus) ? FALSE : TRUE;
													break;
												case USRTAG_UserMonitor:
													*((BOOL *) data)=(up->up_Flags & UPFLG_NoUserMonitor) ? FALSE : TRUE;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetUserPrefsFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usrprefs_tags,TAGFILTER_AND))
					{
						struct UserPrefs *up=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserPrefs,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										up=(struct UserPrefs *) tag->ti_Data;
								}
						}
						if(up)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_ScreenWidth:
													up->up_ScreenWidth=(UBYTE) data;
													break;
												case USRTAG_ScreenHeight:
													up->up_ScreenHeight=(UBYTE) data;
													break;
												case USRTAG_LogonStack:
													if(data && strlen((STRPTR) data)<=20)
														strcpy(up->up_LogonStack,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_Editor:
													if(data && strlen((STRPTR) data)<=32)
														strcpy(up->up_Editor,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_Protocol:
													if(data && strlen((STRPTR) data)<=32)
														strcpy(up->up_Protocol,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_Archiver:
													if(data && strlen((STRPTR) data)<=32)
														strcpy(up->up_Archiver,(STRPTR) data);
													else
														retminus++;
													break;
												case USRTAG_HotKeys:
													if((BOOL) data)
														up->up_Flags|=UPFLG_HotKeys;
													else
														up->up_Flags&=~UPFLG_HotKeys;
													break;
												case USRTAG_Menus:
													if((BOOL) data)
														up->up_Flags&=~UPFLG_NoMenus;
													else
														up->up_Flags|=UPFLG_NoMenus;
													break;
												case USRTAG_UserMonitor:
													if((BOOL) data)
														up->up_Flags&=~UPFLG_NoUserMonitor;
													else
														up->up_Flags|=UPFLG_NoUserMonitor;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		ret-=retminus;
		return(ret);
	}

ULONG GetUserStatsFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usrstats_tags,TAGFILTER_AND))
					{
						struct UserStats *us=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserStats,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										us=(struct UserStats *) tag->ti_Data;
								}
						}
						if(us)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_Sessions:
													*((ULONG *) data)=us->us_Sessions;
													break;
												case USRTAG_EmailRead:
													*((ULONG *) data)=us->us_EmailRead;
													break;
												case USRTAG_EmailWritten:
													*((ULONG *) data)=us->us_EmailWritten;
													break;
												case USRTAG_MessagesRead:
													*((ULONG *) data)=us->us_MessagesRead;
													break;
												case USRTAG_MessagesWritten:
													*((ULONG *) data)=us->us_MessagesWritten;
													break;
												case USRTAG_FilesUploaded:
													*((ULONG *) data)=us->us_FilesUploaded;
													break;
												case USRTAG_FilesDownloaded:
													*((ULONG *) data)=us->us_FilesDownloaded;
													break;
												case USRTAG_FilesCredits:
													*((ULONG *) data)=us->us_FilesCredits;
													break;
												case USRTAG_KilosUploaded:
													*((ULONG *) data)=us->us_KilosUploaded;
													break;
												case USRTAG_KilosDownloaded:
													*((ULONG *) data)=us->us_KilosDownloaded;
													break;
												case USRTAG_KilosCredits:
													*((ULONG *) data)=us->us_KilosCredits;
													break;
												case USRTAG_LastOn:
													*((struct DateStamp *) data)=us->us_LastOn;
													break;
												case USRTAG_TimeUsed:
													*((UWORD *) data)=us->us_TimeUsed;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetUserStatsFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,usrstats_tags,TAGFILTER_AND))
					{
						struct UserStats *us=NULL;
						struct TagItem *tstate,*tag;
						APTR data;
						{
							if(tag=FindTagItem(UNTAG_UserStats,((struct UserNode *) node)->un_Pointers))
								{
									if(tag->ti_Data)
										us=(struct UserStats *) tag->ti_Data;
								}
						}
						if(us)
							{
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(tag->ti_Tag)
											{
												case USRTAG_Sessions:
													us->us_Sessions=(ULONG) data;
													break;
												case USRTAG_EmailRead:
													us->us_EmailRead=(ULONG) data;
													break;
												case USRTAG_EmailWritten:
													us->us_EmailWritten=(ULONG) data;
													break;
												case USRTAG_MessagesRead:
													us->us_MessagesRead=(ULONG) data;
													break;
												case USRTAG_MessagesWritten:
													us->us_MessagesWritten=(ULONG) data;
													break;
												case USRTAG_FilesUploaded:
													us->us_FilesUploaded=(ULONG) data;
													break;
												case USRTAG_FilesDownloaded:
													us->us_FilesDownloaded=(ULONG) data;
													break;
												case USRTAG_FilesCredits:
													us->us_FilesCredits=(ULONG) data;
													break;
												case USRTAG_KilosUploaded:
													us->us_KilosUploaded=(ULONG) data;
													break;
												case USRTAG_KilosDownloaded:
													us->us_KilosDownloaded=(ULONG) data;
													break;
												case USRTAG_KilosCredits:
													us->us_KilosCredits=(ULONG) data;
													break;
												case USRTAG_LastOn:
													if(data)
														us->us_LastOn=*((struct DateStamp *) data);
													break;
												case USRTAG_TimeUsed:
													us->us_TimeUsed=(UWORD) data;
													break;
											}
										ret++;
									}
							}
					}
				FreeTagItems(tags);
			}
		ret-=retminus;
		return(ret);
	}

struct Node *FindUser(BYTE *name)
	{
		struct Node *ret=NULL;
		if(!stricmp(name,"Sysop"))
			{
				struct Node *node=NULL;
				node=UserList.lh_Head;
				if(node->ln_Succ)
					ret=node;
			}
		else
			{
				struct Node *node=NULL;
				for(node=UserList.lh_Head;node->ln_Succ;node=node->ln_Succ)
					{
						ObtainSemaphoreShared(&(((struct UserNode *) node)->un_Semaphore));
						if(!stricmp(name,((struct UserNode *) node)->un_UserName) || !stricmp(name,((struct UserNode *) node)->un_RealName) || !stricmp(name,((struct UserNode *) node)->un_UUCPName))
							ret=node;
						ReleaseSemaphore(&(((struct UserNode *) node)->un_Semaphore));
						if(ret)
							break;
					}
			}
		return(ret);
	}

void LoadUserIndex(BYTE *userindexfile)
	{
		BPTR fh;
		ObtainSemaphore(&UserSemaphore);
		if(fh=Open(userindexfile,MODE_OLDFILE))
			{
				BOOL kg=TRUE;
				LONG cmd;
				struct Node *node=NULL;
				while(kg)
					{
						cmd=FGetC(fh);
						switch(cmd)
							{
								case -1: /* EOF */
									kg=FALSE;
									break;
								case USRCMD_Begin:
									if(node=AllocVec(sizeof(struct UserNode),MEMF_CLEAR))
										{
											InitSemaphore(&(((struct UserNode *) node)->un_Semaphore));
											{
												ULONG count=1;
												int i=0;
												while(usernode_tags[i]!=TAG_END)
													{
														count++;
														i++;
													}
												((struct UserNode *) node)->un_Offsets=AllocateTagItems(count);
												((struct UserNode *) node)->un_Pointers=AllocateTagItems(count);
												if(((struct UserNode *) node)->un_Offsets && ((struct UserNode *) node)->un_Pointers)
													{
														for(i=0;i<count;i++)
															{
																((struct UserNode *) node)->un_Offsets[i].ti_Tag=usernode_tags[i];
																((struct UserNode *) node)->un_Offsets[i].ti_Data=(usernode_tags[i]==TAG_END) ? (ULONG) 0 : (ULONG) ~0;
																((struct UserNode *) node)->un_Pointers[i].ti_Tag=usernode_tags[i];
																((struct UserNode *) node)->un_Pointers[i].ti_Data=(ULONG) 0;
															}
													}
												else
													{
														if(((struct UserNode *) node)->un_Offsets) FreeTagItems(((struct UserNode *) node)->un_Offsets);
														if(((struct UserNode *) node)->un_Pointers) FreeTagItems(((struct UserNode *) node)->un_Pointers);
														FreeVec(node);
														kg=FALSE;
													}
											}
										}
									else
										kg=FALSE;
									break;
								case USRCMD_End:
									if(node)
										{
											AddTail(&UserList,node);
											node=NULL;
										}
									break;
								case USRCMD_Field:
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
																			case USRFLD_UserName:
																				strcpy(((struct UserNode *) node)->un_UserName,(STRPTR) data);
																				break;
																			case USRFLD_RealName:
																				strcpy(((struct UserNode *) node)->un_RealName,(STRPTR) data);
																				break;
																			case USRFLD_UUCPName:
																				strcpy(((struct UserNode *) node)->un_UUCPName,(STRPTR) data);
																				break;
																			case USRFLD_Offset:
																				{
																					struct TagItem *tag;
																					if(tag=FindTagItem(((struct OffsetStruct *) data)->os_Type,((struct UserNode *) node)->un_Offsets))
																						tag->ti_Data=((struct OffsetStruct *) data)->os_Offset;
																				}
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
		ReleaseSemaphore(&UserSemaphore);
	}

void SaveUserIndex(BYTE *userindexfile)
	{
		BPTR fh;
		ObtainSemaphore(&UserSemaphore);
		if(fh=Open(userindexfile,MODE_NEWFILE))
			{
				{
					struct Node *node;
					for(node=UserList.lh_Head;node->ln_Succ;node=node->ln_Succ)
						{
							FPutC(fh,USRCMD_Begin);
							SaveUserField(fh,USRFLD_UserName,strlen(((struct UserNode *) node)->un_UserName)+1,((struct UserNode *) node)->un_UserName);
							SaveUserField(fh,USRFLD_RealName,strlen(((struct UserNode *) node)->un_RealName)+1,((struct UserNode *) node)->un_RealName);
							SaveUserField(fh,USRFLD_UUCPName,strlen(((struct UserNode *) node)->un_UUCPName)+1,((struct UserNode *) node)->un_UUCPName);
							{
								struct OffsetStruct os;
								struct TagItem *tstate,*tag;
								tstate=((struct UserNode *) node)->un_Offsets;
								while(tag=NextTagItem(&tstate))
									{
										if(tag->ti_Data!=(ULONG) ~0)
											{
												os.os_Type=tag->ti_Tag;
												os.os_Offset=tag->ti_Data;
												SaveUserField(fh,USRFLD_Offset,sizeof(struct OffsetStruct),&os);
											}
									}
							}
							FPutC(fh,USRCMD_End);
						}
				}
				Close(fh);
			}
		ReleaseSemaphore(&UserSemaphore);
	}

void UnLoadUserIndex()
	{
		ObtainSemaphore(&UserSemaphore);
		{
			struct Node *node;
			while(node=RemHead(&UserList))
				{
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
				}
		}
		ReleaseSemaphore(&UserSemaphore);
	}

BOOL LoadUserData(BYTE *userdatafile,struct Node *node,ULONG usertag)
	{
		BOOL ret=FALSE;
		ULONG pos;
		if(!GetTagData(usertag,0,((struct UserNode *) node)->un_Pointers))
			{
				if( (pos=GetTagData(usertag,(ULONG) ~0,((struct UserNode *) node)->un_Offsets)) != (ULONG) ~0 )
					{
						BPTR fh;
						if(fh=Open(userdatafile,MODE_OLDFILE))
							{
								Seek(fh,pos,OFFSET_BEGINNING);
								{
									BOOL kg=TRUE;
									LONG cmd;
									APTR data=NULL;
									while(kg)
										{
											cmd=FGetC(fh);
											switch(cmd)
												{
													case -1: /* EOF */
														kg=FALSE;
														break;
													case USRCMD_Begin:
														{
															ULONG size=0;
															switch(usertag)
																{
																	case UNTAG_UserInfo:
																		size=sizeof(struct UserInfo);
																		break;
																	case UNTAG_UserAccess:
																		size=sizeof(struct UserAccess);
																		break;
																	case UNTAG_UserPrefs:
																		size=sizeof(struct UserPrefs);
																		break;
																	case UNTAG_UserStats:
																		size=sizeof(struct UserStats);
																		break;
																}
															if(size)
																{
																	if(data=AllocVec(size,MEMF_CLEAR))
																		{
																			struct TagItem *tag;
																			if(tag=FindTagItem(usertag,((struct UserNode *) node)->un_Pointers))
																				tag->ti_Data=(ULONG) data;
																		}
																	else
																		kg=FALSE;
																}
															else
																kg=FALSE;
														}
														break;
													case USRCMD_End:
														ret=TRUE;
														kg=FALSE;
														break;
													case USRCMD_Field:
														if(data)
															{
																LONG field;
																UWORD size;
																if( ((field=FGetC(fh))!=-1) && (FRead(fh,&size,sizeof(UWORD),1)) )
																	{
																		APTR fielddata;
																		if(fielddata=AllocVec(size,MEMF_CLEAR))
																			{
																				if(FRead(fh,fielddata,size,1))
																					{
																						switch(usertag)
																							{
																								case UNTAG_UserInfo:
																									switch(field)
																										{
																											case USRFLD_Password:
																												strcpy(((struct UserInfo *) data)->ui_Password,(STRPTR) fielddata);
																												break;
																											case USRFLD_Address:
																												strcpy(((struct UserInfo *) data)->ui_Address,(STRPTR) fielddata);
																												break;
																											case USRFLD_City:
																												strcpy(((struct UserInfo *) data)->ui_City,(STRPTR) fielddata);
																												break;
																											case USRFLD_State:
																												strcpy(((struct UserInfo *) data)->ui_State,(STRPTR) fielddata);
																												break;
																											case USRFLD_Zip:
																												strcpy(((struct UserInfo *) data)->ui_Zip,(STRPTR) fielddata);
																												break;
																											case USRFLD_Country:
																												strcpy(((struct UserInfo *) data)->ui_Country,(STRPTR) fielddata);
																												break;
																											case USRFLD_PhoneCode:
																												strcpy(((struct UserInfo *) data)->ui_PhoneCode,(STRPTR) fielddata);
																												break;
																											case USRFLD_PhoneNumber:
																												strcpy(((struct UserInfo *) data)->ui_PhoneNumber,(STRPTR) fielddata);
																												break;
																											case USRFLD_BirthDay:
																												((struct UserInfo *) data)->ui_BirthDay=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_BirthMonth:
																												((struct UserInfo *) data)->ui_BirthMonth=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_BirthYear:
																												((struct UserInfo *) data)->ui_BirthYear=*((UWORD *) fielddata);
																												break;
																										}
																									break;
																								case UNTAG_UserAccess:
																									switch(field)
																										{
																											case USRFLD_AccessLevel:
																												((struct UserAccess *) data)->ua_AccessLevel=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_SessionTimeLimit:
																												((struct UserAccess *) data)->ua_SessionTimeLimit=*((UWORD *) fielddata);
																												break;
																											case USRFLD_InactivityTimeLimit:
																												((struct UserAccess *) data)->ua_InactivityTimeLimit=*((UWORD *) fielddata);
																												break;
																											case USRFLD_RatioByte:
																												((struct UserAccess *) data)->ua_RatioByte=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_RatioFile:
																												((struct UserAccess *) data)->ua_RatioFile=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_Days:
																												((struct UserAccess *) data)->ua_Days=*((ULONG *) fielddata);
																												break;
																										}
																									break;
																								case UNTAG_UserPrefs:
																									switch(field)
																										{
																											case USRFLD_ScreenWidth:
																												((struct UserPrefs *) data)->up_ScreenWidth=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_ScreenHeight:
																												((struct UserPrefs *) data)->up_ScreenHeight=*((UBYTE *) fielddata);
																												break;
																											case USRFLD_LogonStack:
																												strcpy(((struct UserPrefs *) data)->up_LogonStack,(STRPTR) fielddata);
																												break;
																											case USRFLD_Editor:
																												strcpy(((struct UserPrefs *) data)->up_Editor,(STRPTR) fielddata);
																												break;
																											case USRFLD_Protocol:
																												strcpy(((struct UserPrefs *) data)->up_Protocol,(STRPTR) fielddata);
																												break;
																											case USRFLD_Archiver:
																												strcpy(((struct UserPrefs *) data)->up_Archiver,(STRPTR) fielddata);
																												break;
																											case USRFLD_Flags:
																												((struct UserPrefs *) data)->up_Flags=*((ULONG *) fielddata);
																												break;
																										}
																									break;
																								case UNTAG_UserStats:
																									switch(field)
																										{
																											case USRFLD_Sessions:
																												((struct UserStats *) data)->us_Sessions=*((ULONG *) fielddata);
																												break;
																											case USRFLD_EmailRead:
																												((struct UserStats *) data)->us_EmailRead=*((ULONG *) fielddata);
																												break;
																											case USRFLD_EmailWritten:
																												((struct UserStats *) data)->us_EmailWritten=*((ULONG *) fielddata);
																												break;
																											case USRFLD_MessagesRead:
																												((struct UserStats *) data)->us_MessagesRead=*((ULONG *) fielddata);
																												break;
																											case USRFLD_MessagesWritten:
																												((struct UserStats *) data)->us_MessagesWritten=*((ULONG *) fielddata);
																												break;
																											case USRFLD_FilesUploaded:
																												((struct UserStats *) data)->us_FilesUploaded=*((ULONG *) fielddata);
																												break;
																											case USRFLD_FilesDownloaded:
																												((struct UserStats *) data)->us_FilesDownloaded=*((ULONG *) fielddata);
																												break;
																											case USRFLD_FilesCredits:
																												((struct UserStats *) data)->us_FilesCredits=*((ULONG *) fielddata);
																												break;
																											case USRFLD_KilosUploaded:
																												((struct UserStats *) data)->us_KilosUploaded=*((ULONG *) fielddata);
																												break;
																											case USRFLD_KilosDownloaded:
																												((struct UserStats *) data)->us_KilosDownloaded=*((ULONG *) fielddata);
																												break;
																											case USRFLD_KilosCredits:
																												((struct UserStats *) data)->us_KilosCredits=*((ULONG *) fielddata);
																												break;
																											case USRFLD_LastOn:
																												((struct UserStats *) data)->us_LastOn=*((struct DateStamp *) fielddata);
																												break;
																											case USRFLD_TimeUsed:
																												((struct UserStats *) data)->us_TimeUsed=*((UWORD *) fielddata);
																												break;
																										}
																									break;
																							}
																					}
																				else
																					kg=FALSE;
																				FreeVec(fielddata);
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
								}
								Close(fh);
							}
					}
				else
					{
						ULONG size=0;
						switch(usertag)
							{
								case UNTAG_UserInfo:
									size=sizeof(struct UserInfo);
									break;
								case UNTAG_UserAccess:
									size=sizeof(struct UserAccess);
									break;
								case UNTAG_UserPrefs:
									size=sizeof(struct UserPrefs);
									break;
								case UNTAG_UserStats:
									size=sizeof(struct UserStats);
									break;
							}
						if(size)
							{
								APTR data;
								if(data=AllocVec(size,MEMF_CLEAR))
									{
										struct TagItem *tag;
										if(tag=FindTagItem(usertag,((struct UserNode *) node)->un_Pointers))
											{
												tag->ti_Data=(ULONG) data;
												ret=TRUE;
											}
									}
							}
					}
			}
		return(ret);
	}

void SaveUserData(BYTE *userdatafile,ULONG usertag)
	{
		BPTR fh_in,fh_out;
		BYTE infile[64];
		sprintf(infile,"%s.bak",userdatafile);
		ObtainSemaphore(&UserSemaphore);
		Rename(userdatafile,infile);
		fh_in=Open(infile,MODE_OLDFILE);
		fh_out=Open(userdatafile,MODE_NEWFILE);
		if(fh_out)
			{
				struct UserNode *node;
				ULONG offset,pointer,outfilepos=0;
				for(node=(struct UserNode *) UserList.lh_Head;node->un_Node.ln_Succ;node=(struct UserNode *) node->un_Node.ln_Succ)
					{
						{
							offset=GetTagData(usertag,(ULONG) ~0,node->un_Offsets);
							pointer=GetTagData(usertag,0,node->un_Pointers);
						}
						if(pointer)
							{
								{
									struct TagItem *tag;
									if(tag=FindTagItem(usertag,node->un_Offsets))
										tag->ti_Data=outfilepos;
								}
								FPutC(fh_out,USRCMD_Begin);outfilepos++;
								switch(usertag)
									{
										case UNTAG_UserInfo:
											outfilepos+=SaveUserField(fh_out,USRFLD_Password,strlen(((struct UserInfo *) pointer)->ui_Password)+1,((struct UserInfo *) pointer)->ui_Password);
											outfilepos+=SaveUserField(fh_out,USRFLD_Address,strlen(((struct UserInfo *) pointer)->ui_Address)+1,((struct UserInfo *) pointer)->ui_Address);
											outfilepos+=SaveUserField(fh_out,USRFLD_City,strlen(((struct UserInfo *) pointer)->ui_City)+1,((struct UserInfo *) pointer)->ui_City);
											outfilepos+=SaveUserField(fh_out,USRFLD_State,strlen(((struct UserInfo *) pointer)->ui_State)+1,((struct UserInfo *) pointer)->ui_State);
											outfilepos+=SaveUserField(fh_out,USRFLD_Zip,strlen(((struct UserInfo *) pointer)->ui_Zip)+1,((struct UserInfo *) pointer)->ui_Zip);
											outfilepos+=SaveUserField(fh_out,USRFLD_Country,strlen(((struct UserInfo *) pointer)->ui_Country)+1,((struct UserInfo *) pointer)->ui_Country);
											outfilepos+=SaveUserField(fh_out,USRFLD_PhoneCode,strlen(((struct UserInfo *) pointer)->ui_PhoneCode)+1,((struct UserInfo *) pointer)->ui_PhoneCode);
											outfilepos+=SaveUserField(fh_out,USRFLD_PhoneNumber,strlen(((struct UserInfo *) pointer)->ui_PhoneNumber)+1,((struct UserInfo *) pointer)->ui_PhoneNumber);
											outfilepos+=SaveUserField(fh_out,USRFLD_BirthDay,sizeof(UBYTE),&(((struct UserInfo *) pointer)->ui_BirthDay));
											outfilepos+=SaveUserField(fh_out,USRFLD_BirthMonth,sizeof(UBYTE),&(((struct UserInfo *) pointer)->ui_BirthMonth));
											outfilepos+=SaveUserField(fh_out,USRFLD_BirthYear,sizeof(UWORD),&(((struct UserInfo *) pointer)->ui_BirthYear));
											break;
										case UNTAG_UserAccess:
											outfilepos+=SaveUserField(fh_out,USRFLD_AccessLevel,sizeof(UBYTE),&(((struct UserAccess *) pointer)->ua_AccessLevel));
											outfilepos+=SaveUserField(fh_out,USRFLD_SessionTimeLimit,sizeof(UWORD),&(((struct UserAccess *) pointer)->ua_SessionTimeLimit));
											outfilepos+=SaveUserField(fh_out,USRFLD_InactivityTimeLimit,sizeof(UWORD),&(((struct UserAccess *) pointer)->ua_InactivityTimeLimit));
											outfilepos+=SaveUserField(fh_out,USRFLD_RatioByte,sizeof(UBYTE),&(((struct UserAccess *) pointer)->ua_RatioByte));
											outfilepos+=SaveUserField(fh_out,USRFLD_RatioFile,sizeof(UBYTE),&(((struct UserAccess *) pointer)->ua_RatioFile));
											outfilepos+=SaveUserField(fh_out,USRFLD_Days,sizeof(ULONG),&(((struct UserAccess *) pointer)->ua_Days));
											break;
										case UNTAG_UserPrefs:
											outfilepos+=SaveUserField(fh_out,USRFLD_ScreenWidth,sizeof(UBYTE),&(((struct UserPrefs *) pointer)->up_ScreenWidth));
											outfilepos+=SaveUserField(fh_out,USRFLD_ScreenHeight,sizeof(UBYTE),&(((struct UserPrefs *) pointer)->up_ScreenHeight));
											outfilepos+=SaveUserField(fh_out,USRFLD_LogonStack,strlen(((struct UserPrefs *) pointer)->up_LogonStack)+1,((struct UserPrefs *) pointer)->up_LogonStack);
											outfilepos+=SaveUserField(fh_out,USRFLD_Editor,strlen(((struct UserPrefs *) pointer)->up_Editor)+1,((struct UserPrefs *) pointer)->up_Editor);
											outfilepos+=SaveUserField(fh_out,USRFLD_Protocol,strlen(((struct UserPrefs *) pointer)->up_Protocol)+1,((struct UserPrefs *) pointer)->up_Protocol);
											outfilepos+=SaveUserField(fh_out,USRFLD_Archiver,strlen(((struct UserPrefs *) pointer)->up_Archiver)+1,((struct UserPrefs *) pointer)->up_Archiver);
											outfilepos+=SaveUserField(fh_out,USRFLD_Flags,sizeof(ULONG),&(((struct UserPrefs *) pointer)->up_Flags));
											break;
										case UNTAG_UserStats:
											outfilepos+=SaveUserField(fh_out,USRFLD_Sessions,sizeof(ULONG),&(((struct UserStats *) pointer)->us_Sessions));
											outfilepos+=SaveUserField(fh_out,USRFLD_EmailRead,sizeof(ULONG),&(((struct UserStats *) pointer)->us_EmailRead));
											outfilepos+=SaveUserField(fh_out,USRFLD_EmailWritten,sizeof(ULONG),&(((struct UserStats *) pointer)->us_EmailWritten));
											outfilepos+=SaveUserField(fh_out,USRFLD_MessagesRead,sizeof(ULONG),&(((struct UserStats *) pointer)->us_MessagesRead));
											outfilepos+=SaveUserField(fh_out,USRFLD_MessagesWritten,sizeof(ULONG),&(((struct UserStats *) pointer)->us_MessagesWritten));
											outfilepos+=SaveUserField(fh_out,USRFLD_FilesUploaded,sizeof(ULONG),&(((struct UserStats *) pointer)->us_FilesUploaded));
											outfilepos+=SaveUserField(fh_out,USRFLD_FilesDownloaded,sizeof(ULONG),&(((struct UserStats *) pointer)->us_FilesDownloaded));
											outfilepos+=SaveUserField(fh_out,USRFLD_FilesCredits,sizeof(ULONG),&(((struct UserStats *) pointer)->us_FilesCredits));
											outfilepos+=SaveUserField(fh_out,USRFLD_KilosUploaded,sizeof(ULONG),&(((struct UserStats *) pointer)->us_KilosUploaded));
											outfilepos+=SaveUserField(fh_out,USRFLD_KilosDownloaded,sizeof(ULONG),&(((struct UserStats *) pointer)->us_KilosDownloaded));
											outfilepos+=SaveUserField(fh_out,USRFLD_KilosCredits,sizeof(ULONG),&(((struct UserStats *) pointer)->us_KilosCredits));
											outfilepos+=SaveUserField(fh_out,USRFLD_LastOn,sizeof(struct DateStamp),&(((struct UserStats *) pointer)->us_LastOn));
											outfilepos+=SaveUserField(fh_out,USRFLD_TimeUsed,sizeof(UWORD),&(((struct UserStats *) pointer)->us_TimeUsed));
											break;
									}
								FPutC(fh_out,USRCMD_End);outfilepos++;
							}
						else
							{
								if(offset!=~0 && fh_in)
									{
										{
											struct TagItem *tag;
											if(tag=FindTagItem(usertag,node->un_Offsets))
												tag->ti_Data=outfilepos;
										}
										Seek(fh_in,offset,OFFSET_BEGINNING);
										{
											BOOL kg=TRUE;
											LONG cmd;
											while(kg)
												{
													cmd=FGetC(fh_in);
													switch(cmd)
														{
															case -1: /* EOF */
																kg=FALSE;
																break;
															case USRCMD_Begin:
																FPutC(fh_out,cmd);outfilepos++;
																break;
															case USRCMD_End:
																FPutC(fh_out,cmd);outfilepos++;
																kg=FALSE;
																break;
															case USRCMD_Field:
																{
																	LONG field;
																	UWORD size;
																	if( ((field=FGetC(fh_in))!=-1) && (FRead(fh_in,&size,sizeof(UWORD),1)) )
																		{
																			APTR fielddata;
																			if(fielddata=AllocVec(size,MEMF_CLEAR))
																				{
																					if(FRead(fh_in,fielddata,size,1))
																						outfilepos+=SaveUserField(fh_out,field,size,fielddata);
																					else
																						kg=FALSE;
																					FreeVec(fielddata);
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
										}
									}
							}
					}
				if(fh_in) Close(fh_in);
				Close(fh_out);
				DeleteFile(infile);
			}
		else
			{
				if(fh_in) Close(fh_in);
				if(fh_out) Close(fh_out);
			}
		ReleaseSemaphore(&UserSemaphore);
	}

void UnLoadUserData(struct Node *node,ULONG usertag)
	{
		struct TagItem *tag;
		if(tag=FindTagItem(usertag,((struct UserNode *) node)->un_Pointers))
			{
				if(tag->ti_Data)
					{
						FreeVec((APTR) tag->ti_Data);
						tag->ti_Data=0;
					}
			}
	}

ULONG SaveUserField(BPTR fh,LONG field,UWORD size,APTR data)
	{
		ULONG ret=0;
		FPutC(fh,USRCMD_Field);ret++;
		FPutC(fh,field);ret++;
		FWrite(fh,&size,sizeof(UWORD),1);ret+=sizeof(UWORD);
		FWrite(fh,data,(ULONG) size,1);ret+=size;
		return(ret);
	}

BOOL IsTag(Tag *attrs,struct TagItem *tags_orig)
	{
		BOOL ret=FALSE;
		struct TagItem *tags;
		if(tags=CloneTagItems(tags_orig))
			{
				if(FilterTagItems(tags,attrs,TAGFILTER_AND))
					{
						struct TagItem *tstate,*tag;
						tstate=tags;
						while(tag=NextTagItem(&tstate))
							{
								ret=TRUE;
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

#include <intuition/intuition.h>
#include <proto/intuition.h>

void checkpoint(BYTE *s)
	{
		struct IntuitionBase *IntuitionBase;
		if(IntuitionBase=(struct IntuitionBase *) OpenLibrary("intuition.library",0))
			{
				struct EasyStruct es={ sizeof(struct EasyStruct),NULL,"wwbbs.library checkpoint","%s","Okay" };
				EasyRequest(NULL,&es,NULL,s);
				CloseLibrary((struct Library *) IntuitionBase);
			}
	}
