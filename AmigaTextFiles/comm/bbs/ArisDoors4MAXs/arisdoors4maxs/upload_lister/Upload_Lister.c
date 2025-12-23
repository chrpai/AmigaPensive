/*****************************************************************************/
/* UPLOAD LISTER V 1.00 WRITTEN BY ARI TSIRONIS                              */
/*****************************************************************************/



/*****************************************************************************/
/* INCLUDES                                                                  */
/*****************************************************************************/

#include <stat.h>
#include "Aridoorstuff.h"



/*****************************************************************************/
/* DEFINES                                                                   */
/*****************************************************************************/

#define FILE_OPEN   1
#define MEMORY_OPEN 2

#define NO_ERROR     0                                                 
#define FILE_ERROR  -1

#define MAXS_CONFIG   1
#define UPLOAD_CONFIG 2
#define FILE_DATA     3
#define NO_MEMORY     4

#define CRLF        1
#define CTRL     0x0C
#define CR       0x0D
#define LF       0x0B
#define ESC      0x1B

#define USERNAME    1
#define BBSPATH     8
#define ACCESSLEVEL 1
#define SCREENROWS  9

#define FIRST_USER       0x249
#define OFFSET_USER      0xCA
#define OFFSET_FILENAME -0x65
#define OFFSET_LENGTH   -0x6F
#define OFFSET_DATE     -0x77
#define OFFSET_DLOADS   -0x6B
#define OFFSET_SECTION  -0x66
#define OFFSET_DESCRIPT -0x3C

#define FILES_PATH_LENGTH 0x100
#define FILES_PATH_OFFSET 0xB9

#define CONFIG_FILE "DOORS:Upload_Lister/Upload_Lister.config"
#define CONFIG_LINE_LENGTH 80

#define STR 256
#define PRINT 1

#define W 0x01
#define L 0x02

#define byte unsigned char
#define word unsigned int



/*****************************************************************************/
/* PROTOTYPES                                                                */
/*****************************************************************************/

void dmain(void);
int get_params(char *, char *, char *, int *); 
int allocate(int *, char *, byte **, word *, long *);
int search_buffer(char *, byte *, word, char *, int);
void deallocate(int, byte *, long);
long extractwl(byte *, word, int);
void print_error(char *);
void tidy_up(int, byte *, long, int);


/*****************************************************************************/
/* GLOBALS                                                                   */
/*****************************************************************************/

int file;
byte *file_buffer;
word file_length;

char file_name[STR];
char user_name[STR];
char section_str[STR];
char str[STR];
char error_str[5][STR]={"NO_ERROR","MAXSBBS.CONFIG","UPLOAD_LISTER.CONFIG",
                        "FILE.DATA","MEMORY ALLOCATION"};

int print, error=NO_ERROR, sysop;

long allocated=0;


/*****************************************************************************/
/* MAIN                                                                      */
/*****************************************************************************/

void dmain(void)
	{
    /* CLEAR SCREEN */
    
    putdoorchar(CTRL);
    
    
    /* READ IN FILE PATH, FILE NAME, USER NAME, SECTION MASK AND SYSOP ACCESS*/
 
    error=get_params(file_name, user_name, section_str, &sysop);
    if(error!=NO_ERROR)
      {
        print_error(error_str[error]);
        tidy_up(file, file_buffer, allocated, PRINT);
      }


    /* ALLOCATE FILE, BUFFER AND SEARCH BUFFER */

    error=allocate(&file, file_name, &file_buffer, &file_length, &allocated); 
    if(error==NO_ERROR)
      print=search_buffer(user_name, file_buffer, file_length, section_str, sysop);
    else
      {
        print_error(error_str[error]);
        print=PRINT;
      }
      
    tidy_up(file, file_buffer, allocated, print);
	}
  


/*****************************************************************************/
/* GET FILE NAME, FILE PATH AND USER NAME                                    */
/*****************************************************************************/

int get_params(char *f_name, char *u_name, char *sec_str, int *sys)
  {
    char path[STR],str[STR],*serr;
    byte buff[STR];
    int f, error;
    FILE *fp;
    
    
    /* GET USER's NAME */

    getsvar(USERNAME, user_name);


    
    /* CHECK TO SEE IF USER'S NAME IS IN CONFIG FILE, IF IT IS THEN STORE    */
    /* HIDDEN SECTIONS IN THE SECTION STRING, ALSO GET SYSOP ACCESS LEVEL    */
    
    fp=fopen(CONFIG_FILE,"ra");
    if(fp==NULL)
      return(UPLOAD_CONFIG);
      
    
    /* GET SYSOP ACCESS LEVEL, IF IT'S NOT AVAILABLE THEN ERROR */  
    
    serr=fgets(str,CONFIG_LINE_LENGTH,fp);
    if(serr==NULL)
      return(UPLOAD_CONFIG);
    
    
    /* SYSOP ACCESS LEVEL CHANGE STRING INTO INT */
      
    *sys=(int)strtol(str,NULL,10);
    if(sys==0)
      return(UPLOAD_CONFIG);    
        
    
    /* PUT DEFAULT NONAME INTO SECTION STRING */
    
    strcpy(sec_str,"XXXNO_NAMEXXX");
    
    
    /* CHECK IF USER'S NAME IS IN CONFIG FILE, IF IT IS THEN GET USER'S */
    /* SECTION STRING                                                   */
    
    do
      {
        serr=fgets(str,CONFIG_LINE_LENGTH,fp);
        
        
        /* REPLACE RETURN CHAR WITH A 0, SO THAT WE CAN MATCH IT WITH */
        /* USER'S NAME                                                */
        
        str[strlen(str)-1]=0;
        
        if(stricmp(user_name,str)==0)
          {
            serr=fgets(sec_str,CONFIG_LINE_LENGTH,fp);
            if(serr==NULL)
              return(UPLOAD_CONFIG);
              
            break;
          }
      }
    while(serr!=NULL);
      
    fclose(fp);
    
    
    /* GET BBS's PATH */

    getsvar(BBSPATH, path);
    
    
    /* OPEN MAXsBBS.Config AND GET "FILES" FILE PATH */

    strcat(path,"MAXsBBS.Config");

    f=open(path, O_RDONLY, 0);
    if(f==FILE_ERROR)
      return(MAXS_CONFIG);
    

    /* READ IN FILES_PATH_LENGTH BYTES OF MAXsBBS.Config */

    error=read(f, buff, FILES_PATH_LENGTH);
    close(f);
    if(error!=FILES_PATH_LENGTH)
      return(MAXS_CONFIG);

      
    /* EXTRACT FILES PATH FROM BUFFER */  
      
    strcpy(path, &buff[FILES_PATH_OFFSET]);
    
    
    /* ADD "FILE.DATA" NAME TO FILES PATH */

    strcpy(f_name, path);
    strcat(f_name, "FILE.DATA");  

    return(NO_ERROR);
  }
  
  

/*****************************************************************************/
/* FIND FILE SIZE, AND ALLOCATE FILE BUFFER                                  */
/*****************************************************************************/

int allocate(int *f, char *f_name, byte **f_buffer, word *f_length, long *alloc)
  {
    int rc,error;
    struct stat st;



    /* GET FILE STATISTICS */

    rc=stat(f_name, &st);
    if(rc==FILE_ERROR)
      return(FILE_DATA);
      
    
    /* OPEN LEVEL 1 FILE, FILE.DATA */

    *f=open(f_name, O_RDONLY, 0);
    if(*f==FILE_ERROR)
      return(FILE_DATA);

    (*alloc)|=FILE_OPEN;

    
    /* FILE SIZE */

    *f_length=(word)st.st_size;

     
    /* ALLOCATE FILE BUFFER */

    *f_buffer=malloc((size_t)(*f_length));
    if(*f_buffer==NULL)
      return(NO_MEMORY);

    (*alloc)|=MEMORY_OPEN;

    
    /* READ FILE INTO BUFFER */

    error=read(*f, *f_buffer, *f_length);    
    if(error!=*f_length)
      return(FILE_DATA);

    return(NO_ERROR);
  }
  


/*****************************************************************************/
/* DO A SEARCH ON BUFFER AND PRINT RESULTS                                   */
/*****************************************************************************/

int search_buffer(char *u_name, byte *f_buffer, word f_length, char *sec_str, int sys)
  {
    word i, rows=1, fileno, no_of_files=0;
    char *name, *descript;
    char str[STR], find_str[STR], *tok;
    long length, date, dloads;
    int carrier, noshow, find=0;
    byte section,sec;


    putdoorchar(CTRL);
    sendmessage("[0m[1;37;40m[0;32;40mÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿",1);
    sendmessage("³                          [33mBumbleFluckNowhere  B B S                         [32m³°",1);
    sendmessage("³ [31mÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ [32m³°",1);
    sendmessage("³                    [36m¯¯  Upload Lister by Ari Tsironis  ®®                   [32m³°",1);
    sendmessage("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ°",1);
    sendmessage(" °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°",1);
    sendmessage("[2;9H[35m%c[2;63H[35m%d[7;1H",0);


    rows+=6;

   
    /* FIND OUT IF USER IS SYSOP, IF HE IS ALLOW HIM TO LIST OTHERS UPLOADS  */
   
    str[0]=0;
    i=getsnum(ACCESSLEVEL);
    if(i>=sys)
      {
        sendmessage("",1);
        sendmessage("\33[0m\33[12C",0);
        sendmessage("You are a SYSOP, enter name of User or hit return!",CRLF);
        prompt("\33[12C> ",str,40);
        rows+=3;
        if(str[0]>0)
          strcpy(u_name, str);
      }

    sendmessage("\33[40;32m",CRLF);
    hotkey("\33[12C(F)ind, \33[31m(A)bort, \33[33mor <RETURN> to continue...",str);
    sendmessage("",CRLF);
    rows+=3;
    
    if((str[0]=='A') || (str[0]=='a'))
      return(0);     
      
    if((str[0]=='F') || (str[0]=='f'))
      {
        prompt("\33[12C\33[0mEnter search string : ",find_str,40);
        strupr(find_str);
        find=1;
        rows++;
      } 
      

    sendmessage("\33[40;32m",CRLF); 
    sprintf(str,"Finding files that %s has UPLOADED, Please Wait.",u_name);
  	sendmessage(str, CRLF);
    carrier=sendmessage("",CRLF);
    if(carrier==CARRIER_DROPPED)
      return(0);
    
    rows+=3;         


    /* SEARCH THROUGH BUFFER MATCHING OCCURENCES OF USER's NAME */

    for(i=FIRST_USER;i<f_length; i+=OFFSET_USER)
      if(strcmpi(u_name, &f_buffer[i])==0)
        {
          fileno=(i-FIRST_USER)/OFFSET_USER;
          name= &f_buffer[i+OFFSET_FILENAME];
          
          length=extractwl(f_buffer, i+OFFSET_LENGTH, L);
          
          date=extractwl(f_buffer, i+OFFSET_DATE, W);
          
          dloads=extractwl(f_buffer, i+OFFSET_DLOADS, L);
          
          section=f_buffer[i+OFFSET_SECTION];
          
          descript= &f_buffer[i+OFFSET_DESCRIPT];


          /* PULL OUT EACH SECTION FROM SECTION STRING AND COMPARE TO SECTION */
          /* CONTAINED IN FILE, IF MATCH THEN DON'T SHOW, AS THESE ARE HIDDEN */
          /* AS LONG AS USER HAS AN ENTRY IN UPLOAD_LISTER.CONFIG             */
          
          /* DEFAULT TO SHOWING EVERY ENTRY */

          noshow=0;
          
          
          /* CHECK TO SEE IF USER'S NAME IS IN CONFIG FILE */
          
          if(strcmp(sec_str,"XXXNO_NAMEXXX")!=0)
            {
              strcpy(str,sec_str);
              
              
              /* THIS IS A GREAT FUNCTION, PULL OUT EACH SECTION SEPERATED BY */
              /* A COMMA OR SPACE, I LIKE THIS ONE ;-)                        */
              
              tok=strtok(str,", ");

              while(tok!=NULL)
                {

                  /* CHANGE EACH SECTION INTO A BYTE, SO WE CAN COMPARE IT */
                  
                  sec=(byte)strtol(tok,NULL,10);
                  
                  tok=strtok(NULL,", ");
                  
                  if(sec==section)
                    {
                      noshow=1;
                      break;
                    }
                }
            }
         
         
          /* IF ENTRY HAS NOT ALREADY BEEN MARKED AS HIDDEN FROM ABOVE, THEN */
          /* CHECK IF USER ASKED FOR A FIND, IF SO THEN SHOW ONLY THOSE      */
          /* ENTRIES THAT HAVE THE SUBSTRING find_str.                 */
                   
          if((find==1) && (noshow==0))
            {
              noshow=1;
              
              strcpy(str,name);
              strupr(str);
              
              if(strstr(str,find_str)!=NULL)
                noshow=0;
                
              strcpy(str,descript); 
              strupr(str);
               
              if(strstr(str,find_str)!=NULL)
                noshow=0;
            }
          
          
          /* IF SECTION OF FILE EQUALS ANY OF THE SECTIONS WITHIN THE SECTION */
          /* STRING, THEN HIDE THIS FILE ENTRY                                */
          
          if(noshow==0)
            {
              /* DON'T UPDATE FILE COUNTER FOR HIDDEN FILES */
              
              no_of_files++;
              
              
              /*CHECK FIRST PRINT FOR A DROPPED CARRIER */
              
              sprintf(str,"\33[0m%d",fileno); carrier=sendmessage(str, 0);
              if(carrier==CARRIER_DROPPED)
                return(0);
              
              sprintf(str," %-18s",name); sendmessage(str, 0);
              sprintf(str," \33[36m%s",descript); sendmessage(str, CRLF);
          
              sprintf(str,"\33[24C\33[35m%-7ld",length); sendmessage(str, 0);
              sprintf(str,"  \33[32mDloads=%ld",dloads); sendmessage(str, 0);
              sprintf(str,"  \33[33mSection=%d",section); sendmessage(str, CRLF);

          
              /* CHECK TO SEE IF SCREEN IS PAUSED, ALLOW USER TO STOP */
              /* ALSO CHECK FOR A LOST CARRIER!                       */
              /* I KNOW THIS IS KLUDGY, BUT I DON'T REALLY MIND ;-)   */
          
              rows+=2;
              if(getsnum(SCREENROWS)-rows<3)
                {
                  carrier=hotkey("\33[33mHit <RETURN> to continue (S - Stop)...", str);
                  if((str[0]=='s') || (str[0]=='S') || carrier==(CARRIER_DROPPED)) 
                    return(0);

                  putdoorchar(CTRL);
                  rows=1;
                }
            }
        }
        

    sendmessage("",CRLF);  

    if(find==0)
      sprintf(str,"\33[40;32mNo. of files Uploaded = %d",no_of_files);
    if(find==1)
      sprintf(str,"\33[40;32mNo. of files Listed = %d",no_of_files);
    
    sendmessage(str,CRLF);
       
      
    /* NAUGHTY NAUGHTY */  
    
    if((no_of_files==0) && (find==0))
      { 
        sendmessage("\33[14C\33[37;41m",0);
        sendmessage("                                                   ", CRLF);
        sendmessage("\33[14C\33[37;41m",0);
        sendmessage("You have never uploaded anything, SHAME ON YOU!!!!!", CRLF);
        sendmessage("\33[14C\33[37;41m",0);
        sendmessage("                                                   ", CRLF);
      }

    return(PRINT);
  }



/*****************************************************************************/
/* EXTRACT A WORD OR LONG WORD FROM A BUFFER                                 */
/*****************************************************************************/

long extractwl(byte *f_buffer, word pos, int type)
  {
    long result;
    
    
    switch(type)
      {
        case W:  result=f_buffer[pos+0]*0x00000100
                       +f_buffer[pos+1]*0x00000001;
                 break;

        case L:  result=f_buffer[pos+0]*0x01000000
                       +f_buffer[pos+1]*0x00010000
                       +f_buffer[pos+2]*0x00000100
                       +f_buffer[pos+3]*0x00000001;
                 break;
                
        default: result=0;
                 break;
      }
    return(result);
  }          



/*****************************************************************************/
/* DEALLOCATE ALL THAT WAS ALLOCATED                                         */
/*****************************************************************************/

void deallocate(int f, byte *f_buffer, long alloc)
  {
    if(alloc&FILE_OPEN)
        close(f);
    
    if(alloc&MEMORY_OPEN)
        free(f_buffer);
  }



/*****************************************************************************/
/* WELL I WONDER ;-)                                                         */
/*****************************************************************************/

void print_error(char *err_str)
  {
    char str[STR];
    
    
    sendmessage("\33[37;41m",0);
    sprintf(str,"There is a problem with %s, please notify your Sysop", err_str); 
    sendmessage(str, CRLF);
    sendmessage("\33[0m", CRLF);
  }



/*****************************************************************************/
/* DEALLOCATE EVERYTHING AND SAY GOODBYE                                     */
/*****************************************************************************/

void tidy_up(int f, byte *f_buffer, long alloc, int p)
  {
    char str[STR];
    
    
    deallocate(f, f_buffer, alloc);


    /* CHECK IF KEY TO EXIT IS NEEDED */
    
    if (p)
      hotkey( "\33[0mHit any key to exit...", str); 
      
    end();
  }
