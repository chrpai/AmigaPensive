/*
                               VexFind.c
 DESCRIPTION:
 This utility will report on *all* or individual systems in
 the vortex information.  It will list the messages in the
 order they were recieved.  If no rooms are listed on the command
 line, then all rooms will be reported.  If rooms are listed only
 those rooms will be reported on.

 To remove dependancy on the CTDL database, this utility must
 be run from the vortex directory in the netarea. This eliminates
 reading the CTDLTABL.SYS file and all sorts of overhead.

 */
#include "ctdl.h"    /* header file  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vortex.h"
#include <dos.h>
/*
 * main()
 *
 * This is the main manager.
 */
int main(int, char **);
void Process_Rooms(void);
void Process_Room(char *room);

int main(argc, argv)
int  argc;
char **argv;
{
    int i;
    printf("Citadel Vortex Data Report %s-%s\n",VERSION_NAME, COPYRIGHT);
    if ( argc == 1 )
      {
      Process_Rooms();
      }
    else
      {
      for(i=1; i<argc; i++) Process_Room(argv[i]);
      };
  return 0;
}

/*
 * crashout()
 *
 * Fatal error handler.
 */
void crashout(char *str)
{
    exit(printf("%s\n", str));
}

struct Entries
  {
  struct Entries *next; /* link to next entry */
  char *name;           /* pointer to the name of the vortex file(room name) */
  };

void Process_Rooms(void)
/**
  Description:  Get a directory listing of the vortex directory
  and call Process_Room() with each name found.
**/
  {
  struct Entries *Head;  /* current level head */
  struct Entries *Last;  /* current level tail */
  struct Entries  *litem, *citem, *nitem;
  struct FileInfoBlock *info;
  char *filename;              /* set to all files ending in .hlp */
  short not_done;
  int error,attr;

  Head = NULL;
  filename = "#?";

  info  = (struct FileInfoBlock *)calloc(1,sizeof(struct FileInfoBlock));
  attr  = 0;    /* find all files, no directories */
  error = dfind(info,filename,attr);  /* get first one*/

  while( error == 0 )
    {
    citem = (struct Entries *)calloc(1,sizeof(struct Entries));
    if( Head == NULL )
      {
      Last = Head = citem;
      }
    else
      {
      Last->next = citem;
      Last = citem;
      };
    citem->name = strdup(info->fib_FileName);
    error = dnext(info);
    };
  /* sort the directory list into alphabetical order */
  if( Head->next != NULL )
    {
    not_done = TRUE;
    while ( not_done )
      {
      not_done = FALSE;
      litem = NULL;
      citem = Head;
      nitem = citem->next;
      while ( nitem != NULL )
        {
        if( strcmpi(nitem->name,citem->name) < 0 )
          {
          not_done = TRUE;
          if( litem == NULL )
            {
            Head = litem = nitem;
            }
          else litem->next = nitem;
          citem->next = nitem->next;
          nitem->next = citem;
          };
        litem = citem;
        citem = nitem;
        nitem = citem->next;
        };
      };
    };
  /**
    Now Report on Each Room
  **/
  while( Head )
    {
    Process_Room(Head->name);
    Head = Head->next;
    };
  }

int first_flag = TRUE;

void Process_Room(char *room)
  {
  /**
    Given a room name, a file in the current directory,
    open the file and report the contents.
  **/
  ROOM_ENTRY_TYPE record;
  FILE *ip;
  int start;
  int count;
  if( (ip=fopen(room,"r")) == NULL )
    {
    printf("Error: could not open room %s\n",room);
    }
  else
    {
    if( fread(&record, sizeof(ROOM_ENTRY_TYPE), 1, ip) != 1 )
      {
      printf("Error:  could not read the vortex record for %s\n",room);
      }
    else
      {
      start = ( record.index < MAX_VORTEX_SIZE ) ? 0 : record.next_slot;
      count = record.index;
      if( first_flag )
        {
        first_flag = FALSE;
        printf("%-20s %7s %8s %12s %s\n","Room", "Date", "Time","Id", "Orig");
        };
      while( count-- )
        {
        printf("%-20s %7s %8s %12s %s\n", room , record.msg_entry[start].mbdate
        , record.msg_entry[start].mbtime, record.msg_entry[start].mbsrcId
        , record.msg_entry[start].mborig);
        start = (start + 1 ) % MAX_VORTEX_SIZE;
        };
      };
    fclose(ip);
    };
  }

