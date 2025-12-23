/*
**	$VER: start_rexx 37.0 (18.08.1998)
**
**	NOVIA interactive network services
**
**	(C) Copyright 1996-1998 Thorsten Gehler/UFoP
**	All Rights Reserved
*/

char ver[]="$VER: start_rexx 37.0 ("__DATE__" "__TIME__").";

#include <stdio.h>
#include <string.h>
#include <dos/dos.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <pragma/exec_lib.h>
#include <pragma/intuition_lib.h>
#include <pragma/dos_lib.h>
#include <novia/novia_message.h>
#include <novia/novia_types.h>

void PrintDosError(const LONG errorcode)
{
	char buffer[82];
	if (!Fault((LONG)errorcode,NULL,buffer,82))
		strcpy(buffer,"\n");
	printf("DOS-Error  %d: %s\n",errorcode,buffer);
}

void main(int argc, char **argv)
{
	LONG returncode=20;
	struct MsgPort *rexxport=0;
	struct MsgPort *clientport=0;
	struct MsgPort *replyport=CreateMsgPort();
	ULONG ClientID=0;
	char *ProgramName=0;
	char tempfile[50];
	char buffer[256];
	struct message msg;
	ULONG secs;
	ULONG mics;
	ULONG counter;
	BPTR	in=NULL,
			out=NULL;
/*
struct message
{
	struct 	Message msg;
	struct	message *ln_Succ;
	struct	message *ln_Pred;
	ULONG			IDNumber;
	ULONG			IPNumber;		// serial number of message
	ULONG			PortID;
	ULONG			msg_type;
	ULONG			cmd_type;		// if sending a command
	APTR			data;
	ULONG			data_len;		// length of data
};
*/
	if (argc>=3)
	{
		ProgramName=argv[2];
		if ((StrToLong(argv[1],(LONG *)&ClientID)>-1) && (strlen(ProgramName)>0))
		{
			sprintf(buffer,"NOVIAREXX%d");
			rexxport=FindPort(buffer);
			if (ClientID)
				sprintf(buffer,"NOVIACLIENT%d");
			else
				sprintf(buffer,"Novia_MainPort");
			clientport=FindPort(buffer);
			if (rexxport && clientport && replyport)
			{
				CurrentTime(&secs,&mics);
				sprintf(tempfile,"ram:novia_rexx.%d.%d",ClientID,mics);
				if ((in=Open(ProgramName,MODE_OLDFILE)) && (out=Open(tempfile,MODE_NEWFILE)))
				{
					Seek(in,0,OFFSET_END);
					counter=Seek(in,0,OFFSET_BEGINNING);
					if (counter!=-1)
					{
						char *iobuffer;
						if (iobuffer=AllocVec(counter+1,MEMF_ANY))
						{
							if (Read(in,iobuffer,counter)==counter)
							{
								sprintf(buffer,"/* NOVIA REXX PROGRAM */\naddress 'NOVIAREXX%d'\n",ClientID);
								if (Write(out,buffer,strlen(buffer))!=-1)
								{
									if (Write(out,iobuffer,counter)!=-1)
									{
										Close(in);in=NULL;
										Close(out);out=NULL;
										FreeVec(iobuffer);iobuffer=NULL;
										memset(&msg,0,sizeof(message));
										msg.msg.mn_ReplyPort=replyport;
										msg.msg.mn_Length=sizeof(message);
										msg.msg_type=MSG_NREXX_START;
										PutMsg(clientport,(Message *)&msg);
										WaitPort(replyport);
										if (msg.msg_type==MSG_NREXX_OK)
										{
											sprintf(buffer,"rx %s",tempfile);
											if (Execute(buffer,0,0))
											{
												msg.msg_type=MSG_NREXX_END;
												PutMsg(clientport,(Message *)&msg);
												WaitPort(replyport);
											}
											else
											{
												printf("rexx execute error, ");
												PrintDosError(IoErr());
											}
										}
										else
										{
											printf("client failait.\n");
										}
									}
									else
									{
										printf("can't write rexx.tempfile, ");
										PrintDosError(IoErr());
									}
								}
								else
								{
									printf("can't write rexx.tempfile, ");
									PrintDosError(IoErr());
								}
							}
							else
							{
								printf("can't read %s, ",ProgramName);
								PrintDosError(IoErr());
							}
							if (iobuffer)FreeVec(iobuffer);
						}
						else
							printf("can't read %s, not anouth memory\n",ProgramName);
					}
					else
					{
						printf("can't read %s, ",ProgramName);
						PrintDosError(IoErr());
					}
					sprintf(buffer,"delete %s quiet",tempfile);
					Execute(buffer,0,0);
				}
				else
				{
					if (!in)
					{
						printf("can't open %s, ",ProgramName);
						PrintDosError(IoErr());
					}
					else
					{
						printf("can't open %s, ",tempfile);
						PrintDosError(IoErr());
					}
				}
				if (in)Close(in);
				if (out)Close(out);
					
			}
			else
			{
				if (!replyport)printf("can't create rexx-replyport.\n");
				if (!clientport)printf("can't find clientport.\n");
				if (!rexxport)printf("can't find client-rexxport.\n");
			}		
		}
	}
	else
		printf("START_REXX ClientID ProgramName\n");
	if (replyport)DeleteMsgPort(replyport);
	Exit(returncode);
}





