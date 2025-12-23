// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main( void )
{
	struct MainPort *myp;
	struct IMPStruct *impwork, *impnext;
	
	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		ObtainSemaphore(&myp->MPE->sem[3]);
		// walk IMP list and see if "impcheck" matches the "Name" field of IMPStruct 
		impwork = (struct IMPStruct *)myp->MPE->IMPList.lh_Head;
		while( impnext = (struct IMPStruct *) ((struct Node *)impwork)->ln_Succ )
			{
			// compare (ADOS pattern match) impcheck with impwork->Name
			printf("\n        Name: \"%s\"", impwork->Name);
			printf("\n ProcessName: \"%s\"", impwork->ProcessName);
			printf("\nProcessReply: \"%s\"", impwork->ProcessReply);
			printf("\n       dflag: %d", impwork->dflag);
			printf("\n       pflag: %d\n", impwork->pflag);
			impwork=impnext;
			}
		ReleaseSemaphore(&myp->MPE->sem[3]);
		}
	printf("\n");
}
