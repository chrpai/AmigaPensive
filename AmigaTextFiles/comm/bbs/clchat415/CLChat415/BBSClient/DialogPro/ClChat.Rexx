/**/

PARSE ARG NICK LENGTH TIME NAME

/*** Controllo se il server e' attivo ***/
IF ~SHOW(p,"ChatServer.cmd") THEN DO
                             ADDRESS COMMAND "RUN >NIL: AmiTCP:Bin/ClChatServer"
                             /* Wait for loading... */
                             ADDRESS COMMAND 'C:WaitForPort ChatServer.cmd'
                             ADDRESS COMMAND 'RUN >NIL: AmiTCP:Bin/ChatBot Pier "UNIVISION''s robot"'
                             END

ADDRESS COMMAND 'AmiTCP:Bin/ClChatANSI' NICK '"'STRIP(NAME)'"' 'LINES' LENGTH 'RT' TIME
