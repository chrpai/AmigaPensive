//
// Prometheus <-> Connectline Konverter, all.h
// 
// Copyright 1995 by Mathias Mischler
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <proto/exec.h>
#include <proto/dos.h>

#include <dos/dostags.h>

#include <cl/clutil.h>
#include <cl/cl_user.h>
#include <cl/cl_prefs.h>
#include <cl/cl_misc.h>
#include <cl/clb.h>
#include <cl/cls.h>

// Prometheus Defines

#define PMBSPath "Prometheus:"              		// Prometheus:
#define PMBSDataPath PMBSPath "daten/"					// Prometheus:daten/
#define PMBSUserPath PMBSPath "user/"						// Prometheus:user/
#define PMBSUserList PMBSDataPath "user.dat"		// Prometheus:daten/user.dat
#define PMBSBoardList PMBSDataPath "brett.dat"	// Prometheus:daten/brett.dat
#define PMBSSystemList PMBSDataPath "netz.dat"  // Prometheus:daten/netz.dat

void ConvertPMBSUser ( void );									// user.c
void ConvertPMBSBoard ( void );									// board.c
void ConvertPMBSSystem ( void );								// system.c
