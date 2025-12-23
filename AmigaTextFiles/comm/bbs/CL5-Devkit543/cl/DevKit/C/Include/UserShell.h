/*
 * Interface: UserShell
 *
 * $RCSfile$
 * $Author$
 * $Revision$
 * $Date$
 * $State$
 * $Locker$
 *
 * $Log$
 */

#ifndef _UserShell_H
#define _UserShell_H

#include <cl_user.h>

typedef struct CLUserShell CLUserShell;

struct CLUserShell
{
	/*
	 * Login ok?
	 */
	short					login_valid;
	time_t					login_time;
	
	/*
	 * aktueller User
	 */
	struct User				user;
	struct UserProfile		userprofile;
	
	/*
	 * aktuelles Brett des Users
	 * '/' vorn = Brett, sonst User-PM
	 */
	char					currentboard[ 512 ];

	/*
	 *	Restzeit (kann sich ändern)
	 */

	time_t					rest_time;
};

#endif
