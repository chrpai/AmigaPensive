/*
** wwbbs_update.h 2.0 (15.9.94)
*/

struct UpdateMessage {
	struct Message um_Message;
	UBYTE um_Command;
	BYTE um_ID[33];
};
