
/*
 * FAME CoProcess Commands
 * ~~~~~~~~~~~~~~~~~~~~~~~
 *
 * $VER: "FAMECoProcCommands.h" v1.5
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

#ifndef FAME_FAMECOPROCCOMMANDS_H
#define FAME_FAMECOPROCCOMMANDS_H

/*
 * FAMECoProc command defines (struct FAMECoProcessMsg -> fcpm_Command)
 */

/*
 * ##################### COMMAND DEFINES BEGIN ##########################
 */

#define FAMECP_Update               0
/*
 * Refreshes the datas from your FAMECoProcessMsg structure internal
 * and refreshes the FAMECoProc Info Windows if they are open.
 */

#define FAMECP_RequestInstall       1
/*
 * Install a new request.
 *
 * fcpm_Req      <- A filled struct FAMECoProcRequest.
 *
 * Note: Use this command to inform the Server that you want to install
 *       a CoProc time sheduller start request.
 *
 *       You need to handle in fcpm_Req theallocated and filled
 *       struct FAMECoProcRequest. After receiving the reply you can
 *       free it or change it for another request and send it again.
 *
 * Note: You also have to set the bit flag FCPR_ACTIVATED of fcpr_Flag1
 *       in the struct FAMECoProcRequest to activate the request:
 *
 *       MyFAMECoProcRequest -> fcpr_Flag1 |= FCPR_ACTIVATED;
 *
 * Note: Use FAMECP_RequestInstall if your CoProc has benn started with
 *       the argument SERVERSTART (Called from FAME:FAMEStartLate/#? or
 *       FAME:Settings/FAMEStartLate.bat).
 *
 * Note: Set unique values in the field fcpr_ActionFlag of the struct
 *       FAMECoProcRequest in fcpm_Req if you want to install more than
 *       one request now or later. Use fcpr_ActionFlag later to identify
 *       your multiple requests.
 *
 *       DO NOT USE VALUES BETWEEN -100 - +100 !!! THEY ARE RESERVED.
 *
 *       See FAMEDefine.h:
 *
 *       #define FAMECPAF_MINIMUM    -101
 *       #define FAMECPAF_MAXIMUM    +101
 */

#define FAMECP_RequestEdit          2
/*
 * Request the struct FAMECoProcRequest.
 *
 * fcpm_ReqIdentify <- The argument identify number.
 * fcpm_Req         -> The filled struct FAMECoProcRequest.
 *
 * Note: Use this command only if your CoProc has been started with the
 *       arguments SERVERCPEDIT and SERVERCPID=<argument identify number>.
 *
 *       Use the number from SERVERCPID and put it into fcpm_ReqIdentify
 *       and then send the command FAMECP_RequestEdit.
 *
 *       With the reply of this command you will receive in fcpm_Req
 *       a pointer to the struct FAMECoProcRequest your CoProc now can
 *       edit/let edit the SysOp via the GUI the settings for the CoProc
 *       and/or the start times of the request.
 *
 * Note: The bit flag FCPR_ACTIVATED of fcpr_Flag1 in the struct
 *       FAMECoProcRequest will be deleted with this command to disable
 *       the request until it will be reinstalled (FAMECP_RequestEdited).
 */

#define FAMECP_RequestEdited        3
/*
 * Reinstall the struct FAMECoProcRequest.
 *
 * fcpm_ReqIdentify <- The argument identify number.
 * fcpm_Req         <- The filled struct FAMECoProcRequest.
 *
 * Note: Use this command only if your CoProc has been started with the
 *       arguments SERVERCPEDIT and SERVERCPID=<argument identify number>
 *       and you have called FAMECP_RequestEdit succesfully before.
 *
 *       Use the number from SERVERCPID and put it into fcpm_ReqIdentify.
 *       Let fcpm_Req point to the struct FAMECoProcRequest you've got
 *       with FAMECP_RequestEdit and send it back to the Server.
 *       Never free fcpm_Req, because the Server is the owner!
 *
 * Note: The bit flag FCPR_ACTIVATED of fcpr_Flag1 in the struct
 *       FAMECoProcRequest has to be set again to reactivate the request.
 *       So please don't forget this, else the request will stay disabled.
 *
 *       MyFAMECoProcRequest -> fcpr_Flag1 |= FCPR_ACTIVATED;
 */

#define FAMECP_RequestAction        4
/*
 * Request the struct FAMECoProcRequest to start an action.
 *
 * fcpm_ReqIdentify <- The argument identify number.
 * fcpm_Req         -> The filled struct FAMECoProcRequest.
 *
 * Note: Use this command only if your CoProc has been started with the
 *       arguments SERVERCPACTION and SERVERCPID=<argument identify number>.
 *
 *       Use the number from SERVERCPID and put it into fcpm_ReqIdentify
 *       and then send the command FAMECP_RequestAction.
 *
 *       You will receive in fcpm_Req the struct FAMECoProcRequest where
 *       you can now decide what to do. If your CoProc is only a single
 *       action toll then just start whatever your CoProc does, but if it's
 *       a multifuction CoProc then decide with the field fcpr_ActionFlag
 *       out of the struct FAMECoProcRequest what action you should do now.
 *
 *       fcpr_ActionFlag has to be filled by your CoProc on installing
 *       a request (with FAMECP_RequestInstall) with any value you like to
 *       differ between your installed requests (Argument SERVERSTART).
 *
 *       DO NOT USE VALUES BETWEEN -100 - +100 !!! THEY ARE RESERVED.
 *
 *       See FAMEDefine.h:
 *
 *       #define FAMECPAF_MINIMUM    -101
 *       #define FAMECPAF_MAXIMUM    +101
 */

#define FAMECP_RemoveRequests       5
/*
 * Remove one or all requests.
 *
 * fcpm_Data1 <- TRUE = All, FALSE = Single
 * fcpm_Data3 <- ActionFlag
 *
 * Note: With FAMECP_RemoveRequest you are able to remove one are all
 *       requests your CoProc tools has set.
 *
 *       Because of the fact that the fcpm_CoProcName must be a unique
 *       one it will be used by the Server to identify your CoProc
 *       tool's request(s).
 *
 *       Set fcpm_Data1 to TRUE if you want to remove all requests
 *       of your CoProc tool, use FALSE and fcpm_Data3 to remove only
 *       a special request if you have set more than one request of course.
 *
 *       fcpm_Data3 will be the number you have used with FAMECP_RequestInstall
 *       in the field fcpr_ActionFlag of struct FAMECoProcRequest.
 */

/*
 * ###################### COMMAND DEFINES END ###########################
 */

/*
 * -------------------------  Command Template  -------------------------
 */

//#define FAMECP_
/*
 *
 *
 *
 *
 *
 */

#endif /* FAME_FAMECOPROCCOMMANDS_H */

