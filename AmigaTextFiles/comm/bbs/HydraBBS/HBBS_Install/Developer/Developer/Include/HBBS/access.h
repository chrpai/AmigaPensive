/*
   ALPHA NOTE:

   only access settings with a #define'd string as well have been implemented
   also if they have a *A* after the string they have not been implemented yet
   either.

   if you require a particular access setting for your door let me know
   and I'll implement it in the next release!

*/

// the access level flags

// misc access

#define ACS_DLFILES                0
#define ACSSTR_DLFILES               "DownloadFiles"
#define ACS_ULFILES                1
#define ACSSTR_ULFILES               "UploadFiles"
#define ACS_SYSOPCOMMENT           2
#define ACSSTR_SYSOPCOMMENT          "SysopComment"
#define ACS_READMAIL               3
#define ACSSTR_READMAIL              "ReadMail"
#define ACS_WRITEMAIL              4
#define ACSSTR_WRITEMAIL             "WriteMail"
#define ACS_SCANMAIL               5
#define ACSSTR_SCANMAIL              "ScanMail"
#define ACS_ALLOWSTATUS            6
#define ACSSTR_ALLOWSTATUS           "AllowStatus"
#define ACS_ALLOWEDIT              7
#define ACSSTR_ALLOWEDIT             "AllowEdit"
#define ACS_SCANFILES              8
#define ACSSTR_SCANFILES             "ScanFiles"
#define ACS_ALLOWVIEW              7
#define ACSSTR_ALLOWVIEW             "AllowView"


// for W command

#define ACS_EDITHANDLE            40
#define ACSSTR_EDITHANDLE            "EditHandle"
#define ACS_EDITREALNAME          41
#define ACSSTR_EDITREALNAME          "EditRealName"
#define ACS_EDITPASSWORD          42
#define ACSSTR_EDITPASSWORD          "EditPassword"
#define ACS_EDITGROUP             43
#define ACSSTR_EDITGROUP             "EditGroup"
#define ACS_EDITLOCATION          44
#define ACSSTR_EDITLOCATION          "EditLocation"
#define ACS_EDITCOUNTRY           45
#define ACSSTR_EDITCOUNTRY           "EditCountry"
#define ACS_EDITCOMPUTER          46
#define ACSSTR_EDITCOMPUTER          "EditComputer"
#define ACS_EDITPHONENUMBER       47
#define ACSSTR_EDITPHONENUMBER       "EditPhoneNumber"
#define ACS_EDITSCREENTYPE        48
#define ACSSTR_EDITSCREENTYPE        "EditScreenType"
#define ACS_EDITLINES             49
#define ACSSTR_EDITLINES             "EditLines"
#define ACS_EDITEDITOR            50
#define ACSSTR_EDITEDITOR            "EditEditor"
#define ACS_EDITPROTOCOL          51
#define ACSSTR_EDITPROTOCOL          "EditProtocol"

// for mail

#define ACS_DELETEMAIL            80         // delete any mail at all?
#define ACSSTR_DELETEMAIL           "DeleteMail"
#define ACS_DELETEGROUP           81         // delete group mail ?
#define ACSSTR_DELETEGROUP          "DeleteGroup"
#define ACS_DELETEANYONE          82         // delete anyones mail ?
#define ACSSTR_DELETEANYONE         "DeleteAnyone"
#define ACS_ALLOWANSI             83         // allow ansi in mail ? *I*
#define ACS_ALLOWSYSOP            84         // sysop mail ?         *I*
#define ACS_ALLOWPRIVATE          85         // private mail ?       *I*
#define ACS_ALLOWGROUP            86         // group mail ?         *I*
#define ACS_ALLOWCONFERENCE       87         // conference mail ?    *I*
#define ACS_ALLOWALL              88         // all mail ?
#define ACSSTR_ALLOWALL             "AllowALL"
#define ACS_ALLOWEVERYBODY        89         // ealls ?
#define ACSSTR_ALLOWEVERYBODY       "AllowEverybody"
#define ACS_ALLOWMULTIPLE         90         // multiple mail
#define ACSSTR_ALLOWMULTIPLE        "AllowMultiple"

#define ACS_ATTACHFILES           91         // allow file attach ?         *A*
#define ACSSTR_ATTACHFILES          "AttachFiles"                        // *I*

#define ACS_ALLOWMULTIFILES       92 // and & with following settings...
#define ACSSTR_ALLOWMULTIFILES      "AllowMultiFiles"                    // *A*
#define ACS_ALLOWPRIVATEFILES     93
#define ACSSTR_ALLOWPRIVATEFILES    "AllowPrivateFiles"                  // *A*
#define ACS_ALLOWALLFILES         94
#define ACSSTR_ALLOWALLFILES        "AllowAllFiles"                      // *A*
#define ACS_ALLOWEVERYONEFILES    95
#define ACSSTR_ALLOWEVERYONEFILES   "AllowEveryoneFiles"                 // *A*
#define ACS_ALLOWGROUPFILES       96
#define ACSSTR_ALLOWGROUPFILES      "AllowGroupFiles"                    // *A*
#define ACS_AUTOEXPIREMSG         97
#define ACSSTR_AUTOEXPIREMSG        "AutoExpireMsg"

// for overwriting xxxxAllowed settings..

#define ACS_UNLIMTIME            120
#define ACSSTR_UNLIMTIME             "UnlimitedTime"
#define ACS_UNLIMCALLS           121
#define ACSSTR_UNLIMCALLS            "UnlimitedCalls"                    // *A*
#define ACS_UNLIMBYTES           122
#define ACSSTR_UNLIMBYTES            "UnlimitedBytes"                    // *A*
#define ACS_UNLIMCHAT            123
#define ACSSTR_UNLIMCHAT             "UnlimitedChat"                     // *A*

