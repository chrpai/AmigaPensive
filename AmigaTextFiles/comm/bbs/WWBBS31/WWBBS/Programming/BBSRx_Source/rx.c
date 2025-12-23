#include <exec/types.h>
#include <exec/exec.h>
#include <libraries/asl.h>
#include <libraries/wwbbs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <proto/asl.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <proto/rexxsyslib.h>
#include <proto/wwbbs.h>

#include "simplerexx.h"

#include "rx.h"

#include "bbsrx_rev.h"

char *ver=VERSTAG;

void set_rexx_vars(BYTE *,struct Message *);
BOOL FindCommand(struct Command *,BYTE *,UWORD *,UWORD *);
void close_all(void);

long __stack=20000;

void __regargs _CXBRK(void);

void __regargs _CXBRK(void)
	{
		panic=TRUE;
	}

void main(int argc,char **argv)
	{
		if(!(WorldWideBase=OpenLibrary("wwbbs.library",0)))
			close_all();
		if(!(RexxSysBase=OpenLibrary("rexxsyslib.library",0)))
			close_all();

		{
			struct RDArgs *rdargs;
			LONG argarray=NULL;
			if(rdargs=ReadArgs("File/A",&argarray,NULL))
				{
					BYTE id[33];
					if(GetVar("NODE",id,32,NULL)!=-1)
						{
email_current=0;
email_promptcurrent=0;
email_prompthigh=0;

strcpy(message_fullpath,"None");
strcpy(message_path,"MessageBases");
strcpy(message_area,"");
strcpy(message_group,"");
strcpy(message_frompattern,"");
strcpy(message_topattern,"");
strcpy(message_subjectpattern,"");
strcpy(message_textpattern,"");
message_current=0;
strcpy(message_read,"");
message_currentthread=0;
message_promptlowthread=0;
message_prompthighthread=0;
message_promptcurrentresponse=0;
message_prompthighresponse=0;

strcpy(file_fullpath,"None");
strcpy(file_path,"FileBases");
strcpy(file_area,"");
strcpy(file_group,"");
file_current=0;
file_high=0;
file_promptlow=0;
file_prompthigh=0;
strcpy(file_filepattern,"");
strcpy(file_descriptionpattern,"");
strcpy(file_uploaderpattern,"");

SetMode(Input(),1);
{
	AREXXCONTEXT RexxContext;
	if(RexxContext=InitARexx("WWBBS_Rx",NULL))
		{
			if(SendARexxMsg(RexxContext,(char *) argarray,FALSE))
				{
					ULONG mask;
					BOOL kg=TRUE;
					printf("~r");
					while(kg && (mask=ARexxSignal(RexxContext)))
						{
							Wait(mask);
							{
								struct RexxMsg *msg;
								char *error,*result,result_buff[256];
								long errorlevel=0;
								while(msg=GetARexxMsg(RexxContext))
									{
										if(msg==REXX_RETURN_ERROR || msg==REXX_RETURN_OK)
											kg=FALSE;
										else
											{
												error=NULL;result=NULL;errorlevel=0;
												set_rexx_vars(id,(struct Message *) msg);
												{
													UWORD group=0,cmd=0;
													BYTE buff[32],*arg=NULL;
													strcpy(buff,"");
													sscanf(ARG0(msg),"%31s",buff);
													if(arg=strchr(ARG0(msg),' '))
														arg++;
													if(FindCommand(cmds,buff,&group,&cmd))
														{
															switch(group)
																{
																	case GROUP_ABBEREXX:
																		ABBEREXXCommand(id,cmd,arg,result_buff,&result,&error,&errorlevel);
																		break;
																	case GROUP_System:
																		SystemCommand(id,cmd,arg,result_buff,&result,&error,&errorlevel);
																		break;
																	case GROUP_Email:
																		EmailCommand(id,cmd,arg);
																		break;
																	case GROUP_MessageBase:
																		MessageBaseCommand(id,cmd,arg);
																		break;
																	case GROUP_FileBase:
																		FileBaseCommand(id,cmd,arg);
																		break;
																}
														}
												}
												if(error)
													SetARexxLastError(RexxContext,msg,error);
												ReplyARexxMsg(RexxContext,msg,result,errorlevel);
											}
									}
							}
						}
				}
			FreeARexx(RexxContext);
		}
}
SetMode(Input(),0);

						}
					FreeArgs(rdargs);
				}
		}

		close_all();
	}

void set_rexx_vars(BYTE *id,struct Message *msg)
	{
		BOOL local=FALSE;
		UWORD timeremaining=0,timeused=0;
		GetStatusTags(STTAG_Name,id,STTAG_Local,&local,STTAG_TimeRemaining,&timeremaining,STTAG_TimeUsed,&timeused,TAG_END);
		{
			if(local)
				SetRexxVar(msg,"CARRIER","1",1);
			else
				SetRexxVar(msg,"CARRIER",(SerialCommandTags(id,SERCMD_Carrier,TAG_END)) ? "1" : "0",1);
		}
		{
			char s[6];
			sprintf(s,"%d",timeremaining);
			SetRexxVar(msg,"TIMELEFT",s,strlen(s));
		}
		{
			char s[6];
			sprintf(s,"%d",timeused);
			SetRexxVar(msg,"TIMEONLINE",s,strlen(s));
		}
		SetRexxVar(msg,"LOCAL",(local) ? "1" : "0",1);
	}

BOOL FindCommand(struct Command *cmds,BYTE *matchname,UWORD *group,UWORD *id)
	{
		BOOL ret=FALSE;
		int i=0;
		while(!ret && cmds[i].cmd_Name)
			{
				if(!stricmp(cmds[i].cmd_Name,matchname))
					{
						*group=cmds[i].cmd_Group;
						*id=cmds[i].cmd_ID;
						ret=TRUE;
					}
				else
					i++;
			}
		return(ret);
	}

void close_all()
	{
		if(RexxSysBase) CloseLibrary(RexxSysBase);
		if(WorldWideBase) CloseLibrary(WorldWideBase);
		exit(0);
	}
