#ifndef NODES_H
#define NODES_H
/*
** Extra macros for use with Exec lists and Nodes.
**
** Written by Alex May 06-Sep-94
**
** Public domain.
**
*/

/*
** Include list header if it hasn't been already.  Defines two macros:
**
** IsListEmpty & IsMsgPortEmpty
*/

#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif

/*
** GetHead -- Returns the first node in a list.
**            Returns NULL if list is empty
**
** node = GetHead( list );
**
** struct Node *GetHead( struct List * );
*/

#define GetHead(l) \
	((APTR)( ( (l)->lh_TailPred ) == (struct Node *)(l) ? NULL : ( (l)->lh_Head ) ))

/*
** GetTail -- Returns the last node in a list.
**            Returns NULL if list is empty
**
** node = GetTail( list );
**
** struct Node *GetTail( struct List * );
*/

#define GetTail(l) \
	((APTR)( ( (l)->lh_TailPred ) == (struct Node *)(l) ? NULL : ( (l)->lh_TailPred ) ))

/*
** GetSucc -- Returns the next node after the given one.
**            Returns NULL if this is the last node in the list.
**
** node = GetSucc( node );
**
** struct Node *GetSucc( struct Node * );
*/

#define GetSucc(n) \
	((APTR)( ( (((struct Node *)(n))->ln_Succ)->ln_Succ ) ? ( ((struct Node *)(n))->ln_Succ ) : NULL ))

/*
** GetPred -- Returns the node before the given one.
**            Returns NULL if this is the first node in the list.
**
** node = GetPred( node );
**
** struct Node *GetPred( struct Node * );
*/

#define GetPred(n) \
	((APTR)( ( (((struct Node *)(n))->ln_Pred)->ln_Pred ) ? ( ((struct Node *)(n))->ln_Pred ) : NULL ))

#endif /* NODES_H */
