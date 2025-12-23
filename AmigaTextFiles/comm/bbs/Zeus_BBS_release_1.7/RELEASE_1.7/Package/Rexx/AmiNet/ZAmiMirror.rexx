/*
** ZAmiMirror.rexx 1.4 - Zeus BBS Aminet Mirror
**
** Written by Alex May (c)1995 Zeus Developments
*/

Options Results
Address 'ZEUS'

/*
** Beginning of user variable section - See ZAmiMirror.doc
*/

Base      = 'Files:'

AmiAddr   = 'wuarchive.wustl.edu'
SysAddr   = 'wuarchive.wustl.edu'

/*AmiAddr   = 'ftp.uni-paderborn.de'
SysAddr   = 'ftp.uni-paderborn.de'*/

/*AmiAddr   = 'sunsite.doc.ic.ac.uk'
SysAddr   = 'phoenix.doc.ic.ac.uk'*/

/*
AmiAddr   = 'ftp.netnet.net'
SysAddr   = 'laslo.netnet.net'
*/
AmiDir    = 'pub/aminet'
MaxSize   = 10000000
/*
MaxSize   = 524288
MaxSize   = 1536000
*/
RecName   = 'ADT_RECENT_7'
LogFile   = 'Logs:ZAmiMirror.log'
DelRecent = 'No'
FullTree  = 'No'
GetDiz    = 'Yes'
TempFile  = 'T:ZAmiMirror.tmp'

/* 1 for filelists, 2 for files. */

FTPCommand1 = "ncftp -V1"
FTPCommand2 = "ncftp -V0"

/*FTPCommand1 = "MiamiFTP -a -g -V -i"
FTPCommand2 = "MiamiFTP -a -g -V -i"*/

/*
** End of user variables - Don't touch the stuff below!
*/

say ''
say 'Zeus Bulletin Board System (c)1992-1995 Alex May & Nick Loman'
say 'Zeus Aminet Mirror v1.1 - Written by Alex May'
say ''

if FullTree = 'Yes' then do
   say 'Getting TREE from 'AmiAddr'...'

   old = pragma( 'd', Base )

   if exists( 'TREE' ) then call delete( 'TREE' )

   Address Command FTPCommand1' 'AmiAddr':/'AmiDir'/info/adt/TREE'

   call pragma( 'd', old )
   end

if ~GetTree( Base ) then call CleanUp()

say 'Getting RECENT info from 'AmiAddr'...'

old = pragma( 'd', Base )
if exists( RecName ) then call delete( RecName )

Address Command FTPCommand1' 'AmiAddr':/'AmiDir'/info/adt/'RecName

if right( RecName, 2 ) = '.Z' then do
   DecName = left( RecName, length( RecName ) - 2 )
   if exists( DecName ) then call delete( DecName )
   say 'Decompressing 'RecName'...'
   Address Command 'GZip -d 'RecName
   RecName = DecName
   end

call pragma( 'd', old )

if ~open( 'recent', Base || RecName, 'r' ) then do
   say 'Couldn''t open 'RecName'...'
   call CleanUp()
   end

data = readln( 'recent' )

if data ~= '#adt-v2' then do
   say 'Wrong version of ADT...'
   call close( 'recent' )
   call CleanUp()
   end

if ~open( 'mout', TempFile, 'w' ) then do
   say 'Couldn''t open 'TempFile' for writing...'
   call close( 'recent' )
   call CleanUp()
   end

if ~open( 'lout', Base || 'ZAmiMirror.new', 'w' ) then do
   say 'Couldn''t open 'Base'ZAmiMirror.new for writing...'
   call close( 'mout' )
   call close( 'recent' )
   call CleanUp()
   end

call writeln( 'mout', 'binary' )
call writeln( 'mout', 'cd ' || AmiDir )

i     = 0
bytes = 0

do while ~eof( 'recent' )
   data = readln( 'recent' )

   if left( data, 1 ) = '#' then iterate

   parse var data file.date '@' file.dir '@' file.name '@' file.size '@' a '@' b '@' c '@' file.desc

   fname = Base || file.dir || '/' || file.name

   if FindDir( file.dir ) then do
      if file.size = 0 then do
         say file.name' is zero length!'
         iterate
         end

      if file.size > MaxSize then do
         say file.name' exceeds MaxSize...'
         iterate
         end

      if exists( fname ) then do
         if GetFileSize( fname ) = file.size then do
            say 'Skipping 'file.name' - Already present...'
            iterate
            end
         else do
            say 'File has been partially transfered - downloading again...'
            call delete( fname )
            end
         end

      if MakePath( Base, file.dir || '/' ) then do
         files.i.name = file.name
         files.i.dir  = file.dir
         files.i.desc = file.desc
         files.i.size = file.size

         i     = i + 1
         bytes = bytes + file.size

         say file.dir'/'file.name' ('file.size' bytes) : 'file.desc

         tempname = Base || 'Temp/' || file.name
         
         call writeln( 'lout', left( file.name, 25 )' 'right( file.size, 7 )' 'file.desc )

         if exists( tempname ) then do
            if GetFileSize( tempname ) = file.size then do
               say file.name' is already in Temp directory!'
               iterate
               end
            else do
               call delete( tempname )
               end
            end

         call writeln( 'mout', 'get 'file.dir'/'file.name' 'file.name )
         end
      end
   end

call writeln( 'mout', 'exit' )

call close( 'recent' )
call close( 'mout' )
call close( 'lout' )

if i > 0 then do
   say ''
   say 'Downloading 'bytes' bytes in 'i' file(s)...'

   if MakePath( Base, 'Temp/' ) then do
      old = pragma( 'd', Base || 'Temp' )

      Address Command FTPCommand2' 'AmiAddr' <'TempFile

      j = 0

      call LogWrite( 'Processing new files at 'date('e')' 'time('n') )
      call LogWrite( '' )

      do while j < i
         say  'Processing 'files.j.name' ('files.j.size' bytes)'
         if exists( files.j.name ) then do
            if GetFileSize( files.j.name ) = files.j.size then do
               files.j.desc = strip( files.j.desc )
               files.j.desc = translate( files.j.desc, "'", '"' )

               Address Command 'Filenote 'files.j.name' "'files.j.desc'"'
               
               area = GetAreaNum( files.j.dir )

               if area > 0 then do
                  DeleteCatFile 'FILENAME' '"'files.j.name'"'
                  if RC = 0 then do
                     say 'Deleted old version of 'files.j.name'...'
                     end
                     
                  UseDesc = 1

                  if GetDiz = 'Yes' then do
                     Address Command 'DLDiz 'Base'Temp/'files.j.name' NH >T:ZM_DIZ.tmp'

                     fdat = statef( 'T:ZM_DIZ.tmp' )

                     if word( fdat, 2 ) ~= 24 then do
                        UseDesc = 0
                        say 'Extracted FILE_ID.DIZ'
                        end
                     end

                  say 'Uploading 'files.j.name' to area 'area'...'

                  if UseDesc = 1 then
                     AddFile '"'Base'Temp/'files.j.name'"' area 'DESCRIPTION' '"'files.j.desc'"' 'UPLOADER' '"ZAmiMirror"'
                  else
                     AddFile '"'Base'Temp/'files.j.name'"' area 'DESCFILE' 'T:ZM_DIZ.tmp' 'UPLOADER' '"ZAmiMirror"'
                  
                  select
                     when RC = 0 then nop
                     when RC = 1 then say 'Error saving file: 'RESULT
                     when RC = 2 then say 'Couldn''t move file!'
                     when RC = 3 then say 'Bad description!'
                     when RC = 4 then say 'Could not access file!'
                     when RC = 6 then say 'Couldn''t add file, no available paths for upload'
                     otherwise        say 'AddFile returned 'RC
                     end
                     
                  if GetDiz = 'Yes' then call delete( 'T:ZM_DIZ.tmp' )

                  temp1 = left( files.j.dir'/'files.j.name, 25 )
                  temp2 = right( files.j.size, 8 )

                  call LogWrite( temp1' ('temp2' Bytes) - 'files.j.desc )
                  end
               end
            else say 'File is too short!'
            end
         else say 'File doesn''t exist!'
         j = j + 1
         end

      call LogWrite( '' )

      call pragma( 'd', old )
      end
   end

call CleanUp()
exit

GetTree: procedure expose tree.
   parse arg Base

   if open( 'tree', Base || 'TREE', 'r' ) then do
      say 'Reading TREE...'

      cnt = 0

      do while ~eof( 'tree' )
         data = readln( 'tree' )

         if data ~= '' 0 then do
            dir = word( data, 1 )

            if index( dir, '/' ) > 0 then do
               tree.cnt = dir
               cnt = cnt + 1
               end
            end
         end

      tree.cnt = ''

      call close( 'tree' )

      if cnt = 0 then do
         say 'TREE doesn''t contain any directories!'
         return( 0 )
         end

      return( 1 )
      end

   say 'Couldn''t open "'Base'TREE" for reading...'

   return( 0 )

FindDir: procedure expose tree.
   parse arg fdir
   cnt = 0

   do while tree.cnt ~= ''
      if tree.cnt == fdir then return( 1 )

      cnt = cnt + 1
      end

   return( 0 )

MakePath: procedure
   parse arg dir, path

   old = pragma( 'd', dir )

   done = 0
   lpth = ''
   ret  = 0

   do while done = 0
      plen = length( path )
      spos = pos( '/', path )

      if spos > 0 then do
         dirbit = left( path, spos - 1 )
         
         if lpth ~= '' then lpth = lpth'/'dirbit
         else               lpth = dirbit
         
         path = right( path, plen - spos )
         
         if ~exists( lpth ) & ~makedir( lpth ) then do
            say '+++ Error creating 'dir || lpth
            done = 1
            end
         end
      else do
         done = 1
         ret  = 1
         end
      end

   call pragma( 'd', old )

   return( ret )

GetAreaNum: procedure
   parse arg dir

   i   = 0
   len = length( dir )

   do forever
      GetFileArea 'ORDINAL' i 'PATH'
      if RC ~= 0 then return( 0 )

      Path = Right( RESULT, len + 1 )
      Path = Left( Path, len )

/* if processing still fails, remove upper() bits */
      if upper(Path) = upper(dir) then do
         GetFileArea 'ORDINAL' i 'NUMBER'
         return( RESULT )
         end

      i = i + 1
      end

GetFileSize: procedure
   parse arg file

   data = statef( file )

   if data == '' then return( 0 )

   parse var data type size garbage

   if type ~= 'FILE' then return( 0 )

   return( size )

LogWrite: procedure expose LogFile
   parse arg data
   
   if exists( LogFile ) then om = 'a'
   else                      om = 'w'

   if open( 'log', LogFile, om ) then do
      call writeln( 'log', data )
      call close( 'log' )
      end
      
   return

CleanUp:
   if DelRecent = 'Yes' & exists( Base || RecName ) then call delete( Base || RecName )
   if exists( TempFile ) then call delete( TempFile )
   exit
