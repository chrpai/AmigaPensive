
#include <FAME/FAME.h>
#include <proto/dos.h>
#include <clib/dos_protos.h>
#include <dos/dos.h>
#include <pragmas/dos_pragmas.h>
#include <stdio.h>

/*
 * Disable Lattice CTRL-C handling:
 */

#ifdef LATTICE
int CXBRK( void ) { return( 0 ); }
int chkabort( void ) { return( 0 ); }
#endif

struct FAMELibrary				*FAMEBase			= NULL;
struct FAMEConfigRequest	MyFAMEConfigRequest;

char Buffer[200];

struct FAMESystem							*TmpsYSTEM						= NULL;
struct FAMEServer							*TmpsERVER						= NULL;
struct FAMEConf								*TmpcONF							= NULL;
struct FAMEConf								*TmpcONFTest					= NULL;
struct FAMELevels							*TmplEVELS						= NULL;
struct FAMEConfAccess					*TmpcONFaCCESS				= NULL;
struct FAMEPreset							*TmppRESET						= NULL;
struct FAMEActivities					*TmpaCTIVITIES				= NULL;
struct FAMEError							*TmpeRROR							= NULL;
struct FAMEDoors							*TmpdOORS							= NULL;
struct FAMEInternDoors				*TmpiNTERNdOORS				= NULL;
struct FAMEScreens						*TmpsCREENS						= NULL;
struct FAMENode								*TmpnODE							= NULL;
struct FAMESerial							*TmpsERIAL						= NULL;
struct FAMEModem							*TmpmODEM							= NULL;
struct FAMEScreen							*TmpsCREEN						= NULL;
struct FAMEChecker						*TmpcHECKER						= NULL;
struct FAMETextLang						*TmptEXTlANG					= NULL;
struct FAMELangLocale					*TmplANGlOCALE				= NULL;
struct FAMEDoorConfig					*TmpdOORcONFIG				= NULL;
struct FAMEUserHandler				*TmpuSERhANDLER				= NULL;
struct FAMEModules						*TmpfAMEmODULES				= NULL;
struct FAMEComputerTypes			*TmpcOMPUTERtYPES			= NULL;
struct FAMEModemTypes					*TmpmODEMtYPES				= NULL;
struct FAMEMisc								*TmpmISC							= NULL;
struct FAMEProtocol						*TmppROTOCOL					= NULL;
struct FAMEBaudTimes					*TmpbAUDtIMES					= NULL;
struct FAMEUserButtons				*TmpuSERbUTTONS				= NULL;
struct FAMEHydra							*TmpfAMEhYDRA					= NULL;
struct FAMEHydraNode					*TmpfAMEhYDRAnODE			= NULL;
struct FAMESModem							*TmpfAMEsMODEM				= NULL;
struct FAMEDebug							*TmpfAMEdEBUG					= NULL;
struct FAMEAwayMsg						*TmpaWAYmSG						= NULL;
//struct UserExpansion			*TmpUserExpansion			= NULL;
//struct UserConfExpansion	*TmpUserConfExpansion	= NULL;
struct FAMEUser								*TmpUser							= NULL;
struct FAMEUserKeys						*TmpUserKeys					= NULL;

struct FAMEUser								*TmpSUser							= NULL;
struct FAMEUser								*TmpS1User						= NULL;
struct FAMEUser								*TmpS2User						= NULL;
struct FAMEUser								*TmpS3User						= NULL;
struct FAMEUser								*TmpSUserUE						= NULL;
struct FAMEUser								*TmpSUserBL						= NULL;
struct FAMEUser								*TmpSUserSBL					= NULL;

struct FAMEUserConf						*TmpUserConf					= NULL;
struct FAMEUserConf						*TmpSUserConfSBL			= NULL;

void sYSTEM( long Flag );
void sERVER( long Flag );
void cONF( long Flag );
void cONFTest( long Flag );
void lEVELS( long Flag );
void cONFaCCESS( long Flag );
void pRESET( long Flag );
void aCTIVITIES( long Flag );
void eRROR( long Flag );
void dOORS( long Flag );
void iNTERNdOORS( long Flag );
void sCREENS( long Flag );
void nODE( long Flag );
void sERIAL( long Flag );
void mODEM( long Flag );
void sCREEN( long Flag );
void cHECKER( long Flag );
void tEXTlANG( long Flag );
void lANGlOCALE( long Flag );
void dOORcONFIG( long Flag );
void uSERhANDLER( long Flag );
void fAMEmODULES( long Flag );
void cOMPUTERtYPES( long Flag );
void mODEMtYPES( long Flag );
void mISC( long Flag );
void pROTOCOL( long Flag );
void bAUDtIMES( long Flag );
void uSERbUTTONS( long Flag );
void fAMEhYDRA( long Flag );
void fAMEhYDRAnODE( long Flag );
void fAMEsMODEM( long Flag );
void fAMEdEBUG( long Flag );
void aWAYmSG( long Flag );
//void UserExpansion( long Flag );
//void UserConfExpansion( long Flag );
void User( long Flag );
void UserKeys( long Flag );
void SearchUser( long Flag );
void SearchUser1( long Flag );
void SearchUser2( long Flag );
void SearchUser3( long Flag );
void SearchUserUnEqual( long Flag );
void SearchUserByLoc( long Flag );
void SearchUserStrByLoc( long Flag );

void UserConf( long Flag );
void SearchUserConfStrByLoc( long Flag );

void ShutDown( long ErrCode );

void PressAKey( void )
{
	Printf( "Press a key! (q to quit)" );
	Flush( Output( ));

	Flush( Input( ));
	gets( Buffer );

	if( Buffer[ 0 ] == 'q' || Buffer[ 0 ] == 'Q' )
	{
		ShutDown( 0L );
	}
}

void main( void )
{
	Printf( "\n" );

	if( FAMEBase = (struct FAMELibrary *)OpenLibrary( "FAME.library", 5L ))
	{
		FAMEMemSet( &MyFAMEConfigRequest, '\0', sizeof( struct FAMEConfigRequest ));

		MyFAMEConfigRequest . fcr_ConfigMode	= FAMECFG_MODE_READ;
		MyFAMEConfigRequest . fcr_CfgUserTask	= FindTask( NULL );
		MyFAMEConfigRequest . fcr_CfgUserName	= "FAMECfgTest";
		MyFAMEConfigRequest . fcr_CfgUserDesc	= "FAMEConfig system tester.";

		sYSTEM( 1 );
		sERVER( 1 );
		cONF( 1 );
		lEVELS( 1 );
		cONFaCCESS( 1 );
		pRESET( 1 );
		aCTIVITIES( 1 );
		eRROR( 1 );
		dOORS( 1 );
		iNTERNdOORS( 1 );
		sCREENS( 1 );
		nODE( 1 );
		sERIAL( 1 );
		mODEM( 1 );
		sCREEN( 1 );
		cHECKER( 1 );
		tEXTlANG( 1 );
		lANGlOCALE( 1 );
		dOORcONFIG( 1 );
		uSERhANDLER( 1 );
		fAMEmODULES( 1 );
		cOMPUTERtYPES( 1 );
		mODEMtYPES( 1 );
		mISC( 1 );
		pROTOCOL( 1 );
		bAUDtIMES( 1 );
		uSERbUTTONS( 1 );
		fAMEhYDRA( 1 );
		fAMEhYDRAnODE( 1 );
		fAMEsMODEM( 1 );
		fAMEdEBUG( 1 );
		aWAYmSG( 1 );
//		UserExpansion( 1 );
//		UserConfExpansion( 1 );
		User( 1 );
		UserKeys( 1 );

//		UserConf( 1 );

		Printf( "\nOK Now free...\n" );

		sYSTEM( 0 );
		sERVER( 0 );
		cONF( 0 );
		lEVELS( 0 );
		cONFaCCESS( 0 );
		pRESET( 0 );
		aCTIVITIES( 0 );
		eRROR( 0 );
		dOORS( 0 );
		iNTERNdOORS( 0 );
		sCREENS( 0 );
		nODE( 0 );
		sERIAL( 0 );
		mODEM( 0 );
		sCREEN( 0 );
		cHECKER( 0 );
		tEXTlANG( 0 );
		lANGlOCALE( 0 );
		dOORcONFIG( 0 );
		uSERhANDLER( 0 );
		fAMEmODULES( 0 );
		cOMPUTERtYPES( 0 );
		mODEMtYPES( 0 );
		mISC( 0 );
		pROTOCOL( 0 );
		bAUDtIMES( 0 );
		uSERbUTTONS( 0 );
		fAMEhYDRA( 0 );
		fAMEhYDRAnODE( 0 );
		fAMEsMODEM( 0 );
		fAMEdEBUG( 0 );
		aWAYmSG( 0 );
//		UserExpansion( 0 );
//		UserConfExpansion( 0 );
		User( 0 );
		UserKeys( 0 );
//		UserConf( 0 );

/*

		User( 1 );

		SearchUser( 1 );

		SearchUser1( 1 );
		SearchUser2( 1 );
		SearchUser3( 1 );

		SearchUserUnEqual( 1 );
		SearchUserByLoc( 1 );




		User( 0 );

		SearchUser( 0 );

		SearchUser1( 0 );
		SearchUser2( 0 );
		SearchUser3( 0 );

		SearchUserUnEqual( 0 );
		SearchUserByLoc( 0 );
*/
/*
		SearchUserUnEqual( 1 );
		SearchUserUnEqual( 0 );

		SearchUserByLoc( 1 );
		SearchUserByLoc( 0 );

		SearchUserStrByLoc( 1 );
		SearchUserStrByLoc( 0 );
*/
/*
		UserConf( 1 );
		SearchUserConfStrByLoc( 1 );

		UserConf( 0 );
		SearchUserConfStrByLoc( 0 );
*/
//cONFTest( 1 );
//cONFTest( 0 );

	}
	else
	{
		ShutDown( 101 );
	}

	Printf( "\n\n" );

	ShutDown( 0L );
}

void sYSTEM( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );


//MyFAMEConfigRequest . fcr_ConfigMode	= FAMECFG_MODE_MODIFY;


	MyFAMEConfigRequest . fcr_ConfigType = FCDD_System;

	if( Flag )
	{
		Printf( "Installing FCDD_System... " );

		TmpsYSTEM = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "BBSName:%s SysOpName:%s\n", TmpsYSTEM -> BBSName, TmpsYSTEM -> SysOpName );
	}
	else if( TmpsYSTEM )
	{
		Printf( "Removing FCDD_System... " );

		Result = FAMEReleaseConfig( TmpsYSTEM, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void sERVER( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Server;

	if( Flag )
	{
		Printf( "Installing FCDD_Server... " );

		TmpsERVER = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "IconifyLeftEdge:%ld(280) IconifyTopEdge:%ld(8) VisibleNodes:%ld(6)\n", TmpsERVER -> IconifyLeftEdge, TmpsERVER -> IconifyTopEdge, TmpsERVER -> VisibleNodes );
	}
	else if( TmpsERVER )
	{
		Printf( "Removing FCDD_Server... " );

		Result = FAMEReleaseConfig( TmpsERVER, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void cONF( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Conf;
	MyFAMEConfigRequest . fcr_ConfigNum  = 10;

	if( Flag )
	{
		Printf( "Installing FCDD_Conf... " );

		TmpcONF = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ConfName:%s ConfLocation:%s\n", TmpcONF -> ConfName, TmpcONF -> ConfLocation );
	}
	else if( TmpcONF )
	{
		Printf( "Removing FCDD_Conf... " );

		Result = FAMEReleaseConfig( TmpcONF, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void cONFTest( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Conf;
	MyFAMEConfigRequest . fcr_ConfigNum  = 33;

	if( Flag )
	{
		Printf( "Installing FCDD_Conf... " );

		TmpcONFTest = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ConfName:%s ConfLocation:%s\n", TmpcONFTest -> ConfName, TmpcONFTest -> ConfLocation );

		Printf( "\nResult: <%ld> ConfDataPtr: <%lx>", Result, TmpcONFTest );
	}
	else if( TmpcONFTest )
	{
		Printf( "Removing FCDD_Conf... " );

		Result = FAMEReleaseConfig( TmpcONFTest, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void lEVELS( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Levels;
	MyFAMEConfigRequest . fcr_ConfigNum  = 100;

	if( Flag )
	{
		Printf( "Installing FCDD_Levels... " );

		TmplEVELS = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "MaxRelogins:%ld Level:%ld\n", TmplEVELS -> MaxRelogins, TmplEVELS -> Level );
	}
	else if( TmplEVELS )
	{
		Printf( "Removing FCDD_Levels... " );

		Result = FAMEReleaseConfig( TmplEVELS, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void cONFaCCESS( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_ConfAccess;
	MyFAMEConfigRequest . fcr_ConfigStr  = "FAMEBeta";

	if( Flag )
	{
		Printf( "Installing FCDD_ConfAccess... " );

		TmpcONFaCCESS = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ConfAlias:%s\n", TmpcONFaCCESS -> ConfAlias );
	}
	else if( TmpcONFaCCESS )
	{
		Printf( "Removing FCDD_ConfAccess... " );

		Result = FAMEReleaseConfig( TmpcONFaCCESS, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void pRESET( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Preset;
	MyFAMEConfigRequest . fcr_ConfigNum  = 3;

	if( Flag )
	{
		Printf( "Installing FCDD_Preset... " );

		TmppRESET = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ConfAlias:%s TimeLimit:%ld\n", TmppRESET -> ConfAlias, TmppRESET -> TimeLimit );
	}
	else if( TmppRESET )
	{
		Printf( "Removing FCDD_Preset... " );

		Result = FAMEReleaseConfig( TmppRESET, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void aCTIVITIES( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Activities;

	if( Flag )
	{
		Printf( "Installing FCDD_Activities... " );

		TmpaCTIVITIES = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "CallLogNumLines:%ld UDLogNumLines:%ld\n", TmpaCTIVITIES -> CallLogNumLines, TmpaCTIVITIES -> UDLogNumLines );
	}
	else if( TmpaCTIVITIES )
	{
		Printf( "Removing FCDD_Activities... " );

		Result = FAMEReleaseConfig( TmpaCTIVITIES, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void eRROR( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Error;

	if( Flag )
	{
		Printf( "Installing FCDD_Error... " );

		TmpeRROR = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );
	}
	else if( TmpeRROR )
	{
		Printf( "Removing FCDD_Error... " );

		Result = FAMEReleaseConfig( TmpeRROR, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void dOORS( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Doors;
	MyFAMEConfigRequest . fcr_ConfigStr  = "FAME:Settings/Doors/CONF14CMD/TEST";

	if( Flag )
	{
		Printf( "Installing FCDD_Doors... " );

		TmpdOORS = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "DoorCallName:%s DoorPath:%s\n", TmpdOORS -> DoorCallName, TmpdOORS -> DoorPath );
	}
	else if( TmpdOORS )
	{
		Printf( "Removing FCDD_Doors... " );

		Result = FAMEReleaseConfig( TmpdOORS, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void iNTERNdOORS( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_InternDoors;
	MyFAMEConfigRequest . fcr_ConfigStr  = "";

	if( Flag )
	{
		Printf( "Installing FCDD_InternDoors... " );

		TmpiNTERNdOORS = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "Frontend:%ld WHO:%ld\n", TmpiNTERNdOORS -> Frontend, TmpiNTERNdOORS -> WHO );
	}
	else if( TmpiNTERNdOORS )
	{
		Printf( "Removing FCDD_InternDoors... " );

		Result = FAMEReleaseConfig( TmpiNTERNdOORS, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void sCREENS( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Screens;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;

	if( Flag )
	{
		Printf( "Installing FCDD_Screens... " );

		TmpsCREENS = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "PrivateMinNumLines:%ld PrivateMinBrkLvl:%ld\n", TmpsCREENS -> PrivateMinNumLines, TmpsCREENS -> PrivateMinBrkLvl );
	}
	else if( TmpsCREENS )
	{
		Printf( "Removing FCDD_Screens... " );

		Result = FAMEReleaseConfig( TmpsCREENS, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void nODE( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Node;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;

	if( Flag )
	{
		Printf( "Installing FCDD_Node... " );

		TmpnODE = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "NodeStPath:%s NamesNotAll:%s\n", TmpnODE -> NodeStPath, TmpnODE -> NamesNotAll );
	}
	else if( TmpnODE )
	{
		Printf( "Removing FCDD_Node... " );

		Result = FAMEReleaseConfig( TmpnODE, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void sERIAL( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Serial;
	MyFAMEConfigRequest . fcr_ConfigNum  = 2;

	if( Flag )
	{
		Printf( "Installing FCDD_Serial... " );

		TmpsERIAL = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "SerialDriver:%s InitBaud:%ld\n", TmpsERIAL -> SerialDriver, TmpsERIAL -> InitBaud );
	}
	else if( TmpsERIAL )
	{
		Printf( "Removing FCDD_Serial... " );

		Result = FAMEReleaseConfig( TmpsERIAL, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void mODEM( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Modem;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;

	if( Flag )
	{
		Printf( "Installing FCDD_Modem... " );

		TmpmODEM = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ModemInit:%s ModemRing:%s\n", TmpmODEM -> ModemInit, TmpmODEM -> ModemRing );
	}
	else if( TmpmODEM )
	{
		Printf( "Removing FCDD_Modem... " );

		Result = FAMEReleaseConfig( TmpmODEM, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void sCREEN( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Screen;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;

	if( Flag )
	{
		Printf( "Installing FCDD_Screen... " );

		TmpsCREEN = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "FontName:%s ScreenName:%s\n", TmpsCREEN -> FontName, TmpsCREEN -> ScreenName );
	}
	else if( TmpsCREEN )
	{
		Printf( "Removing FCDD_Screen... " );

		Result = FAMEReleaseConfig( TmpsCREEN, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void cHECKER( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Checker;
	MyFAMEConfigRequest . fcr_ConfigStr  = "LHA";

	if( Flag )
	{
		Printf( "Installing FCDD_Checker... " );

		TmpcHECKER = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "Extension:%s CheckerPath:%s\n", TmpcHECKER -> Extension, TmpcHECKER -> CheckerPath );
	}
	else if( TmpcHECKER )
	{
		Printf( "Removing FCDD_Checker... " );

		Result = FAMEReleaseConfig( TmpcHECKER, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void tEXTlANG( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_TextLang;
	MyFAMEConfigRequest . fcr_ConfigNum  = 2;

	if( Flag )
	{
		Printf( "Installing FCDD_TextLang... " );

		TmptEXTlANG = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "TxtLanType:%s Extension:%s\n", TmptEXTlANG -> TxtLanType, TmptEXTlANG -> Extension );
	}
	else if( TmptEXTlANG )
	{
		Printf( "Removing FCDD_TextLang... " );

		Result = FAMEReleaseConfig( TmptEXTlANG, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void lANGlOCALE( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_LangLocale;

	if( Flag )
	{
		Printf( "Installing FCDD_LangLocale... " );

		TmplANGlOCALE = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ServerLocale:%s SystemEdLocale:%s\n", TmplANGlOCALE -> ServerLocale, TmplANGlOCALE -> SystemEdLocale );
	}
	else if( TmplANGlOCALE )
	{
		Printf( "Removing FCDD_LangLocale... " );

		Result = FAMEReleaseConfig( TmplANGlOCALE, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void dOORcONFIG( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_DoorConfig;
	MyFAMEConfigRequest . fcr_ConfigStr  = "1";

	if( Flag )
	{
		Printf( "Installing FCDD_DoorConfig... " );

		TmpdOORcONFIG = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "DoorCnfName:%s DoorCnfPath:%s\n", TmpdOORcONFIG -> DoorCnfName, TmpdOORcONFIG -> DoorCnfPath );
	}
	else if( TmpdOORcONFIG )
	{
		Printf( "Removing FCDD_DoorConfig... " );

		Result = FAMEReleaseConfig( TmpdOORcONFIG, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void uSERhANDLER( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserHandler;
	MyFAMEConfigRequest . fcr_ConfigStr  = "Test";

	if( Flag )
	{
		Printf( "Installing FCDD_UserHandler... " );

		TmpuSERhANDLER = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "UserHandlerName:%s UserHandlerPath:%s\n", TmpuSERhANDLER -> UserHandlerName, TmpuSERhANDLER -> UserHandlerPath );
	}
	else if( TmpuSERhANDLER )
	{
		Printf( "Removing FCDD_UserHandler... " );

		Result = FAMEReleaseConfig( TmpuSERhANDLER, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void fAMEmODULES( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_FameModules;

	if( Flag )
	{
		Printf( "Installing FCDD_FameModules... " );

		TmpfAMEmODULES = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "MainPartLocation:%s FileCheckerLocation:%s\n", TmpfAMEmODULES -> MainPartLocation, TmpfAMEmODULES -> FileCheckerLocation );
	}
	else if( TmpfAMEmODULES )
	{
		Printf( "Removing FCDD_FameModules... " );

		Result = FAMEReleaseConfig( TmpfAMEmODULES, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void cOMPUTERtYPES( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_ComputerTypes;
	MyFAMEConfigRequest . fcr_ConfigNum  = 10;

	if( Flag )
	{
		Printf( "Installing FCDD_ComputerTypes... " );

		TmpcOMPUTERtYPES = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "CompiType:%s\n", TmpcOMPUTERtYPES -> CompiType );
	}
	else if( TmpcOMPUTERtYPES )
	{
		Printf( "Removing FCDD_ComputerTypes... " );

		Result = FAMEReleaseConfig( TmpcOMPUTERtYPES, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void mODEMtYPES( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_ModemTypes;
	MyFAMEConfigRequest . fcr_ConfigNum  = 8;

	if( Flag )
	{
		Printf( "Installing FCDD_ModemTypes... " );

		MyFAMEConfigRequest . fcr_ConfigMode = FAMECFG_MODE_MODIFY;

		TmpmODEMtYPES = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ModemType:%s\n", TmpmODEMtYPES -> ModemType );
	}
	else if( TmpmODEMtYPES )
	{
		Printf( "Removing FCDD_ModemTypes... " );

		Result = FAMEReleaseConfig( TmpmODEMtYPES, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	MyFAMEConfigRequest . fcr_ConfigMode = FAMECFG_MODE_READ;

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void mISC( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Misc;

	if( Flag )
	{
		Printf( "Installing FCDD_Misc... " );

		TmpmISC = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "XDoorPort:%s FAMEToolPort:%s\n", TmpmISC -> XDoorPort, TmpmISC -> FAMEToolPort );
	}
	else if( TmpmISC )
	{
		Printf( "Removing FCDD_Misc... " );

		Result = FAMEReleaseConfig( TmpmISC, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void pROTOCOL( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_Protocol;

	if( Flag )
	{
		Printf( "Installing FCDD_Protocol... " );

		TmppROTOCOL = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "XprLibrary:%s DefaultBuffer:%ld\n", TmppROTOCOL -> XprLibrary[ 0 ], TmppROTOCOL -> DefaultBuffer );
	}
	else if( TmppROTOCOL )
	{
		Printf( "Removing FCDD_Protocol... " );

		Result = FAMEReleaseConfig( TmppROTOCOL, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void bAUDtIMES( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_BaudTimes;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;
	MyFAMEConfigRequest . fcr_ConfigNum2 = 19200;
	MyFAMEConfigRequest . fcr_ConfigStr  = "1"; // Use higer one if 19200 not found

	if( Flag )
	{
		Printf( "Installing FCDD_BaudTimes... " );

		TmpbAUDtIMES = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "BaudRate:%ld StartHH:%ld\n", TmpbAUDtIMES -> BaudRate, TmpbAUDtIMES -> StartHH );
	}
	else if( TmpbAUDtIMES )
	{
		Printf( "Removing FCDD_BaudTimes... " );

		Result = FAMEReleaseConfig( TmpbAUDtIMES, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void uSERbUTTONS( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserButtons;

	if( Flag )
	{
		Printf( "Installing FCDD_UserButtons... " );

		TmpuSERbUTTONS = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ButtonName:%s ButtonCommand:%s\n", TmpuSERbUTTONS -> ButtonName[ 1 ], TmpuSERbUTTONS -> ButtonCommand[ 1 ] );
	}
	else if( TmpuSERbUTTONS )
	{
		Printf( "Removing FCDD_UserButtons... " );

		Result = FAMEReleaseConfig( TmpuSERbUTTONS, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void fAMEhYDRA( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_FameHydra;

	if( Flag )
	{
		Printf( "Installing FCDD_FameHydra... " );

		TmpfAMEhYDRA = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "WriteLogFile:%ld RememberWndPos:%ld\n", TmpfAMEhYDRA -> WriteLogFile, TmpfAMEhYDRA -> RememberWndPos );
	}
	else if( TmpfAMEhYDRA )
	{
		Printf( "Removing FCDD_FameHydra... " );

		Result = FAMEReleaseConfig( TmpfAMEhYDRA, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void fAMEhYDRAnODE( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_FameHydraNode;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;

	if( Flag )
	{
		Printf( "Installing FCDD_FameHydraNode... " );

		TmpfAMEhYDRAnODE = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "ReceiveWndLeft:%ld SendWndLeft:%ld\n", TmpfAMEhYDRAnODE -> ReceiveWndLeft, TmpfAMEhYDRAnODE -> SendWndLeft );
	}
	else if( TmpfAMEhYDRAnODE )
	{
		Printf( "Removing FCDD_FameHydraNode... " );

		Result = FAMEReleaseConfig( TmpfAMEhYDRAnODE, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void fAMEsMODEM( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_FameSmodem;

	if( Flag )
	{
		Printf( "Installing FCDD_FameSmodem... " );

		TmpfAMEsMODEM = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "FallBackPubScreen:%s RTSCTS:%ld\n", TmpfAMEsMODEM -> FallBackPubScreen, TmpfAMEsMODEM -> RTSCTS );
	}
	else if( TmpfAMEsMODEM )
	{
		Printf( "Removing FCDD_FameSmodem... " );

		Result = FAMEReleaseConfig( TmpfAMEsMODEM, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void fAMEdEBUG( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_FameDebug;

	if( Flag )
	{
		Printf( "Installing FCDD_FameDebug... " );

		TmpfAMEdEBUG = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "FIMCmdDataLog:%s XIMCmdDataLog:%s\n", TmpfAMEdEBUG -> FIMCmdDataLog, TmpfAMEdEBUG -> XIMCmdDataLog );
	}
	else if( TmpfAMEdEBUG )
	{
		Printf( "Removing FCDD_FameDebug... " );

		Result = FAMEReleaseConfig( TmpfAMEdEBUG, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void aWAYmSG( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_AwayMsg;

	if( Flag )
	{
		Printf( "Installing FCDD_AwayMsg... " );

		TmpaWAYmSG = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "Msg1:%s Msg5:%s\n", TmpaWAYmSG -> Msg1, TmpaWAYmSG -> Msg5 );
	}
	else if( TmpaWAYmSG )
	{
		Printf( "Removing FCDD_AwayMsg... " );

		Result = FAMEReleaseConfig( TmpaWAYmSG, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

/*void UserExpansion( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserExpansion;

	if( Flag )
	{
		Printf( "Installing FCDD_UserExpansion... " );

		TmpUserExpansion = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "\n" );
	}
	else if( TmpUserExpansion )
	{
		Printf( "Removing FCDD_UserExpansion... " );

		Result = FAMEReleaseConfig( TmpUserExpansion, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void UserConfExpansion( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserConfExpansion;

	if( Flag )
	{
		Printf( "Installing FCDD_UserConfExpansion... " );

		TmpUserConfExpansion = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "\n" );
	}
	else if( TmpUserConfExpansion )
	{
		Printf( "Removing FCDD_UserConfExpansion... " );

		Result = FAMEReleaseConfig( TmpUserConfExpansion, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}
*/

void User( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_User;
	MyFAMEConfigRequest . fcr_ConfigNum  = 11;

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpUser = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "UserName:%s UserLocation:%s\n", TmpUser -> UserName, TmpUser -> UserLocation );
	}
	else if( TmpUser )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpUser, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void UserKeys( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserKeys;
	MyFAMEConfigRequest . fcr_ConfigNum  = 11;

	if( Flag )
	{
		Printf( "Installing FCDD_UserKeys... " );

		TmpUserKeys = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		Printf( "UserName:%s NewUser:%ld\n", TmpUserKeys -> UserName, TmpUserKeys -> NewUser );
	}
	else if( TmpUserKeys )
	{
		Printf( "Removing FCDD_UserKeys... " );

		Result = FAMEReleaseConfig( TmpUserKeys, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUser( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_User;

	MyFAMEConfigRequest . fcr_SearchOffset		= 0;
	MyFAMEConfigRequest . fcr_SearchValue			= "Siegel/tRSi";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_EQUAL; // FAMECFGOP_GREATEREQUAL

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpSUser = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpSUser )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpSUser -> UserName, TmpSUser -> UserLocation );
		}
	}
	else if( TmpSUser )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpSUser, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUser1( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_User;
	MyFAMEConfigRequest . fcr_ConfigNum  = 1;

	MyFAMEConfigRequest . fcr_SearchOffset		= 0;
	MyFAMEConfigRequest . fcr_SearchValue			= "S";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_GREATEREQUAL; //FAMECFGOP_EQUAL; // FAMECFGOP_GREATEREQUAL

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpS1User = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpS1User )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpS1User -> UserName, TmpS1User -> UserLocation );
		}
	}
	else if( TmpS1User )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpS1User, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUser2( long Flag )
{
	long Result;

	MyFAMEConfigRequest . fcr_ConfigType	= FCDD_User;
	MyFAMEConfigRequest . fcr_ConfigNum		++;
	MyFAMEConfigRequest . fcr_ConfigNum2	= 0L;
	MyFAMEConfigRequest . fcr_ConfigStr		= NULL;

	MyFAMEConfigRequest . fcr_SearchOffset		= 0;
	MyFAMEConfigRequest . fcr_SearchValue			= "s";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_GREATEREQUAL; //FAMECFGOP_EQUAL; // FAMECFGOP_GREATEREQUAL

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpS2User = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpS2User )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpS2User -> UserName, TmpS2User -> UserLocation );
		}
	}
	else if( TmpS2User )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpS2User, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUser3( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType	= FCDD_User;
	MyFAMEConfigRequest . fcr_ConfigNum		++;
	MyFAMEConfigRequest . fcr_ConfigNum2	= 0L;
	MyFAMEConfigRequest . fcr_ConfigStr		= NULL;

	MyFAMEConfigRequest . fcr_SearchOffset		= 0;
	MyFAMEConfigRequest . fcr_SearchValue			= "S";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_GREATEREQUAL; //FAMECFGOP_EQUAL; // FAMECFGOP_GREATEREQUAL

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpS3User = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpS3User )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpS3User -> UserName, TmpS3User -> UserLocation );
		}
	}
	else if( TmpS3User )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpS3User, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUserUnEqual( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_User;

	MyFAMEConfigRequest . fcr_SearchOffset		= 0;
	MyFAMEConfigRequest . fcr_SearchValue			= "Strider/trsi";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_UNEQUAL; // FAMECFGOP_GREATEREQUAL

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpSUserUE = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpSUserUE )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpSUserUE -> UserName, TmpSUserUE -> UserLocation );
		}
	}
	else if( TmpSUserUE )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpSUserUE, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUserByLoc( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_User;

	MyFAMEConfigRequest . fcr_SearchOffset		= 54;
	MyFAMEConfigRequest . fcr_SearchValue			= "Syndicate";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_EQUAL; // FAMECFGOP_GREATEREQUAL

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpSUserBL = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpSUserBL )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpSUserBL -> UserName, TmpSUserBL -> UserLocation );
		}
	}
	else if( TmpSUserBL )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpSUserBL, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUserStrByLoc( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_User;

	MyFAMEConfigRequest . fcr_SearchOffset		= 54;
	MyFAMEConfigRequest . fcr_SearchValue			= "receiver";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_MIDSTR;

	if( Flag )
	{
		Printf( "Installing FCDD_User... " );

		TmpSUserBL = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpSUserBL )
		{
			Printf( "UserName:%s UserLocation:%s\n", TmpSUserBL -> UserName, TmpSUserBL -> UserLocation );
		}
	}
	else if( TmpSUserBL )
	{
		Printf( "Removing FCDD_User... " );

		Result = FAMEReleaseConfig( TmpSUserBL, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void UserConf( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserConf;
	MyFAMEConfigRequest . fcr_ConfigNum  = 34;
	MyFAMEConfigRequest . fcr_ConfigNum2 = 7;

	if( Flag )
	{
		Printf( "Installing FCDD_UserConf... " );

		TmpUserConf = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpUserConf )
		{
			Printf( "UserName:%s ConfJoins:%ld\n", TmpUserConf -> UserName, TmpUserConf -> ConfJoins );
		}
	}
	else if( TmpUserConf )
	{
		Printf( "Removing FCDD_UserConf... " );

		Result = FAMEReleaseConfig( TmpUserConf, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void SearchUserConfStrByLoc( long Flag )
{
	long Result;

	FAMERequestReset( &MyFAMEConfigRequest );

	MyFAMEConfigRequest . fcr_ConfigType = FCDD_UserConf;
	MyFAMEConfigRequest . fcr_ConfigNum2 = 7;

	MyFAMEConfigRequest . fcr_SearchOffset		= 0;
	MyFAMEConfigRequest . fcr_SearchValue			= "ld th";
	MyFAMEConfigRequest . fcr_SearchType			= FAMECFGVARTYPE_STRING;
	MyFAMEConfigRequest . fcr_SearchOperator	= FAMECFGOP_MIDSTR;

	if( Flag )
	{
		Printf( "Installing FCDD_UserConf... " );

		TmpSUserConfSBL = FAMEObtainConfig( &MyFAMEConfigRequest, &Result );

		if( TmpSUserConfSBL )
		{
			Printf( "UserName:%s ConfJoins:%ld\n", TmpSUserConfSBL -> UserName, TmpSUserConfSBL -> ConfJoins );
		}
	}
	else if( TmpSUserConfSBL )
	{
		Printf( "Removing FCDD_UserConf... " );

		Result = FAMEReleaseConfig( TmpSUserConfSBL, FAMECFG_MODE_READ, FindTask( NULL ));
	}

	if( Result )
	{
		ShutDown( Result );
	}

	PressAKey( );
}

void ShutDown( long ErrCode )
{
	long TheIoErr = 0L;

	if( ErrCode )
	{
		switch( ErrCode )
		{
			case 101:

				Printf( "FAME.library open fail." );

				TheIoErr	= ERROR_INVALID_RESIDENT_LIBRARY;
				ErrCode		= RETURN_FAIL;

			break;
			default:

				FAMEHandleConfigResults( Buffer, ErrCode, MyFAMEConfigRequest . fcr_ConfigType );

				Printf( Buffer );

				TheIoErr	= 0L;
				ErrCode		= RETURN_ERROR;

			break;
		}
	}
	else
	{
		Printf( "Success" );

		TheIoErr	= 0L;
		ErrCode		= RETURN_OK;
	}

	if( FAMEBase )
	{
		CloseLibrary( (struct Library *)FAMEBase );

		FAMEBase = NULL;
	}

	Printf( "\n\n" );

	SetIoErr( TheIoErr );

	PrintFault( IoErr( ), "FAMECfgTest" );

	exit( ErrCode );
}

/*
extern long __asm FAMEReloadConfig(        register __a0 APTR   ConfigData,
                                           register __d0 ULONG  Flags );
extern long __asm FAMESaveConfig(          register __a0 APTR   ConfigData,
                                           register __d0 long   Modify );
extern long __asm FAMESaveAllConfig(       register __d0 long   SpecialCase );
extern long __asm FAMELockConfig(          register __a0 APTR   ConfigData );
extern long __asm FAMEAttemptLockConfig(   register __a0 APTR   ConfigData );
extern long __asm FAMEUnLockConfig(        register __a0 APTR   ConfigData );
*/

