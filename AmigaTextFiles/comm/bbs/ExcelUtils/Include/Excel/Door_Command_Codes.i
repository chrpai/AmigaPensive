; EXCELSIOR! Door Interface Communication Command Codes

; Commands 0 to 100 are system private. No usable function is performed by
; these commands for a door interface. Commands under 100 are also handled
; differently than commands above 100, and you risk the possibility of 
; a SYSTEM FAILURE if you attempt to use commands below 100.



CMD_CCLOSE_DOWN		Equ	500	; Shuts down the interface

CMD_GETLINE		Equ	100	; Prompts the program to get a line
CMD_GETCH		Equ	101	; Gets a character, will wait
CMD_IRGETCH		Equ	102	; Gets a character, returns 0 if
					; a character isn't waiting.
CMD_WHATIS		Equ	103	; Returns a pointer to the variable
					; requested. All pointers returned by
					; WhatIs() point to public memory
CMD_PRINT_STRING	Equ	104	; Prints out a null terminated string
CMD_CON_WRITE		Equ	105	; Prints out a null terminated string
					; to the console only (handy for debug)
					; NO IPL ALLOWED TO CON_WRITE
CMD_RWUBUF		Equ	106	; R/W an account into the User Buffer
CMD_ACTIVATE_EDITOR	Equ	107	; Starts up the editor
					; Returns ED_SAVE for saving, and
					; ED_ABORT for aborting
CMD_CHANGE_WHAT		Equ	108	; Changes what the user is doing
CMD_CHANGE_WHERE	Equ	109	; Changes where the user is
CMD_GLOBAL_COMMANDS	Equ	110	; Execute a global command
CMD_PRINT_FILE		Equ	111	; Reads a file to the user
CMD_GET_DATE		Equ	112	; Fills a 'MyDateStamp' with the
					; current time and date
CMD_PRINT_DATE		Equ	113	; Returns a pointer to a null
					; terminated string of a 'MyDateStamp'
					; passed to this function
CMD_CONFIRM_ID		Equ	114	; Returns the ID number of a user
					; by handle or ID number
CMD_SEND_MAIL		Equ	115	; Sends Mail to User ID
CMD_EXPORT_FILE		Equ	116	; A Simple ASCII dump of editor to file
CMD_IMPORT_FILE		Equ	117	; Load a file into editor
CMD_IS_LOGOFF		Equ	118	; Is the user logging off
CMD_IS_SYSOP		Equ	119	; Is the user a sysop
CMD_PARSE_COMMAND	Equ	120	; Parse string, return result code
CMD_SER_WRITE		Equ	121	; Serial Write Command
					; Arg1 = buffer, Arg2 = buffer length
					; This command exists mainly for our
					; AUX: handler
CMD_WAIT_CHAR		Equ	122	; Wait x number of micros for a char
					; then return. Arg1 = micros, result1
					; equals a character or a null char
CMD_ACTION_READ		Equ	123	; Read from list - another AUX: handler
					; command that is useless for most
					; doors. Arg1 is buffer to fill, Arg2
					; is number of bytes to read
CMD_ACTION_SCMODE	Equ	124	; Put the BBS in RAW: mode
CMD_DOOR_PORT		Equ	125	; Assign address of ecom port
CMD_QUOTE_MACHINE	Equ	126	; Call the Quote Machine (For VisEd's
CMD_START_CHATMODE	Equ	127	; Start Chat Mode !!BECAREFUL!!
CMD_CLEAR_EDITOR	Equ	128	; Clears the editor buffer
CMD_MARK_FILE		Equ	129	; Marks a file for downloading
CMD_SELECT_PROTO	Equ	130	; Selects protocols
CMD_DOWNLOAD		Equ	131	; Starts a download
CMD_OPEN_SCREEN		Equ	132	; Opens a screen/window
CMD_SER_PRINT		Equ	133	; Print only to the serial!
CMD_PRINT_MENU		Equ	134	; Prints menu file in correct lang directory
CMD_ALTER_TIME		Equ	135	; Alters users time, or returns time online
CMD_OPEN_LOCK		Equ	136	; Opens lock
CMD_PUT_CH		Equ	137	; Puts a char to serial/modem
