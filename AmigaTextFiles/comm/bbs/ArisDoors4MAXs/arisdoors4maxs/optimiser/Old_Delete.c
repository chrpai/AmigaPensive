/******************************************************************************/
/******************************************************************************/
/* OLD MESSAGE DELETE V0.01  WRITTEN BY ARI TSIRONIS                          */
/******************************************************************************/
/******************************************************************************/



/******************************************************************************/
/* INCLUDES                                                                   */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos/datetime.h>


/******************************************************************************/
/* DEFINES                                                                    */
/******************************************************************************/

#define DNAME  "message.data"
#define INAME "message.index"
#define STR 256

#define DATA_SECTIONS 0x46
#define DATA_START    0x1D6
#define DATA_SIZE     0x112
#define DATA_DATE     0x04
#define DATA_FLAGS    0x1A

#define INDEX_START 0x3E
#define INDEX_SIZE  0x06
#define INDEX_DEL   0x05

#define SECTIONS 100

#define W 1
#define L 2



/******************************************************************************/
/* PROTOS                                                                     */
/******************************************************************************/

void main(int, char **);
void initialise(int, char **);
void open_input_files(void);

struct DateStamp *DateStamp(struct DateStamp *);



/******************************************************************************/
/* GLOBALS                                                                    */
/******************************************************************************/

FILE *dfp, *ifp;
char path[STR];

int days_old;

unsigned char del=1, sec;
long i, cnt, deleted;
unsigned short flags;
unsigned long msg_num, total_msgs, msg_date, curr_date;

struct DateStamp ds;

unsigned long sections[SECTIONS];




/******************************************************************************/
/* MAIN                                                                       */
/******************************************************************************/

void main (int argc,char *argv[])
{
  initialise(argc, argv);
  
  open_input_files();

  printf("        Searching for messages files in %s\n",path);
  
  DateStamp(&ds);
  curr_date=ds.ds_Days;
    
  printf("        Everything >=%d days old is being deleted!!!\n",days_old);
  
  cnt=0;
  deleted=0;
  
  
  fseek(dfp,DATA_SECTIONS,SEEK_SET);
  for(i=0;i<SECTIONS;i++)
    fread(&sections[i],4,1,dfp);

  
  for(;;)
    {
      if(fseek(dfp,cnt*DATA_SIZE+DATA_START+DATA_DATE,SEEK_SET))
        break;
        
      fread(&msg_date,4,1,dfp);
      
      if(msg_date<(curr_date-(unsigned long)days_old))
        {
          if(fseek(dfp,cnt*DATA_SIZE+DATA_START,SEEK_SET))
            break;
            
          fread(&msg_num,4,1,dfp);
                   
          if(fseek(dfp,cnt*DATA_SIZE+DATA_START+DATA_FLAGS,SEEK_SET))
            break;
            
          fread(&flags,2,1,dfp);
          flags|=0x01;
        
          if(fseek(dfp,-2,SEEK_CUR))
            break;
            
          fwrite(&flags,2,1,dfp);
          
          if(fseek(dfp,cnt*DATA_SIZE+DATA_START+DATA_FLAGS+2,SEEK_SET))
            break;
          
          fread(&sec,1,1,dfp);
          
          sections[sec]--;
          if(sections[sec]>0xfffffffe)
            sections[sec]=0;

/*          printf("MSG_NUM=%lx FLAGS=%x SEC=%x CURR_DATE=%ld  MSG_DATE = %ld\n",msg_num,flags,sec,curr_date,msg_date);
*/          
          if(fseek(ifp,cnt*INDEX_SIZE+INDEX_START+INDEX_DEL,SEEK_SET))
            break;    
            
          fwrite(&del,1,1,ifp);  
            
          deleted++;
        }
    
      cnt++;
    }

  fseek(dfp,DATA_SECTIONS-8,SEEK_SET);
  fread(&total_msgs,4,1,dfp);
  total_msgs-=deleted;
  fseek(dfp,-4,SEEK_CUR);
  fwrite(&total_msgs,4,1,dfp);

  fseek(dfp,DATA_SECTIONS,SEEK_SET);
  for(i=0;i<SECTIONS;i++)
    fwrite(&sections[i],4,1,dfp);

    
  printf("        [43;32mTOTAL MSGS DELETED=%ld[0m\n\n",deleted);  
    
  fclose(dfp);
  fclose(ifp);
}



/******************************************************************************/
/* INITIALISE                                                                 */
/******************************************************************************/

void initialise(int argc, char *argv[])
{
  printf("\nMAXS BBS 1.52 Message Reader V0.01 written by Ari Tsironis!\n\n");

 
  if(argc!=3)
    {
      printf("\nUsage  OLD_Delete PATH DAYS\n");
      printf("\nPATH1 = path of message.data, message.index and message.text\n");
      printf("\neg. Old_Delete BBS:messages/ 30\n\n\n");
  
      exit(10);
    }
    
  strcpy(path, argv[1]);
  sscanf(argv[2],"%d",&days_old);
}



/******************************************************************************/
/* OPEN_INPUT_FILES                                                                 */
/******************************************************************************/

void open_input_files()
{
  char str[STR];
  
  
  strcpy(str,path);
  strcat(str,DNAME);
  dfp=fopen(str, "rb+");
  if(dfp==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      exit(20);
    }
    
  strcpy(str,path);
  strcat(str,INAME);
  ifp=fopen(str, "rb+");
  if(ifp==NULL)
    {
      printf("ABORTING!!!  Couldn't open %s!\n\n", str);
      
      fclose(dfp);
      exit(20);
    }
}

