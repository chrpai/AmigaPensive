#include <pragma/noviasys_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <stdio.h>
#include <novia/novia_portdata.h>

struct Library *NoviaSysBase;

void ioprintf(const char *string, ...)
{
	char *buffer=AllocVec(10000,MEMF_ANY|MEMF_CLEAR);
	if (buffer)
	{
		vsprintf(buffer, string, unsigned int(&string + 1));
		Writeio(buffer,-1);
		FreeVec(buffer);
	}
}

void main()
{
	if ((NoviaSysBase = OpenLibrary("noviasys.library", 0)))
	{
		struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
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
		CloseLibrary(NoviaSysBase);
	}
}