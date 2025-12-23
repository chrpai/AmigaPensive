/*****************************************************************************
***                                                         HOOK FUNCTIONS ***
******************************************************************************/

ULONG ScreenmodeToStr(REG(a2) Object *object,REG(a1) ScreenModeRequester *asl)
{
	NameInfo *nameinfo=AllocVec(sizeof(NameInfo),MEMF_ANY);
	GetDisplayInfoData(0,(APTR)nameinfo,sizeof(NameInfo),DTAG_NAME,asl->sm_DisplayID);
	set(object,MUIA_String_Contents,nameinfo->Name);
	FreeVec(nameinfo);
	if (object==App->PA_Screenmode)ClientScreenModeID=asl->sm_DisplayID;
	if (object==App->PA_Maintance_Screenmode)MaintanceScreenModeID=asl->sm_DisplayID;
	return 0;
}



LONG DisplayListcport(REG(a2) char **array,REG(a1) struct PortData *cport2)
{
	if (cport2->clientconfig.IDNumber==0)
	{
		*array++="PortID";
		*array++="User";
		*array++="PortStatus";
		*array="What";
	}
	else
	{
		static char buffer[80];
		char *itsme=(char *)&cport2->clientconfig.IDNumber;
		sprintf(buffer,"%0d.%0d.%0d.%0d             ",itsme[0],itsme[1],itsme[2],itsme[3]);
		*array++ = buffer;
		*array++ = cport2->LocalUser.Handle;
		switch (cport2->PortStatus)
		{
			case PORT_STATUS_WAITLOAD:
				strcpy(buffer+30,"Loading...   ");
				break;
			case PORT_STATUS_CLOSE:
				strcpy(buffer+30,"6Clos-e        ");
				break;
			case PORT_STATUS_OFFLINE:
				strcpy(buffer+30,"Offline      ");
				break;
			case PORT_STATUS_WAITCALL:
				strcpy(buffer+30,"Wait for call");
				break;
			case PORT_STATUS_CONNECT:
				strcpy(buffer+30,"Connecting...");
				break;
			case PORT_STATUS_LOGIN:
				strcpy(buffer+30,"Login...     ");
				break;
			case PORT_STATUS_ONLINE:
				strcpy(buffer+30,"Online       ");
				break;
		}
	//      strcpy(buffer+30,getstr(10,cport2->PortStatus-1));
		*array++ = &buffer[30];
		*array = "---";
	}
	return (0);
}



LONG DisplayListcportmin(REG(a2) char **array,REG(a1) struct PortData *cport2)
{
	static char buffer[80];
	char *itsme=(char *)&cport2->clientconfig.IDNumber;
	sprintf(buffer,"%0d.%0d.%0d.%0d                ",itsme[0],itsme[1],itsme[2],itsme[3]);
	*array++ = buffer;
	return(0);
}



LONG DisplayListudata(REG(a2) char **array,REG(a1) struct UserNode *udata)
{
	static char buffer[80];
	sprintf(buffer,"%3d   %s",udata->IDNumber,udata->Handle);
	*array++ = buffer;
	return(0);
}



LONG DisplayListPOP3Server(REG(a2) char **array,REG(a1) struct INet_POP3Server *server)
{
	*array++ = server->Name;
	return(0);
}



LONG DisplayListPOP3Remove(REG(a2) char **array,REG(a1) struct INet_POP3Server *server)
{
#ifdef DEBUG
	printf("REMOVE SERVER: %s\n",server->Name);
#endif
	return(0);
}



LONG DisplayListLocalMailServer(REG(a2) char **array,REG(a1) struct INet_LocalMailServer *server)
{
	*array++ = server->Name;
	return(0);
}




LONG DisplayListMailserverUpdate(REG(a2) char **array,REG(a1) struct INet_LocalMailServer *server)
{
	struct INet_LocalMailServer *displayserver=NULL;
	ULONG ulong = MUIV_List_NextSelected_Start;
	DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS, MUIM_List_NextSelected, &ulong);
	if (ulong!=MUIV_List_NextSelected_End)
	{
		DoMethod(App->LV_SS_TCP_MAILBOX_HOSTS, MUIM_List_GetEntry, ulong, &displayserver);
		if (displayserver)
		{
			INet_MailUser *user;
			if (current_LocalMailServer)
				GUIToINet_LocalMailServer(current_LocalMailServer);
			current_LocalMailServer=displayserver;
			INet_LocalMailServerToGUI(displayserver);
			set(App->GR_SS_TCP_MAILBOX_PARAMETERS,MUIA_Disabled,FALSE);
			DoMethod(App->LV_SS_TCP_MAILBOX_USERS, MUIM_List_Clear);
#ifdef DEBUG
			printf("displayserver: %x empty: %d\n",displayserver,IsListEmpty((List*)&displayserver->Users));
#endif
			user=(struct INet_MailUser *)displayserver->Users.mlh_Head;
#ifdef DEBUG
			printf("user: %x\n",user);
#endif
			while (user->ln_Succ)
			{
#ifdef DEBUG
				printf("user: %s\n",user->Name);
#endif
				DoMethod(App->LV_SS_TCP_MAILBOX_USERS, MUIM_List_Insert, &user, 1, MUIV_List_Insert_Bottom);
				user=user->ln_Succ;
			}
			set(App->LV_SS_TCP_MAILBOX_USERS,MUIA_List_Active, MUIV_List_Active_Top);
		}
	}
	return(0);
}



LONG DisplayListMailserverUser(REG(a2) char **array,REG(a1) struct INet_MailUser *newuser)
{
	*array++ = newuser->Name;
	if (current_MailUser)
		GUIToINet_LocalUser(current_MailUser);
	current_MailUser=newuser;
	INet_LocalUserToGUI(newuser);
	set(App->GR_SS_TCP_MAILBOX_USERS_PARAMETERS,MUIA_Disabled,FALSE);
	return(0);
}



LONG DisplayListPOP3Update(REG(a2) char **array,REG(a1) struct INet_POP3Server *server)
{
	struct INet_POP3Server *displayserver=NULL;
	ULONG ulong = MUIV_List_NextSelected_Start;
	DoMethod(App->LV_SS_TCP_POP_HOSTS, MUIM_List_NextSelected, &ulong);
#ifdef DEBUG
	printf("POP Update: %x\n",server);
#endif
	if (ulong!=MUIV_List_NextSelected_End)
	{
		DoMethod(App->LV_SS_TCP_POP_HOSTS, MUIM_List_GetEntry, ulong, &displayserver);
		if (displayserver)
		{
			if (current_POP3Server)
				GUIToINet_POP3Server(current_POP3Server);
			current_POP3Server=displayserver;
			INet_POP3ServerToGUI(displayserver);
			set(App->GR_SS_TCP_POP_PARA,MUIA_Disabled,FALSE);
		}
	}
	return(0);
}








LONG DisplayListEvent(REG(a2) char **array,REG(a1) struct Event *event)
{
	if (event)
	{
		struct Date *nextdate=&event->NextEvent;
		struct Date *lastdate=&event->LastEvent;
		*array++ = event->Name;
		if (lastdate->datecode)
			sprintf(event->buf,"%s, %02d.%02d.%04d  %02d:%02d:%02d",getstr(90,lastdate->weekday),lastdate->day,lastdate->month,lastdate->year,lastdate->hours,lastdate->mins,lastdate->secs);
		else
			sprintf(event->buf,"---");
		*array++=event->buf;
		if (nextdate->datecode)
			sprintf(&event->buf[50],"%s, %02d.%02d.%04d  %02d:%02d:%02d\n",getstr(90,nextdate->weekday),nextdate->day,nextdate->month,nextdate->year,nextdate->hours,nextdate->mins,nextdate->secs);
		else
			sprintf(&event->buf[50],"---");
		*array++=&event->buf[50];
		*array=getstr(91,event->Running);
	}
	else
	{
		*array++ = "Name";
		*array++ = "Last execute";
		*array++ = "Next execute";
		*array = "Status";
	}
	return(0);
}



LONG DisplayListEventRemove(REG(a2) char **array,REG(a1) struct Event *event)
{
	if (event)
	{
		Remove((Node *)event);
		FreeVec(event);
	}
	return(0);
}



LONG DisplayListEventUpdate(REG(a2) char **array,REG(a1) struct Event *event)
{
	struct Event *display_event=NULL;
	ULONG ulong = MUIV_List_NextSelected_Start;
	DoMethod(App->LV_REMINDER, MUIM_List_NextSelected, &ulong);
	if (ulong!=MUIV_List_NextSelected_End)
	{
		DoMethod(App->LV_REMINDER, MUIM_List_GetEntry, ulong, &display_event);
		if (display_event)
		{
			if (current_event)
				GUIToEvent(current_event);
			current_event=display_event;
			EventToGUI(display_event);
		}
	}
	return(0);
}



