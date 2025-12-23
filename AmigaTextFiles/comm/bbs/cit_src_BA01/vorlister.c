/**
 Vortex File Lister Version 1.00
**/

#include <stdio.h>
#include <string.h>
#include <libraries/dos.h>
#include <dos.h>
#include "ctdl.h"
#include "vortex.h"
#include <stdlib.h>

int sflag = FALSE;

struct FileInfoBlock *info;
char *filename;
void Process_File(char *name);

void main( int argc, char **argv);
void main( int argc, char **argv)
  {
  int error, attr;
  if( argc > 1 )
    {
    if( argv[argc-1][0] == '-' )
      {
      argc--;
      if( argv[argc][1] == 'a' ||  argv[argc][1] == 'a')sflag = TRUE;
      };
    };
  if( argc == 1 )
    {
    filename = "#?";
    info  = (struct FileInfoBlock *)calloc(1,sizeof(struct FileInfoBlock));
    attr = 1;          /* find all files not directories */
    error = dfind(info,filename,attr);  /* get first one*/
    while( error == 0 )
      {
      Process_File(info->fib_FileName);
      error = dnext(info);
      };
    free(info);
    }
  else
    {
    while( argc > 1 )
      {
      argc--;
      if( argv[argc][0] == '-' )
        {
        if( argv[argc][1] == 'a' ||  argv[argc][1] == 'A')sflag = TRUE;
        }
      else  Process_File(argv[argc]);
      };
    };
  }

void Process_File(char *name)
  {
  ROOM_ENTRY_TYPE rec;
  FILE *ip;
  if( ( ip = fopen(name, "r") ) == NULL )
    {
    printf("Error: cannot open %s\n", name);
    }
  else
    {
    if( fread((void *)&rec, sizeof(ROOM_ENTRY_TYPE), 1, ip) != 1 )
      {
      printf("Error: could not read a record for file %s\n",name);
      }
    else
      {
      int node, nnodes;
      int index;
      nnodes = rec.index;
      node   = ( nnodes < MAX_VORTEX_SIZE ) ? 0 : rec.next_slot;
      printf("Vortex File: %s Index: %d Next: %d\n", name, rec.index, rec.next_slot );
      printf("%20s %12s %10s %10s\n","Origin", "Src Id", "Date", "Time");
      while( nnodes-- )
        {
        if( sflag == TRUE )
          {
          printf( "%20s %12s %10s %10s %d\n",
          rec.msg_entry[node].mborig, rec.msg_entry[node].mbsrcId,
          rec.msg_entry[node].mbdate, rec.msg_entry[node].mbtime,
          node);
          }
        else
          {
          for( index=0; index < MAX_VORTEX_SIZE; index++ )
            {
            if( index == node ) continue;
            if( rec.msg_entry[node].mbsrcId[0] )
              if( rec.msg_entry[index].mbsrcId[0] )
                if( strcmp(rec.msg_entry[node].mbsrcId, rec.msg_entry[index].mbsrcId) == 0 )
              {
              printf( "%20s %12s %10s %10s %d\n",
              rec.msg_entry[node].mborig, rec.msg_entry[node].mbsrcId,
              rec.msg_entry[node].mbdate, rec.msg_entry[node].mbtime,
              node);
              };
             };
           };
        node = ( node + 1 ) % MAX_VORTEX_SIZE;
        };
      };
    fclose(ip);
    };
  }
