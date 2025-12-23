/*
** wwbbs_commands.h 2.0 (11.8.94)
*/

struct CommandMessage {
	struct Message cm_Message;
	UBYTE cm_Command;
	ULONG cm_Data;
};
