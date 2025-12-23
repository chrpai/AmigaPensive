/*
 *				Affsys.C
 *
 * Automatic File Forwarding system dependent code.
 */

/*
 *				History
 *
 * 90May15 HAW Created.
 */

/*
 *				Contents
 *
 *	SetBuffer()		Figures out buffer contents
 *	MSDOSparse()		Parses a filename for drive and path
 */

#include "ctdl.h"


void MSDOSparse(char *theDir, char *drive);
/*
 * SetBuffer()
 *
 * This function figures out where to find files to send to another system
 * and sets up internal buffer of fl_send appropriately.
 */
void SetBuffer(char *name, struct fl_send *sendWhat)
{
    strCpy(sendWhat->snArea.naDirname, name);
}
