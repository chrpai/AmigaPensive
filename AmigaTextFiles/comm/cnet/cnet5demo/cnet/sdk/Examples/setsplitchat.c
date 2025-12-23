// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main(int argc, char *argv[])
{
	struct MainPort *myp;
	struct PortData *z;

	if(argc != 2)
		exit(0);
	
	if(myp = (struct MainPort *)FindPort(CNETPORT))
		{
		z=myp->PortZ[atoi(argv[1])];
		z->user1.SplitChatFlags ^= 1;
		}
}
