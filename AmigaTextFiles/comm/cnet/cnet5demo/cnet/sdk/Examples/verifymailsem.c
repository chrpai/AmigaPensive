#include <exec/execbase.h>
#include <proto/Exec.h>

#define __USE_SYSBASE 1
extern struct ExecBase *SysBase;


char *GetTaskName(APTR taskaddr)
{
	struct Node *tasknode, *nextnode;
	struct Task *taskp;
	static char tasknm[40];

	Forbid();
	tasknode = SysBase->TaskReady.lh_Head;
	while( nextnode = tasknode->ln_Succ )
		{
		// print libname etc, here.
		taskp=(struct Task *)tasknode;

		if(taskp == taskaddr)
			{
			strcpy(tasknm, taskp->tc_Node.ln_Name);
			}

		tasknode = nextnode;
		}

	tasknode = SysBase->TaskWait.lh_Head;
	while( nextnode = tasknode->ln_Succ )
		{
		// print libname etc, here.
		taskp=(struct Task *)tasknode;

		if(taskp == taskaddr)
			{
			strcpy(tasknm, taskp->tc_Node.ln_Name);
			}

		tasknode = nextnode;
		}
	Permit();

	return tasknm;
}

int main( int argc, char *argv[])
{
	struct MainPort *myp=NULL;
	short xxx = atoi(argv[1])-1;
	char ownertaskname[40];

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		Printf("\nMailSem[%ld] (%lx)  belongs to %s\n", xxx, &myp->MPE->MailSem[xxx], myp->Key[xxx].Handle);
		Printf("ss_Link        : %lx\n",	myp->MPE->MailSem[xxx].ss_Link);
		Printf("ss_NestCount   : %ld\n", myp->MPE->MailSem[xxx].ss_NestCount);
		Printf("ss_WaitQueue   : 0x%lx\n",	myp->MPE->MailSem[xxx].ss_WaitQueue);
		Printf("ss_MultipleLink: %ld\n",	myp->MPE->MailSem[xxx].ss_MultipleLink);
		Printf("ss_Owner       : 0x%lx\n",	myp->MPE->MailSem[xxx].ss_Owner);
		Printf("ss_QueueCount  : 0x%lx\n",	myp->MPE->MailSem[xxx].ss_QueueCount);

		Printf("Semaphore is ");
		if(AttemptSemaphore(&myp->MPE->MailSem[xxx]))
			{
			Printf("obtainable\n");
			ReleaseSemaphore(&myp->MPE->MailSem[xxx]);
			}
		else
			{
			ownertaskname[0] = 0;
			strcpy(ownertaskname, GetTaskName(myp->MPE->MailSem[xxx].ss_Owner));
			Printf("already locked by task \"%s\" (%lx)\n", ownertaskname[0] ? ownertaskname : "unknown", myp->MPE->MailSem[xxx].ss_Owner);
			}
		}
	else
		Printf("\nNo Control!\n\n");
}
