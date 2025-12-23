/************************************************************************/
/*                              C68Door.h                               */
/*              Header file for doors. (System dependent file.)         */
/************************************************************************/
/************************************************************************/
/*                              History                                 */
/*                                                                      */
/* 89Nov12 AP Created.                                                  */
/************************************************************************/
/* Name of data file */
#define DOOR_DATA       "citadoor.sys"
/* These define privileges. */
#define DOOR_ANYONE     0
#define DOOR_AIDE       0x01
#define DOOR_SYSOP      0x02
#define DOOR_CON        0x04
#define DOOR_MODEM      0x08
#define DOOR_AUTO       0x10
#define DOOR_NEWUSER    0x20
/*
* These constants are used for parameter variable identification.
*/
#define DV_BAUD         1
#define DV_BPS          2
#define DV_USER_NAME    4
#define DV_USER_NUM     5
#define DV_ANSI         6
typedef struct 
  {
  char  entrycode[6];         /* What user uses to specify a door     */
  char  program[14];          /* name of program                      */
  char  location[50];         /* door location                        */
  char  description[80];      /* description of door                  */
  char  flags;                /* sysop, aides, or everyone            */
  char  parameters[100];      /* list of parameters.                  */
  int   TimeLimit;            /* time limit for this door             */
  label RoomName;             /* room name                            */
  
  }
DoorData;
#ifdef CTDL_HEADER
char doDoor(char x);
char BackFromDoor(void);
char NoTimeForDoor(int which, DoorData *DoorInfo);
#endif 
