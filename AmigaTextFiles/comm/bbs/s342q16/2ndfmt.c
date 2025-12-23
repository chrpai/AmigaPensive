/*
 *				2ndfmt.C
 *
 * Creates a pre-processed secondary file from raw input.
 */

/*
 *				History
 *
 * 90May21  HAW V1 Created.
 */

/*
 *				Contents
 *
 *	main()			main manager
 *	InitData()		initializes the data structures
 *	SysCheck()		list search function
 *	SysCmp()		list sorting function
 *	EatDomainLine()		eats a line from the map file
 *	GetDynamic()		malloc bottleneck
 *	ShowArguments()		usage
 *	WriteCompressed()	manages writing out the compressed map
 *	WriteSystems()		work fn to write out map info
 */

#include "2ndfmt.h"
#include "slist.h"

void Motorola32ToIntel(UNS_32 *val);

void crashout(char *str);

void crashout(str)
char *str;
  {
  exit(printf(str));

  }

void Motorola32ToIntel(UNS_32 *val)
  {
  unsigned long temp;
  temp = *val;
  *val = ((ULONG)(temp & 0xff) << 24) + ((ULONG)(temp & 0xff00) << 8) +
  ((ULONG)(temp & 0xff0000) >> 8) + ((ULONG)(temp &0xff000000) >> 24);

  }


#define TRUE		1
#define FALSE		0

typedef struct {
	long ByteCount;
	SListBase SysList;
} Bucket;
Bucket Buckets[BUCKETCOUNT];		/* 0 .. 9, A .. Z   */

typedef struct {
	char DupFlag;
	char *Domain;		/* domain name */
	char *SysName;
	char *RealName;		/* if SysName is an alias, this is the real name */
} ASystem;

extern FILE *upfd;

void *SysCheck(), *EatDomainLine();
int  SysCmp();
char version[90];

/* just so we can automate reading the file */
SListBase Dummy = { NULL, NULL, NULL, NULL, EatDomainLine };

JumpInfo JumpTable[BUCKETCOUNT];

/*
 * Initial Format
 *
 * The initial format of the input file is a collection of "domain" records.
 * Each record begins with the line ".domain <domainname>".  Each following
 * non-blank line designates a system within that domain until EOF or the
 * next line of format ".domain <domainname>".
 *
 * Each line designating a system is generically:
 *
 * <system name>[:<real name>]
 *
 * Real name is only of use when defining a common alias for a system.  For
 * example,
 *
 * Test System : C-86 Test System
 *
 * would make a lot of sense.
 */

/*
 * Final Format
 *
 * Each line will look generically like this:
 *
 * [flag byte]<system><tab><domain>[<tab><real name>]<\n>
 *
 * In English, an entry is composed of an optional flag byte, followed by
 * the system name and its domain separated by a TAB, optionally followed
 * by the real system name (again separated by a TAB) if the name is an
 * alias for someone else.
 *
 * The optional flag byte is distinguishable by its value, which will be
 * non-printing (!isprint(x)).  Flag byte values will be defined as needed.
 * Currently, the list of flag values (see 2ndFmt.h, too) is limited to
 * this:
 *
 * DUPLICATE - 0x01, this means there is more than one system by this name
 * in the list.  Makes processing a bit easier by main program.
 *
 */

extern char      *READ_TEXT, *WRITE_ANY;

FILE *fd;

void ShowArguments(void);
void InitData(void);
void WriteCompressed(void);

/*
 * main()
 *
 * This is the main manager for routemap eater.
 */
int main(int , char **);

int main(argc, argv)
int  argc;
char **argv;
{

	printf("2ndFmt V1.0\nSecondary List Processor\n%s\n", FMT_COPYRIGHT);

	if (argc != 3) {
	    ShowArguments();
	    exit(1);
	}

	InitData();

	if ((fd = fopen(argv[1], READ_TEXT)) == NULL)
	exit(printf("Could not open %s.\n", argv[1]));

	GetAString(version, sizeof version, fd);

	MakeList(&Dummy, "", fd);	/* eat designated file */

	if ((fd = fopen(argv[2], WRITE_ANY)) == NULL)
		exit(printf("Couldn't open output file %s.", argv[2]));

	WriteCompressed();

	return 0;
}

/*
 * InitData()
 *
 * This function initializes the data structures.
 */
void InitData()
{
	int rover;

	for (rover = 0; rover < BUCKETCOUNT; rover++) {
		Buckets[rover].ByteCount = 0l;
		InitListValues(&Buckets[rover].SysList, SysCheck, SysCmp,
								NULL, NULL);
	}
}

/*
 * SysCheck()
 *
 * This is a list search function.
 */
void *SysCheck(ASystem *s, char *str)
{
	return (stricmp(s->SysName, str) == 0) ? s : NULL;
}

/*
 * SysCmp()
 *
 * This is a sorting function for the list.
 */
int  SysCmp(ASystem *s, ASystem *t)
{
	if (stricmp(s->SysName, t->SysName) == 0) {
		if (s->DupFlag) return -1;	/* duplicate flag comes first */
		if (t->DupFlag) return 1;
	}
	return (stricmp(s->SysName, t->SysName));
}

#define DOMAIN		0
#define SYSTEM		1

char MapState = DOMAIN;
char *CurDomain;
/*
 * EatDomainLine()
 *
 * This will eat a line from the map file and return an appropriately
 * initialized structure.
 */
void *EatDomainLine(char *line)
{
	char *c, *d;
	ASystem *data;
	int bucket;

	if ((c = strchr(line, '%')) != NULL)	*c = 0;

	if (strncmp(line, ".domain", 7) == 0) {
		if ((c = strchr(line, ' ')) == NULL || strlen(c) < 2) {
			printf("Bad domain formation (no space).\n");
			return NULL;
		}
		if ((d = strchr(c + 1, ' ')) != NULL) *d = 0;
		CurDomain = strdup(c + 1);
		MapState = SYSTEM;
		return NULL;
	}

	if (MapState == SYSTEM && strlen(line) > 2) {
		data = GetDynamic(sizeof *data);
		if ((c = strchr(line, ':')) != NULL)
			*c++ = 0;

		NormStr(line);		/* clean up string */
		if (!isalpha(*line) && !isdigit(*line)) {
			printf("ERROR: '-%s-' is NOT a valid system name.\n",
							line);
			return NULL;
		}

		data->SysName = strdup(line);
		if (c == NULL)
			data->RealName = NULL;
		else {
			NormStr(c);
			data->RealName = strdup(c);
		}
		data->Domain = CurDomain;

		bucket = (isdigit(line[0])) ?	line[0] - '0' :
						toupper(line[0]) - 'A' + 10;
		data->DupFlag = (SearchList(&Buckets[bucket].SysList, line) != NULL);
		AddData(&Buckets[bucket].SysList, data, NULL, FALSE);
		Buckets[bucket].ByteCount += strlen(data->SysName) + 1;
		Buckets[bucket].ByteCount += strlen(data->Domain) + 1;
		if (data->RealName != NULL)
			Buckets[bucket].ByteCount += strlen(data->RealName) + 1;
		if (data->DupFlag) {
			printf("NOTE: Duplicate found for name %s.\n", data->SysName);
			Buckets[bucket].ByteCount++;
		}
	}
	return NULL;
}


/*
 * ShowArguments()
 *
 * This function will show the usage of this utility to the user.
 */
void ShowArguments(void)
{
	printf("Usage: 2ndfmt <input filename> <output filename>");
}

/*
 * WriteCompressed()
 *
 * This function manages writing out the compressed map.
 */
void WriteCompressed(void)
{
	long count = 0l;
	char vers;
	int rover;
	void WriteSystems();

	vers = TABLE_VERS;

	fwrite(version, VERS_SIZE, 1, fd);
	fwrite(&vers, sizeof vers, 1, fd);
	count = VERS_SIZE + sizeof JumpTable + sizeof vers;

#ifdef IS_MOTOROLA
	for (rover = 0; rover < BUCKETCOUNT; rover++)
		Motorola32ToIntel(&JumpTable[rover].offset);
#endif

	/* write out jump table */
	for (rover = 0; rover < BUCKETCOUNT; rover++) {
		JumpTable[rover].offset = count;
		count += Buckets[rover].ByteCount;
	}

#ifdef IS_MOTOROLA
	for (rover = 0; rover < BUCKETCOUNT; rover++)
		Motorola32ToIntel(&JumpTable[rover].offset);
#endif

	fwrite(JumpTable, sizeof JumpTable, 1, fd);

	for (rover = 0; rover < BUCKETCOUNT; rover++)
		RunList(&Buckets[rover].SysList, WriteSystems);

	fprintf(fd, "\n");	/* marks EOF - eliminate noise */

	fclose(fd);
}

/*
 * WriteSystems()
 *
 * This is the work fn to write out the information.
 */
void WriteSystems(ASystem *data)
{
	if (data->DupFlag)
		fprintf(fd, "%c", DUP);
	fprintf(fd, "%s\t%s", data->SysName, data->Domain);
	if (data->RealName != NULL)
		fprintf(fd, "\t%s", data->RealName);
	fprintf(fd, "\n");
}

