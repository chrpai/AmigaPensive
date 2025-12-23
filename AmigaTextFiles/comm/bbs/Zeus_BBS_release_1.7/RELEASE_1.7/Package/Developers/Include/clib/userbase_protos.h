#ifndef  CLIB_USERBASE_PROTOS_H
#define  CLIB_USERBASE_PROTOS_H

/*
** $VER: userbase_protos.h 37.0 (21.02.95)
**
** C prototypes for Zeus' userbase.library
**
** (C) Copyright 1992-1995 Zeus Developments.
**     Written by Alex May.
**     All rights reserved
*/

long ubDeleteUser( UBYTE * );
void ubEndUserKey( struct UserKey * );
BOOL ubFindUserIdx( UBYTE *, struct UserIdx * );
long ubInitialise( struct SharedData * );
struct UserKey *ubInitUserKey( UWORD );
BOOL ubLoadUser( UBYTE *, struct User * );
BOOL ubLoadUserFromKey( struct UserKey *, struct User * );
BOOL ubNextUserKey( struct UserKey * );
long ubQuit( void );
long ubSaveUser( struct User * );

#endif 	/* CLIB_USERBASE_PROTOS_H */
