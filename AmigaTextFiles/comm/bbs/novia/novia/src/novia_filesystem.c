#include <string.h>
#include <pragma/exec_lib.h>
#include <pragma/noviasys_lib.h>
#include <novia/novia_portdata.h>
#include <novia/novia_dos.h>
#include <exec/tasks.h>

extern MainPortConfig *mainportconfig;

void adddir_atroot(char *name)
{
	struct Directory *dir;
	if ((dir = AllocVec(sizeof(Directory), MEMF_ANY | MEMF_CLEAR)))
	{
		BPTR fh = NULL;
/*		if (!(fh = CreateDir(name)))
		{
			fh = Lock(name,SHARED_LOCK);
		}
		if (fh)*/
		{
//			UnLock(fh);
			strncpy(dir->iib.Title, name, sizeof(dir->iib.Title)-1);// copy subdir name into title
			strncpy(dir->iib.Filename, name, sizeof(dir->iib.Filename)-1);

			dir->iib.Protection = (FIBF_READ | FIBF_WRITE | FIBF_EXECUTE | FIBF_DELETE);

			dir->iib.ItemType=ITEMTYPE_SUBBOARD;						// set ItemType
			dir->iib.IPNumber = mainportconfig->BaseIPCounter++ +1;
			SaveMainPortConfig(mainportconfig);
			dir->fh_messages	= NULL;
			dir->fh_files		= NULL;
			dir->fh_dirs		= NULL;
			dir->fh_text		= NULL;
			dir->fh_replys		= NULL;

			GetTime(&dir->iib.LastChange);								// Get actual systemtime.
			CopyMemQuick(&dir->iib.LastChange, &dir->iib.CreateDate, sizeof(Date)); // 'nd copy date to createtime


			if ((fh = Open("novia:.dirs", MODE_READWRITE)))
			{
				Seek(fh, 0, OFFSET_END);
				Write(fh, dir, sizeof(Directory));
				Close(fh);
				ioprintf("directory %s added\n",name);
			}
			else
				ioprintf("can't add directory entry for %s\n",name);
		}
/*		else
			ioprintf("Can't create %s\n",name);*/
		FreeVec(dir);
	}
}

void LowLevelCommand_addcommand()
{
	struct PortData *cport = (PortData *)FindTask(NULL)->tc_UserData;
	if (cport)
		nslib_list(cport);
}

void LowLevelCommand_adddirectory()
{
}

void LowLevelCommand_makeroot()
{
	char *name="novia:sbin/helloworld";
	ioprintf("name: %s %x\n",name,name);
	ndos_execute(name);
}

void LowLevelCommand_addlink()
{
}


			
void LowLevelCommand_makefs()
{
	DeleteFile("dirs");
	adddir_atroot("bin");
	adddir_atroot("sbin");
	adddir_atroot("etc");
	adddir_atroot("scripts");
	adddir_atroot("etc");
	adddir_atroot("home");
	adddir_atroot("incoming");
	adddir_atroot("outgoing");
	adddir_atroot("lost+found");
}







