
/*
 * FAME Version numbers
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * $VER: FAMEVersion.h v1.5
 *
 * Code by: David 'Strider/tRSi' Wettig
 * E-Mail:  strider@trsi.de
 * URL:     http://www.trsi.de/inno/strider/index.html
 *
 * Copyright note: FAME and all it's contents like programs,
 *                 tools, doors, documents, texts, sources,
 *                 standards like the FIM (FAME door port standard),
 *                 the FAMEToolPort and all depending things,
 *                 the FAMECoProc standard and (data) file formats
 *                 are copyrighted © 1993 - 1997 by David Wettig.
 *
 *                 The include file you are currently reading
 *                 is also copyrighted and you are only allowed
 *                 to use it to develop own programs for the
 *                 FAME BBS System and you are *NOT* allowed
 *                 to use it for own projects like Mailbox
 *                 programs or whatever without my permission.
 */

#ifndef FAME_FAMEVERSION_H
#define FAME_FAMEVERSION_H

/*
 * -----------------------------------------------------------------------------
 */

/*
 * General note:
 *
 * Version and revision numbers which are 0 won't be checked from the
 * FAME Server version/revision check.
 *
 * 0 means no check. 0 is alpha and no real release.
 */

/*
 * FAME.library version check *MUST* be valid!
 */

#define FVRC_L_FAME_LIB_VER              5
#define FVRC_H_FAME_LIB_VER              9999
#define FVRC_L_FAME_LIB_REV              0
#define FVRC_H_FAME_LIB_REV              9999

/*
 * Mainpart version check *MUST* be valid!
 */

#define FVRC_L_MAINPART_VER              1
#define FVRC_H_MAINPART_VER              1
#define FVRC_L_MAINPART_REV              32
#define FVRC_H_MAINPART_REV              32

/*
 * SystemEditor version check *MUST* be valid!
 */

#define FVRC_L_SYSED_VER                 1
#define FVRC_H_SYSED_VER                 1
#define FVRC_L_SYSED_REV                 26
#define FVRC_H_SYSED_REV                 26

/*
 * FAMEFileChecker version check *MUST* be valid!
 */

#define FVRC_L_FAME_FCHK_VER             1
#define FVRC_H_FAME_FCHK_VER             1
#define FVRC_L_FAME_FCHK_REV             18
#define FVRC_H_FAME_FCHK_REV             18

/*
 * UserEditor version check *MUST* be valid!
 */

#define FVRC_L_USERED_VER                0
#define FVRC_H_USERED_VER                0
#define FVRC_L_USERED_REV                0
#define FVRC_H_USERED_REV                0

/*
 * FAMEInfo version check is not a must, but should inform
 */

#define FVRC_L_FAME_INFO_VER             0
#define FVRC_H_FAME_INFO_VER             0
#define FVRC_L_FAME_INFO_REV             0
#define FVRC_H_FAME_INFO_REV             0

/*
 * FAMEHydra version check *MUST* be valid if SYSCMD/HYDRA is installed!
 */

#define FVRC_L_FAME_HYDRA_VER            1
#define FVRC_H_FAME_HYDRA_VER            1
#define FVRC_L_FAME_HYDRA_REV            15
#define FVRC_H_FAME_HYDRA_REV            15

/*
 * FAMESModem version check *MUST* be valid if SYSCMD/SMODEM is installed!
 */

#define FVRC_L_FAME_SMDM_VER             1
#define FVRC_H_FAME_SMDM_VER             1
#define FVRC_L_FAME_SMDM_REV             1
#define FVRC_H_FAME_SMDM_REV             1

/*
 * -----------------------------------------------------------------------------
 */

/*
 * If FAME2_0 is defined the FAME 2.0 contends will be added into the code.
 */

//#define FAME2_0

/*
 * -----------------------------------------------------------------------------
 */

#ifdef FAME2_0

  /*
   * Global version and revision defines for FAME 2.0 alpha
   */

  #define FAME2_GLOB_L_VER               2
  #define FAME2_GLOB_H_VER               2
  #define FAME2_GLOB_L_REV               0
  #define FAME2_GLOB_H_REV               4
  #define FAME2_GLOB_VER_STR             "2"
  #define FAME2_GLOB_REV_STR             "4"

  /*
   * FAME.library version check *MUST* be valid!
   */

	#undef  FVRC_L_FAME_LIB_VER
	#define FVRC_L_FAME_LIB_VER            6

  /*
   * Mainpart version check *MUST* be valid!
   */

  #undef  FVRC_L_MAINPART_VER
  #define FVRC_L_MAINPART_VER            FAME2_GLOB_L_VER

  #undef  FVRC_H_MAINPART_VER
  #define FVRC_H_MAINPART_VER            FAME2_GLOB_H_VER

  #undef  FVRC_L_MAINPART_REV
  #define FVRC_L_MAINPART_REV            FAME2_GLOB_L_REV

  #undef  FVRC_H_MAINPART_REV
  #define FVRC_H_MAINPART_REV            FAME2_GLOB_H_REV

  /*
   * SystemEditor version check *MUST* be valid!
   */

  #undef  FVRC_L_SYSED_VER
  #define FVRC_L_SYSED_VER               FAME2_GLOB_L_VER

  #undef  FVRC_H_SYSED_VER
  #define FVRC_H_SYSED_VER               FAME2_GLOB_H_VER

  #undef  FVRC_L_SYSED_REV
  #define FVRC_L_SYSED_REV               FAME2_GLOB_L_REV

  #undef  FVRC_H_SYSED_REV
  #define FVRC_H_SYSED_REV               FAME2_GLOB_H_REV

  /*
   * FAMEFileChecker version check *MUST* be valid!
   */

  #undef  FVRC_L_FAME_FCHK_VER
  #define FVRC_L_FAME_FCHK_VER           FAME2_GLOB_L_VER

  #undef  FVRC_H_FAME_FCHK_VER
  #define FVRC_H_FAME_FCHK_VER           FAME2_GLOB_H_VER

  #undef  FVRC_L_FAME_FCHK_REV
  #define FVRC_L_FAME_FCHK_REV           FAME2_GLOB_L_REV

  #undef  FVRC_H_FAME_FCHK_REV
  #define FVRC_H_FAME_FCHK_REV           FAME2_GLOB_H_REV

#endif

#endif /* FAME_FAMEVERSION_H */

