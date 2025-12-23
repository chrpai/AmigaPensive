extern struct MsgPort  *replyp; /* Some communication details ...       */
extern struct MainPort *myp;
extern struct PortData *z;
extern struct Library  *CNetBase;
extern struct CMessage cmess;
extern struct CPort    *cport;

void CallHost( UBYTE c );
void ShutDown( char *spawn );
void GetOut( void );
void LoadError( void );
