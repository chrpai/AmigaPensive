#include <exec/exec.h>
#include <exec/types.h>

#include <proto/exec.h>

#include "wwbbs_settings.h"

#include "wwbbs_lib.h"

extern struct SignalSemaphore settings_semaphore;
extern struct SettingsStruct settings;

struct Libraries * __saveds OpenLibraries()
	{
		struct Libraries *libs=NULL;
		BOOL ok=FALSE;
		if(libs=AllocVec(sizeof(struct Libraries),MEMF_CLEAR))
			{
				ok=TRUE;
				if(!(libs->DOSBase=(struct DosLibrary *) OpenLibrary("dos.library",0)))
					ok=FALSE;
				if(!(libs->IntuitionBase=(struct IntuitionBase *) OpenLibrary("intuition.library",0)))
					ok=FALSE;
				if(!(libs->GfxBase=(struct GfxBase *) OpenLibrary("graphics.library",0)))
					ok=FALSE;
				if(!(libs->CxBase=OpenLibrary("commodities.library",0)))
					ok=FALSE;
				if(!(libs->DiskfontBase=OpenLibrary("diskfont.library",0)))
					ok=FALSE;
				if(!(libs->GadToolsBase=OpenLibrary("gadtools.library",0)))
					ok=FALSE;
				if(!(libs->UtilityBase=OpenLibrary("utility.library",0)))
					ok=FALSE;
				if(!(libs->WorldWideBase=OpenLibrary("wwbbs.library",0)))
					ok=FALSE;
				ObtainSemaphore(&settings_semaphore);
				libs->Settings=settings;
				ReleaseSemaphore(&settings_semaphore);
				if(!ok)
					CloseLibraries(libs);
			}
		return(libs);
	}

void __saveds CloseLibraries(struct Libraries *libs)
	{
		if(libs->WorldWideBase) CloseLibrary(libs->WorldWideBase);
		if(libs->UtilityBase) CloseLibrary(libs->UtilityBase);
		if(libs->GadToolsBase) CloseLibrary(libs->GadToolsBase);
		if(libs->DiskfontBase) CloseLibrary(libs->DiskfontBase);
		if(libs->CxBase) CloseLibrary(libs->CxBase);
		if(libs->GfxBase) CloseLibrary((struct Library *) libs->GfxBase);
		if(libs->IntuitionBase) CloseLibrary((struct Library *) libs->IntuitionBase);
		if(libs->DOSBase) CloseLibrary((struct Library *) libs->DOSBase);
		if(libs) { FreeVec(libs); libs=NULL; }
	}
