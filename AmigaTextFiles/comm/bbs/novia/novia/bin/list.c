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
		LONG result = FALSE;
		ItemEntryBlock *ieb;
		char		buf[120];
		char		bits[9];
		char		size[20];
		char		date[20];
		if (cport && cport->currentdir)
		{
			List  newlist;
			ItemEntryBlock *currentieb;
			ULONG cmpresult;
			BOOL quit;
			NewList(&newlist);
			ieb = cport->itementrylist.lh_Head;
			while (ieb->ln_Succ)
			{
				strcpy(bits,"--------");
				if (ieb->Protection & FIBF_SCRIPT)
					bits[1]='s';
				if (ieb->Protection & FIBF_PURE)
					bits[2]='p';
				if (ieb->Protection & FIBF_ARCHIVE)
					bits[3]='a';
				if (ieb->Protection & FIBF_READ)
					bits[4]='r';
				if (ieb->Protection & FIBF_WRITE)
					bits[5]='w';
				if (ieb->Protection & FIBF_EXECUTE)
					bits[6]='e';
				if (ieb->Protection & FIBF_DELETE)
					bits[7]='d';
	
				if (ieb->ItemType == ITEMTYPE_SUBBOARD)
					strcpy(size,"Dir");
				else
				{
					if (ieb->Size >= 100000)
					{
						if (ieb->Size >= 10000000)
						{
							double erg = double(ieb->Size) / (1024 * 1024);
							sprintf(size,"%##.1g MB",erg);
						}
						else
						{
							double erg = double(ieb->Size) / 1024;
							sprintf(size,"%##.1g KB",erg);
						}
					}
					else
						sprintf(size,"%d   ",ieb->Size);
				}
				DateToString(date,&ieb->LastChange, 0);
				if (cport->currentdir->NetworkType)
				{
					if (ieb->selected)
						ioprintf("*%-3d %-3u %-40s %8s %s %s\n",ieb->IDNumber, ieb->IPNumber,ieb->Title,size,date,ieb->Comment);
					else
						ioprintf(" %-3d %-3u %-40s %8s %s %s\n",ieb->IDNumber, ieb->IPNumber,ieb->Title,size,date,ieb->Comment);
				}
				else
				{
					if (ieb->selected)
						ioprintf("*%-3d %-3u %-40s %8s %s %s\n",ieb->IDNumber, ieb->IPNumber,ieb->Title,size,bits,date);
					else
						ioprintf(" %-3d %-3u %-40s %8s %s %s\n",ieb->IDNumber, ieb->IPNumber,ieb->Title,size,bits,date);
				}
				ieb = ieb->ln_Succ;
			}
			ioprintf("%d file(s) - %d directory(s) - %u bytes listed.\n",cport->itementrylist.files,cport->itementrylist.dirs,cport->itementrylist.size);
		}
		CloseLibrary(NoviaSysBase);
	}
}