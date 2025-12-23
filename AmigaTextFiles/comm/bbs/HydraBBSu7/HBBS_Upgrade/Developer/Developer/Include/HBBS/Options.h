// define file locations for set files..


// defines for struct NodeList

UBYTE *OPT_NODE_Location           = "NodeLocation_";
UBYTE *OPT_NODE_AutoStart          = "AutoStart_";
// note: the above options are not read as V_STRINGLIST types, each path
// is read with a V_STRING type and is stored in the NodeData->NodeLocation/AutoStart
// structure for each node.


// defines for struct NodeSettingsData

UBYTE *OPT_NODE_SysopAccount       = "SysopAccount",

      *OPT_NODE_NodeName           = "NodeName",
      *OPT_NODE_NodePhone          = "NodePhone",
      *OPT_NODE_NodeConnectScript  = "NodeConnectScript",

      *OPT_NODE_AskUserPW          = "AskUserPW",

      *OPT_NODE_UseSysPW           = "UseSysPW",
      *OPT_NODE_SysPW              = "SysPW",
      *OPT_NODE_SysPWScript        = "SysPWScript",
      *OPT_NODE_SysPWPrompt        = "SysPWPrompt",

      *OPT_NODE_UseNodePW          = "UseNodePW",
      *OPT_NODE_NodePW             = "NodePW",
      *OPT_NODE_NodePWScript       = "NodePWScript",
      *OPT_NODE_NodePWPrompt       = "NodePWPrompt",

      *OPT_NODE_UserNamePrompt     = "UserNamePrompt",
      *OPT_NODE_UserPWPrompt       = "UserPWPrompt",

      *OPT_NODE_ModemLogFile       = "ModemLogFile",
      *OPT_NODE_NodeLogFile        = "NodeLogFile",
      *OPT_NODE_CallersLogFile     = "CallersLogFile",
      *OPT_NODE_DoorLog            = "DoorLog",
      *OPT_NODE_DoorLogFile        = "DoorLogFile",
      *OPT_NODE_UploadLog          = "UploadLog",
      *OPT_NODE_DownloadLog        = "DownloadLog",

      *OPT_NODE_AccessRequired     = "AccessRequired",

      *OPT_NODE_ChatFlag           = "ChatFlag",
      *OPT_NODE_UseOwnScreen       = "UseOwnScreen",
      *OPT_NODE_OpenWatch          = "OpenWatch",
      *OPT_NODE_Iconified          = "Iconified",

      *OPT_NODE_ScrModeID          = "ScrModeID",
      *OPT_NODE_ScrWidth           = "ScrWidth",
      *OPT_NODE_ScrHeight          = "ScrHeight",
      *OPT_NODE_ScrDepth           = "ScrDepth",

      *OPT_NODE_UseDevice          = "UseDevice",
      *OPT_NODE_AllowNewUsers      = "AllowNewUsers",

      *OPT_NODE_NodePlayPen        = "NodePlayPen",
      *OPT_NODE_MaxDIZLines        = "MaxDIZLines",

      *OPT_NODE_InactivityDisconnectOverride = "InactivityDisconnectOverride",
      *OPT_NODE_InactivitySeconds  = "InactivitySeconds",

      *OPT_NODE_CheckNodeScreens   = "CheckNodeScreens";
