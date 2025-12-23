#include <stdio.h>
#include <string.h>

#ifdef __COMMODORE_DATE__
#define VERSION_ID  "IDFind 0.9 (" __COMMODORE_DATE__ ")"
#else
#define VERSION_ID  "IDFind 0.9 (" __DATE__ ")"
#endif
#define VERSION_TAG "$VER: " VERSION_ID "\0"

int FindName(bbs,dummy,dummy3,foundn,notfn)
char bbs[80],dummy[12],dummy3[80],foundn[80],notfn[80];
{
  int End = 0;
  int f,found;
  char dummy2[80],ch,chr;
  char BBSZeile[80], BBSName[12];
  FILE *BBSHendl;
  FILE *OUTHendl;
  FILE *NOTHendl;
  
  
  found = 0;

  BBSHendl = fopen(bbs,"r");
  if (BBSHendl == NULL)
  {
    printf("Error while opening BBSDirectory!\n");
    return(1);
  }
  do
  {
    strcpy(BBSZeile,"");
    fgets(BBSZeile,80,BBSHendl);
    if (*BBSZeile == NULL)
      End = 1;
    else
    { 
      strcpy(dummy2,"");
      strcpy(dummy2,BBSZeile);
      strcpy(BBSName,"");
      strncat(BBSName, BBSZeile, 12);
/*
      printf("comparing %s with %s\n",dummy, BBSName);
      ch = getch();
      if (ch == '.')
      {
        printf("abort!\n");
        fclose(BBSHendl);
        return(1);
      }*/
      f = strcmpi(BBSName,dummy);
      if (f == 0)
      {
        found = 1;
        OUTHendl = fopen(foundn,"aa");
        if (OUTHendl == NULL)
        {
          printf("Error while writing to Description File!\n");
          End = 1;
          fclose(BBSHendl);
          return(1);
        }
 
        fprintf(OUTHendl,"%s",dummy2);
        do
        {      
          chr = ' ';
          fgets(BBSZeile,80,BBSHendl);
          if (*BBSZeile == NULL)
          {
            chr = 'x';
            End = 1;
          }
          else
          {
            strcpy(dummy2,"");
            strcpy(dummy2,BBSZeile);
          
            chr = BBSZeile[0];

            if (chr == ' ')
              fprintf(OUTHendl,"%s",dummy2);
          }
        }    
        while (chr == ' ');
        fclose(OUTHendl);      
        End = 1;
      } 
    }
  }
  while (End == 0);       
  fclose (BBSHendl);

  if (found == 0)
  {
    NOTHendl = fopen(notfn,"aa");
    if (NOTHendl == NULL)
    {
        printf("Error writing to List of not found Files\n");
        return(1);
    }
    fprintf(NOTHendl,"%s",dummy3);
    fclose(NOTHendl);
  }
  return(0);    
      
}


void main (int argc, char *argv[])
{
  static char *version_tag = VERSION_TAG;
  char FileIDZeile[80], FileIDName[12], zeilebuffer[80];
  FILE *IDHendl;
  int End = 0;
       
  if (argc !=5)
  {
    printf( VERSION_ID " by Michael Gehmair + AYT\n");
    printf("Usage: IDFind FileIDList BBSDirectory FoundList NotFoundList\n\n");
    exit (1);
  }

  IDHendl = fopen(argv[1],"r");
  if (IDHendl == NULL)
  {
    printf("Error while opening FileIDList!\n");
    exit (1);
  }
  else
  {
    do
    {
      fgets(FileIDZeile,80,IDHendl);
      strcpy(zeilebuffer,"");
      strcpy(zeilebuffer,FileIDZeile);
      
      if (*FileIDZeile == NULL)
        End = 1;
      else
      {
        strcpy(FileIDName,"");
        strncat(FileIDName, FileIDZeile, 12);
/*        
        printf ("Examining >%s<\n",FileIDName);
*/    
        /* examine here.. */
        if (FindName(argv[2],FileIDName,zeilebuffer,argv[3],argv[4]) != 0)
          End = 1;

      }
    }
    while (End == 0);      
  }             
    
  fclose (IDHendl);
  printf("...all done!\n");
           
}
