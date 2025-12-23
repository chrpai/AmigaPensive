/******************************************************************************/
/******************************************************************************/
/* SECTION MOVER V0.01  WRITTEN BY ARI TSIRONIS                               */
/******************************************************************************/
/******************************************************************************/



/******************************************************************************/
/* INCLUDES                                                                   */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stat.h>
#include <math.h>
#include <string.h>



/******************************************************************************/
/* DEFINES                                                                    */
/******************************************************************************/

#define DNAME  "message.data"
#define INAME "message.index"
#define TNAME  "message.text"
#define STR 256

#define DATA_START 0x1D6
#define DATA_SIZE  0x112
#define DATA_PNT   0x12
#define DATA_FLAGS 0x1A
#define DATA_SEC   0x1C

#define INDEX_START 0x3E
#define INDEX_SIZE  0x06

#define INDEX_SEC   0x04

#define TEXT_SIZE     0x16
#define MAX_TEXT_SIZE 30000

#define W 1
#define L 2



/******************************************************************************/
/* PROTOS                                                                     */
/******************************************************************************/

void main(int, char **);
void initialise(int, char **);
void open_input_files(void);
void open_output_files(void);
void close_all_files(void);
unsigned long extract(unsigned char *, unsigned int, int);
void insert(unsigned char *, unsigned long, int);


/******************************************************************************/
/* GLOBALS                                                                    */
/******************************************************************************/

FILE *dfp_in, *ifp_in, *tfp_in, *dfp_out, *ifp_out, *tfp_out;
char path1[STR], path2[STR];

unsigned char data[DATA_SIZE];
unsigned char index[INDEX_SIZE];
char text[MAX_TEXT_SIZE];

unsigned char data_header[DATA_START];
unsigned char index_header[INDEX_START];
char text_header[INDEX_START];

int err, never_done=1;
unsigned int text_size, flags;
unsigned long text_pnt, new_text_pnt;

unsigned char section;
int rc,i;
struct stat st;
int done=0;
char ch,str[STR];



/******************************************************************************/
/* MAIN                                                                       */
/******************************************************************************/

void main (int argc,char *argv[])
{
  initialise(argc, argv);
  
  printf("\n        Searching for messages files in %s\n",path1);
  
  open_input_files();
  open_output_files();

  
  printf("\n        Writing Optimised messages files in %s\n\n",path2);
  
  fread(data_header,DATA_START,1,dfp_in);
  fread(index_header,INDEX_START,1,ifp_in);
  fread(text_header,INDEX_START,1,tfp_in);
  
  fwrite(data_header,DATA_START,1,dfp_out);
  fwrite(index_header,INDEX_START,1,ifp_out);
  fwrite(text_header,INDEX_START,1,tfp_out);
  
  new_text_pnt=INDEX_START;
  

  strcpy(str,path1);
  strcat(str,DNAME);  
  rc=stat(str, &st);
  if(rc==-1)
    {
      printf("\n\nABORTING!!!  There is something wrong with message.data!!!\n\n");
      
      close_all_files();
      
      exit(20);
    }  

  printf("        Moving done =    ");
  
  for(;;)
    {
      err=fread(data,DATA_SIZE,1,dfp_in);
      if(err==0)
        break;

      section=(unsigned char)data[DATA_SEC];
      
      text_pnt=extract(data,DATA_PNT,L);
      
      text_size=(unsigned int)extract(data,TEXT_SIZE,W);

      
      if((section==11) || (section==12) || (section==10))
        {
          fseek(tfp_in,text_pnt,SEEK_SET);
          fread(text,text_size,1,tfp_in);

          
          printf("\n\n******************************************************************************\n\n");    

          for(i=0;i<text_size;i++)
            {
              ch=text[i];
              if(ch<32)
                printf("\n");

              printf("%c",ch);
            }
        
          printf("\n\n\n");

          
          printf("ENTER Y/N if you want sec %d changed to sec %d :\n",section,section+8); 
          scanf("%s",str);
          if(str[0]=='y')
            data[DATA_SEC]=section+8;
      
          printf("Old section=%d New section=%d\n",section,data[DATA_SEC]);
        }
      
      fread(index,INDEX_SIZE,1,ifp_in);

      index[INDEX_SEC]=data[DATA_SEC];
      
      fwrite(data,DATA_SIZE,1,dfp_out);
      fwrite(index,INDEX_SIZE,1,ifp_out);
    }      
    
    
  close_all_files();  
  
  
  if(never_done==0)
    {
      printf("\n\nSUCCESSFUL OPTIMISATION\n\n");
      
      exit(0);
    }
  else
    {
      printf("\n\nMESSAGE BASE WAS ALREADY FULLY OPTIMISED\n\n");
  
      exit(5);
    }
}



/******************************************************************************/
/* INITIALISE                                                                 */
/******************************************************************************/

void initialise(int argc, char *argv[])
{
  printf("\nMAXS BBS 1.52 Message Optimiser V0.03 written by Ari Tsironis!\n\n");

 
  if(argc!=3)
    {
      printf("\nUsage  Optimiser PATH1 PATH2\n");
      printf("\nPATH1 = path of unoptimised message.data, message.index and message.text\n");
      printf("PATH2 = path of optimised messages, which will have an opt extension\n");
      printf("\neg. Optimiser BBS:messages/ RAM:\n\n");
  
      exit(10);
    }
    
  strcpy(path1, argv[1]);
  strcpy(path2, argv[2]);
}



/******************************************************************************/
/* OPEN_INPUT_FILES                                                                 */
/******************************************************************************/

void open_input_files()
{
  char str[STR];
  
  
  strcpy(str,path1);
  strcat(str,DNAME);
  dfp_in=fopen(str, "rb");
  if(dfp_in==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      exit(20);
    }
    
  strcpy(str,path1);
  strcat(str,INAME);
  ifp_in=fopen(str, "rb");
  if(ifp_in==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp_in);
      
      exit(20);
    }
  
  strcpy(str,path1);
  strcat(str,TNAME);  
  tfp_in=fopen(str, "rb");
  if(tfp_in==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp_in);
      fclose(ifp_in);
      
      exit(20);
    }
}



/******************************************************************************/
/* OPEN_OUTPUT_FILES                                                                 */
/******************************************************************************/

void open_output_files()
{
  char str[STR];
  
  
  strcpy(str,path2);
  strcat(str,DNAME);
  strcat(str,".opt");
  dfp_out=fopen(str, "wb");
  if(dfp_out==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp_in);
      fclose(ifp_in);
      fclose(tfp_in);
      
      exit(20);
    }
    
  strcpy(str,path2);
  strcat(str,INAME);  
  strcat(str,".opt");
  ifp_out=fopen(str, "wb");
  if(ifp_out==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp_in);
      fclose(ifp_in);
      fclose(tfp_in);
      
      fclose(dfp_out);
      
      exit(20);
    }
    
  strcpy(str,path2);
  strcat(str,TNAME);  
  strcat(str,".opt");  
  tfp_out=fopen(str, "wb");
  if(tfp_out==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp_in);
      fclose(ifp_in);
      fclose(tfp_in);
      
      fclose(dfp_out);
      fclose(ifp_out);
      
      exit(20);
    }
}



/*****************************************************************************/
/* CLOSE ALL FILES                                                           */
/*****************************************************************************/
void close_all_files()
{
  fclose(dfp_in);
  fclose(ifp_in);
  fclose(tfp_in);  
  
  fclose(dfp_out);
  fclose(ifp_out);
  fclose(tfp_out);  
}



/*****************************************************************************/
/* EXTRACT A WORD OR LONG WORD FROM A BUFFER                                 */
/*****************************************************************************/

unsigned long extract(unsigned char *buff, unsigned int pos, int type)
  {
    unsigned long result;
    
    
    switch(type)
      {
        case W:  result=buff[pos+0]*0x00000100
                       +buff[pos+1]*0x00000001;
                 break;

        case L:  result=buff[pos+0]*0x01000000
                       +buff[pos+1]*0x00010000
                       +buff[pos+2]*0x00000100
                       +buff[pos+3]*0x00000001;
                 break;
                
        default: result=0;
                 break;
      }
    return(result);
  }



/*****************************************************************************/
/* INSERT A WORD OR LONG WORD TO A BUFFER                                     */
/*****************************************************************************/

void insert(unsigned char *buff, unsigned long val, int type)
  {
    switch(type)
      {
        case W:  buff[0]=val/0x00000100;
                 buff[1]=val/0x00000001;
                 break;

        case L:  buff[0]=val/0x01000000;
                 buff[1]=val/0x00010000;
                 buff[2]=val/0x00000100;
                 buff[3]=val/0x00000001;
                 break;
                
        default: break;
      }
  }
