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
		CloseLibrary(NoviaSysBase);
	}
}