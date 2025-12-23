void ClonePortPri( void )
{
	struct Process *myproc;

	/* adjust own priority to that of CNet port's */
	if(myproc = (struct Process *)FindTask(NULL))
		{
		SetTaskPri((struct Task *)myproc, z->ThisTask->tc_Node.ln_Pri);
		}
}
