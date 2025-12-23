#include "Aridoorstuff.h"


#define TYPE 13

#define FILENAME "DOORS:SysopActivity/SysopActivity.config"


void dmain(int argc, char **argv)
{
  char str[255];
  FILE *fp;
  
  
  if((fp=fopen(FILENAME,"r"))==NULL)
    end();
    
  fgets(str,80,fp);
  
  domenu(13,0,str);
  
  fclose(fp);
}
