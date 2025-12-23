#ifndef NOVIA_MESSAGE_H
#define NOVIA_MESSAGE_H
#define NOVIA_MESSAGE_H_VERSION "$VER: 0.08 (07.09.1998)"
// (c) Copyright 1996-1999 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.
// VERSION: 0.07 (18.07.1998)
// VERSION: 0.08 (07.09.1998)
// VERSION: 0.09 (26.09.1998)

#ifndef NOVIA_DATE_H
#include <novia/novia_date.h>
#endif


struct message
{
	struct 	Message msg;
	struct	message *ln_Succ;
	struct	message *ln_Pred;
	ULONG			IDNumber;
	ULONG			IPNumber;		// serial number of message
	ULONG			PortID;
	ULONG			msg_type;
	ULONG			cmd_type;		// if sending a command
	APTR			data;
	ULONG			data_len;		// length of data
};

struct EventMessage
{
	struct 	Message msg;
	ULONG			msg_type;
	APTR			data;
};

#define EVENTMSGTYPE_QUIT		1000

struct MsgList
{
	struct message *first;
	struct message *last;
	ULONG  entrys;
};

#define OLM_TYPE_NORMAL			1
#define OLM_TYPE_BROADCAST		2
#define OLM_TYPE_MAGAOLM		3
#define OLM_TYPE_FILEOLM		4

#define OLM_TYPE_CHATREQUEST	10
#define OLM_TYPE_CHATACCEPT		11
#define OLM_TYPE_CHATREJECT		12
#define OLM_TYPE_CHATFAILAT		13
#define OLM_TYPE_CHATBREAK		14

#define OLM_TYPE_SYSTEMMESSAGE	100

struct OnLineMessage
{
	ULONG		ToUser;			// 4
	ULONG		FromUser;		// 8
	struct		Date WriteDate;	// 20
	ULONG		OLM_type;		/* 24 normal OLM, MEGA-OLM, FILE-OLM etc. */
	ULONG		data_len;		// 28
	char		data[1];		// 32
};

struct ChatMessage
{
	ULONG		ToUser;
	ULONG		FromUser;
	ULONG		data_len;
	char		data[1];
};


#define RCMSG_LOGIN						1
#define RCMSG_LOGIN_REPLY_OK			3
#define RCMSG_LOGIN_REPLY_FAILAT		5
#define RCMSG_TEXT						10
#define RCMSG_DUMP						20
#define RCMSG_LOGOFF					21
#define RCMSG_LOGOFF_OK					22
#define RCMSG_CHANGEROOM				30
#define RCMSG_CHANGEROOM_OK				31
#define RCMSG_CHANGEROOM_FAILAT			32

struct NRCServer
{
	ULONG	IDNumber;
	ULONG	IPNumber;
	struct 	Task *nrctask;
	struct	List *roomlist;
	ULONG	rooms;
	ULONG	users;
	ULONG	localusers;
	struct	MsgPort *controlport;
	struct	MsgPort	*nrcport;
	char	ServerOpen;
	char	ServerClose;
};

struct RCMessage
{
	struct RoomUser *user;
	ULONG		msgtype;
	ULONG		ToUser;			// RoomUserID	if RCMSG_LOGIN_REPLY ToUser   = your UserID
	ULONG		FromUser;		// RoomUserID 	if RCMSG_LOGIN_REPLY FromUser = users in first Room
	UWORD		ToRoom;			//				if RCMSG_LOGIN_REPLY ToRoom   = actual Room
	UWORD		FromRoom;
	ULONG		Level;			// 
	char		flag_internal;
	char		flag_private;	// one user message
	char		flag_broadcast;	// to all rooms
	char		flag_sysmsg;	// systemmessage.. User ??? has login... etc.
	char		flag_anonymous; // anonymous message
	char		flag_realname;	// realname
	char		flag_ircchannel;// from InternetRelayChat
	char		flag_res1;
	char		FromName[40];
	char		ToName[40];
	char		FromRealName[40];
	char		ToRealName[40];

	APTR		aptrdata;		// for control-messages etc.
	ULONG		data_len;
	char		data[1];
};

struct RCMessage2
{
	struct Message msg;
	struct RCMessage rcmsg;
};


struct RoomMember
{
	struct RoomMember *ln_Succ;
	struct RoomMember *ln_Pred;
	ULONG  UserID;
};

struct Member
{
	struct Member *ln_Succ;
	struct Member *ln_Pred;
	ULONG  UserID;
};

struct Room
{
	struct	Room *ln_Succ;		// 4
	struct	Room *ln_Pred;		// 8
	UWORD	RoomID;				// 10
	UWORD	RoomIP;				// 12 serial ip number
	struct	MinList userlist;	// 24 acutal Users in this Room
	struct  MinList Memberlist;	// 36   
	struct  MinList Grouplist;	// 48 Groups to entry in this Room
	ULONG	users;				// 52 localusers & irc users
	ULONG	localusers;			// 56
	ULONG	members;			// 60 reserved
	ULONG	maxusers;			// 64 max local & irc users. Set to 0 for no max
	ULONG   maxlocalusers;		// 68
	ULONG	IRC_IP;				// 72 Internet Relay Chat IP-Address		Changed in V0.09 IRCIP -> IRC_IP
	ULONG	IRC_Port;			// 76 IRC-Port Number						Changed in V0.09 IRCChannel -> IRC_Port
	char	RoomName[40];		// 116
	char	password[20];		// 136 EntryPassword
	char	entrypt[20];		// 156 Entrypt PGP-Private Key!!!
	char	flag_internal;		// 157 NRC-Chat, no IRC Chat
	char	flag_membersonly;	// 158 Private Chat, User must be added in Memberslist
	char	flag_usepw;			// 159 Use Password
	char	flag_broadcast;		// 160
	char	flag_sysmsg;		// 161
	char	flag_anonymous;		// 162
	char	flag_realnames;		// 163
	char	flag_ircchannel;	// 164
	char	flag_openircatstart;// 165
	char	flag_reserved;		// 165
	char	res34[34];			// 199
};

struct RoomUser
{
	struct	RoomUser *ln_Succ;
	struct	RoomUser *ln_Pred;
	struct	MsgPort	 *userport;
	struct	Room	 *room;
	struct	PortData *cport;
	ULONG	Level;					// Level;
	char	UserName[40];
	char	RealName[40];
	char	flag_anonyous;
	char	flag_broadcast;
	char 	flag_quit;
};

#endif
