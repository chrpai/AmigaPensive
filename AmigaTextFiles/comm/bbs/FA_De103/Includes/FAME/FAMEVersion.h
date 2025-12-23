
/* FAME Version numbers
 *
 * $VER:FAMEVersion.h v1.3
 *
 * Used tab size: 2
 */

#ifndef	FAME_FAMEVERSION_H
#define	FAME_FAMEVERSION_H

/* General note:
 *
 * Version and revision numbers which are 0 won't be checked from the
 * FAME Server version/revision check.
 *
 * 0 means no check. 0 is alpha and no real release.
 */

/* FAME.library version check *MUST* be valid! */

#define	FVRC_L_FAME_LIB_VER		4
#define	FVRC_H_FAME_LIB_VER		4
#define	FVRC_L_FAME_LIB_REV		0
#define	FVRC_H_FAME_LIB_REV		0

/* Mainpart version check *MUST* be valid! */

#define	FVRC_L_MAINPART_VER		1
#define	FVRC_H_MAINPART_VER		1
#define	FVRC_L_MAINPART_REV		26
#define	FVRC_H_MAINPART_REV		26

/* SystemEditor version check *MUST* be valid! */

#define	FVRC_L_SYSED_VER			1
#define	FVRC_H_SYSED_VER			1
#define	FVRC_L_SYSED_REV			21
#define	FVRC_H_SYSED_REV			21

/* FAMEFileChecker version check *MUST* be valid! */

#define	FVRC_L_FAME_FCHK_VER	1
#define	FVRC_H_FAME_FCHK_VER	1
#define	FVRC_L_FAME_FCHK_REV	14
#define	FVRC_H_FAME_FCHK_REV	14

/* UserEditor version check *MUST* be valid! */

#define	FVRC_L_USERED_VER			0
#define	FVRC_H_USERED_VER			0
#define	FVRC_L_USERED_REV			0
#define	FVRC_H_USERED_REV			0

/* FAMEInfo version check is not a must, but should inform */

#define	FVRC_L_FAME_INFO_VER	0
#define	FVRC_H_FAME_INFO_VER	0
#define	FVRC_L_FAME_INFO_REV	0
#define	FVRC_H_FAME_INFO_REV	0

/* FAMEHydra version check *MUST* be valid if SYSCMD/HYDRA is installed! */

#define	FVRC_L_FAME_HYDRA_VER	1
#define	FVRC_H_FAME_HYDRA_VER	1
#define	FVRC_L_FAME_HYDRA_REV	12
#define	FVRC_H_FAME_HYDRA_REV	12

/* FAMESModem version check *MUST* be valid if SYSCMD/SMODEM is installed! */

#define	FVRC_L_FAME_SMDM_VER	0
#define	FVRC_H_FAME_SMDM_VER	0
#define	FVRC_L_FAME_SMDM_REV	1
#define	FVRC_H_FAME_SMDM_REV	1

#endif /* FAME_FAMEVERSION_H */

