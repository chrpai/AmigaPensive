
typedef struct fidoadr {
	ULONG zone;
	ULONG net;
	ULONG node;
	ULONG point;
} fidoadr;

#define PREFSID_FIDO MAKE_ID('O','F','I','D')

#define SUBID_FIDO_PREFS (0x8441)

typedef struct fidoprefs {
	char inbound[ 128 ];
	char outbound[ 128 ];
	char deforigin[ 80 ];
	char trapdoorcmd[ 128 ];
	char trapdooropts_dial[ 128 ];
	char trapdooropts_called[ 128 ];
} fidoprefs;

#define SUBID_FIDO_NETS (1) // ++

typedef struct fidonet {
	char name[ 32 ];
	char boardmask[ 128 ];
	char origin[ 80 ];
	fidoadr adr;
	fidoadr pointnet;
	UBYTE flg_binary_attached;
} fidonet;
