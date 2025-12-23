/*****************************************************************************/
/* MAKE AN INDEX FILE FOR THE CDROM LISTER                                   */
/*****************************************************************************/



/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <exec/types.h>



/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define INDEX_FILE_IN  "NET:cd0/locators/fishcat/index"
#define INDEX_FILE_OUT "DOORS:CDROM_Lister/cdrom.index"

#define NO_OF_DISKS 795



/*****************************************************************************/
/* PROTOTYPES                                                                */
/*****************************************************************************/

void main(void);



/*****************************************************************************/
/* MAIN                                                                      */
/*****************************************************************************/

void main(void)
{
  FILE *fp_in,*fp_out;
  int i,j;
  UBYTE files,temp[8];
  ULONG disk;
  char filename[80];
  



  fp_in=fopen(INDEX_FILE_IN,"rb");
  if(fp_in==NULL)
  {
    printf("Couldn't open/find input file, ABORTING\n");
    exit(20);
  }

  

  fp_out=fopen(INDEX_FILE_OUT,"wb+");
  if(fp_out==NULL)
  {
    printf("Couldn't open output file, ABORTING\n");
    exit(20);
  }
  
  

  for(j=0;j<NO_OF_DISKS;j++)
  {
    fread(&disk,sizeof(disk),1,fp_in);
    fread(&files,sizeof(files),1,fp_in);

    fwrite(&disk,sizeof(disk),1,fp_out);
    fwrite(&files,sizeof(files),1,fp_out);

    for(i=0;i<files;i++)
    {
      fgets(filename,80,fp_in);
      fread(temp,sizeof(temp),1,fp_in);
 
      fprintf(fp_out,"%s",filename);
    }
  }


  fclose(fp_in);
  fclose(fp_out);
}
