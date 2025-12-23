#ifndef GUI_LIB_H
#define GUI_LIB_H

APTR LockWindow( struct Window * );
void UnLockWindow( APTR );
Class *InitGetFile( void );
BOOL NodeUp( struct List *, struct Node * );
BOOL NodeDown( struct List *, struct Node * );

#endif
