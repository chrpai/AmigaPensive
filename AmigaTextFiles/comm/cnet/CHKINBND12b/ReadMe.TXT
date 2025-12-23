
                 CheckInbound-12 by Charles Johnston
              Sysop of Upper Room BBS (602) 771-8971
                        HOME of FAITH NET
                      May 16,1994   3:25PM

               NOW SUPPORTS FIDO NODEDIFF UPDATES!

   --------------------------------------------------------------------
    Written for systems running FIDO, CLINK, and Faith-Net. All 3
    or any combination thereof or just one of these three. Permission 
    granted to edit code to support other/extra networks. 

  1.Detects .tic files BEFORE tossing any mail and processes them.
    If TicCommand variable is "0" (default) then useless .tic files are 
    DELETED from your inbound. Many have asked for this. Ask no more!

  2.Checks Inbound: for messages or netmail and issues import command

  3.REPLACES ALL old nodelists with new ones and runs Traplist. 
    Applies Nodediff.axx file to existing FIDO nodelist.

  4.REPLACES old echo lists with new ones. (Faith-Net and Clink only)

  5.RECORDS all nodelist and echolist processing in sysdata:log/maint

  6.Deletes all '0' block files in outbound directory. I hated those! 

  7.Deletes files in Mail:bad/ not stamped as 'today' files. 

  8.Deletes files in Mail:BakIn not stamped as 'today' files.
    (If you do not backup your incoming to Mail:BakIn this is skipped.)

  9.Deletes the 60 byte 'ZedZip' pkts from inbound.
 
 10.Clears Inbound: of echolists and nodelists after processing is done
    IF you do not tic out. Otherwise leaves them for TICK prg to process.
    
                                                                   
   --------------------------------------------------------------------

                      How to install checkinbound
                     -----------------------------
    (1) Check that only the most recent nodelist AND echolist for each 
        network is in NODELIST: directory. For FIDO just be sure you
        have the current nodelist and nodediff files in Nodelist:

        From now on Checkinbound will maintain the NODELIST: directory
        REPLACING  the old nodelists AND echolists with the new ones.
        It will then run Traplist on each new nodelist and nodediff
        file for you.
    
    (2) Copy the file 'checkinbound' into 'Mail:scripts/' directory
        or wherever you intend to run it from.
    
    (3) Edit the variables at the beginning of the program code
        to conform to YOUR system's configuration and operation.

    (4) Add the following line at the bottom of your Trapdoor.cfg
        file exactly as shown with quotes included.

        AfterSession "rx mail:scripts/checkinbound"

        Using the aftersession command will prevent incoming calls
        from being answered this one line till tossing is done. If you 
        don't like this run checkinbound as an event or through a scheduler.

    (5) Reload your Trapdoor.cfg with either ALT-C command from
        the trapdoor screen, turning off Trapdoor and restarting
        it, or rebooting your Amiga.
 
    (6) Now after every Trapdoor session 'checkinbound' will
        execute. If inbound is empty it promptly exits. Otherwise
        it performs the functions listed at the top of this DOC
        only as needed.

    (7) For information on Faith Network contact the bbs listed above.
        VOICE SUPPORT and a network startup archive that will
        have you networking in about an hour! It's easy...do it!