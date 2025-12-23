;
;	$VER: AMMS ZConnect Upload 1.00 [English] 17-08-1995
;
;
;

;
;	Starttext
;
STARTTEXT = "^JZCONNECT UPLOAD^J---------------^J"
;
;	Query: Start upload ?
;
MENUTEXT = "Start upload ? Y#left#"
;
;	Key for upload
;
KEY_UPLOAD = "JY^M"
;
;	Key for abort
;
KEY_ABORT = "N"
;
;	Statusmessage: Start upload
;
UPLOADTEXT = "^J^JUPLOAD START ..."
;
;	Statusmessage: Decrunching messages
;
UNPACKTEXT = "^J^JDecrunching messages ..."
;
;	Statusmessage: Archive successfully decrunched, importing messages
;
IMPORTTEXT = " finished !^J^JImporting messages ..."
;
;	Statusmessage: Import finished
;
IMPORTENDTEXT = " finished !^J"
;
;	Statusmessage: Starting parallel import on port IMPORT
;
PARTEXT = "^JStarting parallel import on port IMPORT^J"
;
;	Statusmessage: Upload aborted, try again ?
;
RETRYTEXT = "^JUpload aborted, try again ? Y#left#"
;
;	Key for upload
;
KEY_RETRY = "JY^M"
;
;	Key for abort
;
KEY_BREAK = "N"
;
;	Statusmessage: Command error
;
ERRORTEXT = "^JCommand error --> Please contact sysop !"
