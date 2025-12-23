/********************************************************
**
** This tries to goto the specified URL depending on which
** browser you have, either Voyager or IBrowse, which needs
** to be running at the time.
**
*/

options results
parse arg URLtoGO PADD


TaskRoster = upper( show(p) )

if Index( TaskRoster, "VOYAGER" ) > 0 then do

		address VOYAGER;OpenURL URLtoGO
		
end
	
else if Index( TaskRoster, "IBROWSE" ) > 0 then do
	
		address IBROWSE;GotoURL URLtoGO
		
end

/* NOTE: The public AWeb demo 3 does not have an AREXX port */
/* so this is un-tested!                                    */

else if Index( TaskRoster, "AWEB.1" ) > 0 then do

		address AWEB.1;OPEN URLtoGO
		
end

else say "Couldn't find a browser running!"
