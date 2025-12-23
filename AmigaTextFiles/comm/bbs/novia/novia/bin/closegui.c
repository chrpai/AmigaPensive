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
		CloseLibrary(NoviaSysBase);
	}
}