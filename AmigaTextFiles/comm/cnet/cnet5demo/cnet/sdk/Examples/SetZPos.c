// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main( int argc, char *argv[] )
{
	struct MainPort *myp=NULL;

	if(argc < 3)
		{
		printf("\nsyntax: %s <port> <z.pos setting>\n\n", argv[0]);
		exit(1);
		}

	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		myp->PortZ[atoi(argv[1])]->pos = atoi(argv[2]);
		}
	exit(0);
}
