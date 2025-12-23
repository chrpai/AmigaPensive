#include <stdio.h>
#include <stdlib.h>
#include <dos/dos.h>
#include <exec/memory.h>
#include <proto/dos.h>
#include <proto/exec.h>



int main (int argc, char *argv[])
{
	long howmany = 0;

	if (argc == 2) {
		struct AnchorPath *ap;

		if (ap = AllocVec (sizeof (struct AnchorPath), MEMF_PUBLIC | MEMF_CLEAR)) {
			long err;

			for (err = MatchFirst (argv[1], ap); err == 0; err = MatchNext (ap)) {
				++howmany;
			}

			if (err == ERROR_OBJECT_IN_USE)
				++howmany;

			MatchEnd (ap);

			FreeVec (ap);
		}
	}

	exit (howmany);
}
