/**************************************************************************
 *                    CNetMultiPort function prototypes                   *
 **************************************************************************/
struct	CnmpCluster	*InitCNMP				( struct PortData *z, char *BaseName, LONG PVSize);
struct	MsgPort		*CreateCNMPMsgPort	( char *PortName );
struct	RemotePort	*PortFromMsg			( struct CNMPMsg *CNMPMessage, struct CnmpCluster *cluster);
struct	RemotePort	*PortFromNum			( short InPort, struct CnmpCluster *cluster );
struct	RemotePort	*PortFromName			( char *username, struct CnmpCluster *cluster, struct MainPort *myp);
struct	CNMPMsg		*GetCNMPMsg				( LONG FromPort, struct CnmpCluster *cluster );
struct	CNMPMsg		*GetNTReplyMsg			( LONG FromPort, struct CnmpCluster *cluster );
void						FindCNMPorts			( char *BasePort, struct CnmpCluster *cluster, BOOL PORTPERMISSION);
BOOL						InitTimer				( struct CnmpCluster *cluster);
BOOL						SetTimeout				( long s, long m, struct CnmpCluster *cluster);
LONG						WaitEvents				( ULONG Rexx_Mask, struct CnmpCluster *cluster );
void						CleanupTimeout			( struct CnmpCluster *cluster );
struct	RemotePort	*CheckCNMPExists		( int PortNum, struct CnmpCluster *cluster );
BOOL						SendCNMPMessage		( struct CnmpCluster *cluster, char *portrange, struct CNMPMsg *Message, BOOL EchoLocal, BOOL OverrideCheck);
void						FreeCNMPList			( struct CnmpCluster *cluster );
struct	RemotePort	*CNMPortToList			( struct CnmpCluster *cluster, int portnum, char *PortName );
void						NotifyCNMPRemove		( struct CnmpCluster *cluster );
BOOL						CarrierCheck			( struct PortData *z );
void						KBFlush					( struct PortData *z );
LONG						HandleIO					( ULONG Rexx_Mask, struct CnmpCluster *cluster );
struct	CNMPMsg		*FileUserMsg			( struct CNMPMsg *UMSG, struct CnmpCluster *cluster);
void						FreeUserMsgs			( struct CnmpCluster *cluster );
void						DisposeCNMPMsg			( struct CNMPMsg *killmsg, struct CnmpCluster *cluster );
BOOL						DisposeCNMPType		( LONG CNMP_TYPE, LONG PortNum, struct CnmpCluster *cluster );
BOOL						RemoveCNMPortList		( struct RemotePort *remport, struct CnmpCluster *cluster );
void						PermitEntry				( struct CNMPMsg *rmsg, char *echostr, struct CnmpCluster *cluster );
void						HandleNT_REPLYMSG		( struct CNMPMsg *ntmsg );
BOOL						GetPermission			( long PortNum, struct CnmpCluster *cluster );
void						RefuseEntry				( struct CNMPMsg *rmsg, char *echostr, struct CnmpCluster *cluster );
struct	CnmpCluster	*InitCnmpCluster		( LONG PVSize );
void						FreeCnmpCluster		( struct CnmpCluster *cluster, LONG PVSize );
