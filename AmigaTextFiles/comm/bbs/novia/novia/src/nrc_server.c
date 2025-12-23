/*
void FreeNRCList()
{
	struct Room *room;
	struct Member *member;
	if (room_list)
	{
		while (!IsListEmpty(room_list))
		{
			room=(Room *)room_list->lh_Head;
			ClearList((List *)&room->Memberlist);
			Remove((Node *)room);
			FreeVec(room);
		}
		FreeVec(room_list);
	}
}

void LoadNRCList(List *rlist)
{
	BPTR mulder;
	BPTR mulder2;
	struct Member *member;
	struct Room *room;
	struct RoomMember *rm;
	char buffer[80];
	struct EClockVal eclock;
	struct EClockVal eclock2;
	ULONG ticks;
	timeval tv;
	double double1;
	double double2;
	sprintf(buffer,"%ssys.roomlist",cport->sysdirs->sysdata);

	ReadEClock(&eclock);
	if (mulder=Open(buffer,MODE_OLDFILE))
	{
		char quit=FALSE;
		ULONG ulong;
		while (!IsListEmpty((List *)rlist))
		{
			Remove((Node *)rlist->lh_Head);
			FreeVec(rlist->lh_Head);
		}
		roomcounter=0;
		Writeio("Load NRC-Roomlist.\n",-1);
		while (!quit && (room=AllocVec(sizeof(Room),MEMF_ANY)))
		{
			printf("room: %x\n",room);
			if (Read(mulder,room,sizeof(Room))==sizeof(Room))
			{
				NewList((List *)&room->userlist);
				NewList((List *)&room->Memberlist);
				room->members=0;
				sprintf(buffer,"work:novia/sysdata/sys.nrcmembers.%d",room->RoomID);
				LoadMemberList(buffer,(List *)&room->Memberlist);
				AddTail((List *)rlist,(Node *)room);
				roomcounter++;
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
		Writeio("Can't load NRC-Roomlist. ",-1);
		PrintDosError(IoErr());
	}	
	ticks=ReadEClock(&eclock2)/1000;
	double1=(double)eclock.ev_lo;
	double2=(double)eclock2.ev_lo;
	ioprintf(" time=%f\n",(double2-double1)/(double)ticks);
}
*/
/*
void EditNRCList(List *rlist)
{
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
	while (!quit && !ProgramClose)
	{
		ioprintf("c3%d.root:services/nrc/> c7",cport->clientconfig.IDNumber);
		Getstring(buffer,0,60,BUFFERED);
		GetArg(buffer);
		nostdcmd=TRUE;
		switch(cmdid=GetCommandID(argstring[0]))
		{
			case CMD_QUIT:
				quit=TRUE;
				nostdcmd=FALSE;
				break;
			case CMD_LIST:
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
				if (StrToLong(argstring[1],(LONG *)&ulong))
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
							if (VDE(VDEID_NRC,(char *)room))
								SaveNRC(room);
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
					ioprintf("Name of Channel:");
					Getstring(buffer,0,39,0);
					strcpy(room->RoomName,buffer);
					room->RoomID=roomcounter++;
					if (VDE(VDEID_NRC,(char *)room))
					{
						NewList((List *)&room->userlist);
						NewList((List *)&room->Memberlist);
						AddTail((List *)rlist,(Node *)room);
						SaveNRC(room);
					}
					else
						FreeVec(room);
					
				}
				else
					ioprintf("Can't add room, out of memory.\n");
				nostdcmd=FALSE;
				break;
			case CMD_DEL:
				if (StrToLong(argstring[1],(LONG *)&ulong))
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
				if (strlen(argstring[1]) && StrToLong(argstring[1],(LONG *)&ulong))
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
							if (ulong=FindUser(argstring[2]))
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
										if (Yesno(TRUE))
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
								if (strlen(argstring[2]))
									ioprintf("invalid User %s.\n",argstring[2]);
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
				if (strlen(argstring[1]) && StrToLong(argstring[1],(LONG *)&ulong))
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
							if (ulong=FindUser(argstring[2]))
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
								if (strlen(argstring[2]))
									ioprintf("invalid User %s.\n",argstring[2]);
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
				if (*argstring[1]=='?')
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
						StrToLong(argstring[1],(LONG *)&ulong);
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
*//*				List list;
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

*//*				ReadEClock(&eclock);
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
			if (!StdCommand(cmdid) && strlen(argstring[0]))
			{
				ioprintf("%s: Unknown command\n",argstring[0]);
				ShowText("work:novia/systext/sys.nrc");LF();
			}
		}		
	}
}

void SaveNRCList(List *rlist)
{
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
	struct Member *member;
	char buffer[100];
	sprintf(buffer,"delete work:novia/sysdata/sys.nrcmembers.%d",room->RoomID);
	Execute(buffer,0,0);
	while (!IsListEmpty((List *)&room->Memberlist))
		FreeVec(RemHead((List *)&room->Memberlist.mlh_Head));
	Remove((Node *)room);
	FreeVec(room);

	SaveNRCList(room_list);
}

void SaveNRC(struct Room *room)
{
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
*/
