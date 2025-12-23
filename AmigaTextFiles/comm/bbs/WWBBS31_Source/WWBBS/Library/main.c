#include "wwbbs.h"
#include "config.h"
#include "status.h"
#include "users.h"
#include "messages.h"
#include "files.h"

int __UserLibInit(void)
	{
		SysBase=(struct ExecBase *) 4;
		DOSBase=(struct DosLibrary *) OpenLibrary("dos.library",0);
		UtilityBase=OpenLibrary("utility.library",0);

		UpdatePort=NULL;
		InitSemaphore(&UpdateSemaphore);

		NewList(&ConfigList);
		InitSemaphore(&ConfigSemaphore);
		NewList(&StatusList);
		InitSemaphore(&StatusSemaphore);
		NewList(&UserList);
		InitSemaphore(&UserSemaphore);
		NewList(&MessageGroupList);
		InitSemaphore(&MessageGroupSemaphore);
		NewList(&FileGroupList);
		InitSemaphore(&FileGroupSemaphore);

		LoadConfig(ConfigFile);
		LoadUserIndex(UserIndexFile);

		return(0);
	}

void __UserLibCleanup(void)
	{
		ObtainSemaphore(&FileGroupSemaphore);
		FlushFileGroups();
		ReleaseSemaphore(&FileGroupSemaphore);
		ObtainSemaphore(&MessageGroupSemaphore);
		FlushMessageGroups();
		ReleaseSemaphore(&MessageGroupSemaphore);
		UnLoadUserIndex();
		UnLoadStatus();
		UnLoadConfig();

		if(UtilityBase) CloseLibrary(UtilityBase);
		if(DOSBase) CloseLibrary((struct Library *) DOSBase);
	}
