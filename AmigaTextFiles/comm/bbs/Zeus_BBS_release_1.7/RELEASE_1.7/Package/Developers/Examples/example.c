
#include <proto/pipeutil.h>

struct	Library		*PipeUtilBase;

void
main ( int argc, char **argv )
{
	char	Buf[] = "make this string uppercase";

	if ( PipeUtilBase = OpenLibrary ( "zmf.library", 0 ) )
	{
		PutStr ( Buf ) ;

		MakeUpper ( Buf, strlen ( Buf ) ) ;

		PutStr ( Buf ) ;

		CloseLibrary ( PipeUtilBase ) ;
	}
}
