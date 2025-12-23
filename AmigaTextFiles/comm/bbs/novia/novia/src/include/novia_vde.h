#ifndef NOVIA_VDE_H
#define NOVIA_VDE_H
#define NOVIA_VDE_H_VERSION "$VER: 0.01 (08.05.1998)"
// (c) Copyright 1996-1998 by Thorsten Gehler
// All rights reserved. Read license.readme for copyright informations.

#ifndef NOVIA_TYPES_H
#include "novia/novia_types.h"
#endif

struct VDE_Item{
	VDE_Item	*vde_Succ;	// 4
	VDE_Item	*vde_Pred;	// 8
	UWORD	ItemNumber;		// 10
	char	X;				// 11
	char	Y;				// 12
	UBYTE	Item_Type1;		// 13
	UBYTE	Item_Type2;		// 14
	char	ItemName[25];	// 39
	char	Fieldlen;		// 40
	ULONG	Offset;			// 44
	ULONG	min;			// 48
	ULONG	max;			// 52
};

struct VDE_Menu{
	VDE_Menu	*vde_Succ;
	VDE_Menu	*vde_Pred;
	VDE_Item	*first_Item;
	VDE_Item	*last_Item;
	UWORD		MenuNumber;
	UWORD		VDE_Items;
	UWORD		Spalten;
};

struct VDE_Head{
	VDE_Menu	*first_Menu;
	VDE_Menu	*last_Menu;
	UWORD		MenuEntrys;
	UWORD		ItemEntrys;
};

#endif

