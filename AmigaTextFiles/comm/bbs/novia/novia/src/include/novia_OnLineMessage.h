#ifndef NOVIA_ONLINEMESSAGE_H
#define NOVIA_ONLINEMESSAGE_H
#define NOVIA_ONLINEMESSAGE_H_VERSION "$VER: 0.01 (08.05.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

struct OnLineMessage
{
		long	ToUser;									// 4
		long	FromUser;								// 8
		struct	Date WriteDate;							// 20
		char	*data;
		byte	File_OLM,	/* if > 0, Data=Filename */
				OLM_type,	/* normal OLM, MEGA-OLM */
				Msg_type;	/* SYSTEM REQUEST, ON-LINE-MESSAGE */

};

#endif
