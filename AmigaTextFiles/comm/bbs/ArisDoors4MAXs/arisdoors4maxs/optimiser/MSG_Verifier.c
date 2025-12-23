/******************************************************************************/
/******************************************************************************/
/* MESSAGES VERIFIER V0.01  WRITTEN BY ARI TSIRONIS                           */
/******************************************************************************/
/******************************************************************************/



/******************************************************************************/
/* INCLUDES                                                                   */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exec/types.h>



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
#define DATA_SEC   0x1c


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
unsigned long extract(unsigned char *, unsigned int, int);
void insert(unsigned char *, unsigned long, int);


/******************************************************************************/
/* GLOBALS                                                                    */
/******************************************************************************/

FILE *dfp_in, *ifp_in, *tfp_in;
char str[STR], path[STR], ch;

unsigned char data[DATA_SIZE];
unsigned char index[INDEX_SIZE];
char text[MAX_TEXT_SIZE];

unsigned char data_header[DATA_START];
unsigned char index_header[INDEX_START];
char text_header[INDEX_START];

int err, i, sections[50];
unsigned char sec1,sec2;
unsigned int text_size, flags;
unsigned long text_pnt,count=0;



/******************************************************************************/
/* MAIN                                                                       */
/******************************************************************************/

void main (int argc,char *argv[])
{
  initialise(argc, argv);
  
  open_input_files();

  printf("\n        Searching for messages files in %s\n",path);
  
  fread(data_header,DATA_START,1,dfp_in);
  fread(index_header,INDEX_START,1,ifp_in);
  fread(text_header,INDEX_START,1,tfp_in);
  
  
  for(i=0;i<50;i++)
    sections[i]=0;
  
  for(;;)
    {
      err=fread(data,DATA_SIZE,1,dfp_in);
      if(err==0)
        break;
      
      sec1=(unsigned char)data[DATA_SEC];
      
      text_pnt=extract(data,DATA_PNT,L);
      
      text_size=(UWORD)extract(data,TEXT_SIZE,W);
      if(text_size>MAX_TEXT_SIZE)
        {
          printf("\n\nABORTED!!!  Message to big!!!\n\n");

          fclose(dfp_in);
          fclose(ifp_in);
          fclose(tfp_in);  
  
          exit(20);
        }
      
      flags=(unsigned char)extract(data,DATA_FLAGS,W);
      
      fread(index,INDEX_SIZE,1,ifp_in);
      
      sec2=(unsigned char)index[INDEX_SEC];
      
      if((flags&0x01)==0)
        sections[sec1]++;
      
      if(text_size != (UWORD)extract(data,TEXT_SIZE+2,W))
        {
          count++;
        }
        
      if(1==2)
        {
          fseek(tfp_in,text_pnt,SEEK_SET);
          fread(text,text_size,1,tfp_in);

      
          printf("\n\n******************************************************************************\n\n");    
          printf("Num=%x, pnt=%lx, size1=%d size2=%d Delete=%d Data Sec=%d Index Sec=%d\n\n",extract(data,0,L),text_pnt,text_size,(UWORD)extract(data,TEXT_SIZE+2,W),flags&0x01,sec1,sec2);

          for(i=0;i<text_size;i++)
            {
              ch=text[i];
              if(ch<32)
                printf("\n");

              printf("%c",ch);
            }
        
          printf("\n\n\n");
        }
    }      
    
  for(i=0; i<50; i++)
    printf("Section%d = %x\n",i,sections[i]);

  printf("Number of messages with different sizes = %d\n",count);
    
  fclose(dfp_in);
  fclose(ifp_in);
  fclose(tfp_in);  
}



/******************************************************************************/
/* INITIALISE                                                                 */
/******************************************************************************/

void initialise(int argc, char *argv[])
{
  printf("\nMAXS BBS 1.52 Message Reader V0.01 written by Ari Tsironis!\n\n");

 
  if(argc!=2)
    {
      printf("\nUsage  MSG_Reader PATH MSG_NO.\n");
      printf("\nPATH1 = path of message.data, message.index and message.text\n");
      printf("\neg. MSG_Reader BBS:messages/ 800\n\n\n");
  
      exit(10);
    }
    
  strcpy(path, argv[1]);
}



/******************************************************************************/
/* OPEN_INPUT_FILES                                                                 */
/******************************************************************************/

void open_input_files()
{
  char str[STR];
  
  
  strcpy(str,path);
  strcat(str,DNAME);
  dfp_in=fopen(str, "rb");
  if(dfp_in==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      exit(20);
    }
    
  strcpy(str,path);
  strcat(str,INAME);
  ifp_in=fopen(str, "rb");
  if(ifp_in==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp_in);
      exit(20);
    }
  
  strcpy(str,path);
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
