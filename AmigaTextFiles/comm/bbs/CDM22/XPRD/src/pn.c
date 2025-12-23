#include <stdio.h>
#include <string.h>
#include <proto/dos.h>
#include <proto/exec.h>

static void getpathname(BPTR lock,char *array)
{
	struct FileInfoBlock *fib=AllocMem(sizeof(struct FileInfoBlock),0);
	BPTR newlock;
	if(!fib||!lock) goto xit;
	Examine(lock,fib);
	getpathname(newlock=ParentDir(lock),array);			
	strcat(array,fib->fib_FileName);
	strcat(array,(newlock)?"/":":");
xit:
	if(fib) FreeMem(fib,sizeof(struct FileInfoBlock));
	if(lock) UnLock(lock);
	return;
}

void getcdname(char *buffer)
{
	*buffer=0;
	getpathname(Lock("",SHARED_LOCK),buffer);
}	
