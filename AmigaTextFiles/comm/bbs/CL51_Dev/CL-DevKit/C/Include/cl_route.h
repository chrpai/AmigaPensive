#ifndef INCLUDE_CL_ROUTE_H
#define INCLUDE_CL_ROUTE_H

#include <exec/types.h>

#define CLR_VERSION 7

// Längen der Strings in struct RoutingInfo

#define CLR_ROUTSYSTEMLEN 64    // maximale Stringlänge des Zielsystems
#define CLR_NEWDESTLEN 256      // maximale Stringlänge aufgelöste Adresse


/* Errorcodes CLR_GetRoute();  */

#define CLRE_ERROR_UNKNOWN          1   // sollte eigentlich nicht passieren -> Fehler in der Routingroutine
#define CLRE_NO_PERMISSION          2
#define CLRE_MAIL_TOO_LONG          3
#define CLRE_DESTINATION_UNKNOWN    4   // wenn eigene Box Domainserver und das System unbekannt
#define CLRE_TOO_EXPENSIVE          5
#define CLRE_STRING_TOO_SHORT       6   // einer der beiden Zielstrings ist zu kurz
#define CLRE_INVALID_ADDRESS        7   // ungültiges Format in Adresse
#define CLRE_INVALID_MACRO			8	// Error during macro parse

// Prefs-ID's

#define CLR_RPGLOBALDATA_ID MAKE_ID('F', 'R', 'T', '0')

#define CLR_DOMAIN1_ID MAKE_ID('F', 'R', 'T', '1')
#define CLR_DOMAIN2_ID MAKE_ID('F', 'R', 'T', '2')
#define CLR_DOMAIN3_ID MAKE_ID('F', 'R', 'T', '3')
#define CLR_DOMAIN4_ID MAKE_ID('F', 'R', 'T', '4')
#define CLR_DOMAIN5_ID MAKE_ID('F', 'R', 'T', '5')
#define CLR_DOMAIN6_ID MAKE_ID('F', 'R', 'T', '6')
#define CLR_DOMAIN7_ID MAKE_ID('F', 'R', 'T', '7')
#define CLR_DOMAIN8_ID MAKE_ID('F', 'R', 'T', '8')


#define CLR_SUBID_RPGLOBALDATA 0

struct RPGlobalData {

    char pathalias[256];    // Name der PathAlias-Datei
    long pagrundkosten;
    long pakbkosten;
    long pamaxkb;
    char Domain1[20];       // Namen der Domaingruppen
    char Domain2[20];
    char Domain3[20];
    char Domain4[20];
    char Domain5[20];
    char Domain6[20];
    char Domain7[20];
    char Domain8[20];

};


// Data der NOL mit den Zuordnungen Absendedomain -> ID der zugehörigen
// Routgruppe

#define CLR_SUBID_DOMAINOBJ 1 | TAGT_NOL

struct DomainObj {

    // char *name ist der Name des NOL-Eintrags
    long id;        // derzeit möglich 1-8

};


// Diese Struktur existiert für jede Routinggruppe einmal

#define CLR_SUBID_ROUTINGGLOBAL 0

struct RoutingGlobal{

    char smarthost[256];        // smarthost für die zugehörigen Absendeadressen
    long grundkosten;           // Kosten pro Mail einmal, wenn per PA
    long kbkosten;              // zusätzlich Kosten pro KB, wenn per PA
    long maxkb;                 // KB-Höchstgrenze für diese Absenderinnen bei PA

};


// NOL für die Routingliste pro ID

#define CLR_SUBID_ROUTINGOBJ 1 | TAGT_NOL

struct RoutingObj {

    // char *name als Name des NOL-Eintrags
    char sys[256];      // Domainbeschreibung
    char Verwalter[256];    // Wenn Domainserver, aber nicht @<Domainname>
    char domserver;     // gesetzt wenn eigene Domain als Domainserver,
                        // dafür keine Smarthost-Routing

    long grundkosten;
    long kbkosten;
    long maxkb;

};

// Flags für RoutingInfo

#define CLR_RIF_INTERNAL 1      // Routing erfolgte per interner Systemliste
#define CLR_RIF_PATHALIAS 2     // Routing erfolgte per PathAlias
#define CLR_RIF_DOMAINPREFS 4   // Routing erfolgte per Domain-Prefs
#define CLR_RIF_SMARTHOST   8   // Routing erfolgte per Domainprefs zum Smarthost
#define CLR_RIF_BANG    16      // Bangpfad wurde aufgelöst
#define CLR_RIF_LOCAL   32      // Adresse ist lokaler User (Gültigkeit des Usernamens ungetestet!)
#define CLR_RIF_NOEIL   64      // vorhandener Eintrag in Systemliste wurde nicht zum Routen benutzt
#define CLR_RIF_MACRO	128		// Makro wurde aufgelöst
#define CLR_RIF_PERC	256		// % wurde aufgelöst
                             

// Ergebnisstruktur CLR_GetRoute(), beschreibt das Routing, Kosten usw.


struct RoutingInfo {

    char Routsystem[CLR_ROUTSYSTEMLEN];         // Name des Routsystems
    char Eilsystem[CLR_ROUTSYSTEMLEN];          // Name des Systems falls Eilmail
    char NewDest[CLR_NEWDESTLEN];               // eventuell aufgelöster Bangpfad
    long routkosten;                            // Kosten für das Routing
    long flags;                                 // Infos, was passiert ist

};


#ifndef _NO_PROTO
/*
 *  Prototypes
 */
LONG CLR_GetRoute( STRPTR from, STRPTR to, struct RoutingInfo *routinginfo, ULONG size );
LONG CLR_IsZER( STRPTR sysname );                                    
LONG CLR_IsThisMe( STRPTR sysname );                                    
void CLR_LoadRouteData( void );
void CLR_Bang2Domain( STRPTR bangname, STRPTR domainnamebuffer );
#endif

#ifndef _NO_PRAG
/*
 *  SAS/C specific pragmas
 */
extern struct Library *CLRBase;
#pragma libcall CLRBase CLR_GetRoute 1e 0A9804
#pragma libcall CLRBase CLR_IsZER 24 801
#pragma libcall CLRBase CLR_CalcRouteFee 2a 09803
#pragma libcall CLRBase CLR_LoadRouteData 30 0
#pragma libcall CLRBase CLR_IsThisMe 36 801
#pragma libcall CLRBase CLR_Bang2Domain 3c 9802
#endif

#endif

