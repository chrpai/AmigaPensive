; Sub-board struct

 STRUCTURE NewSubDir_Data,0
	STRUCT	exsdd_Sub_Title,31		; How It Looks In System
	STRUCT	exsdd_Sub_Location,31		; Where It Is On Disk
	STRUCT	exsdd_Sub_Path,81		; Path
	STRUCT	exns_Sub_Password,21		; Password to enter, if flagged

	LONG	exsdd_Entry_Access		; 0 - 32 (Access Level Entry)
	LONG	exsdd_Serial_Number		; Unique ID of this subboard

	BYTE	exsdd_Youngest			; The Youngest Age To Enter
	BYTE	exsdd_Oldest			; The Oldest Age To Enter

	LONG	exsdd_SubDir_Bits		; General Bits, reference 'SUB_' Bits

	LONG	exsdd_PathID			; BBSPath id #

	LONG	exsdd_Download_Group		; Access Group Able To Download
	LONG	exsdd_Upload_Group		; Access Group Able To Upload
	LONG	exsdd_Post_Group		; Access Group Able To Post
	LONG	exsdd_Reply_Group		; Access Group Able TO Reply	

	LONG	exsdd_Archive_Lock		; Able To Lock Out Certain Uploads of a certain Archive Type
	LONG	exsdd_MaxItems			; Max items in allowed in area
	LONG	exsdd_MinBaudRate		; The Minimum Baud Rate
	LONG	exsdd_MinBaudHours		; Hours MinBaud Enforced
	LONG	exsdd_Closed_Groups		; The Access Groups That Are Not Allowed in
	LONG	exsdd_Hours_Closed		; The Hours Area Is Closed
	LONG	exsdd_Extended_Flags		; Extended Access Flags For: ESUB_flags
	LONG	exsdd_DownloadBonus		; %t of filesize rewarded to uploader per dl
	LONG	exsdd_SortPrefs			; The default Sort Prefs
	LONG	exsdd_AccessFlags		; Some more accessflags
	LONG	exsdd_LastResponse		; Date last changed
	LONG	exsdd_sub_First			; UseNet first message
	LONG	exsdd_sub_Next			; UseNet last message

	BYTE	exsdd_AutoWeed_Days		; The # days of inactivity before item is killed (0=never)
	BYTE	exsdd_UpTime_Refresh		; % Of Time To Be Returned After UL 
	BYTE	exsdd_DownTime_Refresh		; % Of Time To Be Taken After DL
	BYTE	exsdd_KludgeFill00

	STRUCT	exsdd_PrefArc,4			; The preferred archive for this area

	WORD	exsdd_FileAdd			; Add messages to uploaded archives
	WORD	exsdd_FileCheck			; Check archives
	WORD	exsdd_TimeLock			; # of mins online before use

	BYTE	exsdd_ByteRatio			; Byte ratio for this area
	BYTE	exsdd_FileRatio			; File ratio for this area

	DOUBLE	exsdd_KCharge			; Charge for each k downloaded
	LONG	exsdd_TimeCharge		; Time charge for this area/min

	STRUCT	exsdd_domain,32			; FidoNet domain

	APTR	exsdd_SUBLIST			; List of subs
	DOUBLE	exsdd_FileCharge		; Charge for each file downloaded
	LABEL exsdd_SIZE
