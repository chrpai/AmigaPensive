/**
  GETENV - replacement for the standard library version to
           limit the I/O for regular processing of ENV:TZ
           in some library calls.
  Version 0.00 -- initial coding
  Verison 1.00 -- fixed bug in loop, everything operational!
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#undef getenv
char *  __getenv(const char *name);

struct namelist
  {
  struct namelist *next;  /* link to next item */
  char *name;             /* name of item      */
  char *value;            /* value of item     */
  };

static struct namelist *head=NULL;

char *  __getenv(const char *name)
  {
  struct namelist *ptr;  /* current item */
  char *value;           /* buffer */
  char filename[128];    /* actual name of ENV variable in file system */
  FILE *ep;              /* file pointer */
/**
  Name is looked up in name list, if not found, an entry is
  created and it will be set to the value of the file ENV:name
  if ENV:name does not exist, the value will be NULL

  LIMITATION:  Maximum ENV valiable is BUFSIZ.
**/
  ptr = head;
  while( NULL != ptr )
    {
    if( NULL == strcmp(name,ptr->name) ) break;
    ptr = ptr->next;
    };
/**
  Now we either have the name in our list or it is new.
  If in the list, return a duplicate of the entry value, else
  add it to the list.
**/
 if( NULL == ptr )
   {
/**
  New entry, first we attempt to open the file ENV:name.
  If the file does not exist, Set value to NULL; otherwise
  open the file and read contents into the buffer.
**/
   ptr = (struct namelist *)calloc(1,sizeof(struct namelist));
   if( NULL == ptr )
     {
     return (char *)NULL;          /* no memory available, return NULL */
     };
   value = (char *)calloc(1,BUFSIZ);
   if( NULL == value )
     {
     return (char *)NULL;
     };
   /**
     build the structure, add it to the list
     Now open the file and read the contents
   **/
   ptr->name = strdup(name);
   ptr->value= NULL;
   ptr->next = head;
   head      = ptr;        /* list is most recently used order */
   strcpy(filename, "ENV:");
   strcat(filename,name);
   if( NULL != (ep = fopen(filename,"r")) )
     {
     (void) fread(value, 1, BUFSIZ,ep);
     ptr->value = value;
     fclose(ep);
     }
   else
     {
     free(value);
     };
   };
  return ( ptr->value );
 /*  return ( ptr->value == NULL) ? (char *)NULL : strdup(ptr->value); */
  }
