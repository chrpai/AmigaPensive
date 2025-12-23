// CNet Amiga/4 Programming examples
// copyright © 1998 Ray A. Akey
// Exclusive license granted to ZenMetal Software and CNet door coders.

char tnhostfile[] = "sysdata:bbs.tnhost";

void main( void )
{
	BPTR fh;
	struct TNHostEntry he;
	int i=0;

	if(fh=Open(tnhostfile, MODE_OLDFILE))
		{
		char hoststring[80];

		while( Read(fh, &he, sizeof(struct TNHostEntry)) )
			{
			i++;
			sprintf(hoststring, "%s:%d", he.he_ip, he.he_port);
			printf("\n%2d. %-24.24s %-20.20s %s", i, he.he_system, hoststring, he.he_data);
			}
		Close(fh);
		}
	printf("\n");
}
