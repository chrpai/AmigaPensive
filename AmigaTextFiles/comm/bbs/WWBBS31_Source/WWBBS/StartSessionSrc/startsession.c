#include <exec/types.h>
#include <dos/dos.h>
#include <libraries/wwbbs.h>
#include <utility/tagitem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/wwbbs.h>

#include "Data:Programs/Include/wwbbs_commands.h"

#include "startsession_rev.h"

char *ver=VERSTAG;

struct Library *WorldWideBase;

char *template[]={
	"-BAUD",NULL };

ULONG send_message(BYTE *,BYTE *,UBYTE,struct TagItem *);

void main(int argc,char **argv)
	{
		ULONG baud=0;
		BYTE id[33];
		if(argc<2)
			exit(RETURN_FAIL);
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			exit(RETURN_FAIL);
		if(strlen(argv[1])>32)
			exit(RETURN_FAIL);
		strcpy(id,argv[1]);
		{
			int match=NULL,i;
			for(i=2;i<argc;i++)
				{
					switch(match)
						{
							case 0:
								{
									int j=0;
									while(template[j])
										{
											if(!stricmp(argv[i],template[j]))
												{
													match=j+1;
													break;
												}
											else
												j++;
										}
								}
								break;
							case 1:
								baud=atol(argv[i]);
								match=NULL;
								break;
						}
				}
		}
		{
			UBYTE nd_type=0;
			ULONG nd_baud=0;
			BOOL nd_frontend=FALSE;
			GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_Type,&nd_type,NDTAG_Baud,&nd_baud,NDTAG_FrontEnd,&nd_frontend,TAG_END);
			if(nd_type==NDTYP_Remote)
				{
					if(nd_frontend)
						{
							struct TagItem tags[2];
							tags[0].ti_Tag=NDSESSIONTAG_Baud;
							tags[0].ti_Data=(baud) ? baud : nd_baud;
							tags[1].ti_Tag=TAG_END;
							tags[1].ti_Data=0;
							if(!send_message("WWBBS Front End",id,NDCMD_Session,&tags[0]))
								printf("Unable to spawn node `%s'.\n",id);
						}
					else
						printf("Node `%s' is not configured to be a front end.\n");
				}
			else
				printf("Node `%s' is not a remote node.\n");
		}
		CloseLibrary(WorldWideBase);
		exit(RETURN_OK);
	}

ULONG send_message(BYTE *basename,BYTE *id,UBYTE cmd,struct TagItem *tags)
	{
		ULONG ret=NULL;
		BYTE name[64];
		struct MsgPort *port=NULL,*replyport;
		struct CommandMessage msg={0};
		if(id)
			sprintf(name,"%s.%s",basename,id);
		else
			strcpy(name,basename);
		if(replyport=CreateMsgPort())
			{
				msg.cm_Message.mn_Node.ln_Type=NT_MESSAGE;
				msg.cm_Message.mn_Length=sizeof(struct CommandMessage);
				msg.cm_Message.mn_ReplyPort=replyport;
				msg.cm_Command=cmd;
				msg.cm_Data=(ULONG) tags;
				Forbid();
				if(port=FindPort(name))
					PutMsg(port,(struct Message *) &msg);
				Permit();
				if(port)
					{
						WaitPort(replyport);
						if(GetMsg(replyport))
							ret=msg.cm_Data;
					}
				DeleteMsgPort(replyport);
			}
		return(ret);
	}
