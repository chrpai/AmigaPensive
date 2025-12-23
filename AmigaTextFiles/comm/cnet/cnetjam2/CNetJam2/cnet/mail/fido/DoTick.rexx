/**************************************************************************\
                $VER: DoTick, v2.1 (28-Jan-95) by Dotoran!

   A fully automated .TIC processor, which is used to copy files found in
   your INBOUND: directory to the appropriate UD Bases or to BADFILES if
   the needed base is NOT found. See further comment below...
\**************************************************************************/
options results;dfile='sysdata:log/poll'
time='[0m[[33m'date()' 'time()'[0m> '

call open(f1,dfile,substr('wa',exists(dfile)+1,1))
a=addlib('rexxsupport.library',0,-30,0);say

/* The following variable "areas" should contain a list of ALL DEFINED FILE
   Areas you have on your board that you wish the TIC files to be processed
   for. Each name should be separated by at least ONE space. You will also
   need to create an ASSIGN for each base, using the SAME NAME as given in
   this variable(for instance, FIDONEWS is assigned as FIDONEWS: etc). You
   should also create a "BADFILES" Area and Assign, as that is where all
   unknown files will be copied to. Lastly, from within CNet, enable Adopt
   Orphans at AutoMaint and the files will be officially made part of CNet,
   or else skip this step if you won't need it(aka: BADFILES, etc.)
*/

areas = "NODEDIFF FIDONEWS HUBSTUFF"

killorphans = 1   /* Kill Orphaned TIC Files? (0=No, 1=Yes)               */
                  /* .TIC files that no longer have a parent file.        */

f=showdir('inbound:','f');files=words(f)

if index(upper(f),'TIC')=0 then do;say "No TIC files to process..."
  call LOG("No TIC files to process...");exit;end

do i=1 to files;a=upper(word(f,i))
  if right(a,4)='.TIC' then do;call open(f2,'inbound:'||a,'r')
    do j=1 to 5;l.j=readln(f2);end j;call close(f2)
    area=strip(word(l.1,2),,x2c(0d));from=strip(word(l.3,2),,x2c(0d))
    file=strip(word(l.4,2),,x2c(0d));desc=strip(subword(l.5,2),,x2c(0d))
    b=exists('inbound:'||file)
    if b=1 then do;say "Processing TIC:"a;call LOG("Processing TIC:"||a)
      if index(' 'areas' ',area)=0 then do;area='BADFILES'
        call LOG("Unknown File Area. Copying to BADFILES...")
        say "Unknown File Area... Copying to BADFILES...";end
      comment=desc
      if area='NODEDIFF' then comment=desc||", from "||from||"."
      if area='FIDONEWS' then comment=desc
      call LOG("Copying INBOUND:"||file||" to "||area||":")
      say "Copying INBOUND:"file" to "area":"
      address command "copy inbound:"||file||" "||area||":"
      call LOG("Creating file comment (For CNet's Short Description)...")
      say "Creating file comment (For CNet's Short Description)..."
      address command "filenote "||area||":"||file||' "'||comment||'"'
      call LOG("Cleaning up INBOUND: directory...")
      say "Cleaning up INBOUND: directory..."
      address command "delete inbound:"a
      address command "delete inbound:"file
    end
    if b=0 then do;say
      if killorphans=0 then do;say "Saved Orphaned TIC File: "a
        call LOG("Saved Orphaned TIC File: "||a);end
      if killorphans=1 then do;say "Killed Orphaned TIC File: "a
        call LOG("Killed Orphaned TIC File: "||a)
        address command "delete inbound:"||a;end
    end
  end
end i
call LOG("TIC Processing Finished!");call close(f1)
exit

LOG:;parse arg text;call writeln(f1,time||text);return
/** Last Edited: 28-Jan-95 ************************************************\
\****************************************** FRONTIERS BBS (716)/823-9892 **/
