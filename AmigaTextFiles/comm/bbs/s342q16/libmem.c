/*  This is a set of functions that will replace
*  malloc(), calloc(), realloc(), and free()
*  from the standard C libraries.  These routines
*  link their allocations into the task's memory list
*  and all allocated memory not freed during the
*  execution of the program will be freed upon task removal.
*/
#include <stdio.h>
#include <string.h>
#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/memory.h>
#include <proto/exec.h>
#define ALLOCERROR 0x80000000
void mem_debug(char *, long);
struct MemList *FindMemListFromMemPtr(void *MemPtr);
ULONG FindMemSizeFromMemListAndPtr(struct MemList *MList, void *MPtr);
void free(void *oldblock)
  {
  struct MemList *MList;
  MList = FindMemListFromMemPtr(oldblock);
  if (MList)
    {
    Remove((struct Node *)MList);
    FreeEntry(MList);

    }
  else
    {
    mem_debug("could not find to free:",(long)oldblock);

    };

  }
void *malloc(unsigned int size)
  {
  void *AllocBlock;
  struct MemList *NeedMList;
  struct MemList mneed;
  AllocBlock = (void *)0;
  if (size)
    {
    mneed.ml_NumEntries = 1;
    mneed.ml_ME[0].me_Reqs = MEMF_CLEAR;
    mneed.ml_ME[0].me_Length = size;
    NeedMList = (struct MemList *)AllocEntry((struct MemList *)&mneed);
    if (!((ULONG)NeedMList & (ULONG)ALLOCERROR))
      {
      AllocBlock = NeedMList->ml_ME[0].me_Addr;
      AddTail(&(FindTask(0)->tc_MemEntry), (struct Node *)NeedMList);

      }

    }
  return(AllocBlock);

  }
void mem_debug(st,xp)
char *st;
long xp;
  {
  FILE *fp;
  fp = fopen("memory_error.log","a");
  fprintf(fp," %8s: %08.8lx\n",st,xp);
  fclose(fp);

  }
void *calloc(unsigned int size, unsigned int count)
  {
  if (!size || !count)  return((void *)0);
  return(malloc(size * count));

  }
void *realloc(void *oldblock, unsigned int size)
  {
  void *newblock;
  struct MemList *MLPtr;
  unsigned int bsize, csize;
  newblock = (void *)0;
  if (oldblock && size)
    {
    MLPtr = FindMemListFromMemPtr(oldblock);
    if (MLPtr)
      {
      bsize = (unsigned int)FindMemSizeFromMemListAndPtr(MLPtr, oldblock);
      if (bsize)
        {
        newblock = malloc(size);
        if (newblock)
          {
          csize = (bsize > size) ? size : bsize;
          CopyMem(oldblock, newblock, csize);
          free(oldblock);

          }

        }

      }

    }
  return(newblock);

  }
struct MemList *FindMemListFromMemPtr(void *MemPtr)
  {
  struct MemList *MList, *MListPtr;
  int loop;
  MList = (struct MemList *)0;
  MListPtr = (struct MemList *)(FindTask(0)->tc_MemEntry.lh_Head);
  while(MListPtr && !MList)
    {
    for (loop = 0; ((loop < MListPtr->ml_NumEntries) && !MList); loop++)
      {
      if (MListPtr->ml_ME[loop].me_Addr == (APTR)MemPtr)
        {
        MList = MListPtr;

        }

      }
    MListPtr = (struct MemList *)MListPtr->ml_Node.ln_Succ;

    }
  return(MList);

  }
ULONG FindMemSizeFromMemListAndPtr(struct MemList *MList, void *MPtr)
  {
  ULONG MSize;
  int loop;
  MSize = 0L;
  for (loop = 0; ((loop < MList->ml_NumEntries) && !MSize); loop++)
    {
    if (MList->ml_ME[loop].me_Addr == (APTR)MPtr)
      {
      MSize = MList->ml_ME[loop].me_Length;

      }

    }
  return(MSize);

  }
