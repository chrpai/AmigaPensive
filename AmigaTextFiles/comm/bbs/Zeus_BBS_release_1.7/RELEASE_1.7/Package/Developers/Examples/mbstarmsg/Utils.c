
#include "library.h"

#define	TIME_OFFSET			252482400

void StripFido( UBYTE *txt )
{
	UBYTE		*s, *e, *b;

	s = txt;

	if( !strncmp( s, "AREA:", 5 ) || !strncmp( s, "\1AREA:", 6 ) )
	{
		while( *s && *s != 0x8d && *s != 0x0a && *s != 0x0d ) s++;
		
		if( *s ) s++;
	}

	while( *s == 0x01 )
	{
		while( *s && *s != 0x8d && *s != 0x0a && *s != 0x0d ) s++;
		
		if( *s ) s++;
	}

	e = s;

	while( *e ) e++;

	b = e - 1;

	while( b > s && ( *b == 0x0a || *b == 0x0d ) ) b--;

	while( 1 )
	{
		while( b > s && *b != 0x0a && *b != 0x0d ) b--;
			
		if( b <= s ) break;

		b++;

		if( *b == 0x01 || !strncmp( b, "SEEN-BY: ", 9 ) || !strncmp( b, " * Origin: ", 11 ) ||
								!strncmp( b, "---", 3 )		   || !strncmp( b, "PATH: ", 6 ) )
		{
			e = b;

			if( *b == '-' ) break;

			b -= 2;
		}
		else
		{
			break;
		}
	}
	
	*e = 0x00;

	strcpy( txt, s );
}

static UBYTE		*mon_names[12] = 
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

static UBYTE		*day_names[7] =
{
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

static void GetNum( UBYTE **s, UWORD *d )
{
	UBYTE		*p = *s;

	while( *p && !isdigit(*p) ) p++;

	while( isdigit(*p) )
	{
		*d *= 10;
		*d += *p++ - '0';
	}

	*s = p;
}

static void GetMon( UBYTE **s, UWORD *d )
{
	UBYTE		*p = *s;

	while( *p && *p == ' ' ) p++;

	for( *d = 0 ; *d < 12 && strnicmp( p, mon_names[*d], 3 ) ; *d++ )

	while( *p && *p != ' ' ) p++;

	*s = p;
}

static void GetDay( UBYTE **s, UWORD *d )
{
	UBYTE		*p = *s;

	while( *p && *p == ' ' ) p++;

	for( *d = 0 ; *d < 7 && strnicmp( p, day_names[*d], 3 ) ; *d++ )

	while( *p && *p != ' ' ) p++;

	*s = p;
}

time_t ParseTime( UBYTE *s )
{
	struct	ClockData	cd;
	time_t					t = NULL;

	while( *s && *s == ' ' ) s++;

	if( *s )
	{
		memset( &cd, NULL, sizeof(cd) );

		if( isdigit( *s ) )
		{
			/* Standard format */
			
			GetNum( &s, &cd.mday );
			GetMon( &s, &cd.month );
			GetNum( &s, &cd.year );
			GetNum( &s, &cd.hour );
			GetNum( &s, &cd.min );
			GetNum( &s, &cd.sec );
		}
		else
		{
			/* SEAdog format */
			
			GetDay( &s, &cd.wday );
			GetNum( &s, &cd.mday );
			GetMon( &s, &cd.month );
			GetNum( &s, &cd.year );
			GetNum( &s, &cd.hour );
			GetNum( &s, &cd.min );			
		}
	}

	cd.year += 1900;

	if( cd.year < 1978 ) cd.year += 100;

	cd.month++;
	
	t = CheckDate( &cd );

	return( t + TIME_OFFSET );
}

UWORD Fido2Zeus( UWORD flags )
{
	UWORD		ret = NULL;

	if( flags & MF_PRIVATE    ) ret |= MI_PRIVATE;
	if( flags & MF_FILEATTACH ) ret |= MI_FILEATTACHED;
	if( flags & MF_RECEIVED   ) ret |= MI_RECEIVED;
	if( flags & MF_CRASH      ) ret |= MI_CRASH;

	return( ret );
}

UWORD Zeus2Fido( UWORD flags )
{
	UWORD		ret = NULL;

	if( flags & MI_PRIVATE      ) ret |= MF_PRIVATE;
	if( flags & MI_FILEATTACHED ) ret |= MF_FILEATTACH;
	if( flags & MI_RECEIVED     ) ret |= MF_RECEIVED;
	if( flags & MI_CRASH        ) ret |= MF_CRASH;

	return( ret );
}
