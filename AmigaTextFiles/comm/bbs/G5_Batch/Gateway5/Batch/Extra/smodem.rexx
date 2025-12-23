/* 
   SModem.Rexx (for TERM) Version 1.0 - Based on Hydra.REXX version 0.9
*/

 OPTIONS results
 OPTIONS FAILAT 6

/* Get some TERM-Attributes */
 GETATTR PATHPREFS.BINARYDOWNLOADPATH VAR dlpath
 GETATTR TERM.SCREEN VAR screenname
 GETATTR SERIALPREFS.DEVICENAME VAR devicename
 GETATTR SERIALPREFS.UNIT VAR serunit
 GETATTR SERIALPREFS.BAUDRATE VAR baudrate
 GETATTR PATHPREFS.BINARYUPLOADPATH   VAR ulpath

/* convert the dlpath */
   IF (LENGTH(dlpath) ~== 0) & (RIGHT(dlpath,1) ~= ':') THEN
     dlpath = INSERT('/',dlpath,LENGTH(dlpath))

/* Request the files*/
   files.count = -1
   REQUESTFILE TITLE '"Select files to upload"' PATH ulpath PATTERN '~(#?.INFO)' MULTI STEM files     

/*  Check the result: CANCEL = open "Start Requester" */
   IF (rc == 0) & (files.count >= 0) THEN  
   DO

/*make upload-batchfile*/
     CALL OPEN('liste','T:ul','w')
     SAY files.count
     DO i = 0 TO files.count - 1
       CALL WRITELN('liste',files.i)   
     END
     CALL CLOSE('liste') 
   

/* Start Hydracom and delete batchfile*/

     PROCESSIO OFF
     address COMMAND 'SMODEM DEVICE 'devicename' UNIT 'serunit' BAUD 'baudrate' CTSRTS ULLIST T:UL DLPATH 'dlpath' PUBSCREEN 'screenname
     PROCESSIO ON

     ADDRESS COMMAND 'DELETE T:UL'
   END
   ELSE
   DO
     PROCESSIO OFF
     address COMMAND 'SMODEM DEVICE 'devicename' UNIT 'serunit' BAUD 'baudrate' CTSRTS DLPATH 'dlpath'  PUBSCREEN 'screenname
     PROCESSIO ON
   END
