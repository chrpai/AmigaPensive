

void main(int argc, char *argv[])
{
	struct MainPort *myp=NULL;

	if(myp=(struct MainPort *)FindPort("cnetport"))
		{
		printf("regto = %s\n", myp->regto);
		printf("regco = %s\n", myp->regco);
		}
}
