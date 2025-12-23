#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/wwbbs.h>

#include "messagemaint_rev.h"

char *ver=VERSTAG;

struct Library *WorldWideBase;

void close_all(void);
BOOL GetNextArea(BYTE *,BYTE *);

void main()
	{
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			close_all();
		{
			struct DateStamp CurrentDate;
			DateStamp(&CurrentDate);
			{
				BYTE path[256],area[33],groupname[21];
				ULONG days=0;
				strcpy(path,"MessageBases");
				strcpy(area,"");
				strcpy(groupname,"");
				while(GetNextArea(path,area))
					{
						if(GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,area,MBTAG_Days,&days,MBTAG_Group,groupname,TAG_END))
							{
								if(days)
									{
										APTR group;
										if(group=OpenMessageGroup(path,area,EXCLUSIVE_LOCK))
											{
												ULONG count=0;
												ULONG next=0;
												struct DateStamp *ds=NULL;
												while(next=GetNextMessage(group,next))
													{
														if(GetMessageTags(group,MSGTAG_ID,next,MSGTAG_Date,&ds,TAG_END))
															{
																if(ds->ds_Days+days<CurrentDate.ds_Days)
																	{
																		if(RemMessageTags(group,MSGTAG_ID,next,MSGTAG_DontSave,TRUE,TAG_END))
																			count++;
																	}
															}
													}
												if(count)
													SetMessageTags(group,MSGTAG_ForceSave,TRUE,TAG_END);
												CloseMessageGroup(group);
												if(count)
													{
														char s[64];
														sprintf(s,"`%ld' messages deleted from %s.",count,groupname);
														LogEntry(NULL,"Message Maintenance",s);
													}
											}
									}
							}
					}
			}
		}
		close_all();
	}

void close_all()
	{
		if(WorldWideBase) CloseLibrary(WorldWideBase);
		exit(RETURN_OK);
	}

BOOL GetNextArea(BYTE *path,BYTE *name)
	{
		BOOL ret=FALSE;
		if(strlen(name))
			{
				BYTE next[33];
				strcpy(next,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,name,CFGTAG_GetNext,next,TAG_END);
				if(strlen(next))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,next,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,next,255);
								strcpy(name,"");
								ret=GetNextArea(path,name);
							}
						else
							{
								strcpy(name,next);
								ret=TRUE;
							}
					}
				else
					{
						if(strchr(path,'/'))
							{
								char *p;
								strcpy(name,FilePart(path));
								p=PathPart(path);
								*p=NULL;
								ret=GetNextArea(path,name);
							}
					}
			}
		else
			{
				BYTE first[33];
				strcpy(first,"");
				GetConfigTags(CFGTAG_Path,path,CFGTAG_GetFirst,first,TAG_END);
				if(strlen(first))
					{
						BOOL child=FALSE;
						GetConfigTags(CFGTAG_Path,path,CFGTAG_Name,first,CFGTAG_Child,&child,TAG_END);
						if(child)
							{
								AddPart(path,first,255);
								ret=GetNextArea(path,name);
							}
						else
							{
								strcpy(name,first);
								ret=TRUE;
							}
					}
			}
		return(ret);
	}
