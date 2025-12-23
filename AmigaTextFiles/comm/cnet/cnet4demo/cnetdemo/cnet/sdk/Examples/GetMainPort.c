
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
