/***********************************************************
 *
 * ConnectLine: Gruppen für Zugriffsrechte
 *
 * Verwalter: Michael Balzer
 * 
 * $RCSfile: Group.h $
 * $Revision: 1.1 $
 * $Date: 1994/02/19 17:05:14 $
 *
 * $Author: balzer $
 * $Locker: balzer $
 * $State: Exp $
 *
 * $Log: Group.h $
 * Revision 1.1  1994/02/19  17:05:14  balzer
 * Initial revision
 *
 */

#ifndef _CL_Group_H
#define _CL_Group_H

#include <exec/types.h>
#include <cl/cl_prefs.h>


struct CLGroup
{
	char	Name[16];
};


struct CLGroupList
{
	struct CLGroup Group[256];
};


#define PREFSID_GROUPLIST MAKE_ID('M','P','G','L')


#endif
