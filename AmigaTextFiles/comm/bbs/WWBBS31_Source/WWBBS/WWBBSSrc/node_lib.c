#include <exec/types.h>
#include <exec/exec.h>
#include <dos/dostags.h>
#include <libraries/wwbbs.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wwbbs.h"

#include "wwbbs_settings.h"

#include "wwbbs_lib.h"

#include "node_lib.h"

#include "libs.h"

#include "Data:Programs/Include/wwbbs_commands.h"

void __saveds SetDoorVariables(struct Libraries *,BYTE *,ULONG);
void __saveds DeleteDoorVariables(struct Libraries *);

void __saveds Session(struct Libraries *libs,BYTE *id,BOOL local,ULONG baud)
	{
		if(StartProcess("WWBBS Console",ConsoleProc,20000,id))
			{
				if(StartProcess("WWBBS Timer",TimerProc,20000,id))
					{
						if(AddStatusTags(STTAG_Name,id,TAG_END))
							{
								SetStatusTags(STTAG_Name,id,
									STTAG_InactivityTimeLimit,3,
									STTAG_Local,local,
									TAG_END);
								if(local)
									ConsoleCommandTags(id,CONCMD_Open,TAG_END);
								SetDoorVariables(libs,id,baud);
								TimerCommandTags(id,TMRCMD_Start,TMRSTARTTAG_TimeRemaining,15,TMRSTARTTAG_TimeUsed,0,TAG_END);
								{
									BPTR fh;
									BYTE consolename[64];
									sprintf(consolename,"WWBBSAUX:%s",id);
									if(fh=Open(consolename,MODE_NEWFILE))
										{
											BYTE command[256];
											strcpy(command,"");
											GetConfigTags(CFGTAG_Path,"Nodes",CFGTAG_Name,id,NDTAG_Command,command,TAG_END);
											SystemTags(command,SYS_Input,fh,SYS_Output,NULL,TAG_END);
											Close(fh);
										}
								}
								TimerCommandTags(id,TMRCMD_Stop,TAG_END);
								DeleteDoorVariables(libs);
								RemStatusTags(STTAG_Name,id,TAG_END);
							}
						KillProcess("WWBBS Timer",id);
					}
				KillProcess("WWBBS Console",id);
			}
	}

void __saveds SetDoorVariables(struct Libraries *libs,BYTE *id,ULONG baud)
	{
		char buff[256];
		sprintf(buff,"%ld",baud);
		SetVar("BAUD",buff,-1,GVF_LOCAL_ONLY);
		SetVar("NODE",(id) ? id : "",-1,GVF_LOCAL_ONLY);
		SetVar("USERNAME","",-1,GVF_LOCAL_ONLY);
		SetVar("REALNAME","",-1,GVF_LOCAL_ONLY);
		SetVar("UUCPNAME","",-1,GVF_LOCAL_ONLY);
		SetVar("HOME","",-1,GVF_LOCAL_ONLY);
		SetVar("ACCESSLEVEL","",-1,GVF_LOCAL_ONLY);
		SetVar("BYTERATIO","",-1,GVF_LOCAL_ONLY);
		SetVar("FILERATIO","",-1,GVF_LOCAL_ONLY);
		SetVar("ROWS","",-1,GVF_LOCAL_ONLY);
		SetVar("COLUMNS","",-1,GVF_LOCAL_ONLY);
		SetVar("EDITOR","",-1,GVF_LOCAL_ONLY);
		SetVar("VISUAL","",-1,GVF_LOCAL_ONLY);
		SetVar("PROTOCOLSEND","",-1,GVF_LOCAL_ONLY);
		SetVar("PROTOCOLRECEIVE","",-1,GVF_LOCAL_ONLY);
		SetVar("ARCHIVERADD","",-1,GVF_LOCAL_ONLY);
		SetVar("ARCHIVEREXTRACT","",-1,GVF_LOCAL_ONLY);
		SetVar("ARCHIVERVIEW","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_PATH","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_AREA","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_ACCESSRANGE","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_GROUP","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_QUOTEHEADER","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_DAYS","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_READONLY","",-1,GVF_LOCAL_ONLY);
		SetVar("MESSAGEBASE_AUTOJOIN","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_PATH","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_AREA","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_ACCESSRANGE","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_GROUP","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_DIRECTORY","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_DISKSPACEREQUIRED","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_DAYS","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_READONLY","",-1,GVF_LOCAL_ONLY);
		SetVar("FILEBASE_AUTOJOIN","",-1,GVF_LOCAL_ONLY);
	}

void __saveds DeleteDoorVariables(struct Libraries *libs)
	{
		DeleteVar("BAUD",GVF_LOCAL_ONLY);
		DeleteVar("NODE",GVF_LOCAL_ONLY);
		DeleteVar("USERNAME",GVF_LOCAL_ONLY);
		DeleteVar("REALNAME",GVF_LOCAL_ONLY);
		DeleteVar("UUCPNAME",GVF_LOCAL_ONLY);
		DeleteVar("HOME",GVF_LOCAL_ONLY);
		DeleteVar("ACCESSLEVEL",GVF_LOCAL_ONLY);
		DeleteVar("BYTERATIO",GVF_LOCAL_ONLY);
		DeleteVar("FILERATIO",GVF_LOCAL_ONLY);
		DeleteVar("ROWS",GVF_LOCAL_ONLY);
		DeleteVar("COLUMNS",GVF_LOCAL_ONLY);
		DeleteVar("EDITOR",GVF_LOCAL_ONLY);
		DeleteVar("VISUAL",GVF_LOCAL_ONLY);
		DeleteVar("PROTOCOLSEND",GVF_LOCAL_ONLY);
		DeleteVar("PROTOCOLRECEIVE",GVF_LOCAL_ONLY);
		DeleteVar("ARCHIVERADD",GVF_LOCAL_ONLY);
		DeleteVar("ARCHIVEREXTRACT",GVF_LOCAL_ONLY);
		DeleteVar("ARCHIVERVIEW",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_PATH",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_AREA",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_ACCESSRANGE",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_GROUP",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_QUOTEHEADER",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_DAYS",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_READONLY",GVF_LOCAL_ONLY);
		DeleteVar("MESSAGEBASE_AUTOJOIN",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_PATH",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_AREA",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_ACCESSRANGE",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_GROUP",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_DIRECTORY",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_DISKSPACEREQUIRED",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_DAYS",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_READONLY",GVF_LOCAL_ONLY);
		DeleteVar("FILEBASE_AUTOJOIN",GVF_LOCAL_ONLY);
	}
