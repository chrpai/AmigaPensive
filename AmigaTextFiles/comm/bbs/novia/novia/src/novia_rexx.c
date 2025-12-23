#include <novia/novia_global.h>
#include <novia/novia_misc.h>
#include <novia/novia_funcs.h>
#include <dos/dostags.h>
#include <rexx/storage.h>
#include <clib/rexxsyslib_protos.h>
#include <pragma/noviasys_lib.h>

extern struct MainPortConfig *mainportconfig;


/****   PROTOTYPES   ****/

struct Task *startrexxprg(char *ProgramName);
void rexx_main();
void DoRexxMsg(RexxMsg *rexxmsg);

/************************/

UserData *uuuu=0;

struct Task *startrexxprg(char *ProgramName)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct Task *RexxTask=NULL;
	struct RexxProg *rexxprog=NULL;
	char buffer[200];
	char error=FALSE;
	BPTR in=NULL;
	BPTR out=NULL;
	if ((rexxprog=AllocVec(sizeof(RexxProg),MEMF_ANY|MEMF_CLEAR)))
	{
		strcpy(rexxprog->ProgramName,ProgramName);
		Forbid();
		mainportconfig->rexxtempcounter++;
		sprintf(rexxprog->TaskName,"novia_rexx_task%u",mainportconfig->rexxtempcounter);
		sprintf(rexxprog->tempfile,"ram:novia_rexx_prog.%d",mainportconfig->rexxtempcounter);
		Permit();
		if ((in=Open(ProgramName,MODE_OLDFILE)) && (out=Open(rexxprog->tempfile,MODE_NEWFILE)))
		{
			ULONG counter;
			Seek(in,0,OFFSET_END);
			counter=Seek(in,0,OFFSET_BEGINNING);
			if (counter!=-1)
			{
				char *iobuffer;
				if ((iobuffer=AllocVec(counter+1,MEMF_ANY)))
				{
					if (Read(in,iobuffer,counter)==counter)
					{
						sprintf(buffer,"/*NOVIA REXX PROGRAM */\naddress '%s'\n",cport->rexxport->mp_Node.ln_Name);
						if (Write(out,buffer,strlen(buffer))!=-1)
						{
							if (Write(out,iobuffer,counter)!=-1)
							{
								FreeVec(iobuffer);
								iobuffer=NULL;
							}
							else
								error=TRUE;
						}
						else
							error=TRUE;
					}
					else
						error=TRUE;
				}
				else
					error=TRUE;
			}
			else
				error=TRUE;
		}
		else
			error=TRUE;
		if (in)Close(in);in=NULL;
		if (out)Close(out);out=NULL;
		if (!error)
		{
			struct TagItem RexxProcTagList[] =
			{
				{	NP_Entry,		(ULONG)rexx_main},
				{	NP_Name,			(ULONG)rexxprog->TaskName},
				{	NP_StackSize,	20000},
				{	TAG_DONE,		TAG_DONE},
			};
			rexxprog->cport=cport;
			Forbid();
			if ((RexxTask=(struct Task *)CreateNewProc(RexxProcTagList)))
			{
				RexxTask->tc_UserData=rexxprog;
			}
			else
			{
				error=TRUE;
				FreeVec(rexxprog);
			}
			Permit();
		}
		else
		{
			FreeVec(rexxprog);
		}
		if (!error)
		{
			ULONG sigs=0;
			BYTE	quit=FALSE;
			ULONG sb_rtport=1<<cport->rtport->mp_SigBit;
			struct message *msg;
			while (!quit && cport->network.online && !cport->ProgramClose)
			{
				sigs=WaitSig(sb_rtport);
				if (sb_rtport & sigs)
				{
					while ((msg=(message *)GetMsg(cport->rtport)))
					{
						switch (msg->msg_type)
						{
							case MSG_NREXX_START:
								cport->RexxOpenCounter++;
								msg->msg_type=MSG_NREXX_OK;
								ReplyMsg((Message *)msg);
								break;
							case MSG_NREXX_END:
								cport->RexxOpenCounter--;
								msg->msg_type=MSG_NREXX_OK;
								ReplyMsg((Message *)msg);
								quit=TRUE;
								break;
						}
					}
				}
			}
		}
	}
	return NULL;
}



void rexx_main()
{
	LONG returncode=20;
	struct RexxProg *rexxprog=(RexxProg *)FindTask(NULL)->tc_UserData;
	struct PortData *cport=rexxprog->cport;
	struct MsgPort *replyport=CreateMsgPort();
	struct message msg;
	ULONG counter;
	char buffer[200];

	memset(&msg,0,sizeof(message));
	msg.msg.mn_ReplyPort=replyport;
	msg.msg.mn_Length=sizeof(message);
	msg.msg_type=MSG_NREXX_START;
	PutMsg(cport->rtport,(Message *)&msg);
	WaitPort(replyport);
	if (msg.msg_type==MSG_NREXX_OK)
	{
		sprintf(buffer,"rx %s",rexxprog->tempfile);
		if (Execute(buffer,0,0))
		{
			msg.msg_type=MSG_NREXX_END;
			PutMsg(cport->rtport,(Message *)&msg);
			WaitPort(replyport);
		}
		else
		{
			printf("rexx execute error, ");
		}
	}
	else
	{
		printf("client failait.\n");
	}
	if (!DeleteFile(rexxprog->tempfile))
		printf("can't delete %s.\n",rexxprog->tempfile);

	if (replyport)DeleteMsgPort(replyport);
}



void DoRexxMsg(RexxMsg *rexxmsg)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char buffer[256];
	if (cport && rexxmsg)
	{
		if (rexxmsg->rm_Node.mn_ReplyPort==cport->rexxport)
		{
	/*		ioprintf("--REPLY--\n");
			ioprintf("rm_Action  : %x\n",rexxmsg->rm_Action);
			ioprintf("rm_Result1 : %d\n",rexxmsg->rm_Result1);
			ioprintf("rm_Result2 : %d\n",rexxmsg->rm_Result2);
			ioprintf("rm_Args[0] : %s\n",rexxmsg->rm_Args[0]);
			ioprintf("rm_Args[1] : %s\n",rexxmsg->rm_Args[1]);
			ioprintf("rm_Args[2] : %s\n",rexxmsg->rm_Args[2]);
			ioprintf("rm_Args[3] : %s\n",rexxmsg->rm_Args[3]);
			ioprintf("rm_CommAddr: %s\n",rexxmsg->rm_CommAddr);
			ioprintf("rm_FileExt : %s\n",rexxmsg->rm_FileExt);
			ioprintf("rm_avail   : %x\n",rexxmsg->rm_avail);*/
			DeleteRexxMsg(rexxmsg);
		}
		else
		{
	/*		ioprintf("--INCOMMING--\n");
			ioprintf("rm_Action  : %x\n",rexxmsg->rm_Action);
			ioprintf("rm_Result1 : %d\n",rexxmsg->rm_Result1);
			ioprintf("rm_Result2 : %d\n",rexxmsg->rm_Result2);
			ioprintf("rm_Args[0] : %s\n",rexxmsg->rm_Args[0]);
			ioprintf("rm_Args[1] : %s\n",rexxmsg->rm_Args[1]);
			ioprintf("rm_Args[2] : %s\n",rexxmsg->rm_Args[2]);
			ioprintf("rm_Args[3] : %s\n",rexxmsg->rm_Args[3]);
			ioprintf("rm_CommAddr: %s\n",rexxmsg->rm_CommAddr);
			ioprintf("rm_FileExt : %s\n",rexxmsg->rm_FileExt);
			ioprintf("rm_avail   : %x\n",rexxmsg->rm_avail);*/
			if (!strncmp("TRANSMIT",(char *)rexxmsg->rm_Args[0],7))
			{
				Writeio((char *)(rexxmsg->rm_Args[0]+9),-1);
				LF();
			}
			if (!strncmp("GETUSER",(char *)rexxmsg->rm_Args[0],7))
			{
				struct Date date;
				ULONG ulong;
				StrToLong((char *)(rexxmsg->rm_Args[0]+7),(LONG *)&ulong);
				switch (ulong)
				{
					case 1:  // User's Handle
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.Handle,strlen(cport->LocalUser.Handle));
						break;
					case 2:  // User's Password
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.PassWord,strlen(cport->LocalUser.PassWord));
						break;
					case 3:  // User's Full Real Name
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.RealName,strlen(cport->LocalUser.RealName));
						break;
					case 4:  // User's City
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.City,strlen(cport->LocalUser.City));
						break;
					case 5:  // User's Zip Code
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.ZipCode,strlen(cport->LocalUser.ZipCode));
						break;
					case 6:  // User's Street Address
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.Street,strlen(cport->LocalUser.Street));
						break;
					case 7:  // Time Remaining (Divide by 10 to Get Minutes)
						break;
					case 8:  // Sysop Comment
						{
							BPTR mulder;
							sprintf(buffer,"%s/%s",cport->sysdirs->userdir,cport->LocalUser);
							if (mulder=Open(buffer,MODE_OLDFILE))
							{
								Seek(mulder,0,OFFSET_END);
								ulong=Seek(mulder,0,OFFSET_BEGINNING);
								if (ulong>200)ulong=199;
								Read(mulder,buffer,ulong);
								Close(mulder);
								buffer[ulong]=0;
								rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
							}
							else
								rexxmsg->rm_Result2=(ULONG)CreateArgstring("",strlen(""));
						}
						break;
					case 9:  // VOICE Phone Number
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.PhoneNo,strlen(cport->LocalUser.PhoneNo));
						break;
					case 10:  // DATA  Phone Number
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.ModemNo,strlen(cport->LocalUser.ModemNo));
						break;
					case 11:  // Last Call Date (In Expanded "Tue 23-Mar-1993  7:25p" Format)
						DateToString(buffer,&cport->LocalUser.LastCall,0);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,sizeof(buffer));
						break;
					case 12:  // Current Date & Time (Can be Changed via EP's Time Zone!)
						GetTime(&date);
						DateToString(buffer,&date,0);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,sizeof(buffer));
						break;
					case 13:  // Total Number of EMails
						sprintf(buffer,"%u",cport->LocalUser.EMails);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 14:  // Total Number of NEW Mail Items Waiting
						sprintf(buffer,"%u",cport->LocalUser.NewMails);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 15:  // User's Access Group
						sprintf(buffer,"%u",cport->LocalUser.AccessGroup);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 16:  // Access Group Name
						sprintf(buffer,"%s",getstr(80,cport->LocalUser.AccessGroup));
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 17:  // System Maintenance Access? (0=No, ~0=Yes)
						sprintf(buffer,"%d",cport->LocalUser.Flag_supervisor);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 18:  // Time Used TODAY (Divide by 10 to Get Minutes)
						break;
					case 19:  // Screen Clear Code (Ascii 12 - `L code)
						break;
					case 20:  // User's Current Owing Balance.
						break;
					case 21:  // Total Game/PFile/Door Points.
						break;
					case 22:  // Total Calls to the System By This User.
						sprintf(buffer,"%u",cport->LocalUser.TotalCalls);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 23:  // Current Port Number
						sprintf(buffer,"%d",cport->clientconfig.IDNumber);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 24:  // Current CPS (For BAUD, Multiply by 10)
						sprintf(buffer,"%d",cport->CurrentCPS/10);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 25:  // User's FILE RATIO Default (#1)
						sprintf(buffer,"%u",cport->LocalUser.FileCredits1);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 26:  // User's BYTE RATIO Default (#1)
						sprintf(buffer,"%u",cport->LocalUser.ByteCredits1);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 27:  // Current Line Width (In Columns)
						sprintf(buffer,"%u",cport->LocalUser.TermWidth);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 28:  // Terminal Type (0=Ascii, 1=C/G, 2=ANSI, 3=IBM, 4=Sky)
						sprintf(buffer,"%u",cport->LocalUser.CharSet);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 29:  // User's Default Protocol (Ascii Value 88=X,89=Y,90=Z,etc)
						sprintf(buffer,"%u",cport->LocalUser.DefProtocol);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 30:  // Total Uploaded KILO-Bytes
						sprintf(buffer,"%u",cport->LocalUser.TotalBytesUploads);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 31:  // Total Uploaded Files
						sprintf(buffer,"%u",cport->LocalUser.TotalUploads);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 32:  // Total Downloaded KILO-Bytes
						sprintf(buffer,"%u",cport->LocalUser.TotalBytesDownloads);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 33:  // Total Downloaded Files
						sprintf(buffer,"%u",cport->LocalUser.TotalDownloads);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 34:  // Total File Credits
						sprintf(buffer,"%u",cport->LocalUser.FileCredits);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 35:  // Total Byte Credits
						break;
						sprintf(buffer,"%u",cport->LocalUser.ByteCredits);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
					case 36:  // Total Public  Messages
						break;
						sprintf(buffer,"%u",cport->LocalUser.TotalPosts);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
					case 37:  // Total Private Messages
						break;
						sprintf(buffer,"%u",cport->LocalUser.TotalEMails);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
					case 38:  // User's WHO Banner
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.Banner,strlen(cport->LocalUser.Banner));
						break;
					case 39:  // Users UUCP Directory
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(cport->LocalUser.Banner,strlen(cport->LocalUser.UUCP));
						break;
					case 40:  // User's Account Number
						sprintf(buffer,"%d",cport->LocalUser.IDNumber);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 41:  // User's Unique ID Number (Serial ID Number)
						sprintf(buffer,"%d",cport->LocalUser.IPNumber);
						rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
						break;
					case 42:  // Total Bytes uploaded   TODAY
						break;
					case 43:  // Total Bytes downloaded TODAY
						break;
					case 44:  // Total Files uploaded   TODAY
						break;
					case 45:  // Total Files downloaded TODAY
						break;
					case 46:  // Time of Day MCI (Morning, Afternoon, Evening)      (Was \v7)
						break;
					case 47:  // Which Caller Am I Today?
						break;
					case 48:  // Current Subboard Number (For Use In Prompts)       (Was \vy)
						break;
					case 49:  // Current Subboard Name   (For Use In Prompts)       (Was \vz)
						break;
					case 50:  // Last Caller Handle MCI (Current Port Only)         (Was \vj)
						break;
					case 51:  // Posting  Access in Current Subboard? (1=Yes, 0=No) (Was \ty)
						break;
					case 52:  // Respond  Access in Current Subboard? (1=Yes, 0=No) (Was \tz)
						break;
					case 53:  // Download Access in Current Subboard? (1=Yes, 0=No) (Was \tw)
						break;
					case 54:  // Upload   Access in Current Subboard? (1=Yes, 0=No) (Was \tx)
						break;
					case 55:  // User's Help Level (0=Nov,1=Int,2=Exp,3=Sup)        (Was \t9)
						break;
					case 56:  // User Online's Country
						break;
					case 57:  // User Online's Logon Time
						break;
					case 58:  // User Online's Birthdate
						break;
					case 59:  // User Online's Organization
						break;
					case 60:  // MCI Numeric Register # 0 (For Math, Testing, Viewing, etc.)
						break;
					case 61:  // MCI Numeric Register # 1 (For Math, Testing, Viewing, etc.)
						break;
					case 62:  // MCI Numeric Register # 2 (For Math, Testing, Viewing, etc.)
						break;
					case 63:  // MCI Numeric Register # 3 (For Math, Testing, Viewing, etc.)
						break;
					case 64:  // MCI Numeric Register # 4 (For Math, Testing, Viewing, etc.)
						break;
					case 65:  // MCI Numeric Register # 5 (For Math, Testing, Viewing, etc.)
						break;
					case 66:  // MCI Numeric Register # 6 (For Math, Testing, Viewing, etc.)
						break;
					case 67:  // MCI Numeric Register # 7 (For Math, Testing, Viewing, etc.)
						break;
					case 68:  // MCI Numeric Register # 8 (For Math, Testing, Viewing, etc.)
						break;
					case 69:  // MCI Numeric Register # 9 (For Math, Testing, Viewing, etc.)
						break;
					case 70:  // MCI String Variable (The {i } Command)
						break;
					case 71:  // MCI Character Value (The {g1} Command)
						break;
					case 72:  // MCI Character Value (The {g2} Command)
						break;
					case 73:  // MCI Character Value (The {g3} Command)
						break;
					case 74:  // MCI Character Value (The {g4} Command)
						break;
					case 75:  // From (Primary name)     (header only)   [Used in BBSTEXT]
						break;
					case 76:  // From (Secondary name)   (header only)   [Used in BBSTEXT]
						break;
					case 77:  // To   (Primary name)     (header only)   [Used in BBSTEXT]
						break;
					case 78:  // To   (Secondary name)   (header only)   [Used in BBSTEXT]
						break;
					case 79:  // Date                    (header only)   [Used in BBSTEXT]
						break;
					case 80:  // Organization (Post/Res) (header only)   [Used in BBSTEXT]
						break;
					case 81:  // Current Item #          (header, scan)  [Used in BBSTEXT]
						break;
					case 82:  // Current response #      (header only)   [Used in BBSTEXT]
						break;
					case 83:  // Total # of responses    (header, scan)  [Used in BBSTEXT]
						break;
					case 84:  // Item size in bytes      (header, scan)  [Used in BBSTEXT]
						break;
					case 85:  // Total downloads         (header, scan)  [Used in BBSTEXT]
						break;
					case 86:  // Best CPS                (header, scan)  [Used in BBSTEXT]
						break;
					case 87:  // Item title              (header, scan)  [Used in BBSTEXT]
						break;
					case 88:  // Size in K/Meg           (scan only)     [Used in BBSTEXT]
						break;
					case 89:  // +/-/!/v/p flag          (scan only)     [Used in BBSTEXT]
						break;
					case 90:  // * flag                  (scan only)     [Used in BBSTEXT]
						break;
					case 91:  // Root filename           (scan only)     [Used in BBSTEXT]
						break;
					case 92:  // Filename suffix         (scan only)     [Used in BBSTEXT]
						break;
					case 93:  // Short date              (scan only)     [Used in BBSTEXT]
						break;
					case 94:  // Download time           (header only)   [Used in BBSTEXT]
						break;
				}
			}
			if (!strncmp("CALLEDITOR",(char *)rexxmsg->rm_Args[0],10))
			{
				VisualEditor("ram:itsmir",0,VISUALEDIT_FILE);
			}
			if (!strncmp("CHANGEWHERE",(char *)rexxmsg->rm_Args[0],11))
			{
				strcpy(cport->Where,rexxmsg->rm_Args[0]+11);
			}
			if (!strncmp("CHANGEWHAT",(char *)rexxmsg->rm_Args[0],10))
			{
				strcpy(cport->Where,rexxmsg->rm_Args[0]+10);
			}
			if (!strncmp("CLOSEDISPLAY",(char *)rexxmsg->rm_Args[0],12))
				Monitor_close();
			{
			}
			if (!strncmp("CLS",(char *)rexxmsg->rm_Args[0],3))
			{
				CLS();
			}
			if (!strncmp("DROPCARRIER",(char *)rexxmsg->rm_Args[0],11))
			{
				cport->network.online=FALSE;
			}
			if (!strncmp("FINDACCOUNT",(char *)rexxmsg->rm_Args[0],11))
			{
//				rexxmsg->rm_Result2=(ULONG)CreateArgstring(buffer,strlen(buffer));
//				FindAccount
			}
			if (!strncmp("GETCARRIER",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("GETSCRATCH",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("GETCHAR",(char *)rexxmsg->rm_Args[0],7))
			{
			}
			if (!strncmp("HANGUP",(char *)rexxmsg->rm_Args[0],6))
			{
				cport->network.online=FALSE;
			}
			if (!strncmp("LOADUSER",(char *)rexxmsg->rm_Args[0],8))
			{
			}
			if (!strncmp("IREADY",(char *)rexxmsg->rm_Args[0],6))		// carrier set ?
			{
			}
			if (!strncmp("LOADEDITOR",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("LOGENTRY",(char *)rexxmsg->rm_Args[0],8))
			{
			}
			if (!strncmp("MAYGETCHAR",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("MODEM",(char *)rexxmsg->rm_Args[0],5))
			{
			}
			if (!strncmp("NEWLINE",(char *)rexxmsg->rm_Args[0],7))
			{
				LF();
			}
			if (!strncmp("OPENDISPLAY",(char *)rexxmsg->rm_Args[0],11))
			{
				Monitor_open();
			}
			if (!strncmp("PRINT",(char *)rexxmsg->rm_Args[0],5))
			{
			}
			if (!strncmp("PROMPT",(char *)rexxmsg->rm_Args[0],6))
			{
			}
			if (!strncmp("PUTSCRATCH",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("PUTUSER",(char *)rexxmsg->rm_Args[0],7))
			{
			}
			if (!strncmp("QUERY",(char *)rexxmsg->rm_Args[0],5))
			{
			}
			if (!strncmp("RECEIVE",(char *)rexxmsg->rm_Args[0],7))
			{
			}
			if (!strncmp("RESETMODEM",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("SAVEEDITOR",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("SAVESCRATCH",(char *)rexxmsg->rm_Args[0],11))
			{
			}
			if (!strncmp("SCREENOUT",(char *)rexxmsg->rm_Args[0],9))
			{
			}
			if (!strncmp("SENDSTRING",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("SEND",(char *)rexxmsg->rm_Args[0],4))
			{
			}
			if (!strncmp("SETMAILSUBJ",(char *)rexxmsg->rm_Args[0],11))
			{
			}
			if (!strncmp("SETMINFREE",(char *)rexxmsg->rm_Args[0],10))
			{
			}
			if (!strncmp("SETNODELOCATION",(char *)rexxmsg->rm_Args[0],15))
			{
			}
			if (!strncmp("SETOBJECT",(char *)rexxmsg->rm_Args[0],9))
			{
			}
			if (!strncmp("SETPROTOCOL",(char *)rexxmsg->rm_Args[0],11))
			{
			}
			if (!strncmp("SHUTDOWN",(char *)rexxmsg->rm_Args[0],8))
			{
			}
			if (!strncmp("SPAWN",(char *)rexxmsg->rm_Args[0],5))
			{
			}
			if (!strncmp("SYSOPLOG",(char *)rexxmsg->rm_Args[0],8))
			{
			}
			if (!strncmp("VERSION",(char *)rexxmsg->rm_Args[0],7))
			{
			}
			if (!strncmp("WRITEMAIL",(char *)rexxmsg->rm_Args[0],9))
			{
			}
			if (!strncmp("XDN",(char *)rexxmsg->rm_Args[0],3))
			{
			}
			if (!strncmp("XUP",(char *)rexxmsg->rm_Args[0],3))
			{
			}

			ReplyMsg((Message *)rexxmsg);
		}
	}
}

