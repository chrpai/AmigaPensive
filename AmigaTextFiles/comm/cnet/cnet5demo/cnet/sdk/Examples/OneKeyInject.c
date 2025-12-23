// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main( int argc, char *argv[] )
{
	struct MainPort *myp;
	LONG xxx=0;
	int portnum = 0;

	if(argc != 3)
		{
		printf("\nUsage: %s <port number> <character>\n\n", argv[0]);
		exit(0);
		}

	portnum = atoi(argv[1]);

	myp=(struct MainPort *)FindPort(CNETPORT);
	if(myp)
		{
		if(myp->PortZ[portnum] == myp->z0)
			{
			printf("\n%s: port not loaded.\n\n", argv[0]);
			exit(0);
			}

		for(xxx=0;xxx<10000;xxx++)
			myp->PortZ[portnum]->keyin= argv[2][0];
		}
}

