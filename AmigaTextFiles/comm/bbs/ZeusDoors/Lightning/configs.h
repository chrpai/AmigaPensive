#include <proto/pipeline.h>
#include <proto/pipeutil.h>

typedef struct FL_Config
{
	char nodenum[108];
	char freqfile[108];
	char insdir[108];
	char about[41];
	char defdesc[108];

	BOOL longdesc;
	BOOL update;
	BOOL skipunval;
	BOOL sortalpha;
	BOOL sortsize;
	BOOL sortfwd;
	BOOL updatefilecat;
	BOOL noregname;

	LONG filebuffers;
	LONG wrapdesc;
	time_t newtime;

	char markunval[2];

	BPTR fhtxt;
	char alltxt[108];
	char arcalltxt[108];
	char cmmalltxt[108];

	BPTR fhnewtxt;
	char newtxt[108];
	char arcnewtxt[108];
	char cmmnewtxt[108];

	BPTR fhibm;
	char allibm[108];
	char arcallibm[108];
	char cmmallibm[108];
	
	BPTR fhnewibm;
	char newibm[108];
	char arcnewibm[108];
	char cmmnewibm[108];

	BPTR fhgde;
	char allgde[108];
	char arcallgde[108];
	char cmmallgde[108];

	BPTR fhnewgde;
	char newgde[108];
	char arcnewgde[108];
	char cmmnewgde[108];

	char areasstring[256];
	struct List *areas;
};

typedef struct FL_AreaNode
{
	struct  MinNode node;
	ULONG Number;
	BOOL Use;
	BOOL New;
};


typedef struct KeyFile {
	char Sysop[40];
	ULONG a[240];
};

