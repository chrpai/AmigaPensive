// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

void main(int argc, char *argv[])
{
	struct MainPort *myp;

	if(myp=(struct MainPort *)FindPort("cnetport"))
		{
		printf("MainPort found!\n");
		}
	else
		{
		printf("Could not find CNet MainPort.\n");
		}
	exit(0);
}
