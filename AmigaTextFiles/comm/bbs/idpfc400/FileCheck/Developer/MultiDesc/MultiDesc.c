/*************************************************************************\
* MultiDesc V3.01  by cANNoN/iDP!  (Ronny Schulz)                         *
*                                                                         *
*  VERSION: 3.01                                                          *
*     DATE: 08.06.1998                                                    *
\*************************************************************************/

/*************************************************************************\

  This code may be freely distributed.

  If you wish to distribute a modified version of this program, you have
  to indicate this in the program and the source code and you have to
  distribute the source code too.

  I am holding the copyright of the source code. Please do not delete
  my name from the program files or the documentation.

  You can contact me under the following addresses. If one will not work,
  try the next please:

   Cannon@BigPlanet.tobcv.de
   Cannon@Outside.in-berlin.de
   100.143122@germanynet.de

  Compiled and tested with:

   CPU      Operating System  Compiler
   MC68030  AMIGA OS 3.1      MaxonDevelop 4.0
   MC68030  AMIGA OS 3.1      SAS-C 6.58

  Modification history:

   Date      Version  Programmer    Description of modification
   98-05-24  3.00     R. Schulz     completly rewritten release
   98-06-08  3.01     R. Schulz     some things to makes it more portable
\*************************************************************************/


/* ### includes ###
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* ### defines ###
*/

#ifdef __MAXON__
#define MODE_READ_BINARY		"rb"
#define MODE_APPEND_BINARY		"ab"
#endif
#ifdef __SASC
#define MODE_READ_BINARY		"rb"
#define MODE_APPEND_BINARY		"ab"
#endif
#ifndef MODE_READ_BINARY
#define MODE_READ_BINARY		"rb"
#define MODE_APPEND_BINARY		"ab+"
#endif

#define BUFSIZE				 16384

/*  errorcodes - new errorcodes can be add, ERROR_NONE have to be 0,
    do not forget the messages and the returncodes for the errorcodes,
    -> see under variables */
#define ERROR_NONE			 0
#define ERROR_PARAMETER			-1
#define ERROR_NODIZ			-2
#define ERROR_NOFILE			-3
#define ERROR_UNKNOWN			-4


/* ### prototypes ###
*/
static void xtract_diz(char *file, char *desc);
static void add_diz(char *file, char *desc);
static void del_diz(char *file);

static long find_diz(char *file, long *startpos);
static long ffindstr(char *sstring, FILE *fp);

static long readfile(char *file, char *buf, long start, long count);
static long writefile(char *file, char *buf, long count);
static long copyfile(char *src, char *dst, long start, long count);
static long convert_diz(char *buf);

static void leave(long rc);


/* ### variables ###
*/
static char *versiontag    = "\0$VER: MultiDesc 3.01  (08.06.98)";

static char  mem[BUFSIZE];
static char *TEMP;

/* Return-Codes */
static int   RCODE[] = {0, 10, 5, 20, 20};

/* Messages */
static char *HEAD_TXT      = "\nMultiDesc 3.01  by cANNoN/iDP!\n";
static char *EXTDIZ_TXT    = "\nExtracting file_id.diz from \"%s\" to \"%s\" ...";
static char *EXTDIZWIN_TXT = "\nExtracting file_id.diz from \"%s\" ...";
static char *DELDIZ_TXT    = "\nRemoving file_id.diz from \"%s\" ...";
static char *ADDDIZ_TXT    = "\nAdding file_id.diz from \"%s\" to \"%s\" ...";
static char *DIZHEAD_TXT   = " VIEWING\n\n-[File_ID:]-------------------------------------------------------------------\n";
static char *DIZFOOT_TXT   = "\n----------------------------------------------------------------[%05d Bytes]-";
static char *ERROR_TXT[]   =
{
	"\n\nAll done. No errors found!\n\n",

	"\n"
	"USAGE: MultiDesc e <file> [description]\n"
	"       to extract a file-description\n\n"
	"       MultiDesc a <file> <description>\n"
	"       to add a file-description\n\n"
	"       MultiDesc d <file>\n"
	"       to remove a file-description\n\n",

	" FILE_ID NOT FOUND!\n\n",
	" FILE NOT FOUND!\n\n",
	" UNKNOWN ERROR!\n\n"
};



/* ### main-program ###
*/
int main(int argc, char **argv)
{
	static char command;
	static char *file1, *file2;

	printf(HEAD_TXT);
	TEMP = tmpnam(NULL);

	if ((argc > 2) && (argc < 5))
	{
		command = toupper(*argv[1]);
		file1 = argv[2];
		file2 = argv[3];

		if (command == 'E')
		{
			xtract_diz(file1, file2);
			leave(ERROR_NONE);
		}
		else if ((command == 'A') && (argc == 4))
		{
			add_diz(file1, file2);
			leave(ERROR_NONE);
		}
		else if ((command == 'D') && (argc == 3))
		{
			del_diz(file1);
			leave(ERROR_NONE);
		}
	}

	leave(ERROR_PARAMETER);
}



/* ### extract file_id from gif/jpeg/text/... ###
 *
 *	Parameters:
 *		char *file	filename where the file_id can be found
 *		char *desc	filename of the file-description
 *				(or NULL for windowoutput)
 *	Return:
 *		None
*/
static void xtract_diz(char *file, char *desc)
{
	static long rc, size, startpos;

	if (desc == NULL) printf(EXTDIZWIN_TXT, file);
	else printf(EXTDIZ_TXT, file, desc);

	if ((size = find_diz(file, &startpos)) <= ERROR_NONE) leave(size);

	if (size >= BUFSIZE) size = BUFSIZE-1;
	if ((size = readfile(file, mem, startpos, size)) <= ERROR_NONE) leave(size);
	mem[size] = 0;
	size = convert_diz(mem);

	if (desc == NULL)
	{
		printf(DIZHEAD_TXT);
		printf(mem);
		printf(DIZFOOT_TXT, size);
	}
	else
	{
		remove(desc);
		if ((rc = writefile(desc, mem, size)) <= ERROR_NONE) leave(rc);
	}
	return;
}


/* ### add file_id to gif/jpeg/text/... ###
 *
 *	Parameters:
 *		char *file	filename where the file_id has to be add
 *		char *desc	filename of the file-description
 *
 *	Return:
 *		None
*/
static void add_diz(char *file, char *desc)
{
	static long rc, size;
	static FILE *fp;

	printf(ADDDIZ_TXT, desc, file);
	rc = ERROR_NOFILE;
	if ((fp = fopen(file, MODE_APPEND_BINARY)) != NULL)
	{
		if (fseek(fp, 0, SEEK_END) == 0)
		{
			fputs("@BEGIN_FILE_ID.DIZ\n", fp);
			rc = ERROR_NONE;
		}
		fclose(fp);
	}

	if (rc == ERROR_NONE)
	{
		if ((size = readfile(desc, mem, 0, 0)) <= ERROR_NONE) leave(size);
		mem[size] = 0;
		size = convert_diz(mem);
		if ((rc = writefile(file, mem, size)) <= ERROR_NONE) leave(rc);

		rc = ERROR_UNKNOWN;
		if ((fp = fopen(file, MODE_APPEND_BINARY)) != NULL)
		{
			if (fseek(fp, 0, SEEK_END) == 0)
			{
				fputs("\n@END_FILE_ID.DIZ", fp);
				rc = ERROR_NONE;
			}
			fclose(fp);
		}
	}

	if (rc > ERROR_NONE) leave(rc);
	return;
}


/* ### delete file_id ###
 *
 *	Parameters:
 *		char *file	filename where the file_id can be found
 *
 *	Return:
 *		None
*/
static void del_diz(char *file)
{
	static long  rc, size, startpos;

	printf(DELDIZ_TXT, file);
	if ((size = find_diz(file, &startpos)) <= ERROR_NONE) leave(size);

	if ((rc = copyfile(file, TEMP, 0, startpos-18)) < ERROR_NONE) leave(rc);
	if ((rc = copyfile(file, TEMP, startpos+size+16, 0)) < ERROR_NONE) leave(rc);

	remove(file);
	if ((rc = copyfile(TEMP, file, 0, 0)) <= ERROR_NONE) leave(rc);
	return;
}



/* ### find the file_id ###
 *
 *	Parameters:
 *		char *file	filename of the file
 *	
 *
 *	Return:
 *		long		size of the file_id
 *				ERROR_NODIZ  = no file_id found
 *				ERROR_NOFILE = file not found
 *		long startpos	startposition of the file_id
*/
static long find_diz(char *file, long *startpos)
{
	static long  rc, id_pos, id_size;
	static FILE *fp;

	id_pos = 0;
	id_size = 0;

	rc = ERROR_NOFILE;
	if ((fp = fopen(file, MODE_READ_BINARY)) != NULL)
	{
		rc = ERROR_NODIZ;
		if ((id_pos = ffindstr("@BEGIN_FILE_ID.DIZ", fp)+18) > 17)
		{
			if (fseek(fp, id_pos, SEEK_SET) == 0)
			{
				if ((id_size = ffindstr("@END_FILE_ID.DIZ", fp)) > 0)
				{
					rc = id_size;
					memcpy(startpos, &id_pos, sizeof(id_pos));
				}
			}
		}
		fclose(fp);
	}
	return(rc);
}


/* ### search for a string in the given !open! file ###
 *
 *	Parameters:
 *		char *sstring	string to be search
 *		FILE *fp	filehandle of the file
 *
 *	Return:
 *		long		position in the file, after the line
 *				-1 = string not found
*/
static long ffindstr(char *sstring, FILE *fp)
{
	static long  buflen = BUFSIZE-2;
	static long  pos1, pos2, bytes;
	static char *result;

	pos1 = ftell(fp);
	pos2 = 0;

	while ((bytes = fread(mem+buflen/2, sizeof(*mem), (size_t)(buflen/2), fp)) != 0)
	{
		result = mem-1;
		while ((result = memchr(result+1, sstring[0], (size_t)(buflen-(result-mem)))) != NULL)
		{
			if ((result+strlen(sstring) < mem+buflen) && (strncmp(result, sstring, strlen(sstring)) == 0))
			{
				fseek(fp, pos1, SEEK_SET);
				return(pos2+(result-mem)-buflen/2);
			}
		}
		pos2 = pos2+bytes;
		memcpy(mem, mem+buflen/2, (size_t)buflen/2);
	}
	fseek(fp, pos1, SEEK_SET);
	return(-1);
}


/* ### read file ###
 *
 *	Parameters:
 *		char *file	source-file
 *		char *buf	buffer
 *		long  start	first byte to be read in the file
 *		long  count	bytes to be read
 *
 *	Return:
 *		long		number of read bytes (default == count)
 *				ERROR_NOFILE  = file not found
 *				ERROR_UNKNOWN = file-error
*/
static long readfile(char *file, char *buf, long start, long count)
{
	static long  rc;
	static FILE *fp;

	rc = ERROR_NOFILE;
	if ((fp = fopen(file, MODE_READ_BINARY)) != NULL)
	{
		rc = ERROR_NONE;

		if (count == 0)
		{
			rc = ERROR_UNKNOWN;
			if (fseek(fp, start, SEEK_END) == 0)
			{
				rc = ERROR_NONE;
				count = ftell(fp);
			}
		}

		if (rc == ERROR_NONE)
		{
			rc = ERROR_UNKNOWN;
			if (fseek(fp, start, SEEK_SET) == 0)
			{
				rc = fread(buf, sizeof(*buf), (size_t)count, fp);
			}
		}
		fclose(fp);
	}
	return(rc);
}


/* ### write file (file will be append) ###
 *
 *	Parameters:
 *		char *file	destination-file
 *		char *buf	buffer
 *		long  count	bytes to be write
 *
 *	Return:
 *		long		number of written bytes (default == count)
 *								ERROR_UNKNOWN = file-error
*/
static long writefile(char *file, char *buf, long count)
{
	static long  rc;
	static FILE *fp;

	rc = ERROR_UNKNOWN;
	if ((fp = fopen(file, MODE_APPEND_BINARY)) != NULL)
	{
		rc = fwrite(buf, sizeof(*buf), (size_t)count, fp);
		fclose(fp);
	}
	return(rc);
}


/* ### copy file (destination will be append) ###
 *
 *	Parameters:
 *		char *src	source-file
 *		char *dst	destination-file
 *		long  start	startpos in the file
 *				0 = first position
 *		long  count	maximal bytes to be copy
 *				0 = to the end of file
 *
 *	Return:
 *		long		number of written bytes
 *				ERROR_NOFILE  = file not found
 *				ERROR_UNKNOWN = file-error
*/
static long copyfile(char *src, char *dst, long start, long count)
{
	static long  rc, limit, bytes, calc;
	static FILE *fsrc, *fdst;

	rc = ERROR_NOFILE;
	if ((fsrc = fopen(src, MODE_READ_BINARY)) != NULL)
	{
		rc = ERROR_UNKNOWN;
		if ((fdst = fopen(dst, MODE_APPEND_BINARY)) != NULL)
		{
			if (fseek(fsrc, start, SEEK_SET) == 0)
			{
				rc = ERROR_NONE;
				calc = 0;
				limit = BUFSIZE;
				if ((count != 0) && (limit > count)) limit = count;
				while ((bytes = fread(mem, sizeof(*mem), (size_t)limit, fsrc)) != 0)
				{
					calc = calc+bytes;
					if (fwrite(mem, sizeof(*mem), (size_t)bytes, fdst) != bytes)
					{
						rc = ERROR_UNKNOWN;
					}
					if (rc == ERROR_UNKNOWN) break;
					if ((count != 0) && (calc == count)) break;
					limit = BUFSIZE;
					if ((count != 0) && (calc+limit > count)) limit = count-calc;
				}
				rc = calc;
			}
			fclose(fdst);
		}
		fclose(fsrc);
	}
	return(rc);
}


/* ### optimize the file_id ###
 *
 *	Parameters:
 *		char *buf	buffer to convert
 *
 *	Return:
 *		long		size of the string
*/
static long convert_diz(char *buf)
{
	static long  size;
	static char *ch1, *ch2;

	size = strlen(buf);

	/*** strip spaces on the end of a line ***/
	ch1 = buf;
	while ((ch1 = strchr(ch1, '\n')) != NULL)
	{
		ch2 = ch1-1;
		while ((ch2 > buf) && (ch2[0] == ' '))
		{
			strcpy(ch2, ch2+1);
			size--;
			ch2--;
		}
		ch1++;
	};

	/*** strip empty lines ***/
	while ((*buf == '\n') && (size > 0))
	{
		strcpy(buf, buf+1);
		size--;
	};
	while ((size > 0) && (buf[size-1] == '\n')) size--;
	mem[size] = 0;

	return(size);
}


/* ### leave program ###
*/
static void leave(long rc)
{
	remove(TEMP);
	printf(ERROR_TXT[abs(rc)]);
	exit(RCODE[abs(rc)]);
}
