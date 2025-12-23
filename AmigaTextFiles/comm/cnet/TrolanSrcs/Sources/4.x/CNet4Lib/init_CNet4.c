/*
** CNet/4 Library Autoinitialization Routines
** by Kelly Cochran
** Last updated: Tue Feb 24 01:32:47 1998
*/

struct Library *CNet4Base = NULL;

/* SAS C 6.50 kludge */
#if __VERSION__ > 6 || __REVISION__ >= 50
#define exit(x) return(x)
#endif

/*
 * Using __stdargs prevents creation of register arguments entry point.
 * If both stack args and reg. args entry points are created, this
 * function is called _twice_, which is not wanted.
 */
LONG __stdargs _STI_200_openCNet4(void)
{
   UBYTE *libname = "cnet4.library";

   if(CNet4Base = OpenLibrary(libname, 4))
   {
      return 0;
   }

   /*
   ** Hrm.. didn't open, return error
   */

   Printf("Cannot open %s\n", libname);

   exit(RETURN_FAIL);
}

void __stdargs _STD_200_closeCNet4(void)
{
   if(CNet4Base)
   {
      CloseLibrary(CNet4Base);
      CNet4Base = NULL;
   }
}
