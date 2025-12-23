// ---------------- internet structures used by CNet/4+ Pro-------------------
// **     FIRST APPEARED in CNet V4.2bR2 (post final-beta version)



// config structure saved as a **header** in mail:users/xxx/prefs/cnirc
// following this structure - on disk - are server, info and auto-join channel
// instances each on a separate line.
struct IrcUserConfig
	{
	UBYTE ANSI; // 0=ANSI scrolling, 1=ANSI locking, 2=simple ANSI
	ULONG flags; // currently unused - expansion
	struct List *serverlist;	// Exec list of servers to autojoin - currently internal use only
                              // may be read directly from user cnirc config
                              // file.
	};



// server structure used internally by CNetIRC
struct ServerNode
	{
	struct Node snode;	// IP/hostname in snode.ln_Name
	char sinfo[36];		// textual description of server
	char channel[40];		// autojoin channel
	};


// NEW 4.26c+ server structure used internally by CNetIRC
struct ServerNode1
	{
	struct Node snode;	// IP/hostname in snode.ln_Name
	char sinfo[36];		// textual description of server
	char channel[40];	// autojoin channel
	char RealName[31];	// max length = 30
	char NickName[11];	// max length = 10
	char QuitMsg[80];	// msg to print when quitting - maxlength = 79
	};



// identd structure saved in linked list myp->MPE->idd
// always ObtainSemaphore(myp->MPE->sem[1]) before accessing myp->MPE->idd
// as IdentData may be removed at any time when a client is done with it's
// TCP socket(s).
//
// Also see cnetfuncs.h for CNet 3rd party-friendly identd
// insert/remove/lookup functions present in cnet.library.
//
// an example of identd insertion/removal/lookup is present as
// sdk/examples/identdtest.c
//
struct IdentdData
	{
	unsigned int socket;	// local port owned by user
	short port;	   // local user's CNet port number
	struct IdentdData *next;
	};
