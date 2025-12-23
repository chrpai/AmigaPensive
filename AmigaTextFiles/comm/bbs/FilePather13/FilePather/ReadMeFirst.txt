 [Please read the whole textfile!]

 	     Fast introduction to the world of FilePather!
             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    FilePather is utility which creates a history within an archive
 or a textfile. Each time FilePather is used it will update the list
 with one line of text which is configurable by the user. An example list
 might look something like this:    (line wrap should be turned off :)

 User	John Smith		Dallas,Texas		16:43 18-03-95 +1-214-UNPUBL
 BBS	Wedlock			Halmstad,Sweden		19:54 18-03-95 +46-035-186514
 User	Karl Olsson		Stockholm,Sweden	21:23 22-03-95 +46-08-UNPUBL
 BBS	ANetDevelopment		Stockholm,Sweden	22:23 23-03-95 +46-08-59140072
 User   Johan Torin		Halmstad,Sweden		03:43 24-03-95 +46-035-UNPUBL

 Or, if you are more fond of the 'elite' way:

 Board	Freeside		Webber/WSK		12:33 02-03-95 +1-212-822-2653
 Board	Assembly Hall		Mosqito/Defiance	02:43 03-03-95 +46-044-240816
 Trader	Armitage		Ice			05:28 03-03-95 +49-0232-UNPUBL
 Board	Inside Out		Cateye/Insane		22:44 04-03-95 +46-0346-58697|59245	Modem="28.8VFC | 16.8DS"


 In standard file archives, FilePather will add a file namned 'FilePath.lst',
 and in text files, FilePather will add a '@' node, like this:

 @Begin_FilePath.lst
 User	Karl Olsson		Stockholm,Sweden	21:23 22-03-95 +46-08-UNPUBL
 BBS	Wedlock			Halmstad,Sweden		19:54 18-03-95 +46-035-186514
 @End_FilePath.lst


				FPBase
				------

   FPBase is a small database utility which works automagically together
 with FilePather. It will keep track on which BBS's/Users that have been
 found, and also how many times they have been discovered in the
 FilePath.lst. FPBase is also capable of creating toplists over the X most
 found entries.

 It can be used in two ways. Either you always run it in the background,
 which is recommended for BBS usage, or you run it from you User-startup
 or likewise to process the data from FilePather each boot.

 Either way you probably won't notice it until you needs it!


                              OH, NOOO!
                              ---------

   Here could be the moment to warn you: neither FilePather or FPBase has
 any graphical interface! Both are totally CLI interfaced. BUT! The
 installation script will fix most of the work for you. Read below.


		              Installation
                              ------------

    Just doubleclick the install icon. The installation script will ask
 you for your name, number etc. and store this information as environment
 variables (i.e as files in ENV:FilePather/).
 The installation script requries that Commodores Installer utility is
 somewhere in the path. Check so it's installed.

 Note for 'Users': if you select any other location than 'C:' for the
 executables, you must edit the 'ProcessAllFiles' script file accordingly
 to your choice. So, keep them in 'C:' to spare some work.

 The script will the help the ordinary user with the installing of a simple
 method of getting all your downloaded files FilePathed.

 Sysops are presumed to have some knowledge of their system, but if you can
 add a filechecker to the upload procedure, then all should be fine!
 Look at the examples for some BBS programs which are included.

 A word about the installation: although it is POSSIBLE to install FilePather
 by hand, it is NOT recommended! It's MUCH simpler to use the installation
 script! Thrust me on this one! There have been too many users already, who
 has neglected to run the script, and all sort of errors has occured (nothing
 dangerous though). Still suspicious? Read next section:


		            Uninstallation(!)
                            -----------------

 Delete FilePather and FPBase from the choosen directory (C: ).
 Delete the FilePather directory in ENVARC:
 Remove all references to FPBase and Filepather from your User-startup.
 In BBS mode, remove FilePather from the upload procedure.
 In User mode, restore default download dir to your old dir, and delete
  'ProcessAllFiles' and icon (S: ).



                       Upgrading from previous versions
                       --------------------------------

 Well, users of FilePather 1.0 has nothing to worry about. Just run the
 installation script as stated above. However, you should delete the file
 'ENV:FilePather/Number' before reinstalling. The definition for the number
 has become more strict since version 1.0, and has therefore to be reentered.
 As from version 1.3 should the number contain country code and area code
 irrespectively of if you HAVE a number or not. If you by any reason feels
 that your local number is nobody elses business, then set the local number
 to 'UNPUBL'. Hmm, anyway: just follow the instructions!


                   Documentation - What do I need to read?
                   ---------------------------------------

   In the 'Docs' drawer you will find these files, listed in priority:

  FilePather.guide       - This guide file contains all docs below marked
                           with a '*'.

 * FilePather.doc         \_ The script will take care of most of the setup.
 * FPBase.doc             /  Look here incase of any problems.
 * Tricks.txt             - For the 'advanced' FP user.
 * Custom_Fields.txt      - If you want to place some own fields on the line.
   Text_Encapsulation.txt - Both users and programmers!
   FilePather_Fields.txt  - Mostly programmers, but have a go.
 * USG.txt                - Information about USG, and how and why to join!
 * USGNet.txt             - Information about USG's own network.

