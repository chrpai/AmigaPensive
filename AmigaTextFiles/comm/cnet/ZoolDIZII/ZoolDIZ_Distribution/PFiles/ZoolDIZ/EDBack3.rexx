/*

This file is PART of: ZoolDIZ II by Zool 'N' Cool / EMbASSy (Lauri Ahonen)

 COPYRIGHT AT THE AUTHOR (Zool 'N' Cool / EMbASSy (Lauri Ahonen))

*/

options results;getuser 23;port=result;if exists('ram:T/Editor'port) then;do;call open file,('ram:T/Editor'port),'R';ed=readln(file);call close file;end;if ~exists('ram:T/Editor'port) then exit;setobject ed;putuser 1100645;exit

 /**************************************************************************/
  /* ZoolDIZ II - Zool 'N' Cool / EMbASSy (lauri ahonen)                  */
   /*                                               |          \o/ Wopee */
    /*  This IS a copyrighted program, hands off!  -+-  da Zool |   --- */
     /*   (c)opyright 1994-1996                     |          / \     */
      /****************************************************************/
