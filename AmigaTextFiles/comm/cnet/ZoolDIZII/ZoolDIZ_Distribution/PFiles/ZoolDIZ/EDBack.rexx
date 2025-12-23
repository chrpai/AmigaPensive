/*

This file is PART of: ZoolDIZ II by Zool 'N' Cool / EMbASSy (Lauri Ahonen)

 COPYRIGHT AT THE AUTHOR (Zool 'N' Cool / EMbASSy (Lauri Ahonen))

*/

options results;parse arg port;if right(port,1)='|' then;do;port=left(port,length(port)-1);fuN=1;end;ADDRESS VALUE "CNETREXX"port;if fun=1 then;do;call open file,('ram:T/Editor'port),'W';call writeln file,1;call close file;exit;end;address command 'wait 4';if ~exists('ram:_ZDIZtemp'port) then;do;setobject 1;putuser 1100645;end;exit

 /**************************************************************************/
  /* ZoolDIZ II - Zool 'N' Cool / EMbASSy (lauri ahonen)                  */
   /*                                               |          \o/ Wopee */
    /*  This IS a copyrighted program, hands off!  -+-  da Zool |   --- */
     /*   (c)opyright 1994-1996                     |          / \     */
      /****************************************************************/
