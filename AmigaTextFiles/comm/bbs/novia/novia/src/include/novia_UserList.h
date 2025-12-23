#ifndef NOVIA_USERLIST_H
#define NOVIA_USERLIST_H
#define NOVIA_USERLIST_H_VERSION "$VER: 0.07 (17.10.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

// V0.06 (31.07.1998) (c) 1996-1998 by Thorsten Gehler
// V0.07 (17.10.1998) (c) 1998 by Thorsten Gehler

#ifndef NOVIA_DATE_H
#include <novia/novia_date.h>
#endif

struct UserNode
{
	long 		IDNumber;		// 4
	long		IPNumber;		// 8
	ULONG		Seek;			// 12 Seek in Userdata
	BYTE		Status;			// 13
	char		Handle[21];		// 34 
	WORD		Emptybyte;		// 36

	ULONG		Access;			// 40
	ULONG		AccessGroup;	// 44
	char		RealName[41];	// 85
	char		Street[41];		// 126
	UWORD		reserverd;		// 128
	char		City[31];		// 159
	char		ZipCode[11];	// 170
	UWORD		CountryCode;	// 172
	char		Country[4];		// 176

	char		AreaCode[8];	// 188
	char 		Area[17];		// 205
	char		PhoneNo[25];	// 230
	char	 	ModemNo[25];	// 255
	char		FaxNo[25];		// 280
	ULONG		ComputerType;	// 284
	Date 		Birthday;	// 296
	Date		FirstCall;	// 308
	Date 		LastCall;	// 320
	Date 		ExpireDate;// 332
	UWORD		ExpireDays;		// 334

	UBYTE		Sex;			// 335
	UBYTE		Language;		// 336

	char		UUCP[31];		// 367
	char		Data_dir[31];	// 398

};

struct UserLoaded				// !!! new in V0.07 !!!
{
	struct		UserLoaded *ln_Succ;
	struct		UserLoaded *ln_Pred;
	struct		UserNode *unode;		// Pointer of UserNode
	struct		UserData *udata;		// Pointer of loaded UserData
	ULONG		OpenCounter;
};

struct UserList			// Resident Userlist for fast login  (!!! New Structure sience V0.06 !!!)
{
	long		UserEntrys;		// 4
	UserNode	*utable;		// 8
	BYTE		List_Status;	// 9 *** RESERVED FOR FUTURE ***
	BYTE		reserved[7];	// 16
};

#endif