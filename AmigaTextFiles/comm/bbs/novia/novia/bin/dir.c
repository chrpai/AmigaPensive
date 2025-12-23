#include <pragma/noviasys_lib.h>
#include <pragma/exec_lib.h>
#include <pragma/dos_lib.h>
#include <stdio.h>
#include <novia/novia_portdata.h>

struct Library *NoviaSysBase;

void main()
{
	if ((NoviaSysBase = OpenLibrary("noviasys.library", 0)))
	{
		ndos_execute("list");
		CloseLibrary(NoviaSysBase);
	}
}