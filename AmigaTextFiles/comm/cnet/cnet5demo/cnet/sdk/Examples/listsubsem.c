// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main( void )
{
	struct MainPort *myp;
	struct SubboardType4 *sp;
	BPTR inputfh=Input();
	short i=0;
	char inchar;

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		for( i=0; i<myp->ns; i++ )
			{
			sp = &myp->Subboard[i];

			if(sp->sem->ss_NestCount || sp->sem->ss_Owner)
				{
				printf("\n        Title: %s\n", sp->Title);
				printf("         &sem: %x\n", sp->sem);
				printf(" ss_NestCount: %x\n", sp->sem->ss_NestCount);
				printf("     ss_Owner: %x\n", sp->sem->ss_Owner);
				}

			if(WaitForChar(inputfh, 0L))
				{
				Read(inputfh, (char *)&inchar, 1);
				if(inchar == 27)
					exit(0);
				}
			}
		}
}
