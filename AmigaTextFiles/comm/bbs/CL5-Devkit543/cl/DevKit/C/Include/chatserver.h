#ifndef _CL_CHATSERVER_H
#define _CL_CHATSERVER_H

struct chatmsg {
	struct Message m;
	APTR	id;
	ULONG	cmd;
	char	data[ 512 ];
};

#define CM_LOGIN 1		// id is ServerPort, Data is Nickname
#define CM_LOGOFF 2		// leave chat
#define CM_DATA 3
#define CM_GETUSERLIST 4
#define CM_GETSERVERNAME 5
#define CM_BBSMSG 6
#define CM_NOTICE 7

#define CE_NICKINUSE 77	// Error NICK in Use
#define CE_INVNICK 78	// Error NICK invalid
#define CE_BANNED 79	// Error User banned

#define CPNAME "ChatServer.cmd"

#endif
