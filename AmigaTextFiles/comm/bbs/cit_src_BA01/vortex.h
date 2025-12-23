/**
  This include is used by vortex.c and vexfind.c
  to ensure a common definition for all vortex checking
  code.

  Structure:
  Each File is named by Room name(normalized).
  Up to the last 120 message in the room will be tracked.
  The data structure maintained is a partial message header.

**/

#define MAX_VORTEX_SIZE  (120)   /* Maximum of message header info to keep */

typedef struct
  {
  label mborig;         /* Node Creating the Message      */
  label mbsrcId;        /* message ID on system of origin */
  label mbdate ;        /* creation date                  */
  label mbtime ;        /* creation time                  */
  }  MESSAGE_ENTRY_TYPE;

typedef struct
  {
  int   index;                      /* Number of entries   */
  int   next_slot;                  /* next free slot      */
  MESSAGE_ENTRY_TYPE  msg_entry[MAX_VORTEX_SIZE];  /* Array msg headers per room */
  } ROOM_ENTRY_TYPE;

struct elist
  {
  struct elist *next;
  label room;    /* room with problem */
  label nodeid;  /* nodeid of system  */
  };

