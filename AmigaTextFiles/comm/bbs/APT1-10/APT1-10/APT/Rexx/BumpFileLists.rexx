/* BumpFileLists.rexx
**
** $VER: bumpfilelists 0.0.1 (01.9.93)
**
** ARexx program for controlling ApT-BBS by ApT-Design.
**
** Copyright © 1993 ApT-Design All Rights Reserved,AG.
**
** Updates both the FRQUEST lists, as well as the file lists
** used for download by the user while online.
*/

CR = 'A'x
parse arg lineno

options results
portname = 'APTMANAGER'
address value portname

say CR CR "Bumping file lists & updating freq lists.." CR

/*
** Some defines that we will be using, ... change according
** to your system.
**/
WorkDir  = "T:"
DestDir  = "FLIST:"
FreqFile = "FILES.TXT"
FreqArc  = "FILES.LHA"
Header   = "Mail:QSilver/QSilver.Header"

RecentFile	= "RECENT.TXT"
RecentArc 	= "RECENT.LHA"
RecentHeader= "Mail:QSilver/QSilver.Header"

/*
** Included is a list of all of the areas that I am
** allowing to be included within the list.
**
** You can even arrange the order that they should be
** listed with a little bit of thought..
**
** For some people they would require a few INCLUDE
** tokens (but different) so that they could archive
** things such as related 'FOOBETA.LHA' for products
** that you may be producing/supporting etc..
**
**        BaseNumber
**        ---------
*/
INCLUDE.1	= 0	/* Pictures			*/
INCLUDE.2 	= 2	/* Odds&Sods			*/
INCLUDE.3 	= 3	/* Amiga Music			*/
INCLUDE.4 	= 4	/* Amiga Comms			*/
INCLUDE.5 	= 5	/* Amiga Demos			*/
INCLUDE.6 	= 6	/* Amiga Games			*/
INCLUDE.7 	= 7	/* Amiga General		*/
INCLUDE.8 	= 8	/* Amiga Programming		*/
INCLUDE.9 	= 9	/* Amiga Utility		*/
INCLUDE.10 	= 10	/* Amiga Pictures		*/
INCLUDE.11 	= 11	/* Amiga Text			*/
INCLUDE.12	= 12	/* Amiga BBS			*/
INCLUDE.13	= 13	/* Arexx			*/
INCLUDE.14	= 14	/* Amiga Usenet			*/
INCLUDE.15	= 18	/* Amiga BBS CLI		*/
INCLUDE.16	= 19	/* Amiga Emulators		*/
INCLUDE.17	= 30	/* AGA Only			*/
INCLUDE.18	= 31	/* CD32 General			*/
INCLUDE.19	= 20	/* IBM General			*/
INCLUDE.20	= 21	/* IBM Animation		*/
INCLUDE.21 	= 22	/* IBM Comms			*/
INCLUDE.22	= 23	/* IBM Games			*/
INCLUDE.23	= 24	/* IBM Pics			*/
INCLUDE.24	= 25	/* IBM Utilities		*/
Limit = 25

/*
** Its nice to have a header/advert at the top of your FREQ list
** explaining something about the magic names etc..
** Depending on the 'freqhandler' used you may not require to
** include one yourself, rather the 'freqhandler' would do that
** for you - Here we use our own custom generated one and attach
** it ourselves.
**/
address command "Copy "Header" to "WorkDir||FreqFile
address command "Copy "RecentHeader" to "WorkDir||RecentFile

/*
** We now do the actual creating of the FREQuest
** listings, only the areas defined within the INCLUDE
** list will be included.
**/
'FREQLIST' "OPEN="WorkDir||FreqFile
DO Pos=1 WHILE Pos < Limit ; 'FREQLIST' "BASE "INCLUDE.Pos ; END
'FREQLIST' "CLOSE"

'FREQLIST' "OPEN="WorkDir||RecentFile" DAYS=30"
DO Pos=1 WHILE Pos < Limit ; 'FREQLIST' "BASE "INCLUDE.Pos ; END
'FREQLIST' "CLOSE"

/*
** Create the FILES.LHA file in the work directory.
** Once created we move it over to our destination directory
** and remove the existing .LHA file in the work dir.
**/
address command "lha a "WorkDir||FreqArc WorkDir||FreqFile
address command "Copy "WorkDir||FreqArc "to "DestDir||FreqArc
address command "Delete "WorkDir||FreqArc

address command "lha a "WorkDir||RecentArc WorkDir||RecentFile
address command "Copy "WorkDir||RecentArc "to "DestDir||RecentArc
address command "Delete "WorkDir||RecentArc

/*
** Copy the .TXT file over to our destination dir and then
** remove it from the work dir.
**/
address command "Copy "WorkDir||FreqFile "to "DestDir||FreqFile
address command "Delete "WorkDir||FreqFile

address command "Copy "WorkDir||RecentFile "to "DestDir||RecentFile
address command "Delete "WorkDir||RecentFile

/*
** Now bump ALL of the file areas with new filelists to be placed
** within the FLIST: directory.
**/

'BUMPFILELISTS' "DAYS=14"

say CR "Finished...."

EXIT

/*##########################################################################*/

