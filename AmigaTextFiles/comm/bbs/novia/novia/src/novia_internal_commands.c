#include <exec/types.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <novia/novia_global.h>
#include <novia/novia_misc.h>
#include <novia/novia_registers.h>
#include <novia/novia_portdata.h>
#include <novia/novia_gui.h>
#include <pragma/noviasys_lib.h>


extern struct PortData *FindClient(ULONG PortID);
extern void EditNRCList(List *);


extern struct Task *GUITask;
extern struct Task *MainTask;
extern struct GUIFrontend guifrontend;
extern List *client_list;
extern struct NRCServer *nrcserver;


char StdCommand(ULONG cmdid);

char StdCommand(ULONG cmdid)
{
	struct PortData *cport=(PortData *)FindTask(NULL)->tc_UserData;
	struct PortData *mport=(PortData *)MainTask->tc_UserData;
	ULONG ulong,ibm_global_network_ist_scheisse;
	PortData *cport2;
	PortData2 *nc;
	char buffer[100];
	BOOL quit;
	ULONG secs;
	ULONG mics;
	ULONG mics2;
	ULONG progtype;
	if (cport == mport)
		progtype = NOVIA_MAIN;
	else
		progtype = NOVIA_CLIENT;

	switch(cmdid)
	{
		case CMD_SUBBOARD_DEL:
			if ((ulong = (ULONG) ndos_delete(cport->argptr)))
			{
				ioprintf("can't delete %s.\n",cport->argptr);
				PrintDosError((LONG)ulong);
			}
			else
				ioprintf("%s deleted.\n",cport->argptr);

			return TRUE;
			break;
		case CMD_INFO:
			if ((ulong = (ULONG) ndos_info(cport->argstring[1])))
			{
				ioprintf("can't rename %s.\n",cport->argptr);
				PrintDosError((LONG)ulong);
			}
			else
				ioprintf("%s renamed.\n",cport->argptr);

			return TRUE;
			break;
		case CMD_RENAME:
			if ((ulong = (ULONG) ndos_rename(cport->argstring[1],cport->argstring[2])))
			{
				ioprintf("can't rename %s.\n",cport->argptr);
				PrintDosError((LONG)ulong);
			}
			else
				ioprintf("%s renamed.\n",cport->argstring[1]);
			return TRUE;
			break;
		case CMD_POST:
			ioprintf("\nc7Subject:> ");
			Getstring(buffer,0,50,0,0);
			if (strlen(buffer)>=3)
			{
//				VisualEditor("ram:temp");
				{
					BPTR mulder;
					if ((mulder = Open("ram:temp",MODE_OLDFILE)))
					{
						Seek(mulder,0,OFFSET_END);
						if ((ulong = Seek(mulder,0,OFFSET_BEGINNING)))
						{
							struct NewItem *newitem;
							if ((newitem = AllocVec(sizeof(NewItem),MEMF_ANY|MEMF_CLEAR)))
							{
								if ((newitem->text = AllocVec(ulong+1,MEMF_ANY)))
								{
									if (Read(mulder, newitem->text, ulong))
									{
										char quit = FALSE;
										newitem->ItemType			= ITEMTYPE_MESSAGE;
										GetTime(&newitem->CreateDate);
										newitem->Protection		= FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE;
										newitem->CreateUserIP	= cport->LocalUser.IPNumber;
										newitem->FromUser			= cport->LocalUser.IDNumber;
										ioprintf("Write as anonymous ? ");
										if (Yesno(FALSE,0))
										{
											newitem->anonymous = TRUE;
										}
										else
										{
											ioprintf("Write as alias ? ");
											if (Yesno(FALSE,0))
											{
												ioprintf("Enter the SENDER's Username [ENTER=YOU]\n:");
												Getstring(newitem->Alias,0,60,0,0);
											}
										}
										strncpy(newitem->Subject,		buffer,70);
//										Filename[108]
										strncpy(newitem->From,			cport->LocalUser.Handle,sizeof(newitem->From)-1);
										strncpy(newitem->FromRealName,	cport->LocalUser.RealName,sizeof(newitem->FromRealName)-1);
										while (!quit)
										{
											ioprintf("Enter the ADDRESS handle [ENTER=ALL]\n:");
											Getstring(newitem->To,0,20,0,0);
											if (strlen(newitem->To))
											{
												if ((newitem->ToUser = SearchUser(newitem->To)))
												{
													strncpy(newitem->To,			cport->UL.utable[newitem->ToUser].Handle,sizeof(newitem->To)-1);
													strncpy(newitem->ToRealName,cport->UL.utable[newitem->ToUser].RealName,sizeof(newitem->ToRealName)-1);
													quit = TRUE;
												}
												else
												{
													ioprintf("%s is not a user of this system. use anyway ?");
													if (Yesno(FALSE,0))
														quit = TRUE;
												}
											}
											else
												quit = TRUE;
										}
//										MailListMail
//										MailListID
										ioprintf("errror: %d\n",ndos_addmail(newitem));
									}
									FreeVec(newitem->text);
								}
								FreeVec(newitem);
							}
						}
						Close(mulder);
					}
				}
			}
			return TRUE;
			break;
		case CMD_SUBBOARD_NEW:
			{
				Directory *dir;
				if ((dir = ndos_mkdir(cport->argptr)))
				{
					ioprintf("directory %s created.\n",cport->argptr);
					ndos_closedir(dir);
				}
			}
			return TRUE;
			break;
		case CMD_PARRENT:
			ndos_parrentdir();
			return TRUE;
			break;
		case CMD_DIR:
			ndos_execute("novia:sbin/list");
			return TRUE;
			break;
		case CMD_PORTOPEN:
			if (cport->systemservice)
			{
				UBYTE *table;
				UBYTE invalid=TRUE;
				if ((table=AllocVec(10000,MEMF_ANY|MEMF_CLEAR)))
				{
					SelectFromTo(table+1,9999,cport->argstring[1]);
					nc=(PortData2 *)client_list->lh_Head;
					while (nc->port_node.ln_Succ)
					{
						if (table[nc->port_address->clientconfig.IDNumber])
						{
							cport2=nc->port_address;
							Writeio("loadport: ",-1);
							Writelong(cport2->clientconfig.IDNumber);
							LF();
							if (cport2->PortStatus==PORT_STATUS_CLOSE)
							{
								cport2->PortStatus=PORT_STATUS_WAITLOAD;
								CreateClient(cport2);
							}
							else
							{
								SendMsg(cport2->LocalPort,0,0,MSG_PORT_TOFRONT,0);
							}
							invalid=FALSE;
						}
						nc=(PortData2 *)nc->port_node.ln_Succ;
					}
					if (invalid)
					{
						ioprintf("USAGE: OpenPort #IDNumber(s)\n");
						ioprintf("	Example: OP 1-3,8-,5  (Open Port 1,2,3,5,8 and all other Ports higher 8.)\n");
					}
					FreeVec(table);
				} 
				return TRUE;
			}
			break;
		case CMD_PORTCLOSE:
/*			if (cport->systemservice)
			{
				UBYTE *table;
				UBYTE invalid=TRUE;
				if ((table=AllocVec(10000,MEMF_ANY|MEMF_CLEAR)))
				{
					SelectFromTo(table+1,9999,cport->argstring[1]);
					nc=(PortData2 *)client_list->lh_Head;
					while (nc->port_node.ln_Succ)
					{
						if (table[nc->port_address->clientconfig.IDNumber])
						{
							cport2 = nc->port_address;
							Writeio("loadport: ",-1);
							Writelong(cport2->clientconfig.IDNumber);
							LF();
							if (ulong! = cport->clientconfig.IDNumber)
								CloseClientPort(ulong);
							else
								ioprintf("You can't close your own port.\n");
							invalid=FALSE;
						}
						nc=(PortData2 *)nc->port_node.ln_Succ;
					}
					if (invalid)
					{
						ioprintf("USAGE: ClosePort #IDNumber(s)\n");
						ioprintf("	Example: OP 1-3,8-,5  (Open Port 1,2,3,5,8 and all other Ports higher 8.)\n");
					}
					FreeVec(table);
				} 
				return TRUE;
			}	*/
			if (cport->systemservice)
			{
				ULONG PortID;
				StrToLong(cport->argstring[1],(LONG *)&PortID);
				ioprintf("closeport: %d\n",PortID);
				if (PortID != cport->clientconfig.IDNumber)
					CloseClientPort(PortID);
				else
					ioprintf("You can't close your own port.\n");
				return TRUE;
			}
			break;
		case CMD_NEWPORT:
			if (cport->systemservice)
			{
				NewClientPort();
				return TRUE;
			}
			break;							
		case CMD_PORTREMOVE:
			if (cport->systemservice)
			{
				return TRUE;
			}
			break;
		case CMD_RESETBBS:
			if (cport->systemservice)
			{
				return TRUE;
			}
			break;
		case CMD_EDITCONFIG:
			if (cport->systemservice)
			{
/*				struct MainPortConfig *config2;
				if (config2=AllocVec(sizeof(MainPortConfig),MEMF_ANY|MEMF_CLEAR))
				{
					CopyMemQuick(mainportconfig,config2,sizeof(MainPortConfig));
					if (VDE(VDEID_MAINPORTCONFIG,(char *)config2))
					{
						if ((config2->ScreenMode!=mainportconfig->ScreenMode)|
							(config2->CH_Main!=mainportconfig->CH_Main)|
							(config2->Colors!=mainportconfig->Colors)|
//							(config2->FontName!=mainportconfig->FontName)|
							(config2->Fontsize!=mainportconfig->Fontsize)|
//							(config2->TermFontName!=mainportconfig->TermFontName)|
							(config2->TermFontsize!=mainportconfig->TermFontsize))
						{
							cport->clientconfig.ScreenModeID=config2->ScreenMode;
							cport->clientconfig.Colors=config2->Colors;
							CopyMem(cport->clientconfig.ScreenFont,config2->FontName,32);
							cport->clientconfig.ScreenFontsize=config2->Fontsize;
							CopyMem(cport->clientconfig.TermFont,config2->TermFontName,32);
							cport->clientconfig.TermFontsize=config2->TermFontsize;
							Monitor_close();
							Monitor_open();
						}
						CopyMemQuick(config2,mainportconfig,sizeof(MainPortConfig));
						SaveMainPortConfig(mainportconfig);
					}
					FreeVec(config2);
				}
				else
					ioprintf("Not anouth memory to edit mainport.config.\n");
				return TRUE;*/
				execute_nsl("novia:scripts/edit_mainconfig.nsl");
			}
			break;
		case CMD_EDITCLIENT:
			if (cport->systemservice)
			{
				StrToLong(cport->argstring[1],(LONG *)&ulong);
				if (ulong && (cport2=FindClient(ulong)))
				{
/*					if (VDE(VDEID_CLIENTCONFIG,(char *)cport2))
					{
						SaveClientConfig(&cport2->clientconfig);
					}*/
				}
				else
				{
					ioprintf("USAGE: OpenPort [#IDNumber|ALL]\n");
				}
				return TRUE;
			}
			break;
		case CMD_LOGIN:
			ioprintf("\nLogin Username:> ");
			Getstring(buffer,0,20,0,0);
			if ((ulong=SearchUser(buffer)))
			{
				ndos_saveselectlist((List *)&cport->select_list);
				ioprintf("load user\n");
				LoadUser(&cport->LocalUser,ulong);

				ioprintf("\nPassword      :> ");
				ioprintf("\nCurrent User is :%s (%d)\n",cport->LocalUser.Handle,ulong);
				ndos_loadselectlist((List *)&cport->select_list);
				ndos_getlist();
			}
			else
			{
				ioprintf("\nunknown User: %s \n",buffer);
			}
			return TRUE;
			break;
		case CMD_IDENTIFY:
			if (cport->LocalUser.Flag_supervisor)
			{
				ioprintf("\nPassword :> ");
				ioprintf("\nEncrypt  :> \n\n");
				ioprintf("system services commands now available.\n");
				cport->systemservice=TRUE;
				return TRUE;
			}
			break;
		case CMD_PING:
			StrToLong(cport->argstring[1],(LONG *)&ulong);
			cport2=FindClient(ulong);
			if (cport2)
			{
				if (cport2->PortStatus>0)
				{
					ULONG ticks;
					timeval tv;
					double double1;
					double double2;
					struct EClockVal eclock;
					struct EClockVal eclock2;
					ReadEClock(&eclock);
					SendMsg(cport2->LocalPort,0,0,MSG_PING,0);
					Wait(1<<cport->MainPort->mp_SigBit);
					ticks=ReadEClock(&eclock2)/1000;
					double1=(double)eclock.ev_lo;
					double2=(double)eclock2.ev_lo;

					ioprintf("\n%d, bytes from %d: time=%f ms\n",sizeof(message),cport2->clientconfig.IDNumber,(double2-double1)/(double)ticks);
				}
			}
			return TRUE;
			break;
		case CMD_JOIN:
			if (cport==mport)
			{
				EditNRCList(nrcserver->roomlist);
			}
			else
				OpenConference();
			return TRUE;
			break;
		case CMD_SHELL:
//			NewShell();
			break;
		case CMD_CONFIG:
			if (cport->argcounter==0)
			{
				Writeio("Usage: CONFIG COMMAND|MENU...\n\n",-1);
			}
			else
			{
/*				if (!stricmp(cport->argstring[1],"COMMAND"))
					VDEEditCommandList();*/
			}			
			return TRUE;
			break;
		case CMD_CLOSEGUI:
			if (cport->systemservice)
			{
				if (progtype==NOVIA_MAIN)
				{
					if (guifrontend.GUIOpen && GUITask)
					{
						guifrontend.ProgramClose=TRUE;
						Signal(GUITask,GUITask->tc_SigWait);
						ioprintf("wait for closeing GUI Sig\n");
						while (guifrontend.GUIOpen)
						{
							Delay(5);
						}
					}
				}
				else
					ioprintf("GUI can't close from remote Port.\n");
				return TRUE;
			}
			break;
		case CMD_LOADGUI:
			if (cport->systemservice)
			{
				if (progtype==NOVIA_MAIN)
				{
					StartGUI();
				}
				else
					ioprintf("GUI can't open from remote Port.\n");
				return TRUE;
			}
			break;
		case CMD_LOGOFF:
			if (progtype==NOVIA_CLIENT)
			{
				ClearMemQuick(&cport->LocalUser,sizeof(UserData));
				ClearMemQuick(&cport->EditUser,sizeof(UserData));
				cport->systemservice=FALSE;
				ioprintf("\nc7%s c3logged off\n",cport->LocalUser.Handle);
			}
			else
				ioprintf("This is a client command.\n");
			return TRUE;
			break;
		case CMD_QUIT:
			if (progtype==NOVIA_CLIENT)
			{
				ClearMemQuick(&cport->LocalUser,sizeof(UserData));
				ClearMemQuick(&cport->EditUser,sizeof(UserData));
				cport->systemservice=FALSE;
				ioprintf("\nc7%s c3logged off\n",cport->LocalUser.Handle);
				cport->PortStatus=PORT_STATUS_WAITCALL;
				cport->network.online=FALSE;
			}
			else
			{
				if (cport->systemservice)
				{
					cport->ProgramClose=TRUE;
					cport->network.online = FALSE;
				}
			}
			return TRUE;
			break;
		case CMD_EDITACCOUNT:
			if (cport->systemservice)
			{
				if (ulong=FindUser(cport->argstring[1]))
					UserEdit(ulong);
				return TRUE;
			}
			break;
		case CMD_EDITFILE:
			if (cport->systemservice)
			{
				if (strlen(cport->argstring[1]))
				{
//					VisualEditor(cport->argstring[1]);
				}
				else
				{
					Writeio("Usage: EDITFILE filename\n",-1);
					LF();
				}
				return TRUE;
			}
			break;
		case CMD_SENDMAIL:
			ioprintf("NOViA MailSend ©1997-1999 by Campino/UFoP\n");
			if (cport->LocalUser.Flag_send_email | cport->LocalUser.Flag_supervisor)
			{
/*				if (strlen(cport->argstring[1]))
				{
					NewMail *newmail;
					if ((newmail = AllocVec(sizeof(NewMail),MEMF_ANY|MEMF_CLEAR)))
					{
						if (!(newmail->ToUser = SearchUser(cport->argstring[1])))
						{
							BOOL uucp = FALSE;
							char *ptr = cport->argstring[1];
							while (*ptr && !uucp)
								if (*ptr++ == '@') uucp = TRUE;
							if (uucp)
							{
								UWORD strlength = strlen(cport->argstring[1]);
								if ((newmail->To = AllocVec(strlength + 1,MEMF_ANY)))
									strncpy(newmail->To, cport->argstring[1], sizeof(newmail->To)-1);
							}
						}
						if (newmail->ToUser || newmail->To)
						{
							ULONG strlength;
							ioprintf("Enter Subject:");
							Getstring(newmail->subject,0,60,0,0);
							ioprintf("Write as an alias ? y/[N]");
							if (Yesno(FALSE,0))
							{
								ioprintf("Enter alias:");
								Getstring(newmail->Alias,0,60,0,0);
							}
							ioprintf("CC:");
							Getstring(buffer,0,60,0,0);
							strlength = strlen(buffer);
							if (strlength >1)
							{
								if (!(newmail->CCUser = SearchUser(buffer)))
								{
									char *ptr = buffer;
									BOOL uucp = FALSE;
									while (!uucp && *ptr && *ptr != '@')
										ptr++;
									if (*ptr == '@')
									{
										if ((newmail->CC = AllocVec(strlength + 1,MEMF_ANY)))
											strncpy(newmail->CC, buffer, sizeof(newmail->CC)-1);
									}
								}
							}
							ioprintf("BCC:");
							Getstring(buffer,0,60,0,0);
							strlength = strlen(buffer);
							if (strlength >1)
							{
								if (!(newmail->BCCUser = SearchUser(buffer)))
								{
									char *ptr = buffer;
									BOOL uucp = FALSE;
									while (!uucp && *ptr && *ptr != '@')
										ptr++;
									if (*ptr == '@')
									{
										if ((newmail->BCC = AllocVec(strlength + 1,MEMF_ANY)))
											strncpy(newmail->BCC, buffer,sizeof(newmail->BCC)-1);
									}
								}
							}
							ioprintf("Importance ? low/normal/high");
							GetChar(buffer,0);
							if (*buffer == 'l' | *buffer == 'L')
								newmail->importance = MAIL_IMPORTANCE_LOW;
							else
							{
								if (*buffer == 'h' | *buffer == 'H')
									newmail->importance = MAIL_IMPORTANCE_HIGH;
								else
									newmail->importance = MAIL_IMPORTANCE_NORMAL;
							}
							ioprintf("Copy Mail to your own Mailbox ? y/[N]");
							if (Yesno(FALSE,0))
								newmail->copy_own = TRUE;
							VisualEditor("ram:temp");
							ioprintf("%d\n",uucp_sendmail(newmail));
						}
						FreeVec(newmail);
					}
					else
					{
						ioprintf("ERROR: out of memory.\n");
					}
				}
				else
				{
					ioprintf("Usage: MS username|usernumber\n");
				}
*/
				VisualEditor("ram:test",0,VISUALEDIT_FILE);
			}
			else
			{
				ioprintf("\nYour are can't write mails.\n");
			}
			return TRUE;
			break;
		case CMD_MAILVERIFY:
			ulong=SearchUser(cport->argstring[1]);
			if (ulong==0)
			{
				LF();ioprintf("NOViA MailVerify ©1997-1999 by Thorsten Gehler/UFoP\n");
				ioprintf("Usage: MV username|usernumber\n");
			}
//			else
//				MailVerify(ulong);
			return TRUE;
			break;
		case CMD_STATUS:
			if (cport==mport)
			{
				nc=(PortData2 *)client_list->lh_Head;
				ioprintf("\n");
				while (nc->port_node.ln_Succ)
				{
					cport2=nc->port_address;
					ioprintf("PortNo: %d, Handle: %s, status: %s\n",cport2->clientconfig.IDNumber,cport2->LocalUser.Handle,getstr(60,(UWORD)cport2->PortStatus));
					nc=(PortData2 *)nc->port_node.ln_Succ;
				}
				return TRUE;
			}
			else
			{
				LF();
				Writeio("IDNumber : ",-1);Writelong(cport->LocalUser.IDNumber);LF();
				Writeio("IPNumber : ",-1);Writelong(cport->LocalUser.IPNumber);LF();
				Writeio("Handle   : ",-1);ioprintf("%s\n",cport->LocalUser.Handle);
				Writeio("Password : ",-1);ioprintf("%s\n",cport->LocalUser.PassWord);
				Writeio("PWEncrypt: ",-1);ioprintf("%s\n",cport->LocalUser.PassWordEncrypt);
				LF();
				ioprintf("Press a key to continue.\n");
				WaitKey(0);
			}
			return TRUE;
			break;
		case CMD_READMAIL:
			if (cport->LocalUser.Flag_read_priv_msg|cport->LocalUser.Flag_supervisor)
			{
//				MailVerify(cport->LocalUser.IDNumber);
//				GetINETMail();
				return TRUE;
			}
			break;
		case CMD_SHOWUSERLIST:
			if (cport->LocalUser.Flag_use_userlist|cport->LocalUser.Flag_supervisor)
			{
				ShowUserList();
				return TRUE;
			}
			else
			{
				ioprintf("\nSorry. You're can't use this command.\n");
			}
			break;		
		case CMD_OLM:
			SendOnlineMessage(cport);
			return TRUE;
			break;
		case CMD_USERINFO:
			LF();
			ulong=SearchUser(cport->argstring[1]);
			if (ulong>0)
			{
				if (LoadUser(&cport->EditUser,ulong)==TRUE)
				{
					sprintf(buffer,"%ssys.userinfo",cport->sysdirs->systext);
					ShowText(buffer);
					ioprintf("Press a key to continue\n");
					WaitKey(0);
					return TRUE;
				}
			}
			Writeio("Unknown User: ",-1);
			ioprintf("%s\n",cport->argstring[1]);
			ioprintf("Usage: Userinfo [Username|Usernumber]\n");
			return TRUE;
			break;
		case CMD_WHO:
			if (cport==mport)
			{
/*				struct PortData2 *clientport;
				struct PortData *cport2;
				char *txtptr=AllocVec(1000,MEMF_ANY|MEMF_CLEAR);
				if (txtptr=AllocVec(1000,MEMF_ANY|MEMF_CLEAR))
				{
					sprintf(txtptr,"Port Username            Logon  Spd From                Where        \n");
					txtptr=txtptr+strlen(txtptr);
					sprintf(txtptr,"==== =================== ====== === =================== =============\n");
					txtptr=txtptr+strlen(txtptr);
					clientport=(PortData2 *)client_list->lh_Head;
					while (clientport->port_node.ln_Succ)
					{
						cport2=clientport->port_address;
						sprintf(buffer,"%4d %-19s %2d:%02d  %3d %-19s %s\n",
							cport2->clientconfig.IDNumber,
							cport2->LocalUser.Handle,
							cport2->LastLogin.hours,
							cport2->LastLogin.mins,
							cport2->CurrentCPS,
							cport2->LocalUser.City,
							cport2->Where);
						strcpy(txtptr,buffer);
						txtptr=txtptr+strlen(buffer);
						clientport=(PortData2 *)clientport->port_node.ln_Succ;
					}
					ioprintf(txtptr,-1);
					FreeVec(txtptr);
				}*/
				startrexxprg("rexxc:noviatest.rexx");
			}
			else
			{
				ioprintf("Your User: %d from %s\n",cport->clientconfig.IDNumber,cport->whostring);
				SendMsg(cport->MainPort,0,0,MSG_COMMAND,CMD_WHO);
			}
			return TRUE;
			break;
		case CMD_REPAIR:
//			RepairUserList(&cport->UL);
			ioprintf("cport: %x\n",cport);
			{
				char buffer[100];
				sprintf(buffer,"mcpp:/novia/convert %d",cport);
				Execute(buffer,(BPTR)NULL,NULL);
			}
			return TRUE;
			break;
		case CMD_EDIT:
			ndos_editflags(cport->argptr);
			return TRUE;
			break;
		case CMD_VIEWSELECTLIST:
			{
				struct SelectItem *select = (SelectItem *)cport->select_list.mlh_Head;
				struct Item item;
				while (select->ln_Succ)
				{
//					if (GetItem_byIP(select->dirIP,select->itemIP,&item,sizeof(ItemHeader)));
						ioprintf("selected\n");
//						ioprintf("%-40d\n",item.itemheader.Title);
					select = select->ln_Succ;
				}
			}
			return TRUE;
			break;
		case CMD_DOWNLOAD_SELECT:
			{
				char quit = 0;
				struct Select *select=(Select *)cport->select_list.mlh_Head;
				while (quit<1 && select->ln_Succ)
				{
					ioprintf("download: (%s)%-40s ([Yes],No,All,Cancel,Quit) ? ",select->dirpath,select->filepath);
					switch (AskKey(RESULT_YES , FLAG_YES | FLAG_NO | FLAG_ALL | FLAG_CANCEL | FLAG_QUIT , 0))
					{
						case RESULT_YES:
							select->downloadnow=TRUE;
							break;
						case RESULT_NO:
							select->downloadnow=FALSE;
							break;
						case RESULT_ALL:
							while (select->ln_Succ)
							{
								select->downloadnow=TRUE;
								select=select->ln_Succ;
							}
							quit=1;
							break;
						case RESULT_CANCEL:
							while (select->ln_Succ)
							{
								select->downloadnow=FALSE;
								select=select->ln_Succ;
							}
							quit=1;
							break;
						case RESULT_QUIT:
							select=(Select *)cport->select_list.mlh_Head;
							while (select->ln_Succ)
							{
								select->downloadnow=FALSE;
								select=select->ln_Succ;
							}
							quit=2;
							break;
					}
					select=select->ln_Succ;
				}
				if (quit == 1)
				{
					ioprintf("do transfer\n");
					if (!IsListEmpty((List *)&cport->select_list))
					{
						struct SelectItem *select=(SelectItem *)cport->select_list.mlh_Head;
						char quit=FALSE;
						cport->actual = NULL;							// reset actual download file
						while (select->ln_Succ && !quit)
						{
							if (select->downloadnow)
							{
								cport->actual = select;					// set first file to download
								quit = TRUE;
							}
							select=select->ln_Succ;
						}
					}
					else
						ioprintf("no files to download\n");
				}
					
				if (IsListEmpty((List *)&cport->select_list))
					ioprintf("You have no no files selected\n");
				else
					DoTransfer();

			}
			return TRUE;
			break;
	}
	return NULL;
}