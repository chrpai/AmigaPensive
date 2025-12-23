/******************************************************************************
 *                                                                            *
 * PROGRAM:       CXBaud.rexx                                                 *
 *                                                                            *
 * PURPOSE:       Show user's BPS in BIG ANSI and a few stats.                *
 *                                                                            *
 * AUTHOR:        Sune Stjerneby (RubberDuck)                                 *
 *                                                                            *
 * DATE:          06 Apr 96                         			      *
 *                                                                            *
 * TIME:          16:08:42                                                    *
 *                                                                            *
 * VERSION:       0                                                           *
 * REVISION:      1                                                           *
 * SUBREVISION:   0                                                           *
 *                                                                            *
 * CNET VERSION:  3.05c - 4.05                                                *
 *                                                                            *
 * LOG:                                                                       *
 *                                                                            *
 *  06 Apr 96 First Version!.                                                 *
 *                                                                            *
 * INSTALLATION:                                                              *
 *                                                                            *
 *  Copy the contents of systext/baud/#? to systext:baud/.                    *
 *                                                                            *
 *  Add this to your SYSTEXT:SYS.WELCOME (or where ever you want it)          *
 *                                                                            *
 *  #0Pfiles:CXBaud.rexx}                                                    *
 *                                                                            *
 *   Another                                                                  *
 *   _____________  _____________  ___________   _____________  _____  ______ *
 *   \_    ______/  \______     /  \_______   \  \_     _____/  \_   \/     / *
 *    /   /     /    /    /    /    /     /   /   /    _____/     \      __/  *
 *   /   /     /    /    /    /    /     /   /   /    /    /      /      \_   *
 * _/   /     /   _/    /    /   _/     /   /  _/    /    /   ___/        /   *
 * \_________/    \_________/    \_________/   \_________/    \______/\__/    *
 *                                                                            *
 *                          P R O D U C T I O N                               *
 *                                                                            *
 *               Call +45 5627-6730     -    28k8 vFC/v34                     *
 *                                                                            *
 ******************************************************************************/

options results
getuser 1200018                      /* Get BPS in 3-digit format (24,144,288 etc.) */
bps=result*10".ansi"                 /* Multiplicate with 10 and add .ansi */
changewhere "Showing Baud/Stats"     /* For Sysop's and WHO-users eyes only, changes 'Action' in CNet Controlpanel and WHO command */
if baud<2400 or baud>57600 then exit /* Make sure we have an ANSI for user's BPS. Change values and create new .ansi's if you have many 300 bps users |) (or ISDN) */
bps="systext:baud/"||bps||""         /* 'Compile' file-name by joining path and bps-rate.ansi */
sendfile bps                         /* Show it to user */
sendfile "systext:baud/stats.ansi"   /* ..And a few stats - add your own if you want */
transmit 'n2c4CXBaud v0.1 (C) 06.04.96 by Sune Stjerneby - Codex Amiga BBS  +45 5627 6730' /* Yatta yatta yatta... */
bufferflush                          /* Clean up */
exit                                 /* ...and we're outtahere... */
