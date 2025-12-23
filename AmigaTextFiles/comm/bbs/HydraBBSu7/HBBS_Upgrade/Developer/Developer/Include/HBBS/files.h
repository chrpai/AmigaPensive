#ifdef MAIN

  // defines file locations for set files..

  UBYTE *FILE_NODEGLOBAL="HBBS:Nodes/NodeGlobal",
        *FILE_NODELIST="HBBS:Nodes/NodeList",
        *FILE_CONFLIST="HBBS:Conferences/ConfList",
        *FILE_BBSGLOBAL="HBBS:BBSGlobal",
        *FILE_CALLERSLOG="HBBS:Nodes/CallersLog",
        *FILE_ERRORLOG="HBBS:Logs/ErrorLog",
        *FILE_ACSLEVELLIST="HBBS:Access/Levels/Level_List",
        *FILE_PROTOCOLLIST="HBBS:System/Protocols",
        *FILE_MODEIFILE="HBBS:System/Prefs/ModeID";

  UBYTE *FILE_HBBS="HBBS:";

  UBYTE *FILENAME_NODELOCAL="NodeLocal";
  UBYTE *FILENAME_DEVICE="Device";
  UBYTE *FILENAME_CONFCONFIG="ConfConfig";

  UBYTE *FILENAME_CMDSSYSTEM="COMMANDS/SYSTEM";

  UBYTE *FILENAME_COMMANDS="Commands";
  UBYTE *FILENAME_ACCESS="Access";
  UBYTE *FILENAME_SCREENS="Screens";
  UBYTE *FILENAME_LOSTFILES="LostFiles"; // LostCARRIERfiles that is..

  UBYTE *DIRNAME_SCREENSSPECIAL="Screens/Special/";
  UBYTE *DIRNAME_SCREENSBULLETINS="Screens/Bulletins/";

  UBYTE LEN_FILENAME_CONF_LONGEST=10;

  UBYTE *FILE_PRIVATEDATA="HBBS:System/Data/PrivateData";

  #ifdef HBBSCOMMON

    UBYTE *PRG_ERRORMESSAGE="HBBS:Doors/System/ErrorMessage/ErrorMessage";

  #endif

  #ifdef ERRORMESSAGE

    UBYTE *FILE_ERRORMESSAGE="HBBS:Doors/System/ErrorMessage/ErrorMessage.TXT";

  #endif


#else

  // defines file locations for set files..

  extern UBYTE *FILE_NODEGLOBAL,
        *FILE_NODELIST,
        *FILE_BBSGLOBAL,
        *FILE_CONFLIST,
        *FILE_CALLERSLOG,
        *FILE_ERRORLOG,
        *FILE_ACSLEVELLIST,
        *FILE_MODEIFILE;

  extern UBYTE *FILE_HBBS;

  extern UBYTE *FILENAME_NODELOCAL;
  extern UBYTE *FILENAME_DEVICE;
  extern UBYTE *FILENAME_CONFCONFIG;

  extern UBYTE *FILENAME_CMDSSYSTEM;

  extern UBYTE *FILENAME_COMMANDS;
  extern UBYTE *FILENAME_ACCESS;
  extern UBYTE *FILENAME_SCREENS;
  extern UBYTE *FILENAME_LOSTFILES;

  extern UBYTE *DIRNAME_SCREENSSPECIAL;
  extern UBYTE *DIRNAME_SCREENSBULLETINS;

  extern UBYTE LEN_FILENAME_CONF_LONGEST;

  extern UBYTE *FILE_PRIVATEDATA;

  #ifdef HBBSCOMMON

    extern UBYTE *PRG_ERRORMESSAGE;

  #endif

#endif
