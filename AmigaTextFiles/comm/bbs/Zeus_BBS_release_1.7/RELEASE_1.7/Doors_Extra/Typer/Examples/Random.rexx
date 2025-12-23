/* 
-------------------------------------------------------------------------
This is an AREXX script. You should have REXXMAST running before executing
the script. Execution of this script will need the RX command.

This Example script will copy one of the 8 English wordfiles over the
default words.txt file.

To change the words.txt file after every caller, simply add the
following line to your 'DLGConfig:Batch/Logout.dlgbatch' file:

  RX Doors:Typer/Examples/Random.rexx


                                                        Wim Rumping
-------------------------------------------------------------------------
*/

Defname="Doors:Typer/Wordfiles/Words.txt"

number=random(1,8,time('s'))	/* Pick a number between 1 - 8 		*/
				/* If you plan to use this script with	*/
				/* a time scheduler such as TPTCron,	*/
				/* you should change the 's' into	*/
				/* something else, because it uses the  */
				/* seconds since 00:00 each day as a	*/
				/* random, which will be the same when  */
				/* using a scheduler.			*/

address command "copy "||Defname||".Eng_"||number||" "||Defname

exit
