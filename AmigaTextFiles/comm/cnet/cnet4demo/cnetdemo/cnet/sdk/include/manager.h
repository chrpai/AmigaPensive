// introduced as of CNet v4.21 - 06-Aug-96 to perform background
// test and transform of file items.

#ifndef CNET_MANAGER
	#define CNET_MANAGER



#define CNETFILEBOX "cnet:MANAGER"

// defines for ProcessType of struct TXFile
#define TX_TEST      0x01
#define TX_TRANSFORM 0x02

struct TXFile
	{
	LONG id;					// serial id of the user who posted/uploaded the file
                     	// account number not used to avoid awarding creds to the wrong user

	char UniqueBase[75];	// physical location (GO keyword) of file to be tested or transformed
								// this is only used if PhysNum is -1

	short PhysNum;			// (2-Feb-1997) -  Physical subboard NUMBER of the base where the file exists
	ULONG ItemNum;			// unique serial number of message
	char FileName[72];	// name of file to test or transform. FILENAME only, no path
	short ArcNum;			// number of archiver to use in test/tx..
	LONG ProcessType;		// See TX_* bits above
	UBYTE NotifyUser;		// 0=do not notify user
                        // 1=notify user - if online, send SYSTEM OLM, if offline,
                        //   sends mail to user regarding successful or failed
								//   test or transform and includes the test results if a test.
								// this field is currently ALWAYS 1 (show results)

	LONG checksum;	// this must be 0xAAAA or file will not be processed!
	};

// to have cnet's manager perform a test or transform, file a single FILLED in
// TXFile structure to a file (with any UNIQUE filename) to CNETFILEBOX
// (defined above).
//
// When the file is closed, cnet-manager will be notified that a new file
// is to be tested or transformed and the manager will then take action.

#endif
