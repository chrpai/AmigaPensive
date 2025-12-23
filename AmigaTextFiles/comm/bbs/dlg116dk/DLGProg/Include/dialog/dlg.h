/***************************************************************************

 © copyright 1995-96 by DLG Development
   All rights reserved

*****************************************************************************/


#include <dialog/dlgvers.h>

#ifdef ANSICOL
#undef printf
#define printf  DummyPrintf
#define sprintf DummySPrintf
#endif

#define DLGNAME "dlg.library" /* Library name      */
#define DLGVERSION 2          /* Library version   */

