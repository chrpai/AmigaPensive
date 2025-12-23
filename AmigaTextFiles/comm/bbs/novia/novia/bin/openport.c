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
			}		CloseLibrary(NoviaSysBase);
	}
}