/* telnet.h - structures/variables used by CNet's telnet client */


// TCP includes containing typedefs & "struct sockaddr_in"
#include "netinclude:sys/types.h"
#include "netinclude:netinet/in.h"

// each host entry in the sysdata:bbs.tnhost file is parsed and entered into
// list of TNHostEntry structures.
struct TNHostEntry
	{
	char he_system[40];			// 0   - system name (VDE)
	char he_ip[40];				// 40  - pre-defined hostname or IP (VDE)
	int he_port;					// 80  - telnet port number (VDE)
	char he_data[40];				// 84  - additional/optional text (VDE)
	LONG he_Access;				// 124 - access groups allowed to see/use host (VDE)
	LONG he_Flags;					// 128 - user must have these Host flags (VDE)
	struct sockaddr_in he_sin;	// ??? - copy of the socket/address info after successful connect. (dynamic remote query)
	LONG he_status;				// ??? - status of host (ie. &128 = killed)
	BYTE he_expansion[100];			// future expansion
	struct TNHostEntry *next;	// 132 - pointer to next TNHostEntry (dynamic)
	};
