
	struct BirthDate 
		{
		ULONG	MDay,
				Month,
				Year;
		};

	struct  ExAccess
	    {                                  
	    ULONG   AccBits,           /* 32 Access Flag Bits */
	      		More_AccBits;      /* A lame way,but heres 32 more Flag Bits */

	    ULONG   Private_Area;   /* How many bytes allowed in a users private 
	                               area at logoff */

	    UWORD   Bad_Logins,     /* How many bad logins allowed before the
    	                           sysop is sent a notification letter */

	            Max_TB_Credits, /* How much time allowed in the timebank */

	            Time_Idle,      /* Idle time allowed */

	            Call_Minutes,           /* Minutes per call */
	            Call_Feedback,          /* Feedback per call */
	            Call_Posts,             /* Posts per call */
	            Call_TB_Withdraw,       /* How much time allowed per call */
	            Call_TB_Deposit,        /* How much time allowed per call */
	            Call_Upload_Files,      /* Uploaded files per call */
	            Call_Download_Files;    /* Downloaded files per call */
	    ULONG   Call_Upload_Bytes,      /* Uploaded bytes per call */
	            Call_Download_Bytes;    /* Downloaded bytes per call */

	    UBYTE   Period_Type;            /* Period type, 0 = none, 1 = daily,
	                                       2 = effective between first call 
	                                       of a period and period's length */

	    UWORD   Period_Length,          /* How many hours between periods */
	            Period_Calls,           /* Calls per period */
	            Period_Minutes,         /* Minutes Per Period */
	            Period_Feedback,        /* Pieced of feedback per period */
	            Period_Posts,           /* Items posted allowed per period */
	            Period_Charges,         /* How much can you rack up */
	            Period_TB_Deposit,      /* How much can you deposit per period*/
	            Period_TB_Withdraw,     /* How much can you withdraw per period*/
	            Period_Upload_Files,    /* Uploaded Files Allowed Per Period */
	            Period_Download_Files;  /* Downloaded  "     "     "    "    */
	    ULONG   Period_Upload_Bytes,    /* Uploaded Bytes    "     "    "    */
	            Period_Download_Bytes;  /* Downloaded  "     "     "    "    */

	    UBYTE   Download_Byte_Ratio,    /* Users Download Ratio (BYTES) */
	            Download_File_Ratio;    /* Users Download Ratio (FILES) */

	    UWORD   Expire_Days;            /* Inactive days allowed */

	   	LONG	FIDO_Credits;      		/* Users MAX Fido Balance */

		ULONG	Editor_Lines;			/* Max # of lines a user is allowed */

		ULONG	Period_Interuser_Chats,   /* Max # of interuser chats allowed */
				Period_Interuser_Mins,	  /* Max # of interuser mins allowed */	  
	            Period_Conference_Entry,  /* Max # of conference entries */
				Period_Conference_Lines,  /* Max # of conference lines sent */
	            Period_Conference_Mins,	  /* Max # of conference lines */ 
				Period_InterUserMessages; /* Max # of im's user can send */

		ULONG	Call_Interuser_Chats,     /* Max # of interuser chats allowed */
				Call_Interuser_Mins,	  /* Max # of interuser mins allowed */	  
	            Call_Conference_Entry,    /* Max # of conference entries */
				Call_Conference_Lines,    /* Max # of conference lines sent */
	            Call_Conference_Mins,	  /* Max # of conference mins */ 
				Call_InterUserMessages;   /* Max # of im's user can send */

		ULONG	Max_ActionCmds;			/* Max # of personal action commands */
		ULONG	expansion[4];
	    };


	struct ExData /* Data required for Extended Access limits */
	    {
	    ULONG	Period;  /* Start of the period for this user */ 
                
	    ULONG   Period_Up_Bytes,   /* Bytes uploaded in this period*/
	            Period_Down_Bytes; /* Bytes downloaded */
	    UWORD   Period_Up_Files,   /* Files uploaded */
	            Period_Down_Files, /* Files downloaded */
	            Period_Calls,      /* Calls */
	            Period_Minutes,    /* Time used */
	            Period_Charges,    /* Charges racked up in this period */
	            Period_Posts,      /* Items posted */
	            Period_Feedback,   /* Feedback left in this period */
	            Period_TB_Withdraw,/* Time taken out this period */
	            Period_TB_Deposit; /* Time deposited this period */

		ULONG	Period_Interuser_Chats,   /* interuser chats */
				Period_Interuser_Mins,	  /* interuser */	  
	            Period_Conference_Entry,  /* conference entries */
				Period_Conference_Lines,  /* conference lines sent */
	            Period_Conference_Mins,	  /* conference mins */ 
				Period_InterUserMessages; /* # of im's sent */

		LONG	FIDO_Credits;      /* Users Fido Balance */

		ULONG   Mail_Forward,   	 /* ID# of User that is receiving mail */
				Mail_Forward_Serial; /* There serial # so nothing happens */

		ULONG	expansion[5];
		};


	struct AccessGroup_Data   
		{
		struct	ExAccess	E_Access;
	
		TEXT	Access_Title[31];   /* The Title */

		ULONG 	User_Info_Bits;   	/* Data For More Prompt, Etc */
 
		struct 	ExData			E_Data;

	    ULONG   AccBits,        /* 32 Access Flag Bits */
				More_AccBits;   /* 32 More Flag Bits */

		UBYTE	ReducedAccess;
	
		ULONG	ReductionDate;

	    ULONG	Up_Bytes,        /* Bytes Uploaded */
	 			Up_MBytes,       /* MBytes Uploaded */
	 			Down_Bytes,      /* Bytes Downloaded */
	 			Down_MBytes,     /* MBytes Downloaded */
	 			Up_Files,        /* Files Uploaded */
	 			Down_Files,      /* Files Downloaded */

	 			Posts,           /* Items posted */
	  			Mail,            /* How many letters sent */
	 			Mail_Recieved,   /* Letters this user has recieved */
                
	 			Call_Number;     /* How many calls to the system */
                
	 	LONG	Balance;         /* How much has the user charged */

	  	LONG	Credit_Bytes,   /* UD byte credits */
	 			Credit_Files,   /* UD file credits */
	 			Credit_Time;    /* Timebank credits */

	 	ULONG	Last_Baud,      /* What baud rate they connected at last */

	 			Bad_Logins;     /* How many failed attempts since last call? */

		ULONG	Free_Bytes,		/* The initial file credits given to a user */
				Free_Files;		/* The initial byte credits given to a user */

		ULONG	Expansion[5];
		};



	struct UserInfo
	    {
	    ULONG	Serial_Number; /* Unique serial number for each account */

	    ULONG   LastPvt_Entry;		/* Last time in personal area */

		ULONG	LastCall,
				FirstCall;

	    UBYTE   AccessGroup,
	            ReducedAccess;

	    ULONG   ReductionDate;

	    struct  ExAccess        E_Access;         

	    TEXT    Alias[31],
	            Real_Name[31],
	            Phone[21],
				WorkPhone[21],
	  	        Street[31],	
	            Mail_Route[15],
	            Country[4],
	            State[4],
	            City[26],
	            Comments[41],
	            Computer[31],
	  			Net_Alias[41],  /* A Users Special Net Address for UUCP */
	  	        Macro[4][91];	/* The New Macro Key Settings */
        
		UWORD	Passwd;

	    ULONG   User_Info_Bits;  /* Bits for sex, more prompts, etc.. */

	    UBYTE   Term_Type,      /* Terminal Type */
	            Term_Max_Col,   /* Number of Columns */
	            Term_Max_Lines, /* Number of Lines */
	            Default_Proto,  /* Default UD Protocal */
				Scan_Prefs;		/* The scan preferences for scanning */

	    BYTE	Time_Zone;      /* Time zone offset */

	    struct  ExData          E_Data;
                              
	    ULONG   Up_Bytes,        /* Bytes Uploaded */
	            Up_MBytes,       /* MBytes uploaded */
	            Down_Bytes,      /* Bytes Downloaded */
	            Down_MBytes,     /* MBytes downloaded */
	            Up_Files,        /* Files Uploaded */
	            Down_Files,      /* Files Downloaded */

	            Posts,           /* Items posted */
	            Mail,            /* How many letters sent */
	            Mail_Received,   /* Letters this user has recieved */
                
	            Call_Number;     /* How many calls to the system */
                
	   	LONG    Balance;         /* How much has the user charged */

	   	ULONG   Credit_Bytes,   /* UD byte credits */
	            Credit_Files,   /* UD file credits */
	            Credit_Time,    /* Timebank credits */

	            Last_Baud,      /* What baud rate they connected at last */
	            Bad_Logins;     /* How many failed attempts since last call? */

		ULONG	FreeFiles,      /* Files that are free! */
				FreeBytes;		/* Free bytes! */
	
		TEXT	WhoNote[61];	/* Info to other users in WHO */

		UBYTE	Header_Prefs;	/* Preferences for all the message headers */

		ULONG	User_ID;		/* The User's ID# */

		ULONG	ScreenClear;	/* Clears screen for message reading */

		ULONG	WhoPrefs;		/* Default Who Listing */

		struct	BirthDate	Birthdate;

		ULONG	MBase_Flags,
				FBase_Flags,
				PBase_Flags;

		UBYTE	BatchMethod;
		TEXT	Organization[81];
		ULONG	Language;
		ULONG	expansion[20];
		};

