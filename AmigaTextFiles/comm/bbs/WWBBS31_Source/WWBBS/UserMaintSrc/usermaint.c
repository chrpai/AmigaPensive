#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/wwbbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/wwbbs.h>

#include "usermaint_rev.h"

char *ver=VERSTAG;

struct Library *WorldWideBase;

void close_all(void);

struct BufferNode {
	struct Node bn_Node;
	BYTE bn_Buffer[33];
};

void main()
	{
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			close_all();
		{
			BOOL savelist=FALSE;
			struct DateStamp CurrentDate;
			DateStamp(&CurrentDate);
			{
				BOOL onlyrealnames=FALSE;
				struct List BufferList;
				NewList(&BufferList);
				GetConfigTags(CFGTAG_Name,"System",SYSTAG_OnlyRealNames,&onlyrealnames,TAG_END);
				{
					ULONG next=0;
					BYTE *username=NULL,*realname=NULL;
					ULONG days=0;
					struct DateStamp laston={0};
					while(next=GetUserTags(USRTAG_UserName,&username,USRTAG_RealName,&realname,USRTAG_Next,next,
							USRTAG_Days,&days,USRTAG_LastOn,&laston,
							TAG_END))
						{
							if(days && laston.ds_Days)
								{
									if(laston.ds_Days+days<CurrentDate.ds_Days)
										{
											struct BufferNode *node;
											if(node=AllocVec(sizeof(struct BufferNode),MEMF_CLEAR))
												{
													strcpy(node->bn_Buffer,(onlyrealnames) ? realname : username);
													node->bn_Node.ln_Name=node->bn_Buffer;
													AddTail(&BufferList,(struct Node *) node);
												}
										}
								}
						}
				}
				{
					struct BufferNode *node;
					while(node=(struct BufferNode *) RemHead(&BufferList))
						{
							{
								char s[64];
								sprintf(s,"Deleted user `%s'.",node->bn_Buffer);
								LogEntry(NULL,"User Maintenance",s);
							}
							RemUserTags(USRTAG_Name,node->bn_Buffer,TAG_END);
							FreeVec(node);
						}
				}
			}
			if(savelist)
				SetUserTags(USRTAG_ForceSave,TRUE,TAG_END);
		}
		close_all();
	}

void close_all()
	{
		if(WorldWideBase) CloseLibrary(WorldWideBase);
		exit(RETURN_OK);
	}
