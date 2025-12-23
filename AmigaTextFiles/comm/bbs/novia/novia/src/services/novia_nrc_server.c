#include <time.h>
#include <devices/timer.h>
#include <pragma/timer_lib.h>
#include <clib/timer_protos.h>
#include <exec/types.h>
#include <exec/io.h>
#include <exec/memory.h>
#include <devices/conunit.h>
#include <intuition/intuition.h>
#include <dos/dos.h>
#include <libraries/fifo.h>
#include <pragma/exec_lib.h>
#include <pragma/graphics_lib.h>
#include <pragma/dos_lib.h>
#include <pragma/noviasys_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <novia/novia_global.h>
#include <novia/novia_misc.h>
#include <novia/novia_funcs.h>
#include <novia/novia_gui.h>
#include <novia/novia_registers.h>
#include <pragma/noviasys_lib.h>

#define YELL	 		1
#define WHISPER 		2
#define BBSCOMMAND	4
#define SIGNAL			8

extern struct NRCServer *nrcserver;
extern struct Task *MainTask;
extern struct MainPortConfig *mainportconfig;

void FreeNRCList();
void LoadNRCList(List *rlist);
void EditNRCList(List *rlist);
void SaveNRCList(List *rlist);
void LoadNRC(struct Room *room);
void KillNRC(struct Room *room);
void SaveNRC(struct Room *room);
void nrc_main();
struct RCMessage2 *SendRCMsg(struct MsgPort *myrcport, struct RCMessage *rcmsg);
struct RCMessage2 *SendRCText(struct MsgPort *myrcport, struct RCMessage *rcmsg, char *text);
char nrc_getline(RCMessage *msg, char *bp,char maxlen,ULONG sigs);

void FreeNRCList()
{
	struct Room *room;
	struct Member *member;
	if (nrcserver->roomlist)
	{
		while (!IsListEmpty(nrcserver->roomlist))
		{
			room=(Room *)nrcserver->roomlist->lh_Head;
			ClearList((List *)&room->Memberlist);
			Remove((Node *)room);
			FreeVec(room);
		}
		FreeVec(nrcserver->roomlist);
		nrcserver->roomlist=0;
	}
}

void LoadNRCList(List *rlist)
{
	struct PortData *cport=(PortData *)MainTask->tc_UserData;
	BPTR mulder;
	BPTR mulder2;
	struct Member *member;
	struct Room *room;
	struct RoomMember *rm;
	char buffer[80];
	sprintf(buffer,"%ssys.roomlist",cport->sysdirs->sysdata);

	if (mulder=Open(buffer,MODE_OLDFILE))
	{
		char quit=FALSE;
		ULONG ulong;
		while (!IsListEmpty((List *)rlist))
		{
			Remove((Node *)rlist->lh_Head);
			FreeVec(rlist->lh_Head);
		}
		nrcserver->rooms=0;
		while (!quit && (room=AllocVec(sizeof(Room),MEMF_ANY)))
		{
			if (Read(mulder,room,sizeof(Room))==sizeof(Room))
			{
				NewList((List *)&room->userlist);
				NewList((List *)&room->Memberlist);
				room->members=0;
				sprintf(buffer,"work:novia/sysdata/sys.nrcmembers.%d",room->RoomID);
				LoadMemberList(buffer,(List *)&room->Memberlist);
				AddTail((List *)rlist,(Node *)room);
				nrcserver->rooms++;
			}
			else
			{
				quit=TRUE;
				FreeVec(room);
			}
		}
		Close(mulder);
	}
	else
	{
		printf("Can't load NRC-Roomlist. %s\n",buffer);
		PrintDosError(IoErr());
	}	
}


void EditNRCList(List *rlist)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char quit=FALSE;
	struct Room *room;
	char nostdcmd;
	char buffer[80];
	ULONG cmdid;
	ULONG ulong;
	struct EClockVal eclock;
	struct EClockVal eclock2;
	ULONG ticks;
	timeval tv;
	double double1;
	double double2;
	ClearMemQuick(buffer,20);
	if (IsListEmpty((List *)rlist))
	{
		Writeio("No Room's available.\n",-1);
	}
	else
	{
		room=(Room *)rlist->lh_Head;
		while (room->ln_Succ)
		{
			ioprintf("RoomNo: %d, RoomName: %s, Users: %d\n",room->RoomID,room->RoomName,room->users);
			room=room->ln_Succ;
		}
	}
	Writeio("List, Edit...\n",-1);
	while (!quit && !cport->ProgramClose)
	{
		ioprintf("c3%d.root:services/nrc/> c7",cport->clientconfig.IDNumber);
		Getstring(buffer,0,60,BUFFERED,0);
		GetArg(buffer);
		nostdcmd=TRUE;
		switch(cmdid=GetCommandID(cport->argstring[0]))
		{
			case CMD_QUIT:
				quit=TRUE;
				nostdcmd=FALSE;
				break;
			case CMD_LIST:
				ioprintf("Users: %d, %d\n",nrcserver->users,nrcserver->localusers);
				if (IsListEmpty((List *)rlist))
				{
					Writeio("No Room's available.\n",-1);
				}
				else
				{
					room=(Room *)rlist->lh_Head;
					while (room->ln_Succ)
					{
						ioprintf("RoomNo: %d, RoomName: %s, Users: %d\n",room->RoomID,room->RoomName,room->users);
						room=room->ln_Succ;
					}
				}
				nostdcmd=FALSE;
				break;
			case CMD_EDIT:
				if (StrToLong(cport->argstring[1],(LONG *)&ulong))
				{
					if (IsListEmpty((List *)rlist))
					{
						Writeio("No Room's available.\n",-1);
					}
					else
					{
						room=(Room *)rlist->lh_Head;
						while (room->ln_Succ && (room->RoomID!=ulong))
						{
							room=room->ln_Succ;
						}
						if (room->RoomID==ulong && room->ln_Succ)
						{
//							if (VDE(VDEID_NRC,(char *)room))
//								SaveNRC(room);
						}
						else
							ioprintf("invalid ChannelID.\n");

					}
				}
				else
				{
					ioprintf("USAGE: Edit [RoomID #]\n");
				}
				nostdcmd=FALSE;
				break;
			case CMD_ADD:
				if (room=AllocVec(sizeof(Room),MEMF_ANY|MEMF_CLEAR))
				{
					struct Room *room2;
					UWORD counter;
					BOOL isfree=FALSE;
					ioprintf("Name of Channel:");
					Getstring(buffer,0,39,0,0);
					strcpy(room->RoomName,buffer);
					for (counter=1;(isfree==FALSE);counter++)
					{
						isfree=TRUE;
						room2=(struct Room *)nrcserver->roomlist->lh_Head;
						while (room2->ln_Succ && isfree)
						{
							if (room2->RoomID==counter)
								isfree=FALSE;
							room2=room2->ln_Succ;
						}
					}
					room->RoomID=counter-1;
					room->RoomIP=mainportconfig->nrc_ip_counter++;
					NewList((List *)&room->userlist);
					NewList((List *)&room->Memberlist);
/*					if (VDE(VDEID_NRC,(char *)room))
					{
						SaveMainPortConfig(mainportconfig);
						AddTail((List *)rlist,(Node *)room);
						SaveNRC(room);
					}
					else
						FreeVec(room);*/
				}
				else
					ioprintf("Can't add room, out of memory.\n");
				nostdcmd=FALSE;
				break;
			case CMD_DEL:
				if (StrToLong(cport->argstring[1],(LONG *)&ulong))
				{
					if (IsListEmpty((List *)rlist))
					{
						Writeio("No Room's available.\n",-1);
					}
					else
					{
						room=(Room *)rlist->lh_Head;
						while (room->ln_Succ && (room->RoomID!=ulong))
						{
							room=room->ln_Succ;
						}
						if (room->RoomID==ulong && room->ln_Succ)
						{
							KillNRC(room);
						}
						else
							ioprintf("invalid ChannelID.\n");
					}
				}
				else
				{
					ioprintf("USAGE: DELete [RoomID #]\n");
				}
				break;
			case CMD_DELMEMBER:
				if (strlen(cport->argstring[1]) && StrToLong(cport->argstring[1],(LONG *)&ulong))
				{
					if (IsListEmpty((List *)rlist))
					{
						Writeio("No Room's available.\n",-1);
					}
					else
					{
						room=(Room *)rlist->lh_Head;
						while (room->ln_Succ && (room->RoomID!=ulong))
						{
							room=room->ln_Succ;
						}
						if (room->RoomID==ulong && room->ln_Succ)
						{
							if (ulong=FindUser(cport->argstring[2]))
							{
								BOOL userfound=FALSE;
								struct Member *member;
								member=(Member *)room->Memberlist.mlh_Head;
								while (member->ln_Succ && !userfound)
								{
									if (member->UserID==ulong)
									{
										ioprintf("Remove User %s (%d) from %s ?",
											cport->UL.utable[member->UserID].Handle,
											cport->UL.utable[member->UserID].IDNumber,
											room->RoomName);
										if (Yesno(TRUE,0))
										{
											ioprintf("User %s (%d) in not longer a member of %s.\n",
												cport->UL.utable[member->UserID].Handle,
												cport->UL.utable[member->UserID].IDNumber,
											room->RoomName);
											Remove((Node *)member);
											FreeVec(member);
											SaveNRC(room);
										}
										userfound=TRUE;
									}
									else
										member=member->ln_Succ;
								}
								if (!userfound)
									ioprintf("User %s (%d) is not a member of %s.\n",
										cport->UL.utable[ulong].Handle,
										cport->UL.utable[ulong].IDNumber,
										room->RoomName);
							}
							else
								if (strlen(cport->argstring[2]))
									ioprintf("invalid User %s.\n",cport->argstring[2]);
								else
									ioprintf("USAGE: DELMember [RoomID #],[UserID #|Username]\n");
						}
						else
							ioprintf("invalid ChannelID %d.\n",ulong);

					}
				}
				else
				{
					ioprintf("USAGE: DELMember [RoomID #],[UserID #|Username]\n");
				}
				nostdcmd=FALSE;
				break;
			case CMD_ADDMEMBER:
				if (strlen(cport->argstring[1]) && StrToLong(cport->argstring[1],(LONG *)&ulong))
				{
					if (IsListEmpty((List *)rlist))
					{
						Writeio("No Room's available.\n",-1);
					}
					else
					{
						room=(Room *)rlist->lh_Head;
						while (room->ln_Succ && (room->RoomID!=ulong))
						{
							room=room->ln_Succ;
						}
						if (room->RoomID==ulong && room->ln_Succ)
						{
							if (ulong=FindUser(cport->argstring[2]))
							{
								BOOL userfound=FALSE;
								struct Member *member;
								member=(Member *)room->Memberlist.mlh_Head;
								while (member->ln_Succ && !userfound)
								{
									if (member->UserID==ulong)
										userfound=TRUE;
									else
										member=member->ln_Succ;
								}
								if (userfound)
									ioprintf("User %s (%d) is already a member of %s.\n",
										cport->UL.utable[ulong].Handle,
										cport->UL.utable[ulong].IDNumber,
										room->RoomName);
								else
								{
									if (member=AllocVec(sizeof(Member),MEMF_ANY))
									{
										member->UserID=ulong;
										AddTail((List *)&room->Memberlist,(Node *)member);
										ioprintf("User %s (%d) is now a member of %s.\n",
											cport->UL.utable[ulong].Handle,
											cport->UL.utable[ulong].IDNumber,
											room->RoomName);
											SaveNRC(room);
									}
									else
										Writeio("Can't add user, out of memory.\n",-1);
								}
							}
							else
								if (strlen(cport->argstring[2]))
									ioprintf("invalid User %s.\n",cport->argstring[2]);
								else
									ioprintf("USAGE: ADDMember [RoomID #],[UserID #|Username]\n");
						}
						else
							ioprintf("invalid ChannelID %d.\n",ulong);

					}
				}
				else
				{
					ioprintf("USAGE: ADDMember [RoomID #],[UserID #|Username]\n");
				}
				nostdcmd=FALSE;
				break;
			case CMD_MEMBERS: case CMD_LISTMEMBERS:
				if (*cport->argstring[1]=='?')
					ioprintf("USAGE: MEMBERS/LISTMEMBERS [RoomID #].\n");
				else
				{
					if (IsListEmpty((List *)rlist))
					{
						Writeio("No Room's available.\n",-1);
					}
					else
					{
						struct Member *member;
						StrToLong(cport->argstring[1],(LONG *)&ulong);
						if (ulong)
						{
							room=(Room *)rlist->lh_Head;
							while (room->ln_Succ && (room->RoomID!=ulong))
							{
								room=room->ln_Succ;
							}
							if (room->RoomID==ulong && room->ln_Succ)
							{
								member=(Member *)room->Memberlist.mlh_Head;
								while (member->ln_Succ)
								{
									if (member->UserID>0 && member->UserID<=cport->UL.UserEntrys)
										ioprintf("User: %21s (%d)\n",cport->UL.utable[member->UserID].Handle,member->UserID);
									member=member->ln_Succ;
								}
							}
							else
								ioprintf("invalid ChannelID %d.\n",ulong);
						}
						else
						{
							room=(Room *)rlist->lh_Head;
							while (room->ln_Succ)
							{
								ioprintf("c7Members in Room %s (%d):\n",room->RoomName,room->RoomID);
								member=(Member *)room->Memberlist.mlh_Head;
								if (member->ln_Succ)
								{
									while (member->ln_Succ)
									{
										if (member->UserID>0 && member->UserID<=cport->UL.UserEntrys)
											ioprintf("c3   User: %21s (%d)\n",cport->UL.utable[member->UserID].Handle,member->UserID);
										member=member->ln_Succ;
									}
								}
								else
									ioprintf("c1   (this room has no members)\n");
								member=(Member *)room->Grouplist.mlh_Head;
								if (member->ln_Succ)
								{
									while (member->ln_Succ)
									{
										if (member->UserID>0 && member->UserID<=cport->UL.UserEntrys)
											ioprintf("c3   Group: %21s (%d)\n",cport->UL.utable[member->UserID].Handle,member->UserID);
										member=member->ln_Succ;
									}
								}
								else
									ioprintf("c1   (this room has no membergroup)\n");
								room=room->ln_Succ;
							}
						}

					}
				}
				nostdcmd=FALSE;
				break;
			case CMD_ADDGROUP:
			{
				List list;
				struct Node *node;
				NewList(&list);
				ReadEClock(&eclock);
				for (ulong=0;ulong<100000;ulong++)
				{
					if (node=AllocVec(sizeof(Node),MEMF_ANY))
					{
						AddTail(&list,node);
					}
				}
				ticks=ReadEClock(&eclock2)/1000;
				double1=(double)eclock.ev_lo;
				double2=(double)eclock2.ev_lo;
				ioprintf("time=%f ms\n",(double2-double1)/(double)ticks);
				node=list.lh_Head;

				ReadEClock(&eclock);
				while (node->ln_Succ)
					node=node->ln_Succ;
				ticks=ReadEClock(&eclock2)/1000;
				double1=(double)eclock.ev_lo;
				double2=(double)eclock2.ev_lo;
				ioprintf("time=%f ms\n",(double2-double1)/(double)ticks);

				ReadEClock(&eclock);
//				while (list.lh_Head->ln_Succ)
//					FreeVec(RemTail(&list));
				ticks=ReadEClock(&eclock2)/1000;
				double1=(double)eclock.ev_lo;
				double2=(double)eclock2.ev_lo;
				ioprintf("time=%f ms\n",(double2-double1)/(double)ticks);

			}
				nostdcmd=FALSE;
				break;
			case CMD_DELGROUP:
				nostdcmd=FALSE;
				break;
		}
		if (nostdcmd)
		{	
			if (!StdCommand(cmdid) && strlen(cport->argstring[0]))
			{
				ioprintf("%s: Unknown command\n",cport->argstring[0]);
				ShowText("work:novia/systext/sys.nrc");LF();
			}
		}		
	}
}

void SaveNRCList(List *rlist)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct Room *room=(Room *)rlist->lh_Head;
	Execute("delete work:novia/sysdata/sys.roomlist",0,0);
	while (room->ln_Succ)
	{
		SaveNRC(room);
		room=room->ln_Succ;
	}	

}

void LoadNRC(struct Room *room)
{
}

void KillNRC(struct Room *room)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct Member *member;
	char buffer[100];
	sprintf(buffer,"delete work:novia/sysdata/sys.nrcmembers.%d",room->RoomID);
	Execute(buffer,0,0);
	while (!IsListEmpty((List *)&room->Memberlist))
		FreeVec(RemHead((List *)&room->Memberlist.mlh_Head));
	Remove((Node *)room);
	FreeVec(room);

	SaveNRCList(nrcserver->roomlist);
}

void SaveNRC(struct Room *room)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	char buffer[100];
	BPTR mulder=Open("work:novia/sysdata/sys.roomlist",MODE_READWRITE);
	struct RoomMember *rm;
	if (mulder)
	{
		BPTR mulder2;
		sprintf(buffer,"work:novia/sysdata/sys.nrcmembers.%d",room->RoomID);
		SaveMemberList(buffer,(List *)&room->Memberlist);
		Seek(mulder,sizeof(Room)*(room->RoomID-1),OFFSET_BEGINNING);
		Write(mulder,room,sizeof(Room));
		Close(mulder);
	}
	else
	{
		ioprintf("Can't write NRC-Room: %d. ",room->RoomID);
		PrintDosError(IoErr());
	}	
}


void nrc_main()
{
	if ((nrcserver=AllocVec(sizeof(NRCServer),MEMF_ANY|MEMF_CLEAR)))
	{
		char quit=FALSE;
		ULONG sigs=0;
		ULONG sb_nrcport;
		ULONG	sb_controlport;
		struct RCMessage2 *rcmsg;
		struct RCMessage2 *newrcmsg;
		struct RoomUser *user;
		struct RoomUser *newuser;
		struct Room *room=NULL;
		struct Room *oldroom=NULL;
		struct Room *newroom=NULL;
		char buffer[200];
		struct Date date;
		BOOL logoff_messages_send=FALSE;
		if ((nrcserver->roomlist=AllocVec(sizeof(List),MEMF_ANY|MEMF_CLEAR)))
		{
			NewList(nrcserver->roomlist);
			LoadNRCList(nrcserver->roomlist);
		}
		else
		{
			printf("Can't start nrc-server.\n");
		}
		if (!quit)
		{
			nrcserver->nrcport=CreateMsgPort();
			nrcserver->controlport=CreateMsgPort();
			if (nrcserver->nrcport && nrcserver->controlport)
			{
				sb_nrcport=1<<nrcserver->nrcport->mp_SigBit;
				sb_controlport=1<<nrcserver->controlport->mp_SigBit;
			}
			else
				quit=TRUE;
		}
		if (!quit)
		{
			nrcserver->nrctask=FindTask(NULL);
			nrcserver->ServerOpen=TRUE;
		}
		while (!quit)
		{
			if (nrcserver->ServerClose)
			{
				if (nrcserver->users<=0)
					quit=TRUE;
				else
				{
					if (!logoff_messages_send)
					{
						user=(RoomUser *)room->userlist.mlh_Head;
						while (user->ln_Succ)
						{
							user->flag_quit=TRUE;
 							Signal(user->userport->mp_SigTask,1<<user->userport->mp_SigBit);
							user=user->ln_Succ;
						}
						logoff_messages_send=TRUE;
					}
				}
				Delay(10);
			}
			else
				sigs=Wait(sb_nrcport|sb_controlport);
			if ((sb_nrcport & sigs) | nrcserver->ServerClose)
			{
				while(rcmsg=(RCMessage2 *)GetMsg(nrcserver->nrcport))
				{
					if (rcmsg->msg.mn_ReplyPort==nrcserver->nrcport)
					{
						FreeVec(rcmsg);
					}
					else
					{
						switch (rcmsg->rcmsg.msgtype)
						{
							case RCMSG_LOGIN:
								if (!nrcserver->ServerClose && !IsListEmpty(nrcserver->roomlist) )
								{
									if ((newuser=AllocVec(sizeof(RoomUser),MEMF_ANY|MEMF_CLEAR)))
									{
										room=(Room *)nrcserver->roomlist->lh_Head;
										newuser->userport=rcmsg->msg.mn_ReplyPort;
										newuser->Level=10;
										newuser->room=room;
										newuser->flag_quit=FALSE;
										strcpy(newuser->UserName,newuser->cport->LocalUser.Handle);
										strcpy(newuser->RealName,newuser->cport->LocalUser.RealName);
										room->users++;
										room->localusers++;
										nrcserver->users++;
										nrcserver->localusers++;
										rcmsg->rcmsg.FromUser=1;
										rcmsg->rcmsg.FromRoom=1;
										rcmsg->rcmsg.ToRoom=1;
										rcmsg->rcmsg.msgtype=RCMSG_LOGIN_REPLY_OK;
										rcmsg->rcmsg.user=newuser;
										rcmsg->rcmsg.user->userport=rcmsg->msg.mn_ReplyPort;
										user=(RoomUser *)room->userlist.mlh_Head;
										while (user->ln_Succ)
										{
											if (newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR))
											{
												newrcmsg->rcmsg.msgtype=RCMSG_TEXT;
												newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
												newrcmsg->rcmsg.flag_sysmsg=TRUE;
												GetTime(&date);
												DateToString(buffer,&date,0);
												sprintf(newrcmsg->rcmsg.data,"---- %s has entered in this room (%s).",user->cport->LocalUser.Handle,buffer);
												PutMsg(user->userport,(Message *)newrcmsg);
											}
											user=user->ln_Succ;
										}
										AddTail((List *)&room->userlist,(Node *)newuser);
									}
									else
									{
										rcmsg->rcmsg.msgtype=RCMSG_LOGIN_REPLY_FAILAT;
									}
								}
								else
								{
									rcmsg->rcmsg.msgtype=RCMSG_LOGIN_REPLY_FAILAT;
									ioprintf("\nNRV service failat. no rooms installet.\n");
								}
								break;
							case RCMSG_TEXT:
								room=rcmsg->rcmsg.user->room;
								if (room)
								{
									user=(RoomUser *)room->userlist.mlh_Head;
									while (user->ln_Succ)
									{
										if (!rcmsg->rcmsg.flag_private | rcmsg->rcmsg.ToUser==user->cport->LocalUser.IDNumber)
										{
											if (newrcmsg=AllocVec(rcmsg->msg.mn_Length,MEMF_ANY))
											{
												CopyMem(rcmsg,newrcmsg,rcmsg->msg.mn_Length);
												newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
												PutMsg(user->userport,(Message *)newrcmsg);
											}
										}
										user=user->ln_Succ;
									}
								}
								break;
							case RCMSG_LOGOFF:
								room=rcmsg->rcmsg.user->room;
								user=(RoomUser *)room->userlist.mlh_Head;
								while (user->ln_Succ)
								{
									if (user!=rcmsg->rcmsg.user)
									{
										if (newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR))
										{
											newrcmsg->rcmsg.msgtype=RCMSG_TEXT;
											newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
											newrcmsg->rcmsg.flag_sysmsg=TRUE;
											GetTime(&date);
											DateToString(buffer,&date,0);
											sprintf(newrcmsg->rcmsg.data,"---- %s has going out this room (%s).",user->cport->LocalUser.Handle,buffer);
											PutMsg(user->userport,(Message *)newrcmsg);
										}
									}
									user=user->ln_Succ;
								}
								rcmsg->rcmsg.msgtype=RCMSG_LOGOFF_OK;
								rcmsg->rcmsg.user->room->users--;
								rcmsg->rcmsg.user->room->localusers--;
								nrcserver->users--;
								nrcserver->localusers--;
								Remove((Node *)rcmsg->rcmsg.user);
								FreeVec(rcmsg->rcmsg.user);
								break;
							case RCMSG_CHANGEROOM:
								room=(Room *)nrcserver->roomlist->lh_Head;
								newrcmsg=NULL;
								oldroom=room;
								while (room->ln_Succ)
								{
									if (room->RoomID==(UWORD)rcmsg->rcmsg.aptrdata)
									{
										user=rcmsg->rcmsg.user;
										if (room->RoomID==rcmsg->rcmsg.user->room->RoomID)
										{
											if (newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR))
											{
												newrcmsg->rcmsg.msgtype=RCMSG_CHANGEROOM_FAILAT;
												newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
												newrcmsg->rcmsg.flag_sysmsg=TRUE;
												sprintf(newrcmsg->rcmsg.data,"---- You are already in room %s (%d).",room->RoomName,room->RoomID);
												PutMsg(user->userport,(Message *)newrcmsg);
											}
											
										}
										else
										{
											if ((newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR)))
											{
												oldroom->users--;
												oldroom->localusers--;
												Remove((Node *)rcmsg->rcmsg.user);
												room->users++;
												room->localusers++;
												AddTail((List *)&room->userlist,(Node *)rcmsg->rcmsg.user);
												newrcmsg->rcmsg.msgtype=RCMSG_CHANGEROOM_OK;
												newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
												newrcmsg->rcmsg.flag_sysmsg=TRUE;
												newrcmsg->rcmsg.user=rcmsg->rcmsg.user;
												newrcmsg->rcmsg.user->room=room;
												rcmsg->rcmsg.FromRoom=room->RoomID;
												rcmsg->rcmsg.ToRoom=room->RoomID;

												sprintf(newrcmsg->rcmsg.data,"---- You are now in room %s (%d).",room->RoomName,room->RoomID);
												PutMsg(user->userport,(Message *)newrcmsg);
												user=(RoomUser *)oldroom->userlist.mlh_Head;
												while (user->ln_Succ)
												{
													if (user!=rcmsg->rcmsg.user)
													{
														if ((newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR)))
														{
															newrcmsg->rcmsg.msgtype=RCMSG_TEXT;
															newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
															newrcmsg->rcmsg.flag_sysmsg=TRUE;
															GetTime(&date);
															DateToString(buffer,&date,0);
															sprintf(newrcmsg->rcmsg.data,"---- %s has going out this room (%s).",user->cport->LocalUser.Handle,buffer);
															PutMsg(user->userport,(Message *)newrcmsg);
														}
													}
													user=user->ln_Succ;
												}
												user=(RoomUser *)room->userlist.mlh_Head;
												while (user->ln_Succ)
												{
													if (user!=rcmsg->rcmsg.user)
													{
														if ((newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR)))
														{
															newrcmsg->rcmsg.msgtype=RCMSG_TEXT;
															newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
															newrcmsg->rcmsg.flag_sysmsg=TRUE;
															GetTime(&date);
															DateToString(buffer,&date,0);
															sprintf(newrcmsg->rcmsg.data,"---- %s login this room (%s).",user->cport->LocalUser.Handle,buffer);
															PutMsg(user->userport,(Message *)newrcmsg);
														}
													}
													user=user->ln_Succ;
												}
											}
										}
										break;
									}
									room=room->ln_Succ;
								}
								if (!newrcmsg)
								{
									if (newrcmsg=AllocVec(sizeof(RCMessage2)+100,MEMF_ANY|MEMF_CLEAR))
									{
										newrcmsg->rcmsg.msgtype=RCMSG_CHANGEROOM_FAILAT;
										newrcmsg->msg.mn_ReplyPort=nrcserver->nrcport;
										newrcmsg->rcmsg.flag_sysmsg=TRUE;
										sprintf(newrcmsg->rcmsg.data,"---- Room not found.");
										PutMsg(user->userport,(Message *)newrcmsg);
									}
								}
								break;
						}
						ReplyMsg((Message *)rcmsg);
					}
				}
			}
		}
		Forbid();
		nrcserver->ServerOpen=FALSE;
		nrcserver->ServerClose=FALSE;
		nrcserver->nrctask=NULL;
		FreeNRCList();
		if (nrcserver->nrcport)DeleteMsgPort(nrcserver->nrcport);
		if (nrcserver->controlport)DeleteMsgPort(nrcserver->controlport);
		FreeVec(nrcserver);
		nrcserver=NULL;
		Permit();
	}
}



struct RCMessage2 *SendRCMsg(struct MsgPort *myrcport, struct RCMessage *rcmsg)
{
	struct RCMessage2 *sendmsg=NULL;
	if (nrcserver)
	{
		if (nrcserver->nrcport && myrcport)
		{
			if ((sendmsg=AllocVec(sizeof(RCMessage2)+rcmsg->data_len,MEMF_ANY)))
			{
				CopyMem(rcmsg,&sendmsg->rcmsg,sizeof(RCMessage)+rcmsg->data_len);
				sendmsg->msg.mn_ReplyPort=myrcport;
				sendmsg->msg.mn_Length=sizeof(RCMessage2)+rcmsg->data_len;
				PutMsg(nrcserver->nrcport,(Message *)sendmsg);
			}
		}
	}
	return sendmsg;
}

struct RCMessage2 *SendRCText(struct MsgPort *myrcport, struct RCMessage *rcmsg, char *text)
{
	struct RCMessage2 *sendmsg;
	ULONG txtlen=strlen(text)+1;
	if (nrcserver)
	{
		if (nrcserver->nrcport && myrcport)
		{
			if ((sendmsg=AllocVec(sizeof(RCMessage2)+txtlen,MEMF_ANY|MEMF_CLEAR)))
			{
				rcmsg->msgtype=RCMSG_TEXT;
				CopyMem(rcmsg,&sendmsg->rcmsg,sizeof(RCMessage));
				CopyMem(text,sendmsg->rcmsg.data,txtlen);
				sendmsg->rcmsg.data_len=txtlen;
				sendmsg->msg.mn_ReplyPort=myrcport;
				sendmsg->msg.mn_Length=sizeof(RCMessage2)+txtlen;
				PutMsg(nrcserver->nrcport,(Message *)sendmsg);
			}
		}
	}
	return sendmsg;
}

char nrc_getline(RCMessage *msg, char *bp,char maxlen,ULONG sigs)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	BOOL flag_whisper=FALSE;
	BOOL flag_yell=FALSE;
	BOOL flag_bbscommand=FALSE;
	ULONG userfound=0;
	BOOL flag_beginn=FALSE;
	char	ToName[50];
	if (cport)
	{
		char cp[80];
		char bp2[80];
		char pos=0;
		char end=0;
		BOOL Quit=FALSE;
		BOOL habschon;
		int i;
		char wert;
		short counter;
		List *his_list=(List *)&cport->his_list;
		struct HisEntry *hisentry=(HisEntry *)his_list->lh_TailPred;
		BOOL first=FALSE;
		BOOL ende=FALSE;
		*bp=0;
		Writeio("c3",-1);
		bp[end]=0;
		if (msg)
			ClearMemQuick(msg->ToName,40);

		while(!Quit && cport->network.online && !cport->ProgramClose && !msg->user->flag_quit)
		{
			habschon=FALSE;
			if (Conread(cp,1,sigs) && pos==0 && (!flag_yell && !flag_whisper && !flag_bbscommand && !flag_beginn))
			{
				return SIGNAL;
			}
			if (*cp==13)
			{
				if (end)
				{
					*cp=10;
					Writeio(cp,1);
					if (!flag_beginn)
					{
						if (flag_whisper | flag_yell)
						{
							if ((userfound=FindUser(bp)))
							{
								strncpy(ToName,bp,39);
								flag_beginn=TRUE;
								ioprintf(":");
							}
							else
							{
								ioprintf("unknown User: %s\n",bp);
								flag_yell=FALSE;
								flag_whisper=FALSE;
								flag_beginn=FALSE;
							}
							memset(bp,0,maxlen);
							end=0;
							pos=0;
						}
						if (flag_bbscommand)
						{
							Quit=TRUE;
						}
					}
					else
					{
						if (!flag_beginn)
							memset(bp,0,maxlen);
						Quit=TRUE;
					}
				}
				habschon=TRUE;
			}
			if (*cp==-101)
			{
				Conread(cp,1,sigs);
				switch (*cp)
				{
					case 'D':				// curser right
						if (pos>0)
						{
							Writeio("[D",-1);
							pos--;
					 	}
						break;
					case 'C':				// curser left
						if (pos<end && end<=maxlen)
						{
							Writeio("[C",-1);
							pos++;
						}
						break;
				}
				habschon=TRUE;
			}
			if (*cp==8 && habschon==FALSE)		// backspace
			{
				if (pos>0)
				{
					Writeio(cp,1);
					pos--;
					for (i=pos;i<end;i++)
					{
						bp[i]=bp[i+1];
					}
					end--;
					Writeio(&bp[pos],end-pos);
					Writeio(" ",1);
					cleft((end-pos)+1);
					habschon=TRUE;
				}
				else
				{
					flag_bbscommand=FALSE;
					flag_whisper=FALSE;
					flag_yell=FALSE;
					flag_beginn=FALSE;
					userfound=0;
					ioprintf("\r[K");
					pos=0;
					end=0;
					memset(bp,0,maxlen);
					Quit=TRUE;
				}
			}
			if (*cp==127 && pos<end && habschon==FALSE)	// delete
			{
				for (i=pos;i<end;i++)
				{
					bp[i]=bp[i+1];
				}
				end--;
				Writeio(&bp[pos],end-pos);
				Writeio(" ",1);
				cleft((end-pos)+1);
				habschon=TRUE;
			}
			if (!habschon && ((*cp>=-96 && *cp<0) | (*cp>=32 && *cp<127)) && end<maxlen && pos<maxlen)
			{
				if (end==0)
				{
					if (!flag_whisper && !flag_yell && !flag_bbscommand && !flag_beginn)
					{
						if ((*cp=='+'|*cp=='-'|*cp=='/'))
						{
							switch (*cp)
							{
								case '+':	// Yell at whom
									if (flag_yell==FALSE)
									{
										ioprintf("\r[KYell at whom: ");
										flag_yell=TRUE;
										habschon=TRUE;
									}
									break;
								case '-':	// Whisper to whom
									if (flag_whisper==FALSE)
									{
										ioprintf("\r[KWhisper to whom:");
										flag_whisper=TRUE;
										habschon=TRUE;
									}
									break;
								case '/':	// BBS-Command;
									if (flag_bbscommand==FALSE)
									{
										ioprintf("\r[Kcommand:");
										flag_bbscommand=TRUE;
										habschon=TRUE;
									}
									break;
							}
						}
						else
						{
							ioprintf(":");
							flag_beginn=TRUE;
						}
					}
				}
				if (!habschon)
				{
					if (pos==end)
					{
						Writeio(cp,1);
						bp[pos]=*cp;
						pos++;
						end++;
						bp[end]=0;
					}
					else
					{
						for (i=end;i>=pos;i--)
						{
							bp[i+1]=bp[i];
						}
						end++;
						bp[pos]=*cp;
						bp[end]=0;
						ioprintf("[%dC[P[%dD[@%c",end-pos,end-pos,*cp);
						pos++;
					}
					habschon=TRUE;
					bp[end]=0;
				}
			}
		}
		bp[end]=0;
		bp[end+1]=0;
	}
	msg->flag_private=0;
	msg->flag_broadcast=0;
	msg->flag_sysmsg=0;
	msg->flag_anonymous=0;
	msg->flag_realname=0;

	if (userfound)
	{
		msg->ToUser=userfound;
		if (flag_yell)
		{
			strncpy(msg->ToName,ToName,39);
			return YELL;
		}
		if (flag_whisper)
		{
			msg->flag_private=TRUE;
			strncpy(msg->ToName,ToName,39);
			return WHISPER;
		}
	}
	if (flag_bbscommand)
		return BBSCOMMAND;
	return NULL;
}

void SelectFromTo(UBYTE *table, ULONG tablesize, char *string)
{
		{
			ULONG ulong=0;
			ULONG from=0;
			ULONG to=0;
			char *pt=string;
			while (*pt)
			{
				if (*pt>=48 && *pt<=57)
				{
					StrToLong(pt,(long *)&from);
					while (*pt>=48 && *pt<=57)
						pt++;
					if (*pt=='-')
					{
						while (*pt && (*pt<48|*pt>57))
							pt++;
						if (*pt>=48 && *pt<=57)
						{
							StrToLong(pt,(long *)&to);
							while (*pt>=48 && *pt<=57)pt++;
							if (from>to)
							{
								ulong=to;
								to=from;
								from=ulong;
							}
							if (!from)from=1;
							for (ulong=from;(ulong<=tablesize && ulong<=to);ulong++)
							{
								if (table[ulong-1])
								{
									table[ulong-1]=FALSE;
								}
								else
								{
									table[ulong-1]=TRUE;
								}
							}
						}
						else
						{
							if (from && from<=tablesize)
							{
								for (ulong=from-1;ulong<tablesize;ulong++)
								{
									if (table[ulong])
									{
										table[ulong]=FALSE;
									}
									else
									{
										table[ulong]=TRUE;
									}
								}
							}
						}
					}
					else
					{
						if (from>0 && from<=tablesize)
						{
							if (table[from-1])
							{
								table[from-1]=FALSE;
							}
							else
							{
								table[from-1]=TRUE;
							}
						}
					}
				}
				else
				{
					if (*pt=='-')
					{
						while (*pt && (*pt<48|*pt>57))
							pt++;
						if (*pt>=48 && *pt<=57)
						{
							StrToLong(pt,(long *)&to);
							if (to>tablesize)to=tablesize;
							for (ulong=0;ulong<to;ulong++)
							{
								if (table[ulong])
								{
									table[ulong]=FALSE;
								}
								else
								{
									table[ulong]=TRUE;
								}
							}
						}
					}
				}
				if(*pt)pt++;
			}
		}
}
