#include "cnet4lib.h"

void checkcarrier(void)
{
   if( !z->Carrier || !z->TimeLeft || z->Dumped )
      GetOut();
}  /* checkcarrier() */
