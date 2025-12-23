
/* FAME Door Defines
 *
 * Some additional and helpfull functions which doesn't need extra
 * doorcommands. Normally macros.
 *
 * $VER:FAMEDoorDefines.h v1.3
 *
 * Used tab size: 2
 */

#ifndef FAME_FAMEDOORDEFINES_H
#define FAME_FAMEDOORDEFINES_H

#include <FAME/FAMEDoorProto.h>

#define ClearFlagFiles(x) PutCommand("*",(x),0,0,NR_DeleteFlagFile);

/* Clears all file flags in (x) given conference.
 * x < 1 means current conference.
 */

#endif  /* FAME_FAMEDOORDEFINES_H */
