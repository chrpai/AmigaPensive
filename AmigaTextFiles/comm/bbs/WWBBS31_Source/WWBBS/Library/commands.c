#include "wwbbs.h"

__asm
ULONG NodeCommand(register __a0 BYTE *id,register __d0 UBYTE cmd,register __a1 struct TagItem *tags)
	{
		ULONG ret=NULL;
		BYTE *dest=NULL;
		if(GetStatusTags(STTAG_Name,id,STTAG_Exists,TRUE,TAG_END))
			{
				BOOL local=FALSE;
				GetStatusTags(STTAG_Name,id,STTAG_Local,&local,TAG_END);
				dest=(local) ? "WWBBS Console Command" : "WWBBS Serial Command";
			}
		else
			dest="WWBBS Node";
		switch(cmd)
			{
				case NDCMD_Quit:
				case NDCMD_Panic:
					ret=DoCommandMessage(dest,id,cmd,tags);
					break;
				case NDCMD_Page:
					{
						BYTE *fromnode=NULL,*text=NULL;
						BYTE username[33];
						strcpy(username,"");
						if(tags)
							{
								struct TagItem *tag;
								if(tag=FindTagItem(NDPAGETAG_FromNode,tags))
									fromnode=(BYTE *) tag->ti_Data;
								if(tag=FindTagItem(NDPAGETAG_Text,tags))
									text=(BYTE *) tag->ti_Data;
							}
						GetStatusTags(STTAG_Name,fromnode,STTAG_UserName,username,TAG_END);
						{
							BYTE filename[64];
							BPTR fh;
							sprintf(filename,"T:wwbbs_page.%s",id);
							if(fh=Open(filename,MODE_READWRITE))
								{
									BYTE s[128];
									Seek(fh,0,OFFSET_END);
									FPutC(fh,'\n');
									sprintf(s,"Page from %s on node %s\n",username,fromnode);
									FPuts(fh,s);
									sprintf(s," %s\n",text);
									FPuts(fh,s);
									Close(fh);
									SetStatusTags(STTAG_Name,id,STTAG_PageReceived,TRUE,TAG_END);
									ret=SendCommandMessage(dest,id,cmd,NULL,0,NULL,0,TRUE);
								}
						}
					}
					break;
			}
		return(ret);
	}

ULONG NodeCommandTags(BYTE *id,UBYTE cmd,Tag tag,...)
	{
		return(NodeCommand(id,cmd,(struct TagItem *) &tag));
	}

__asm
ULONG ConsoleCommand(register __a0 BYTE *id,register __d0 UBYTE cmd,register __a1 struct TagItem *tags)
	{
		ULONG ret=NULL;
		switch(cmd)
			{
				case CONCMD_Quit:
				case CONCMD_Open:
				case CONCMD_Close:
				case CONCMD_Read:
				case CONCMD_Write:
				case CONCMD_Abort:
				case CONCMD_Query:
					ret=DoCommandMessage("WWBBS Console",id,cmd,tags);
					break;
				case CONCMD_Wait:
					{
						ULONG waitmask=NULL,*waitmaskret=NULL;
						if(tags)
							{
								struct TagItem *tag;
								if(tag=FindTagItem(CONWAITTAG_WaitMask,tags))
									waitmask=tag->ti_Data;
								if(tag=FindTagItem(CONWAITTAG_WaitMaskRet,tags))
									waitmaskret=(ULONG *) tag->ti_Data;
							}
						ret=SendCommandMessage("WWBBS Console",id,cmd,tags,waitmask,waitmaskret,CONCMD_Abort,FALSE);
					}
					break;
			}
		return(ret);
	}

ULONG ConsoleCommandTags(BYTE *id,UBYTE cmd,Tag tag,...)
	{
		return(ConsoleCommand(id,cmd,(struct TagItem *) &tag));
	}

__asm
ULONG SerialCommand(register __a0 BYTE *id,register __d0 UBYTE cmd,register __a1 struct TagItem *tags)
	{
		ULONG ret=NULL;
		switch(cmd)
			{
				case SERCMD_Quit:
				case SERCMD_Open:
				case SERCMD_Close:
				case SERCMD_Read:
				case SERCMD_Write:
				case SERCMD_Abort:
				case SERCMD_SetBaud:
				case SERCMD_Carrier:
				case SERCMD_Query:
					ret=DoCommandMessage("WWBBS Serial",id,cmd,tags);
					break;
				case SERCMD_Wait:
					{
						ULONG waitmask=NULL,*waitmaskret=NULL;
						if(tags)
							{
								struct TagItem *tag;
								if(tag=FindTagItem(SERWAITTAG_WaitMask,tags))
									waitmask=tag->ti_Data;
								if(tag=FindTagItem(SERWAITTAG_WaitMaskRet,tags))
									waitmaskret=(ULONG *) tag->ti_Data;
							}
						ret=SendCommandMessage("WWBBS Serial",id,cmd,tags,waitmask,waitmaskret,SERCMD_Abort,FALSE);
					}
					break;
			}
		return(ret);
	}

ULONG SerialCommandTags(BYTE *id,UBYTE cmd,Tag tag,...)
	{
		return(SerialCommand(id,cmd,(struct TagItem *) &tag));
	}

__asm
ULONG TimerCommand(register __a0 BYTE *id,register __d0 UBYTE cmd,register __a1 struct TagItem *tags)
	{
		ULONG ret=NULL;
		switch(cmd)
			{
				case TMRCMD_Quit:
				case TMRCMD_Start:
				case TMRCMD_Stop:
					ret=DoCommandMessage("WWBBS Timer",id,cmd,tags);
					break;
			}
		return(ret);
	}

ULONG TimerCommandTags(BYTE *id,UBYTE cmd,Tag tag,...)
	{
		return(TimerCommand(id,cmd,(struct TagItem *) &tag));
	}
