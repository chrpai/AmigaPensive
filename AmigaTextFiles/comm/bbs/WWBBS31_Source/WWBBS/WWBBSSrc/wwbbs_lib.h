struct Libraries {
	struct DosLibrary *DOSBase;
	struct IntuitionBase *IntuitionBase;
	struct GfxBase *GfxBase;
	struct Library *CxBase;
	struct Library *DiskfontBase;
	struct Library *GadToolsBase;
	struct Library *UtilityBase;
	struct Library *WorldWideBase;
	struct SettingsStruct Settings;
};

struct Libraries * __saveds OpenLibraries(void);
void __saveds CloseLibraries(struct Libraries *);
