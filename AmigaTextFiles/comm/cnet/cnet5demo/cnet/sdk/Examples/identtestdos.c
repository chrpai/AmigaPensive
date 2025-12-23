// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main( void )
{
	struct MainPort *myp;

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		if(myp->MPE->idd)
			{
			struct IdentdData *iddptr=myp->MPE->idd;
			// walk the list of identd data and print specifics
			while(iddptr)
				{
				Printf("Socket number: %ld, CNet Owner Port: %ld\n", iddptr->socket, iddptr->port);
				iddptr=iddptr->next;
				}
			}
		else
			{
			Printf("No identd entries\n");
			}
		}
}
