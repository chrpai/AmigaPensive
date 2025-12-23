#include "wwbbs.h"
#include "config.h"

struct Node *GetConfigNode(BYTE *path,BYTE *name,LONG accessmode)
	{
		struct List *list=NULL;
		struct Node *ret=NULL;
		BOOL error=FALSE;
		list=&ConfigList;
		ObtainSemaphoreShared(&ConfigSemaphore);
		if(path && strlen(path))
			{
				char buff[256],*p,*q;
				p=path;
				while(!error && p)
					{
						if(q=strchr(p,'/'))
							{
								strncpy(buff,p,q-p);
								buff[q-p]=NULL;
								p=q+1;
							}
						else
							{
								strcpy(buff,p);
								p=NULL;
							}
						if(list)
							{
								struct Node *node=NULL;
								if(node=FindName(list,buff))
									{
										ObtainSemaphoreShared(&(((struct ConfigNode *) node)->cn_Semaphore));
										list=((struct ConfigNode *) node)->cn_Child;
										ret=node;
									}
								else
									error=TRUE;
							}
						else
							error=TRUE;
					}
			}
		if(!error)
			{
				if(name && strlen(name))
					{
						struct Node *node=NULL;
						if(node=FindName(list,name))
							{
								switch(accessmode)
									{
										case EXCLUSIVE_LOCK:
											ObtainSemaphore(&(((struct ConfigNode *) node)->cn_Semaphore));
											break;
										case SHARED_LOCK:
										default:
											ObtainSemaphoreShared(&(((struct ConfigNode *) node)->cn_Semaphore));
											break;
									}
								ret=node;
							}
						else
							error=TRUE;
					}
				else
					error=TRUE;
			}
		if(error)
			{
				struct Node *node=NULL;
				node=ret;
				while(node)
					{
						ReleaseSemaphore(&(((struct ConfigNode *) node)->cn_Semaphore));
						node=((struct ConfigNode *) node)->cn_Parent;
					}
				ReleaseSemaphore(&ConfigSemaphore);
				ret=NULL;
			}
		return(ret);
	}

void FreeConfigNode(struct Node *node)
	{
		struct Node *wn;
		wn=node;
		while(wn)
			{
				ReleaseSemaphore(&(((struct ConfigNode *) wn)->cn_Semaphore));
				wn=((struct ConfigNode *) wn)->cn_Parent;
			}
		ReleaseSemaphore(&ConfigSemaphore);
	}

ULONG GetConfigFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0;
		struct TagItem *tags;
		Tag *tags_filter=NULL;
		if(tags=CloneTagItems(tags_orig))
			{
				switch(((struct ConfigNode *) node)->cn_Type)
					{
						case CFGTYP_AccessGroup:
							tags_filter=accessgroup_tags;
							break;
						case CFGTYP_Archiver:
							tags_filter=archiver_tags;
							break;
						case CFGTYP_Editor:
							tags_filter=editor_tags;
							break;
						case CFGTYP_FileBase:
							if(AttemptSemaphoreShared(&FileGroupSemaphore))
								tags_filter=filebase_tags;
							break;
						case CFGTYP_Menu:
							tags_filter=menu_tags;
							break;
						case CFGTYP_MessageBase:
							tags_filter=messagebase_tags;
							break;
						case CFGTYP_News:
							tags_filter=news_tags;
							break;
						case CFGTYP_Node:
							tags_filter=node_tags;
							break;
						case CFGTYP_Protocol:
							tags_filter=protocol_tags;
							break;
						case CFGTYP_System:
							tags_filter=system_tags;
							break;
					}
				if(tags_filter)
					{
						if(FilterTagItems(tags,tags_filter,TAGFILTER_AND))
							{
								struct TagItem *tstate,*tag;
								APTR data;
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(((struct ConfigNode *) node)->cn_Type)
											{
												case CFGTYP_AccessGroup:
													switch(tag->ti_Tag)
														{
															case AGTAG_AccessLevel:
																*((UBYTE *) data)=((struct AccessGroupNode *) node)->agn_AccessLevel;
																break;
															case AGTAG_SessionTimeLimit:
																*((UWORD *) data)=((struct AccessGroupNode *) node)->agn_SessionTimeLimit;
																break;
															case AGTAG_InactivityTimeLimit:
																*((UWORD *) data)=((struct AccessGroupNode *) node)->agn_InactivityTimeLimit;
																break;
															case AGTAG_RatioByte:
																*((UBYTE *) data)=((struct AccessGroupNode *) node)->agn_RatioByte;
																break;
															case AGTAG_RatioFile:
																*((UBYTE *) data)=((struct AccessGroupNode *) node)->agn_RatioFile;
																break;
															case AGTAG_Days:
																*((ULONG *) data)=((struct AccessGroupNode *) node)->agn_Days;
																break;
														}
													break;
												case CFGTYP_Archiver:
													switch(tag->ti_Tag)
														{
															case ARCTAG_Extension:
																strcpy((STRPTR) data,((struct ArchiverNode *) node)->an_Extension);
																break;
															case ARCTAG_AddCommand:
																strcpy((STRPTR) data,((struct ArchiverNode *) node)->an_AddCommand);
																break;
															case ARCTAG_ExtractCommand:
																strcpy((STRPTR) data,((struct ArchiverNode *) node)->an_ExtractCommand);
																break;
															case ARCTAG_ViewCommand:
																strcpy((STRPTR) data,((struct ArchiverNode *) node)->an_ViewCommand);
																break;
														}
													break;
												case CFGTYP_Editor:
													switch(tag->ti_Tag)
														{
															case EDTAG_Type:
																*((UBYTE *) data)=((struct EditorNode *) node)->en_Type;
																break;
															case EDTAG_Command:
																strcpy((STRPTR) data,((struct EditorNode *) node)->en_Command);
																break;
															case EDTAG_Stack:
																*((ULONG *) data)=((struct EditorNode *) node)->en_Stack;
																break;
														}
													break;
												case CFGTYP_FileBase:
													switch(tag->ti_Tag)
														{
															case FBTAG_AccessRange:
																strcpy((STRPTR) data,((struct FileBaseNode *) node)->fbn_AccessRange);
																break;
															case FBTAG_Group:
																strcpy((STRPTR) data,((struct FileBaseNode *) node)->fbn_Group);
																break;
															case FBTAG_Directory:
																strcpy((STRPTR) data,((struct FileBaseNode *) node)->fbn_Directory);
																break;
															case FBTAG_DiskSpaceRequired:
																*((ULONG *) data)=((struct FileBaseNode *) node)->fbn_DiskSpaceRequired;
																break;
															case FBTAG_Days:
																*((ULONG *) data)=((struct FileBaseNode *) node)->fbn_Days;
																break;
															case FBTAG_ReadOnly:
																*((BOOL *) data)=(((struct FileBaseNode *) node)->fbn_Flags & FBFLG_ReadOnly) ? TRUE : FALSE;
																break;
															case FBTAG_AutoJoin:
																*((BOOL *) data)=(((struct FileBaseNode *) node)->fbn_Flags & FBFLG_AutoJoin) ? TRUE : FALSE;
																break;
														}
													break;
												case CFGTYP_Menu:
													switch(tag->ti_Tag)
														{
															case MNTAG_AccessRange:
																strcpy((STRPTR) data,((struct MenuNode *) node)->mn_AccessRange);
																break;
															case MNTAG_Character:
																*((BYTE *) data)=((struct MenuNode *) node)->mn_Character;
																break;
															case MNTAG_Default:
																*((BYTE *) data)=((struct MenuNode *) node)->mn_Default;
																break;
															case MNTAG_Number:
																*((BYTE *) data)=((struct MenuNode *) node)->mn_Number;
																break;
															case MNTAG_Prompt:
																if(((struct ConfigNode *) node)->cn_Child)
																	strcpy((STRPTR) data,((struct MenuNode *) node)->mn_Buffer);
																else
																	strcpy((STRPTR) data,"");
																break;
															case MNTAG_Command:
																if(((struct ConfigNode *) node)->cn_Child)
																	strcpy((STRPTR) data,"");
																else
																	strcpy((STRPTR) data,((struct MenuNode *) node)->mn_Buffer);
																break;
														}
													break;
												case CFGTYP_MessageBase:
													switch(tag->ti_Tag)
														{
															case MBTAG_AccessRange:
																strcpy((STRPTR) data,((struct MessageBaseNode *) node)->mbn_AccessRange);
																break;
															case MBTAG_Group:
																strcpy((STRPTR) data,((struct MessageBaseNode *) node)->mbn_Group);
																break;
															case MBTAG_QuoteHeader:
																strcpy((STRPTR) data,((struct MessageBaseNode *) node)->mbn_QuoteHeader);
																break;
															case MBTAG_Days:
																*((ULONG *) data)=((struct MessageBaseNode *) node)->mbn_Days;
																break;
															case MBTAG_ReadOnly:
																*((BOOL *) data)=(((struct MessageBaseNode *) node)->mbn_Flags & MBFLG_ReadOnly) ? TRUE : FALSE;
																break;
															case MBTAG_AutoJoin:
																*((BOOL *) data)=(((struct MessageBaseNode *) node)->mbn_Flags & MBFLG_AutoJoin) ? TRUE : FALSE;
																break;
														}
													break;
												case CFGTYP_News:
													switch(tag->ti_Tag)
														{
															case NWSTAG_AccessRange:
																strcpy((STRPTR) data,((struct NewsNode *) node)->nn_AccessRange);
																break;
															case NWSTAG_File:
																strcpy((STRPTR) data,((struct NewsNode *) node)->nn_File);
																break;
														}
													break;
												case CFGTYP_Node:
													switch(tag->ti_Tag)
														{
															case NDTAG_AccessRange:
																strcpy((STRPTR) data,((struct NodeNode *) node)->nn_AccessRange);
																break;
															case NDTAG_Type:
																*((UBYTE *) data)=((struct NodeNode *) node)->nn_Type;
																break;
															case NDTAG_Device:
																strcpy((STRPTR) data,((struct NodeNode *) node)->nn_Device);
																break;
															case NDTAG_Unit:
																*((ULONG *) data)=((struct NodeNode *) node)->nn_Unit;
																break;
															case NDTAG_BufferSize:
																*((ULONG *) data)=((struct NodeNode *) node)->nn_BufferSize;
																break;
															case NDTAG_Baud:
																*((ULONG *) data)=((struct NodeNode *) node)->nn_Baud;
																break;
															case NDTAG_Handshaking:
																*((BOOL *) data)=(((struct NodeNode *) node)->nn_Flags & NDFLG_Handshaking) ? TRUE : FALSE;
																break;
															case NDTAG_AutoBaud:
																*((BOOL *) data)=(((struct NodeNode *) node)->nn_Flags & NDFLG_AutoBaud) ? TRUE : FALSE;
																break;
															case NDTAG_FrontEnd:
																*((BOOL *) data)=(((struct NodeNode *) node)->nn_Flags & NDFLG_FrontEnd) ? TRUE : FALSE;
																break;
															case NDTAG_Command:
																strcpy((STRPTR) data,((struct NodeNode *) node)->nn_Command);
																break;
														}
													break;
												case CFGTYP_Protocol:
													switch(tag->ti_Tag)
														{
															case PRTAG_SendCommand:
																strcpy((STRPTR) data,((struct ProtocolNode *) node)->pn_SendCommand);
																break;
															case PRTAG_ReceiveCommand:
																strcpy((STRPTR) data,((struct ProtocolNode *) node)->pn_ReceiveCommand);
																break;
															case PRTAG_Batch:
																*((BOOL *) data)=(((struct ProtocolNode *) node)->pn_Flags & PRFLG_Batch) ? TRUE : FALSE;
																break;
															case PRTAG_Bidirectional:
																*((BOOL *) data)=(((struct ProtocolNode *) node)->pn_Flags & PRFLG_Bidirectional) ? TRUE : FALSE;
																break;
														}
													break;
												case CFGTYP_System:
													switch(tag->ti_Tag)
														{
															case SYSTAG_InputColor:
																strcpy((STRPTR) data,((struct SystemNode *) node)->sn_InputColor);
																break;
															case SYSTAG_HeaderColor:
																strcpy((STRPTR) data,((struct SystemNode *) node)->sn_HeaderColor);
																break;
															case SYSTAG_OutputColor:
																strcpy((STRPTR) data,((struct SystemNode *) node)->sn_OutputColor);
																break;
															case SYSTAG_PromptColor:
																strcpy((STRPTR) data,((struct SystemNode *) node)->sn_PromptColor);
																break;
															case SYSTAG_SystemColor:
																strcpy((STRPTR) data,((struct SystemNode *) node)->sn_SystemColor);
																break;
															case SYSTAG_OnlyRealNames:
																*((BOOL *) data)=(((struct SystemNode *) node)->sn_Flags & SYSFLG_OnlyRealNames) ? TRUE : FALSE;
																break;
															case SYSTAG_FileIDDizExtractor:
																strcpy((STRPTR) data,((struct SystemNode *) node)->sn_FileIDDizExtractor);
																break;
														}
													break;
											}
										ret++;
									}
							}
						switch(((struct ConfigNode *) node)->cn_Type)
							{
								case CFGTYP_FileBase:
									ReleaseSemaphore(&FileGroupSemaphore);
									break;
							}
					}
				FreeTagItems(tags);
			}
		return(ret);
	}

ULONG SetConfigFields(struct Node *node,struct TagItem *tags_orig)
	{
		ULONG ret=0,retminus=0;
		struct TagItem *tags;
		Tag *tags_filter=NULL;
		if(tags=CloneTagItems(tags_orig))
			{
				switch(((struct ConfigNode *) node)->cn_Type)
					{
						case CFGTYP_AccessGroup:
							tags_filter=accessgroup_tags;
							break;
						case CFGTYP_Archiver:
							tags_filter=archiver_tags;
							break;
						case CFGTYP_Editor:
							tags_filter=editor_tags;
							break;
						case CFGTYP_FileBase:
							tags_filter=filebase_tags;
							break;
						case CFGTYP_Menu:
							tags_filter=menu_tags;
							break;
						case CFGTYP_MessageBase:
							tags_filter=messagebase_tags;
							break;
						case CFGTYP_News:
							tags_filter=news_tags;
							break;
						case CFGTYP_Node:
							tags_filter=node_tags;
							break;
						case CFGTYP_Protocol:
							tags_filter=protocol_tags;
							break;
						case CFGTYP_System:
							tags_filter=system_tags;
							break;
					}
				if(tags_filter)
					{
						if(FilterTagItems(tags,tags_filter,TAGFILTER_AND))
							{
								struct TagItem *tstate,*tag;
								APTR data;
								tstate=tags;
								while(tag=NextTagItem(&tstate))
									{
										data=(APTR) tag->ti_Data;
										switch(((struct ConfigNode *) node)->cn_Type)
											{
												case CFGTYP_AccessGroup:
													switch(tag->ti_Tag)
														{
															case AGTAG_AccessLevel:
																((struct AccessGroupNode *) node)->agn_AccessLevel=(UBYTE) data;
																break;
															case AGTAG_SessionTimeLimit:
																((struct AccessGroupNode *) node)->agn_SessionTimeLimit=(UWORD) data;
																break;
															case AGTAG_InactivityTimeLimit:
																((struct AccessGroupNode *) node)->agn_InactivityTimeLimit=(UWORD) data;
																break;
															case AGTAG_RatioByte:
																((struct AccessGroupNode *) node)->agn_RatioByte=(UBYTE) data;
																break;
															case AGTAG_RatioFile:
																((struct AccessGroupNode *) node)->agn_RatioFile=(UBYTE) data;
																break;
															case AGTAG_Days:
																((struct AccessGroupNode *) node)->agn_Days=(ULONG) data;
																break;
														}
													break;
												case CFGTYP_Archiver:
													switch(tag->ti_Tag)
														{
															case ARCTAG_Extension:
																if(data && strlen((STRPTR) data)<=3)
																	strcpy(((struct ArchiverNode *) node)->an_Extension,(STRPTR) data);
																else
																	retminus++;
																break;
															case ARCTAG_AddCommand:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct ArchiverNode *) node)->an_AddCommand,(STRPTR) data);
																else
																	retminus++;
																break;
															case ARCTAG_ExtractCommand:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct ArchiverNode *) node)->an_ExtractCommand,(STRPTR) data);
																else
																	retminus++;
																break;
															case ARCTAG_ViewCommand:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct ArchiverNode *) node)->an_ViewCommand,(STRPTR) data);
																else
																	retminus++;
																break;
														}
													break;
												case CFGTYP_Editor:
													switch(tag->ti_Tag)
														{
															case EDTAG_Type:
																((struct EditorNode *) node)->en_Type=(UBYTE) data;
																break;
															case EDTAG_Command:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct EditorNode *) node)->en_Command,(STRPTR) data);
																else
																	retminus++;
																break;
															case EDTAG_Stack:
																((struct EditorNode *) node)->en_Stack=(ULONG) data;
																break;
														}
													break;
												case CFGTYP_FileBase:
													switch(tag->ti_Tag)
														{
															case FBTAG_AccessRange:
																if(data && strlen((STRPTR) data)<=20)
																	strcpy(((struct FileBaseNode *) node)->fbn_AccessRange,(STRPTR) data);
																else
																	retminus++;
																break;
															case FBTAG_Group:
																if(data && strlen((STRPTR) data)<=40)
																	strcpy(((struct FileBaseNode *) node)->fbn_Group,(STRPTR) data);
																else
																	retminus++;
																break;
															case FBTAG_Directory:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct FileBaseNode *) node)->fbn_Directory,(STRPTR) data);
																else
																	retminus++;
																break;
															case FBTAG_DiskSpaceRequired:
																((struct FileBaseNode *) node)->fbn_DiskSpaceRequired=(ULONG) data;
																break;
															case FBTAG_Days:
																((struct FileBaseNode *) node)->fbn_Days=(ULONG) data;
																break;
															case FBTAG_ReadOnly:
																if((BOOL) data)
																	((struct FileBaseNode *) node)->fbn_Flags|=FBFLG_ReadOnly;
																else
																	((struct FileBaseNode *) node)->fbn_Flags&=~FBFLG_ReadOnly;
																break;
															case FBTAG_AutoJoin:
																if((BOOL) data)
																	((struct FileBaseNode *) node)->fbn_Flags|=FBFLG_AutoJoin;
																else
																	((struct FileBaseNode *) node)->fbn_Flags&=~FBFLG_AutoJoin;
																break;
														}
													break;
												case CFGTYP_Menu:
													switch(tag->ti_Tag)
														{
															case MNTAG_AccessRange:
																if(data && strlen((STRPTR) data)<=20)
																	strcpy(((struct MenuNode *) node)->mn_AccessRange,(STRPTR) data);
																else
																	retminus++;
																break;
															case MNTAG_Character:
																((struct MenuNode *) node)->mn_Character=(BYTE) data;
																break;
															case MNTAG_Default:
																((struct MenuNode *) node)->mn_Default=(BYTE) data;
																break;
															case MNTAG_Number:
																((struct MenuNode *) node)->mn_Number=(BYTE) data;
																break;
															case MNTAG_Prompt:
																if(((struct ConfigNode *) node)->cn_Child)
																	{
																		if(data && strlen((STRPTR) data)<=63)
																			strcpy(((struct MenuNode *) node)->mn_Buffer,(STRPTR) data);
																		else
																			retminus++;
																	}
																else
																	retminus++;
																break;
															case MNTAG_Command:
																if(!(((struct ConfigNode *) node)->cn_Child))
																	{
																		if(data && strlen((STRPTR) data)<=63)
																			strcpy(((struct MenuNode *) node)->mn_Buffer,(STRPTR) data);
																		else
																			retminus++;
																	}
																else
																	retminus++;
																break;
														}
													break;
												case CFGTYP_MessageBase:
													switch(tag->ti_Tag)
														{
															case MBTAG_AccessRange:
																if(data && strlen((STRPTR) data)<=20)
																	strcpy(((struct MessageBaseNode *) node)->mbn_AccessRange,(STRPTR) data);
																else
																	retminus++;
																break;
															case MBTAG_Group:
																if(data && strlen((STRPTR) data)<=20)
																	strcpy(((struct MessageBaseNode *) node)->mbn_Group,(STRPTR) data);
																else
																	retminus++;
																break;
															case MBTAG_QuoteHeader:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct MessageBaseNode *) node)->mbn_QuoteHeader,(STRPTR) data);
																else
																	retminus++;
																break;
															case MBTAG_Days:
																((struct MessageBaseNode *) node)->mbn_Days=(ULONG) data;
																break;
															case MBTAG_ReadOnly:
																if((BOOL) data)
																	((struct MessageBaseNode *) node)->mbn_Flags|=MBFLG_ReadOnly;
																else
																	((struct MessageBaseNode *) node)->mbn_Flags&=~MBFLG_ReadOnly;
																break;
															case MBTAG_AutoJoin:
																if((BOOL) data)
																	((struct MessageBaseNode *) node)->mbn_Flags|=MBFLG_AutoJoin;
																else
																	((struct MessageBaseNode *) node)->mbn_Flags&=~MBFLG_AutoJoin;
																break;
														}
													break;
												case CFGTYP_News:
													switch(tag->ti_Tag)
														{
															case NWSTAG_AccessRange:
																if(data && strlen((STRPTR) data)<=20)
																	strcpy(((struct NewsNode *) node)->nn_AccessRange,(STRPTR) data);
																else
																	retminus++;
																break;
															case NWSTAG_File:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct NewsNode *) node)->nn_File,(STRPTR) data);
																else
																	retminus++;
																break;
														}
													break;
												case CFGTYP_Node:
													switch(tag->ti_Tag)
														{
															case NDTAG_AccessRange:
																if(data && strlen((STRPTR) data)<=20)
																	strcpy(((struct NodeNode *) node)->nn_AccessRange,(STRPTR) data);
																else
																	retminus++;
																break;
															case NDTAG_Type:
																((struct NodeNode *) node)->nn_Type=(UBYTE) data;
																break;
															case NDTAG_Device:
																if(data && strlen((STRPTR) data)<=32)
																	strcpy(((struct NodeNode *) node)->nn_Device,(STRPTR) data);
																else
																	retminus++;
																break;
															case NDTAG_Unit:
																((struct NodeNode *) node)->nn_Unit=(ULONG) data;
																break;
															case NDTAG_BufferSize:
																((struct NodeNode *) node)->nn_BufferSize=(ULONG) data;
																break;
															case NDTAG_Baud:
																((struct NodeNode *) node)->nn_Baud=(ULONG) data;
																break;
															case NDTAG_Handshaking:
																if((BOOL) data)
																	((struct NodeNode *) node)->nn_Flags|=NDFLG_Handshaking;
																else
																	((struct NodeNode *) node)->nn_Flags&=~NDFLG_Handshaking;
																break;
															case NDTAG_AutoBaud:
																if((BOOL) data)
																	((struct NodeNode *) node)->nn_Flags|=NDFLG_AutoBaud;
																else
																	((struct NodeNode *) node)->nn_Flags&=~NDFLG_AutoBaud;
																break;
															case NDTAG_FrontEnd:
																if((BOOL) data)
																	((struct NodeNode *) node)->nn_Flags|=NDFLG_FrontEnd;
																else
																	((struct NodeNode *) node)->nn_Flags&=~NDFLG_FrontEnd;
																break;
															case NDTAG_Command:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct NodeNode *) node)->nn_Command,(STRPTR) data);
																else
																	retminus++;
																break;
														}
													break;
												case CFGTYP_Protocol:
													switch(tag->ti_Tag)
														{
															case PRTAG_SendCommand:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct ProtocolNode *) node)->pn_SendCommand,(STRPTR) data);
																else
																	retminus++;
																break;
															case PRTAG_ReceiveCommand:
																if(data && strlen((STRPTR) data)<=255)
																	strcpy(((struct ProtocolNode *) node)->pn_ReceiveCommand,(STRPTR) data);
																else
																	retminus++;
																break;
															case PRTAG_Batch:
																if((BOOL) data)
																	((struct ProtocolNode *) node)->pn_Flags|=PRFLG_Batch;
																else
																	((struct ProtocolNode *) node)->pn_Flags&=~PRFLG_Batch;
																break;
															case PRTAG_Bidirectional:
																if((BOOL) data)
																	((struct ProtocolNode *) node)->pn_Flags|=PRFLG_Bidirectional;
																else
																	((struct ProtocolNode *) node)->pn_Flags&=~PRFLG_Bidirectional;
																break;
														}
													break;
												case CFGTYP_System:
													switch(tag->ti_Tag)
														{
															case SYSTAG_InputColor:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct SystemNode *) node)->sn_InputColor,(STRPTR) data);
																else
																	retminus++;
																break;
															case SYSTAG_HeaderColor:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct SystemNode *) node)->sn_HeaderColor,(STRPTR) data);
																else
																	retminus++;
																break;
															case SYSTAG_OutputColor:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct SystemNode *) node)->sn_OutputColor,(STRPTR) data);
																else
																	retminus++;
																break;
															case SYSTAG_PromptColor:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct SystemNode *) node)->sn_PromptColor,(STRPTR) data);
																else
																	retminus++;
																break;
															case SYSTAG_SystemColor:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct SystemNode *) node)->sn_SystemColor,(STRPTR) data);
																else
																	retminus++;
																break;
															case SYSTAG_OnlyRealNames:
																if((BOOL) data)
																	((struct SystemNode *) node)->sn_Flags|=SYSFLG_OnlyRealNames;
																else
																	((struct SystemNode *) node)->sn_Flags&=~SYSFLG_OnlyRealNames;
																break;
															case SYSTAG_FileIDDizExtractor:
																if(data && strlen((STRPTR) data)<=80)
																	strcpy(((struct SystemNode *) node)->sn_FileIDDizExtractor,(STRPTR) data);
																else
																	retminus++;
																break;
														}
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

void LoadConfig(BYTE *configfile)
	{
		BPTR fh;
		ObtainSemaphore(&ConfigSemaphore);
		if(fh=Open(configfile,MODE_OLDFILE))
			{
				BOOL kg=TRUE;
				LONG cmd;
				struct List *list=NULL;
				struct Node *node=NULL,*parent=NULL;
				list=&ConfigList;
				while(kg)
					{
						cmd=FGetC(fh);
						switch(cmd)
							{
								case -1: /* EOF */
									kg=FALSE;
									break;
								case CFGCMD_Begin:
									{
										ULONG size=0;
										LONG typ;
										typ=FGetC(fh);
										switch(typ)
											{
												case -1: /* EOF */
													kg=FALSE;
													break;
												case CFGTYP_Config:
													size=sizeof(struct ConfigNode);
													break;
												case CFGTYP_AccessGroup:
													size=sizeof(struct AccessGroupNode);
													break;
												case CFGTYP_Archiver:
													size=sizeof(struct ArchiverNode);
													break;
												case DUMMY_DoorVariable:
													size=sizeof(struct ConfigNode);
													break;
												case CFGTYP_Editor:
													size=sizeof(struct EditorNode);
													break;
												case CFGTYP_FileBase:
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
												if(node=AllocVec(size,MEMF_CLEAR))
													{
														node->ln_Name=((struct ConfigNode *) node)->cn_Name;
														InitSemaphore(&(((struct ConfigNode *) node)->cn_Semaphore));
														((struct ConfigNode *) node)->cn_Type=typ;
														((struct ConfigNode *) node)->cn_Parent=parent;
													}
												else
													kg=FALSE;
											}
									}
									break;
								case CFGCMD_Child:
									if(node)
										{
											if(((struct ConfigNode *) node)->cn_Child=AllocVec(sizeof(struct List),MEMF_CLEAR))
												{
													NewList(((struct ConfigNode *) node)->cn_Child);
													list=((struct ConfigNode *) node)->cn_Child;
													parent=node;
												}
											else
												kg=FALSE;
										}
									break;
								case CFGCMD_End:
									if(node)
										AddTail(list,node);
									break;
								case CFGCMD_Field:
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
																	if(field==CFGFLD_Name)
																		strcpy(node->ln_Name,(STRPTR) data);
																	else
																		{
																			switch(((struct ConfigNode *) node)->cn_Type)
																				{
																					case CFGTYP_AccessGroup:
																						switch(field)
																							{
																								case AGFLD_AccessLevel:
																									((struct AccessGroupNode *) node)->agn_AccessLevel=*((UBYTE *) data);
																									break;
																								case AGFLD_SessionTimeLimit:
																									((struct AccessGroupNode *) node)->agn_SessionTimeLimit=*((UWORD *) data);
																									break;
																								case AGFLD_InactivityTimeLimit:
																									((struct AccessGroupNode *) node)->agn_InactivityTimeLimit=*((UWORD *) data);
																									break;
																								case AGFLD_RatioByte:
																									((struct AccessGroupNode *) node)->agn_RatioByte=*((UBYTE *) data);
																									break;
																								case AGFLD_RatioFile:
																									((struct AccessGroupNode *) node)->agn_RatioFile=*((UBYTE *) data);
																									break;
																								case AGFLD_Days:
																									((struct AccessGroupNode *) node)->agn_Days=*((ULONG *) data);
																									break;
																							}
																						break;
																					case CFGTYP_Archiver:
																						switch(field)
																							{
																								case ARCFLD_Extension:
																									strcpy(((struct ArchiverNode *) node)->an_Extension,(STRPTR) data);
																									break;
																								case ARCFLD_AddCommand:
																									strcpy(((struct ArchiverNode *) node)->an_AddCommand,(STRPTR) data);
																									break;
																								case ARCFLD_ExtractCommand:
																									strcpy(((struct ArchiverNode *) node)->an_ExtractCommand,(STRPTR) data);
																									break;
																								case ARCFLD_ViewCommand:
																									strcpy(((struct ArchiverNode *) node)->an_ViewCommand,(STRPTR) data);
																									break;
																							}
																						break;
																					case CFGTYP_Editor:
																						switch(field)
																							{
																								case EDFLD_Type:
																									((struct EditorNode *) node)->en_Type=*((UBYTE *) data);
																									break;
																								case EDFLD_Command:
																									strcpy(((struct EditorNode *) node)->en_Command,(STRPTR) data);
																									break;
																								case EDFLD_Stack:
																									((struct EditorNode *) node)->en_Stack=*((ULONG *) data);
																									break;
																							}
																						break;
																					case CFGTYP_FileBase:
																						switch(field)
																							{
																								case FBFLD_AccessRange:
																									strcpy(((struct FileBaseNode *) node)->fbn_AccessRange,(STRPTR) data);
																									break;
																								case FBFLD_Group:
																									strcpy(((struct FileBaseNode *) node)->fbn_Group,(STRPTR) data);
																									break;
																								case FBFLD_Directory:
																									strcpy(((struct FileBaseNode *) node)->fbn_Directory,(STRPTR) data);
																									break;
																								case FBFLD_DiskSpaceRequired:
																									((struct FileBaseNode *) node)->fbn_DiskSpaceRequired=*((ULONG *) data);
																									break;
																								case FBFLD_Days:
																									((struct FileBaseNode *) node)->fbn_Days=*((ULONG *) data);
																									break;
																								case FBFLD_Flags:
																									((struct FileBaseNode *) node)->fbn_Flags=*((ULONG *) data);
																									break;
																							}
																						break;
																					case CFGTYP_Menu:
																						switch(field)
																							{
																								case MNFLD_AccessRange:
																									strcpy(((struct MenuNode *) node)->mn_AccessRange,(STRPTR) data);
																									break;
																								case MNFLD_Character:
																									((struct MenuNode *) node)->mn_Character=*((BYTE *) data);
																									break;
																								case MNFLD_Default:
																									((struct MenuNode *) node)->mn_Default=*((BYTE *) data);
																									break;
																								case MNFLD_Number:
																									((struct MenuNode *) node)->mn_Number=*((BYTE *) data);
																									break;
																								case MNFLD_Buffer:
																									strcpy(((struct MenuNode *) node)->mn_Buffer,(STRPTR) data);
																									break;
																							}
																						break;
																					case CFGTYP_MessageBase:
																						switch(field)
																							{
																								case MBFLD_AccessRange:
																									strcpy(((struct MessageBaseNode *) node)->mbn_AccessRange,(STRPTR) data);
																									break;
																								case MBFLD_Group:
																									strcpy(((struct MessageBaseNode *) node)->mbn_Group,(STRPTR) data);
																									break;
																								case MBFLD_QuoteHeader:
																									strcpy(((struct MessageBaseNode *) node)->mbn_QuoteHeader,(STRPTR) data);
																									break;
																								case MBFLD_Flags:
																									((struct MessageBaseNode *) node)->mbn_Flags=*((ULONG *) data);
																									break;
																								case MBFLD_Days:
																									((struct MessageBaseNode *) node)->mbn_Days=*((ULONG *) data);
																									break;
																							}
																						break;
																					case CFGTYP_News:
																						switch(field)
																							{
																								case NWSFLD_AccessRange:
																									strcpy(((struct NewsNode *) node)->nn_AccessRange,(STRPTR) data);
																									break;
																								case NWSFLD_File:
																									strcpy(((struct NewsNode *) node)->nn_File,(STRPTR) data);
																									break;
																							}
																						break;
																					case CFGTYP_Node:
																						switch(field)
																							{
																								case NDFLD_AccessRange:
																									strcpy(((struct NodeNode *) node)->nn_AccessRange,(STRPTR) data);
																									break;
																								case NDFLD_Type:
																									((struct NodeNode *) node)->nn_Type=*((UBYTE *) data);
																									break;
																								case NDFLD_Device:
																									strcpy(((struct NodeNode *) node)->nn_Device,(STRPTR) data);
																									break;
																								case NDFLD_Unit:
																									((struct NodeNode *) node)->nn_Unit=*((ULONG *) data);
																									break;
																								case NDFLD_BufferSize:
																									((struct NodeNode *) node)->nn_BufferSize=*((ULONG *) data);
																									break;
																								case NDFLD_Baud:
																									((struct NodeNode *) node)->nn_Baud=*((ULONG *) data);
																									break;
																								case NDFLD_Flags:
																									((struct NodeNode *) node)->nn_Flags=*((ULONG *) data);
																									break;
																								case NDFLD_Command:
																									strcpy(((struct NodeNode *) node)->nn_Command,(STRPTR) data);
																									break;
																							}
																						break;
																					case CFGTYP_Protocol:
																						switch(field)
																							{
																								case PRFLD_SendCommand:
																									strcpy(((struct ProtocolNode *) node)->pn_SendCommand,(STRPTR) data);
																									break;
																								case PRFLD_ReceiveCommand:
																									strcpy(((struct ProtocolNode *) node)->pn_ReceiveCommand,(STRPTR) data);
																									break;
																								case PRFLD_Flags:
																									((struct ProtocolNode *) node)->pn_Flags=*((ULONG *) data);
																									break;
																							}
																						break;
																					case CFGTYP_System:
																						switch(field)
																							{
																								case SYSFLD_InputColor:
																									strcpy(((struct SystemNode *) node)->sn_InputColor,(STRPTR) data);
																									break;
																								case SYSFLD_HeaderColor:
																									strcpy(((struct SystemNode *) node)->sn_HeaderColor,(STRPTR) data);
																									break;
																								case SYSFLD_OutputColor:
																									strcpy(((struct SystemNode *) node)->sn_OutputColor,(STRPTR) data);
																									break;
																								case SYSFLD_PromptColor:
																									strcpy(((struct SystemNode *) node)->sn_PromptColor,(STRPTR) data);
																									break;
																								case SYSFLD_SystemColor:
																									strcpy(((struct SystemNode *) node)->sn_SystemColor,(STRPTR) data);
																									break;
																								case SYSFLD_Flags:
																									((struct SystemNode *) node)->sn_Flags=*((ULONG *) data);
																									break;
																								case SYSFLD_FileIDDizExtractor:
																									strcpy(((struct SystemNode *) node)->sn_FileIDDizExtractor,(STRPTR) data);
																									break;
																							}
																						break;
																				}
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
								case CFGCMD_Parent:
									if(parent)
										{
											parent=((struct ConfigNode *) parent)->cn_Parent;
											if(parent)
												list=((struct ConfigNode *) parent)->cn_Child;
											else
												list=&ConfigList;
										}
									break;
							}
					}
				Close(fh);
				ReleaseSemaphore(&ConfigSemaphore);
			}
		else
			{
				ReleaseSemaphore(&ConfigSemaphore);
				AddConfigTags(
					CFGTAG_Name,"AccessGroups",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"Archivers",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"Editors",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"FileBases",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"Menus",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"MessageBases",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"News",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"Nodes",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"Protocols",
					CFGTAG_Type,CFGTYP_Config,
					CFGTAG_DontSave,TRUE,
					TAG_END);
				AddConfigTags(
					CFGTAG_Name,"System",
					CFGTAG_Type,CFGTYP_System,
					CFGTAG_DontSave,TRUE,
					TAG_END);
			}
	}

void SaveConfig(BYTE *configfile)
	{
		BPTR fh;
		ObtainSemaphore(&ConfigSemaphore);
		if(fh=Open(configfile,MODE_NEWFILE))
			{
				SaveConfigList(fh,&ConfigList);
				Close(fh);
			}
		ReleaseSemaphore(&ConfigSemaphore);
	}

void SaveConfigList(BPTR fh,struct List *list)
	{
		struct Node *node;
		for(node=list->lh_Head;node->ln_Succ;node=node->ln_Succ)
			{
				switch(((struct ConfigNode *) node)->cn_Type)
					{
						case DUMMY_DoorVariable:
							break;
						default:
							SaveConfigNode(fh,node);
					}
				if(((struct ConfigNode *) node)->cn_Child)
					{
						FPutC(fh,CFGCMD_Child);
						SaveConfigList(fh,((struct ConfigNode *) node)->cn_Child);
						FPutC(fh,CFGCMD_Parent);
					}
			}
	}

void SaveConfigNode(BPTR fh,struct Node *node)
	{
		/* Begin Node */
		FPutC(fh,CFGCMD_Begin);

		/* Node Type */
		FPutC(fh,(LONG) ((struct ConfigNode *) node)->cn_Type);

		/* Name Field */
		SaveConfigField(fh,CFGFLD_Name,strlen(node->ln_Name)+1,node->ln_Name);

		switch(((struct ConfigNode *) node)->cn_Type)
			{
				case CFGTYP_AccessGroup:
					SaveConfigField(fh,AGFLD_AccessLevel,sizeof(UBYTE),&(((struct AccessGroupNode *) node)->agn_AccessLevel));
					SaveConfigField(fh,AGFLD_SessionTimeLimit,sizeof(UWORD),&(((struct AccessGroupNode *) node)->agn_SessionTimeLimit));
					SaveConfigField(fh,AGFLD_InactivityTimeLimit,sizeof(UWORD),&(((struct AccessGroupNode *) node)->agn_InactivityTimeLimit));
					SaveConfigField(fh,AGFLD_RatioByte,sizeof(UBYTE),&(((struct AccessGroupNode *) node)->agn_RatioByte));
					SaveConfigField(fh,AGFLD_RatioFile,sizeof(UBYTE),&(((struct AccessGroupNode *) node)->agn_RatioFile));
					SaveConfigField(fh,AGFLD_Days,sizeof(ULONG),&(((struct AccessGroupNode *) node)->agn_Days));
					break;
				case CFGTYP_Archiver:
					SaveConfigField(fh,ARCFLD_Extension,strlen(((struct ArchiverNode *) node)->an_Extension)+1,((struct ArchiverNode *) node)->an_Extension);
					SaveConfigField(fh,ARCFLD_AddCommand,strlen(((struct ArchiverNode *) node)->an_AddCommand)+1,((struct ArchiverNode *) node)->an_AddCommand);
					SaveConfigField(fh,ARCFLD_ExtractCommand,strlen(((struct ArchiverNode *) node)->an_ExtractCommand)+1,((struct ArchiverNode *) node)->an_ExtractCommand);
					SaveConfigField(fh,ARCFLD_ViewCommand,strlen(((struct ArchiverNode *) node)->an_ViewCommand)+1,((struct ArchiverNode *) node)->an_ViewCommand);
					break;
				case CFGTYP_Editor:
					SaveConfigField(fh,EDFLD_Type,sizeof(UBYTE),&(((struct EditorNode *) node)->en_Type));
					SaveConfigField(fh,EDFLD_Command,strlen(((struct EditorNode *) node)->en_Command)+1,((struct EditorNode *) node)->en_Command);
					SaveConfigField(fh,EDFLD_Stack,sizeof(ULONG),&(((struct EditorNode *) node)->en_Stack));
					break;
				case CFGTYP_FileBase:
					SaveConfigField(fh,FBFLD_AccessRange,strlen(((struct FileBaseNode *) node)->fbn_AccessRange)+1,((struct FileBaseNode *) node)->fbn_AccessRange);
					SaveConfigField(fh,FBFLD_Group,strlen(((struct FileBaseNode *) node)->fbn_Group)+1,((struct FileBaseNode *) node)->fbn_Group);
					SaveConfigField(fh,FBFLD_Directory,strlen(((struct FileBaseNode *) node)->fbn_Directory)+1,((struct FileBaseNode *) node)->fbn_Directory);
					SaveConfigField(fh,FBFLD_DiskSpaceRequired,sizeof(ULONG),&(((struct FileBaseNode *) node)->fbn_DiskSpaceRequired));
					SaveConfigField(fh,FBFLD_Days,sizeof(ULONG),&(((struct FileBaseNode *) node)->fbn_Days));
					SaveConfigField(fh,FBFLD_Flags,sizeof(ULONG),&(((struct FileBaseNode *) node)->fbn_Flags));
					break;
				case CFGTYP_Menu:
					SaveConfigField(fh,MNFLD_AccessRange,strlen(((struct MenuNode *) node)->mn_AccessRange)+1,((struct MenuNode *) node)->mn_AccessRange);
					SaveConfigField(fh,MNFLD_Character,sizeof(BYTE),&(((struct MenuNode *) node)->mn_Character));
					SaveConfigField(fh,MNFLD_Default,sizeof(BYTE),&(((struct MenuNode *) node)->mn_Default));
					SaveConfigField(fh,MNFLD_Number,sizeof(BYTE),&(((struct MenuNode *) node)->mn_Number));
					SaveConfigField(fh,MNFLD_Buffer,strlen(((struct MenuNode *) node)->mn_Buffer)+1,((struct MenuNode *) node)->mn_Buffer);
					break;
				case CFGTYP_MessageBase:
					SaveConfigField(fh,MBFLD_AccessRange,strlen(((struct MessageBaseNode *) node)->mbn_AccessRange)+1,((struct MessageBaseNode *) node)->mbn_AccessRange);
					SaveConfigField(fh,MBFLD_Group,strlen(((struct MessageBaseNode *) node)->mbn_Group)+1,((struct MessageBaseNode *) node)->mbn_Group);
					SaveConfigField(fh,MBFLD_QuoteHeader,strlen(((struct MessageBaseNode *) node)->mbn_QuoteHeader)+1,((struct MessageBaseNode *) node)->mbn_QuoteHeader);
					SaveConfigField(fh,MBFLD_Flags,sizeof(ULONG),&(((struct MessageBaseNode *) node)->mbn_Flags));
					SaveConfigField(fh,MBFLD_Days,sizeof(ULONG),&(((struct MessageBaseNode *) node)->mbn_Days));
					break;
				case CFGTYP_News:
					SaveConfigField(fh,NWSFLD_AccessRange,strlen(((struct NewsNode *) node)->nn_AccessRange)+1,((struct NewsNode *) node)->nn_AccessRange);
					SaveConfigField(fh,NWSFLD_File,strlen(((struct NewsNode *) node)->nn_File)+1,((struct NewsNode *) node)->nn_File);
					break;
				case CFGTYP_Node:
					SaveConfigField(fh,NDFLD_AccessRange,strlen(((struct NodeNode *) node)->nn_AccessRange)+1,((struct NodeNode *) node)->nn_AccessRange);
					SaveConfigField(fh,NDFLD_Type,sizeof(UBYTE),&(((struct NodeNode *) node)->nn_Type));
					SaveConfigField(fh,NDFLD_Device,strlen(((struct NodeNode *) node)->nn_Device)+1,((struct NodeNode *) node)->nn_Device);
					SaveConfigField(fh,NDFLD_Unit,sizeof(ULONG),&(((struct NodeNode *) node)->nn_Unit));
					SaveConfigField(fh,NDFLD_BufferSize,sizeof(ULONG),&(((struct NodeNode *) node)->nn_BufferSize));
					SaveConfigField(fh,NDFLD_Baud,sizeof(ULONG),&(((struct NodeNode *) node)->nn_Baud));
					SaveConfigField(fh,NDFLD_Flags,sizeof(ULONG),&(((struct NodeNode *) node)->nn_Flags));
					SaveConfigField(fh,NDFLD_Command,strlen(((struct NodeNode *) node)->nn_Command)+1,((struct NodeNode *) node)->nn_Command);
					break;
				case CFGTYP_Protocol:
					SaveConfigField(fh,PRFLD_SendCommand,strlen(((struct ProtocolNode *) node)->pn_SendCommand)+1,((struct ProtocolNode *) node)->pn_SendCommand);
					SaveConfigField(fh,PRFLD_ReceiveCommand,strlen(((struct ProtocolNode *) node)->pn_ReceiveCommand)+1,((struct ProtocolNode *) node)->pn_ReceiveCommand);
					SaveConfigField(fh,PRFLD_Flags,sizeof(ULONG),&(((struct ProtocolNode *) node)->pn_Flags));
					break;
				case CFGTYP_System:
					SaveConfigField(fh,SYSFLD_InputColor,strlen(((struct SystemNode *) node)->sn_InputColor)+1,((struct SystemNode *) node)->sn_InputColor);
					SaveConfigField(fh,SYSFLD_HeaderColor,strlen(((struct SystemNode *) node)->sn_HeaderColor)+1,((struct SystemNode *) node)->sn_HeaderColor);
					SaveConfigField(fh,SYSFLD_OutputColor,strlen(((struct SystemNode *) node)->sn_OutputColor)+1,((struct SystemNode *) node)->sn_OutputColor);
					SaveConfigField(fh,SYSFLD_PromptColor,strlen(((struct SystemNode *) node)->sn_PromptColor)+1,((struct SystemNode *) node)->sn_PromptColor);
					SaveConfigField(fh,SYSFLD_SystemColor,strlen(((struct SystemNode *) node)->sn_SystemColor)+1,((struct SystemNode *) node)->sn_SystemColor);
					SaveConfigField(fh,SYSFLD_Flags,sizeof(ULONG),&(((struct SystemNode *) node)->sn_Flags));
					SaveConfigField(fh,SYSFLD_FileIDDizExtractor,strlen(((struct SystemNode *) node)->sn_FileIDDizExtractor)+1,((struct SystemNode *) node)->sn_FileIDDizExtractor);
					break;
			}

		/* End Node */
		FPutC(fh,CFGCMD_End);
	}

void SaveConfigField(BPTR fh,LONG field,UWORD size,APTR data)
	{
		FPutC(fh,CFGCMD_Field);
		FPutC(fh,field);
		FWrite(fh,&size,sizeof(UWORD),1);
		FWrite(fh,data,(ULONG) size,1);
	}

void UnLoadConfig()
	{
		ObtainSemaphore(&ConfigSemaphore);
		UnLoadConfigList(&ConfigList);
		ReleaseSemaphore(&ConfigSemaphore);
	}

void UnLoadConfigList(struct List *list)
	{
		struct Node *node;
		while(node=RemHead(list))
			{
				if(((struct ConfigNode *) node)->cn_Child)
					{
						UnLoadConfigList(((struct ConfigNode *) node)->cn_Child);
						FreeVec(((struct ConfigNode *) node)->cn_Child);
					}
				FreeVec(node);
			}
	}
