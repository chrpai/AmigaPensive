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
		struct PortData *cport2;
		struct PortData *cport = (struct PortData *)FindTask(NULL)->tc_UserData;
		if ((cport2 = FindClient(ulong)))
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
		CloseLibrary(NoviaSysBase);
	}
}