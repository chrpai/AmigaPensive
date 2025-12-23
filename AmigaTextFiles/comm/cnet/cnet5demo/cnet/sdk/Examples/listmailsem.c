/*
 * Mail semaphore lister..  Unmolested output should be similar to that
 * below, with ss_Link, ss_NestCount, ss_MultipleLink and ss_Owner all 0.
 *
 * MailSem[21]
 * ss_Link        : 0
 * ss_NestCount   : 0
 * ss_WaitQueue   : 114A
 * ss_MultipleLink: 0
 * ss_Owner       : 0
 * ss_QueueCount  : FFFF
 *
 */

struct DosLibrary *DOSBase=NULL;

__saveds void main( void )
{
	struct MainPort *myp=NULL;

	if(DOSBase = (struct DosLibrary *)OpenLibrary("dos.library", 37L))
		{
		if(myp=(struct MainPort *)FindPort(CNETPORT))
			{
			int xxx;

			if(SetSignal(0L, SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
				{
				CloseLibrary((struct Library *)DOSBase);
				return;
				}

			for(xxx=0;xxx<myp->Nums[0];xxx++)
				{
				Printf("\nMailSem[%ld]\n", 		xxx);
				Printf("ss_Link        : %x\n",	myp->MPE->MailSem[xxx].ss_Link);
				Printf("ss_NestCount   : %ld\n", myp->MPE->MailSem[xxx].ss_NestCount);
				Printf("ss_WaitQueue   : 0x%x\n",	myp->MPE->MailSem[xxx].ss_WaitQueue);
				Printf("ss_MultipleLink: %ld\n",	myp->MPE->MailSem[xxx].ss_MultipleLink);
				Printf("ss_Owner       : 0x%x\n",	myp->MPE->MailSem[xxx].ss_Owner);
				Printf("ss_QueueCount  : 0x%x\n",	myp->MPE->MailSem[xxx].ss_QueueCount);
				}
			}
		else
			Printf("\nNo Control!\n\n");
		CloseLibrary((struct Library *)DOSBase);
		}
}
