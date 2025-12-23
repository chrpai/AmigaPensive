// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

struct Library      *CNet4Base = NULL;	// CNet4 library base
struct Library      *CNetBase = NULL;	// CNet4 library base

struct MainPort     *myp = NULL; // pointer into context for MainPort; same idea as for z above


// Exit/cleanup code goes in here
void GetOut(void)
{
	if(CNet4Base)
		{
		CloseLibrary(CNet4Base);
		CNet4Base=NULL;
		}
	if(CNetBase)
		{
		CloseLibrary(CNetBase);
		CNetBase=NULL;
		}
}


void main(int argc, char **argv)
{
	LONG zzz=0;
	char date[40];
	struct UserData *u;

   // ----------------------- CNETC DOOR SETUP STARTS HERE --------------------
	atexit(GetOut);
   if( !(CNet4Base = OpenLibrary("cnet4.library", 4)) ) // library required for CalculateAge() and other date-related routines
   	GetOut();
   if( !(CNetBase = OpenLibrary("cnet.library", 4)) ) // library required for CalculateAge() and other date-related routines
   	GetOut();

	// -------------------------- YOUR CODE BEGINS HERE ------------------------

	if(u=LockAccount(1))
		{
		CNetPrintDate(&u->LastCall, date, NULL, NULL);
		printf("     Last Call: %s\n\n", date);
		CNetPrintDate(&u->Birthdate, date, NULL, NULL);
		printf("     Birthdate: %s\n\n", date);
		UnLockAccount(1, FALSE);
		}
		
   exit(0); // exit back to CNet
}

