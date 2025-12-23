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
		CloseLibrary(NoviaSysBase);
	}
}