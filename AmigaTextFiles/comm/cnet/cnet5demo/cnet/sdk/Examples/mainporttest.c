

int main(int argc, char *argv[])
{
	short account;
	struct Task *taskaddr = FindTask(NULL);

	struct MainPort *myp=(struct MainPort *)FindPort(CNETPORT);
	if(myp)
		{
		Printf("\nMainPort address: 0x%lx\n", myp);
		Printf("MPE address     : 0x%lx\n", myp->MPE);
		Printf("MailSem Var     : 0x%lx\n", &myp->MPE->MailSem);
		Printf("MailSem base    : 0x%lx\n", myp->MPE->MailSem);
		Printf("MailSem 0       : 0x%lx\n", &myp->MPE->MailSem[0]);

		if(argc > 1)
			{
			account = atoi(argv[1]);
			Printf("	Examining mail semaphore for account %ld..\n", account);
			Printf("		Address     : 0x%lx\n", &myp->MPE->MailSem[account-1]);
			Printf("		Owner task  : 0x%lx (%s)\n", myp->MPE->MailSem[account-1].ss_Owner, (taskaddr == myp->MPE->MailSem[account-1].ss_Owner) ? argv[0]:"unknown");

			if(AttemptSemaphore(&myp->MPE->MailSem[account-1]))
				{
				Printf("	(Semaphore attempable)\n");
				Printf("		Owner task (temp)  : 0x%lx (%s)\n", myp->MPE->MailSem[account-1].ss_Owner, (taskaddr == myp->MPE->MailSem[account-1].ss_Owner) ? argv[0]:"unknown");
				ReleaseSemaphore(&myp->MPE->MailSem[account-1]);

				Printf("	Obtaining Semaphore..\n");
				ObtainSemaphore(&myp->MPE->MailSem[account-1]);
				Printf("	(Semaphore obtainable)\n");
				Printf("		Owner task (temp)  : 0x%lx (%s)\n", myp->MPE->MailSem[account-1].ss_Owner, (taskaddr == myp->MPE->MailSem[account-1].ss_Owner) ? argv[0]:"unknown");
				ReleaseSemaphore(&myp->MPE->MailSem[account-1]);
				Printf("	Owner task (final)  : 0x%lx (%s)\n", myp->MPE->MailSem[account-1].ss_Owner, (taskaddr == myp->MPE->MailSem[account-1].ss_Owner) ? argv[0]:"unknown");
				}
			}
		}
}
