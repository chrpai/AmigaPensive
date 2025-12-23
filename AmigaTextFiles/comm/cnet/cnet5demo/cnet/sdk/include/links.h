
#ifndef CNET_LINKS
#define CNET_LINKS

struct TermLink
	{
	char	dial[32];
	long	baud;
	char	ports[40];
	long	access;
	short	rate;
	char	name[38];
	short	databits;
	};

struct LinkPortUser
	{
	short	tick;

	UBYTE	channel;
	char	name[21];
	};

struct LinkPort
	{
	long	s;				// serial number */

	short	ports;
	char	name[22];

	struct	LinkPortUser user[100];
	};


#endif
