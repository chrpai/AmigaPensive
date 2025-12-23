#ifndef INCLUDE_CL_DOMAINS_H
#define INCLUDE_CL_DOMAINS_H

/*
 *	Preference definitions for system name and FQDN
 */


#define CL_DOMAINS_ID MAKE_ID('O', 'D', 'O', 'M')
#define CL_DOMAINS_SUBID 			(TAGT_NOL|0)               	// NOL mit Liste der Domainnamen
#define CL_DOMAINS_SUBID_NAME 		(TAGT_STRING|1)				// Systemnamen
#define CL_DOMAINS_SUBID_SYSOPNAME 	(TAGT_STRING|2)				// Username des Sysops (für POSTMASTER et al)
#define CL_DOMAINS_SUBID_ALIAS		(TAGT_NOL|3)				// Aliase des eigenen Systems (FQDN)

// Info-Struktur zu jedem Domaineintrag

struct DomainInfo {
    long OwnDomain;         // Wenn TRUE, ist das eigene System auch unter
	                        // @<Domainname> erreichbar (in der Hauptkonfiguration einstellbar)
};

#endif
