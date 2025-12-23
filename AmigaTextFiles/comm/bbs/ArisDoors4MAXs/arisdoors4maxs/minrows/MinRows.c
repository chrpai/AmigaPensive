#include "Aridoorstuff.h"


/* 4 : D/U Ratio      */
/* 6 : Page Length    */
/* 9 : No. of Uploads */


void dmain(int argc, char **argv)
{
  int rows;
  
  
  rows=getsnum(9);
  
  if(rows<27)
    changeuserint(6,27);
  
  end();
}
  
