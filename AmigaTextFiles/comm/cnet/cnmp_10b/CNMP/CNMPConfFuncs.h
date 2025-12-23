/************************************************************************
 *                        CNet Function protos                          *
 ************************************************************************/
void		CallHost( UBYTE c );
void		ShutDown( char *spawn );
void		GetOut( char *text, short errornum );
void		LoadError( void );


/**************************************************************************
 *                              ARexx Protos                              *
 **************************************************************************/
void HandleRexx( void );


/**************************************************************************
 * Functions not needed by CnetMultiPort but required for the
 * mini-conference
 **************************************************************************/
void	DoHelp					( void );
BOOL	CheckDigits				( char *str );
void	RemoveLeadingSpace	( char *str );
void	KickUser					( struct RemotePort *TempRemotePort );
void	OlmCheck					( void );
BOOL	FileExists				( char *Name );
