
/*
 * FAME ToolPort Commands
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * $VER: "FAMEToolPortCommands.h" v1.5
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

#ifndef FAME_FAMETOOLPORTCOMMANDS_H
#define FAME_FAMETOOLPORTCOMMANDS_H

/*
 * FAMEToolPort command defines (struct FAMEToolPortMsg -> ftpm_Command)
 */

/*
 * ##################### COMMAND DEFINES BEGIN ##########################
 */

/*
 * -------------------------  Data Refresh  -----------------------------
 */

#define FAMETP_Update               0
/*
 * Refreshes the datas from your FAMEToolPortMsg structure internal
 * and refreshes the FAMEToolPort Info Windows if they are open.
 */

#define FAMETP_UpdateAll            5
/*
 * The same as FAMETP_Update but it updates all entries in the ToolPort
 * info windows.
 */

/*
 * -------------------------  Must Commands  ----------------------------
 */

#define FAMETP_Register             1
/*
 * Register a new FAMEToolPort using tool (FAMEToolPort tool),
 * an internal counter increases.
 *
 * Note: This has to be the first command of your tool else the Server
 *       will refuse all your incomming messages.
 */

#define FAMETP_EndRegister          2
/*
 * Unregister a FAMEToolPort using tool (FAMEToolPort tool),
 * an internal counter decreases.
 *
 * Note: If your tool ends or it receives a CTRL-C to end you have
 *       a maximum of 20 seconds time (more less) to send this
 *       command.
 */

/*
 * -------------------------  Special commands  -------------------------
 */

#define FAMETP_InfoStructs          3
/*
 * Request an information struct.
 *
 * ftpm_Data1      <- The InfoStruct number (define).
 * ftpm_StringPtr1 -> A pointer to the requested InfoStruct.
 *
 * Note: struct FAMEInfoList is such a InfoStruct.
 *
 * Available InfoStructs: #define FIS_FAMEInfoList 1
 *
 *                        (FAMEDefine.h)
 *
 * Note: In the case of FIS_FAMEInfoList (1) you will get a pointer
 *       to a list of FAMEInfoLists pointing to the first Node (Node 0).
 *       You have to walk through this list with the fili_Next
 *       pointer if you need struct FAMEInfoList from a different Node.
 */

#define FAMETP_FDCMP                4
/*
 * With FAMETP_FDCMP you transmit your message port (MsgPort) pointer.
 *
 * ftpm_StringPtr1 <- Your struct MsgPort.
 *
 * Note: FAMETP_FDCMP must be used if you want to use the FDCMP feature.
 *       You have to create a message port where you can receive the
 *       FDCMP events. This port must be known by the Server to send
 *       your requests to this port. With FAMETP_FDCMP you will handle
 *       your message port to the Server.
 */

#define FAMETP_DisplayTextToNode    7
/*
 * Display a fixed text line on a Node CON screen.
 *
 * ftpm_Data1      <- The line number.
 * ftpm_StringPtr1 <- A pointer to the text.
 * ftpm_Node       <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_DisplayTextToNode you can display a text line
 *       (max 79 chars) to a Node's console.
 *
 *       In ftpm_Data1 you specify the line number in which the
 *       text shall be displayed. This number will be checked if
 *       it fits on the screen - if not the request will be refused.
 */

#define FAMETP_DisplayAwaitTextNode 8
/*
 * Display a fixed text line on a Node CON screen in "Await Call" mode.
 *
 * ftpm_Data1      <- The line number.
 * ftpm_StringPtr1 <- A pointer to the text.
 * ftpm_Node       <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_DisplayAwaitTextNode you can display a text line
 *       (max 79 chars) to a Node's console if it's in "Await Call" mode.
 *
 *       In ftpm_Data1 you specify the line number in which the
 *       text shall be displayed. This number will be checked if
 *       it fits on the screen - if not the request will be refused.
 *
 *       So text will only be displayed if the line number fits and
 *       the Node is in "Await Call" mode.
 *
 *       FAMETP_DisplayAwaitTextNode will normally be used to display
 *       online bulletins at the Await screen of the Node.
 *
 *       See the FAMETPDigiClock example ToolPort tool for more.
 */

#define FAMETP_RequestAwaitBullMode 9
/*
 * Requests the Await Bull FDCMP state.
 *
 * ftpm_Node       <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_RequestAwaitBullMode you request either the
 *       FDCMP_SERVER_AwaitBulletinStart or FDCMP_SERVER_AwaitBulletinStop
 *       FDCMP event from one or all Nodes.
 *
 *       FAMETP_RequestAwaitBullMode is normally needed from online
 *       bulletin ToolPort tools like FAMETPDigiClock for the case
 *       that the tool is started while the depending Node screen is
 *       already open and the Node in "Await Call" mode.
 */

#define FAMETP_FIMCommand           10
/*
 * Execute a FIM Doorport command.
 *
 * ftpm_StructDummy1 <- The allocated and filled struct FAMEDoorMsg.
 * ftpm_Node         <- The Node number
 *
 * Note: With FAMETP_FIMCommand you can use (nearly) every command
 *       a online FIM-Door can use.
 *
 *       You have to allocate the normal FAMEDoorMsg like a door:
 *
 *       struct FAMEDoorMsg *MyFAMEDoorMsg = FAMEAllocObject( FOBJ_FAMEDoorMsg );
 *
 *       and the put the pointer into ftpm_StructDummy1:
 *
 *       MyFAMEToolPortMsg -> ftpm_StructDummy1 = MyFAMEDoorMsg;
 *
 *       Like a door you have to fill out all needed fields of FAMEDoorMsg
 *       and then send the command.
 *
 *       Example: The FIM command (see FAMEDoorCommands.h):
 *
 *                MyFAMEDoorMsg -> fdom_Command   = AR_SendStr;
 *                MyFAMEDoorMsg -> fdom_StringPtr = "Hello user!";
 *
 *       It's not allowed for a ToolPort tool to use the FIM Must Commands
 *       MC_DoorStart, MC_ShutDown and MC_ShutDownLastWords !!!
 *       If you do fdom_ReturnCode will be: FIM_CMDDENIED .
 *
 *       There will be also some command which arn't working correctly
 *       so please check all commands you use! For example commands like
 *       NR_PromptChars or other functions to ask for user strings
 *       arn't working, because the GetString command is if a user is online
 *       the main loop of the Node and is already in use - it's not able to
 *       work recursive yet. Maybe this will be changed in future or you
 *       write your own GetString function.
 *       Single char prompt functions like NR_WaitChar are working.
 *
 *       Well ok, that's easy and logical till here, but using FIM commands
 *       via FAMEToolPort is not that simple. The problem is the asynchronous
 *       work of the Server. If you send a ToolPort command like
 *       FAMETP_FIMCommand it will immediately executed and the message
 *       replied. In the case of FAMETP_FIMCommand the Server sends the
 *       command and the datas directly to the Node you've defined with
 *       the command... but the Server is not waiting till the Node replies
 *       no it continues work and will get the reply sometime later...
 *       only on doing it that way the Server is able to react parallel
 *       to all Nodes and tools or whatever signals the Server could get.
 *       That means if the Server has send the request of your command to
 *       the Node the Server will not wait but will reply your ToolPort
 *       message... well the problem is that FIM DoorPort will work completely
 *       different. A door sends the message and wait as long as the command
 *       execution is in progress... in case of a WaitKey or GetString
 *       command it could take theoretically hours until the Node replies
 *       the message to the Door. The Server couldn't wait so that's the
 *       problem.
 *
 *       The solution for this problem is the FDCMP event feature of the
 *       FAMEToolPort.
 *
 *       That means you have to support FDCMP if you want to use
 *       FAMETP_FIMCommand.
 *
 *       FDCMP_SERVER_FIMCommandReturn (ftpm_BitRequests5) will be the FDCMP
 *       event bit your tool will receive if the Node is ready with your
 *       FIM command request.
 *
 *       Now there is a second problem.
 *
 *       Your tool has to end if it receives CTRL-C as every FAMEToolPort
 *       using tool has to do.
 *
 *       Only waiting will not be possible because if your tool has to end
 *       before the command has been returned to you you can't deallocate
 *       the FAMEDoorMsg structure, because it's in use.
 *
 *       The solution for this second problem is easy. You have not much
 *       to do with it... if you receive a CTRL-C or whatever your tool
 *       quits then deallocate the FAMEDoorMsg structure in any case.
 *
 *       The Server will handle this internal, but there is something
 *       different to the way a door handles this and you have to know this
 *       so now it will be explained what happens internally.
 *
 *       First you have to know that it's like a door only possible to send
 *       one FAMETP_FIMCommand at one time so you have to wait for the
 *       FDCMP_SERVER_FIMCommandReturn event before you send the next one.
 *       Multiple requests will be refused by the Server with the returncode
 *       FAMETPRC_FIMCommandInProgress.
 *
 *       Now to the explanation.
 *
 *       If the Server receives your request (FAMETP_FIMCommand) your
 *       FAMEDoorMsg structure will be copied into an internal buffer
 *       (that's why it's safe to deallocate your memory after the
 *       reply of your request). Now this buffer will be used to send
 *       the request to the Node. If the Node replies the Server will
 *       send you this message (pointer) within the FAMEToolNotifyMsg
 *       (ftnm_StructDummy1) to your FDCMP in port. Now you can read
 *       this message to get the results from your FAMETP_FIMCommand
 *       request.. but beware.. the message you receive within the
 *       FAMEToolNotifyMsg is not yours!!! Do not free it or change it!
 *       The best is you copy it into your own FAMEDoorMsg structure
 *       and reply the FAMEToolNotifyMsg - this will be the safest way.
 *       Then interpret your copy.
 *
 * Note: Like a door you have to check the field fdom_ReturnCode of the
 *       FAMEDoorMsg you receive after the call of FAMETP_FIMCommand
 *       with the FDCMP event FDCMP_SERVER_FIMCommandReturn.
 *
 *       For a ToolPort tool it's not important to end like a door
 *       if the fdom_ReturnCode is negative, but you can react on possible
 *       problems and better stop using FAMETP_FIMCommand for this Node.
 */

#define FAMETP_SysOpAway            11
/*
 * Sets the SysOp Away state.
 *
 * ftpm_Data1      <- Number of the defined Away messages (1-10 / 0 be back).
 * ftpm_StringPtr1 <- A Away message.
 *
 * Note: With FAMETP_SysOpAway it's possible to set a new Away message
 *       or select one of the 10 pre-defined Away messages.
 *
 *       ftpm_Data1 can be a value from 1 - 10 for the pre-defined Away
 *       messages or ftpm_Data1 can be 0 to let the SysOp be back - means
 *       the chat flag for the Nodes will be restored to the old state
 *       (if an Away message will be set the chat flags for all Nodes will
 *       be deleted - chat off).
 *
 *       If ftpm_StringPtr1 is not NULL it means that you have your own
 *       Away message which will be used then.
 *
 * Note: If ftpm_StringPtr1 is not NULL it has a higher priority than
 *       ftpm_Data1 !
 */

/*
 * -------------------------  Server depending commands  ----------------
 */

#define FAMETP_SVNodesBankSwitch    1000
/*
 * Switches the Node banks.
 *
 * Note: FAMETP_SVNodesBankSwitch switches between the available Node
 *       banks.
 */

#define FAMETP_SVOpenAboutWin       1001
/*
 * Open Server's About window.
 *
 * ftpm_Data1 <- The gadget number.
 *
 * Note: With FAMETP_SVOpenAboutWin you can open the About window of the
 *       Server.
 *
 *       ftpm_Data1 is the gadget number of the About window (0-10)
 *
 *       Possible defines (FAMEDefines.h):
 *
 *       #define SVAbout_Default        0
 *       #define SVAbout_QuitAbout      1
 *       #define SVAbout_MainCode       2
 *       #define SVAbout_DevelopTeam    3
 *       #define SVAbout_SupportTeam    4
 *       #define SVAbout_SupportBBS     5
 *       #define SVAbout_BetaTesters    6
 *       #define SVAbout_DistSites      7
 *       #define SVAbout_NotUsed1       8
 *       #define SVAbout_CopyRContact   9
 *       #define SVAbout_FAMEInfos      10
 */

#define FAMETP_SVCloseAboutWin      1002
/*
 * Close Server's About window.
 *
 * Note: FAMETP_SVCloseAboutWin can be used to close Server's About window.
 */

#define FAMETP_SVSaveWin            1003
/*
 * Save window positions.
 *
 * Note: FAMETP_SVSaveWin saves (fixes) the window positions
 *       and sizes of the Server main window and the Spy window.
 */

#define FAMETP_SVIconify            1004
/*
 * Iconify the Server.
 *
 * Note: With FAMETP_SVIconify it's possible to iconify the Server.
 */

#define FAMETP_SVUnIconify          1005
/*
 * Uniconify the Server.
 *
 * Note: With FAMETP_SVUnIconify it's possible to uniconify the Server.
 */

#define FAMETP_SVOpenTPCPWin        1006
/*
 * Open Server's ToolPort/CoProcess window.
 *
 * Note: With FAMETP_SVOpenTPCPWin it's possible to open Server's
 *       FAMECoProcess / FAMEToolPort window.
 */

#define FAMETP_SVCloseTPCPWin       1007
/*
 * Close Server's ToolPort/CoProcess window.
 *
 * Note: With FAMETP_SVCloseTPCPWin it's possible to close Server's
 *       FAMECoProcess / FAMEToolPort window.
 */

#define FAMETP_SVOpenAwayWin        1008
/*
 * Open Server's Away window.
 *
 * Note: With FAMETP_SVOpenAwayWin it's possible to open Server's
 *       Away window.
 */

#define FAMETP_SVCloseAwayWin       1009
/*
 * Close Server's Away window.
 *
 * Note: With FAMETP_SVOpenAwayWin it's possible to close Server's
 *       Away window.
 */

#define FAMETP_SVSmall              1010
/*
 * Minimize Server's main window.
 *
 * Note: With FAMETP_SVSmall it's possible to minimize Server's main window.
 */

#define FAMETP_SVFull               1011
/*
 * Maximize Server's main window.
 *
 * Note: With FAMETP_SVFull it's possible to maximize Server's main window.
 */

#define FAMETP_SVLCSelect           1012
/*
 * Switch to the "Last 10 Callers".
 *
 * Note: With FAMETP_SVLCSelect it's possible to switch to the
 *       "Last 10 Callers".
 */

#define FAMETP_SVLUSelect           1013
/*
 * Switch to the "Last 10 Uploaders".
 *
 * Note: With FAMETP_SVLCSelect it's possible to switch to the
 *       "Last 10 Uploaders".
 */

#define FAMETP_SVLDSelect           1014
/*
 * Switch to the "Last 10 Downloaders".
 *
 * Note: With FAMETP_SVLCSelect it's possible to switch to the
 *       "Last 10 Downloaders".
 */

#define FAMETP_SVGadgetBank         1015
/*
 * Changes the Gadget Bank / Retrieve the actual Gadget Bank number.
 *
 * ftpm_Data1 <- The Gadget Bank number (0-3).
 *
 * Note: With FAMETP_SVGadgetBank you can change the actual Gadget Bank
 *       of the Server.
 *
 *       ftpm_Data1 can be a value from 0 - 3 or better one of the following
 *       defines (FAMEDefine.h):
 *
 *       #define SVGadBnk_SystemBank1  0
 *       #define SVGadBnk_SystemBank2  1
 *       #define SVGadBnk_UserBank1    2
 *       #define SVGadBnk_UserBank2    3
 */

#define FAMETP_SVDropAllUser        1016
/*
 * Drop all online users.
 *
 * Note: Wanna be sadistic?
 *
 *       Ok, then use this command and you will drop all online users.
 */

#define FAMETP_SVEndAllNodes        1017
/*
 * End all running Nodes.
 *
 * ftpm_Data1 <- FALSE = without, TRUE = with online users.
 *
 * Note: With FAMETP_SVEndAllNodes you can quit all running Nodes,
 *
 *       If ftpm_Data1 is set to TRUE (not 0) all online users will
 *       be droped before the Nodes are quitting, else if ftpm_Data1
 *       is FALSE (0) only Nodes without users online will quit.
 */

#define FAMETP_SVExit               1018
/*
 * End the Server.
 *
 * ftpm_Data1 <- 0 = No Nodes, 1 = Nodes, but no user online, 2 = ever.
 *
 * Note: With FAMETP_SVExit you can quit the Server.
 *
 *       If ftpm_Data1 is set to 0 the Server will only quit if no
 *       Node is running, with 1 also if Nodes are running, but no
 *       user is online and with 2 also if users are online.
 */

#define FAMETP_SVBBSLocationPath    1019
/*
 * Retreive the BBS location path.
 *
 * ftpm_StringPtr1 -> A pointer to the BBS location path.
 *
 * Note: FAMETP_SVBBSLocationPath is used to retreive the BBS location path.
 *       ftpm_StringPtr1 will be a pointer to the BBS location path.
 */

#define FAMETP_SVBBSName            1020
/*
 * Retreive the BBS name.
 *
 * ftpm_StringPtr1 -> A pointer to the BBS name.
 *
 * Note: FAMETP_SVBBSName is used to retreive the BBS name.
 *       ftpm_StringPtr1 will be a pointer to the BBS name.
 */

#define FAMETP_SVBBSOrigin          1021
/*
 * Retreive the BBS origin.
 *
 * ftpm_StringPtr1 -> A pointer to the BBS origin.
 *
 * Note: FAMETP_SVBBSOrigin is used to retreive the BBS origin.
 *       ftpm_StringPtr1 will be a pointer to the BBS origin.
 */

#define FAMETP_SVSysOpName          1022
/*
 * Retreive the SysOp name.
 *
 * ftpm_StringPtr1 -> A pointer to the SysOp name.
 *
 * Note: FAMETP_SVSysOpName is used to retreive the SysOp name.
 *       ftpm_StringPtr1 will be a pointer to the SysOp name.
 */

#define FAMETP_SVNumOfNodes         1023
/*
 * Retreive the amount of Nodes.
 *
 * ftpm_Data2 -> The number of Nodes.
 *
 * Note: With FAMETP_SVNumOfNodes you will get in ftpm_Data2
 *       the number of Nodes.
 */

#define FAMETP_SVVersion            1024
/*
 * Retreive the Server version.
 *
 * ftpm_Data2 -> Server's version.
 *
 * Note: With FAMETP_SVVersion you will get the version string
 *       of the Server in ftpm_Data2.
 */

#define FAMETP_SVRevision           1025
/*
 * Retreive the Server revision.
 *
 * ftpm_Data2 -> Server's revision.
 *
 * Note: With FAMETP_SVRevision you will get the revision string
 *       of the Server in ftpm_Data2.
 */

#define FAMETP_SVStartedTime        1026
/*
 * Retreive the start time of the Server.
 *
 * ftpm_Data2 -> The start time of the Server.
 *
 * Note: With FAMETP_SVStartedTime you will get in ftpm_Data2 the
 *       time when the Server has been started (Unix time value).
 */

#define FAMETP_SVEndFAMEForce       1027
/*
 * Quit FAME in every case.
 *
 * Command will: - Drop all online users.
 *               - End all running Nodes.
 *               - End the Server.
 */

/*
 * -------------------------  Node Commands  ----------------------------
 */

#define FAMETP_Node_Start           2000
/*
 * Start a Node.
 *
 * ftpm_Node <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_Node_Start you can start a specific Node.
 */

#define FAMETP_Node_End             2001
/*
 * End a Node.
 *
 * ftpm_Node <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_Node_End you can quit a specific Node.
 */

#define FAMETP_Node_Suspend         2002
/*
 * Suspend a Node.
 *
 * ftpm_Node <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_Node_Suspend you can bring a Node in suspend mode.
 */

#define FAMETP_Node_Resume          2003
/*
 * Resume a Node.
 *
 * ftpm_Node <- The Node number / -1 for all Nodes.
 *
 * Note: With FAMETP_Node_Resume you can resume a suspended Node.
 */

#define FAMETP_Node_ChatOnOff       2004
/*
 * Turn the chat flag on/off.
 *
 * ftpm_Node  <- The Node number / -1 for all Nodes.
 * ftpm_Data1 <- The chat flag (TRUE = on, FALSE = off)
 *
 * Note: With FAMETP_Node_ChatOnOff you can turn the chat
 *       flag(s) of a Node or all Nodes on or off.
 */

#define FAMETP_Node_OpenInfo        2005
/*
 * Open the Info window for a Node.
 *
 * ftpm_Node <- The Node number.
 *
 * Note: With FAMETP_Node_OpenInfo you can open Server's Info window
 *       for a specific Node. If it's already open it will display
 *       then the data for the Node you specify.
 */

#define FAMETP_Node_CloseInfo       2006
/*
 * Closes the Info window.
 *
 * Note: With FAMETP_Node_CloseInfo you can close Server's Info window.
 */

#define FAMETP_Node_InfoTool        2007
/*
 * Start the InfoTool for a Node.
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_Node_InfoTool starts an installed Info tool for a
 *       specific Node.
 */

#define FAMETP_Node_UserEditor      2008
/*
 * Start the UserEditor for a Node.
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_Node_UserEditor starts the external UserEditor and loads
 *       the on the Node online user into it (if the UserEditor supports
 *       this - FAME UserEditor by SieGeL/tRSi does it) and if no user is
 *       online the SysOp.
 */

#define FAMETP_Node_SendString      2009
/*
 * Send (display) a string to a Node.
 *
 * ftpm_Node       <- The Node number / -1 for all Nodes.
 * ftpm_Data1      <- Display flag.
 * ftpm_StringPtr1 <- String pointer to the string to be displayed.
 *
 * Note: FAMETP_Node_SendString prints out a text (string) to a Node
 *       defined by ftpm_Node. Use -1 to display it to every online
 *       Node.
 *
 *       ftpm_StringPtr1 will be the pointer you have to use to add
 *       your string to.
 *
 *       ftpm_Data1 defines the display case.
 *
 *       Following display cases are possible:
 *
 *       #define  FAME_CONSER 0 - to display the text to console and serial
 *       #define  FAME_CON    1 - to display the text to console
 *       #define  FAME_SER    2 - to display the text to serial
 *
 *       On every other value passed with ftpm_Data1 FAME_CONSER will be
 *       used as default.
 */

/*
 * -------------------------  System Bank 1 Commands  -------------------
 */

#define FAMETP_SB1_SysOpLogin       3000
/*
 * Executes the command of System Bank 1 Gadget 1 (SysOpLogin)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_SysOpLogin executes the command of the System Bank 1
 *       Gadget 1. This will be a SysOpLogin on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_SysOpFastLogin   3001
/*
 * Executes the command of System Bank 1 Gadget 2 (SysOpFastLogin)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_SysOpFastLogin executes the command of the System Bank 1
 *       Gadget 2. This will be a SysOpFastLogin on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_LocalLogin       3002
/*
 * Executes the command of System Bank 1 Gadget 3 (LocalLogin)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_LocalLogin executes the command of the System Bank 1
 *       Gadget 3. This will be a LocalLogin on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_LocalFastLogin   3003
/*
 * Executes the command of System Bank 1 Gadget 4 (LocalFastLogin)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_LocalFastLogin executes the command of the System Bank 1
 *       Gadget 4. This will be a LocalFastLogin on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_ExitNode         3004
/*
 * Executes the command of System Bank 1 Gadget 5 (ExitNode)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_ExitNode executes the command of the System Bank 1
 *       Gadget 5. This will be an exit of the Node set in ftpm_Node.
 */

#define FAMETP_SB1_ExitNodeOffHook  3005
/*
 * Executes the command of System Bank 1 Gadget 6 (ExitNodeOffHook)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_ExitNodeOffHook executes the command of the System Bank 1
 *       Gadget 6. This will be an exit Off Hook of the Node set in ftpm_Node.
 */

#define FAMETP_SB1_RemoteLogin      3006
/*
 * Executes the command of System Bank 1 Gadget 7 (RemoteLogin)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_RemoteLogin executes the command of the System Bank 1
 *       Gadget 7. This will be a RemoteLogin on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_RemoteFastLogin  3007
/*
 * Executes the command of System Bank 1 Gadget 8 (RemoteFastLogin)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_RemoteFastLogin executes the command of the System Bank 1
 *       Gadget 8. This will be a RemoteFastLogin on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_IconifyNode      3008
/*
 * Executes the command of System Bank 1 Gadget 9 (IconifyNode)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_IconifyNode executes the command of the System Bank 1
 *       Gadget 9. This will iconify the Node set in ftpm_Node.
 */

#define FAMETP_SB1_SystemEditor     3009
/*
 * Executes the command of System Bank 1 Gadget 10 (SystemEditor)
 *
 * Note: FAMETP_SB1_SystemEditor executes the command of the System Bank 1
 *       Gadget 10. This will start the SystemEditor.
 */

#define FAMETP_SB1_InitModem        3010
/*
 * Executes the command of System Bank 1 Gadget 11 (InitModem)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_InitModem executes the command of the System Bank 1
 *       Gadget 11. This will initialize the Modem of the Node set in
 *       ftpm_Node.
 */

#define FAMETP_SB1_NodeChat         3011
/*
 * Executes the command of System Bank 1 Gadget 12 (NodeChat)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_NodeChat executes the command of the System Bank 1
 *       Gadget 12. This will be a SysOp chat on the Node set in ftpm_Node.
 */

#define FAMETP_SB1_DropUser         3012
/*
 * Executes the command of System Bank 1 Gadget 13 (DropUser)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB1_DropUser executes the command of the System Bank 1
 *       Gadget 13. This will be a drop user of the Node set in ftpm_Node.
 */

#define FAMETP_SB1_UserEditor       3013
/*
 * Executes the command of System Bank 1 Gadget 14 (UserEditor)
 *
 * Note: FAMETP_SB1_UserEditor executes the command of the System Bank 1
 *       Gadget 14. This will start the UserEditor.
 */

/*
 * -------------------------  System Bank 2 Commands  -------------------
 */

#define FAMETP_SB2_ReserveNode      3100
/*
 * Executes the command of System Bank 2 Gadget 1 (ReserveNode)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB2_ReserveNode executes the command of the System Bank 2
 *       Gadget 1. This will do a Reserve Node on the Node set in ftpm_Node.
 */

#define FAMETP_SB2_SpyNode          3101
/*
 * Executes the command of System Bank 2 Gadget 2 (SpyNode)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB2_SpyNode executes the command of the System Bank 2
 *       Gadget 2. This open the Spy window for the Node set in ftpm_Node.
 */

#define FAMETP_SB2_QuietNode        3102
/*
 * Executes the command of System Bank 2 Gadget 3 (QuietNode)
 *
 * ftpm_Node <- The Node number.
 *
 * Note: FAMETP_SB2_QuietNode executes the command of the System Bank 2
 *       Gadget 3. This will toggle the quiet flag of the Node set in
 *       ftpm_Node.
 */

#define FAMETP_SB2_NotUsed1         3103
/*
 * Executes the command of System Bank 2 Gadget 4 (NotUsed1)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_NotUsed2         3104
/*
 * Executes the command of System Bank 2 Gadget 5 (NotUsed2)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_NotUsed3         3105
/*
 * Executes the command of System Bank 2 Gadget 6 (NotUsed3)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_NotUsed4         3106
/*
 * Executes the command of System Bank 2 Gadget 7 (NotUsed4)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_NotUsed5         3107
/*
 * Executes the command of System Bank 2 Gadget 8 (NotUsed5)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_AllIconify       3108
/*
 * Executes the command of System Bank 2 Gadget 9 (AllIconify)
 *
 * Note: FAMETP_SB2_AllIconify executes the command of the System Bank 2
 *       Gadget 9. This will iconify all Nodes.
 */

#define FAMETP_SB2_NotUsed6         3109
/*
 * Executes the command of System Bank 2 Gadget 10 (NotUsed6)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_NotUsed7         3110
/*
 * Executes the command of System Bank 2 Gadget 11 (NotUsed7)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_SysOpAway        3111
/*
 * Executes the command of System Bank 2 Gadget 12 (SysOpAway)
 *
 * Note: FAMETP_SB2_SysOpAway executes the command of the System Bank 2
 *       Gadget 12. This will open the SysOp Away window.
 */

#define FAMETP_SB2_NotUsed8         3112
/*
 * Executes the command of System Bank 2 Gadget 13 (NotUsed8)
 *
 * Note: Gadget not used yet.
 */

#define FAMETP_SB2_NotUsed9         3113
/*
 * Executes the command of System Bank 2 Gadget 14 (NotUsed9)
 *
 * Note: Gadget not used yet.
 */

/*
 * -------------------------  User Bank 1 Commands  ---------------------
 */

#define FAMETP_UB1_Gadget1          3200
/*
 * Executes the command installed on User Bank 1 Gadget 1
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget2          3201
/*
 * Executes the command installed on User Bank 1 Gadget 2
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget3          3202
/*
 * Executes the command installed on User Bank 1 Gadget 3
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget4          3203
/*
 * Executes the command installed on User Bank 1 Gadget 4
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget5          3204
/*
 * Executes the command installed on User Bank 1 Gadget 5
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget6          3205
/*
 * Executes the command installed on User Bank 1 Gadget 6
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget7          3206
/*
 * Executes the command installed on User Bank 1 Gadget 7
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget8          3207
/*
 * Executes the command installed on User Bank 1 Gadget 8
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget9          3208
/*
 * Executes the command installed on User Bank 1 Gadget 9
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget10         3209
/*
 * Executes the command installed on User Bank 1 Gadget 10
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget11         3210
/*
 * Executes the command installed on User Bank 1 Gadget 11
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */
                                    
#define FAMETP_UB1_Gadget12         3211
/*
 * Executes the command installed on User Bank 1 Gadget 12
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget13         3212
/*
 * Executes the command installed on User Bank 1 Gadget 13
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB1_Gadget14         3213
/*
 * Executes the command installed on User Bank 1 Gadget 14
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

/*
 * -------------------------  User Bank 2 Commands  ---------------------
 */

#define FAMETP_UB2_Gadget1          3300
/*
 * Executes the command installed on User Bank 2 Gadget 1
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget2          3301
/*
 * Executes the command installed on User Bank 2 Gadget 2
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget3          3302
/*
 * Executes the command installed on User Bank 2 Gadget 3
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget4          3303
/*
 * Executes the command installed on User Bank 2 Gadget 4
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget5          3304
/*
 * Executes the command installed on User Bank 2 Gadget 5
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget6          3305
/*
 * Executes the command installed on User Bank 2 Gadget 6
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget7          3306
/*
 * Executes the command installed on User Bank 2 Gadget 7
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget8          3307
/*
 * Executes the command installed on User Bank 2 Gadget 8
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget9          3308
/*
 * Executes the command installed on User Bank 2 Gadget 9
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget10         3309
/*
 * Executes the command installed on User Bank 2 Gadget 10
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget11         3310
/*
 * Executes the command installed on User Bank 2 Gadget 11
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget12         3311
/*
 * Executes the command installed on User Bank 2 Gadget 12
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget13         3312
/*
 * Executes the command installed on User Bank 2 Gadget 13
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

#define FAMETP_UB2_Gadget14         3313
/*
 * Executes the command installed on User Bank 2 Gadget 14
 *
 * Note: If on this gadget a command has been installed by the SysOp it will
 *       be executed.
 */

/*
 * ###################### COMMAND DEFINES END ###########################
 */

/*
 * -------------------------  Command Template  -------------------------
 */

//#define FAMETP_
/*
 *
 *
 *
 *
 *
 */

#endif /* FAME_FAMETOOLPORTCOMMANDS_H */

