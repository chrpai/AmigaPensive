// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

struct ItemType3 it;
struct ItemHeader ih;

void main(int argc, char *argv[])
{
	struct MainPort *myp=NULL;
	struct Library *CNetBase=NULL;

	if(!(CNetBase = OpenLibrary("cnet.library", 4)))
		{
		printf("No library\n");
		exit(0);
		}


	if(myp=(struct MainPort *)FindPort(CNETPORT))
		{
		printf("Found port!\n");
		if(OneMoreUser(&myp->Subboard[1], FALSE))
			{
			printf("Entered subboard\n");
			ZGetItem(&it, &ih, &myp->Subboard[1], 0);
			if(it.loaded)
				{
				printf("Item Title: %s\n", it.Title);
				}
			else
				{
				printf("Item not loaded!\n");
				}
			OneLessUser(&myp->Subboard[1]);
			}
		}
	exit(0);
}
