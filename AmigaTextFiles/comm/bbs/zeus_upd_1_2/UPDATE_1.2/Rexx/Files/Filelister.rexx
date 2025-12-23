/*
** FileLister.rexx - ZEUS external file lister.
**
** v1.1 (C) 1995-9 Zeus Developments
**
** v1.0 by Nick Loman, v1.1 by Neil Williams
**
** For 1.1: variable width fields, displays multiple line descriptions which are correctly wrapped and indented.
*/

Options Results
Options Failat 21
Parse Arg Area Outputfile DescSize Days

Address 'ZEUS'

if Area       = '' then Signal Usage
if Outputfile = '' then Signal Usage

DescSize   = Strip( DescSize )
Days       = Strip( Days )
Outputfile = Strip( Outputfile )

SearchDate = 0

if Days ~= '' then do
    CurrentDays = Date( 'I' )
    SearchDays  = (CurrentDays - Days)
    Conv = Date( 'NORMAL', SearchDays, 'I' )
    Conv = DelStr( Conv, '8', '2' )
    ConvertDate '"'Conv'"'
    SearchDate = RESULT
    say 'Scanning for files uploaded after 'Conv'...'
    end

if DescSize ~= '' then do
   if DescSize < 10 then DescSize=10
   if DescSize > 50 then DescSize=55
end
else
   DescSize = 36

DescSize = DescSize+2 /* possible GetFile Desc /Wrap/ bug - it seems to wrap 2 chars early */

Hdr           = '  /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/'

EndAreas = 0

PeruseFileAreas 'First'
if RC ~= 0 then do
    say 'Fatal Error! Are ZEUS and FileControl running?'
    exit
    end

AreaNum = RESULT

if ~Open( 'fl', Outputfile, 'WRITE' ) then do
    say 'Cannot open 'Outputfile'!'
    exit
    end

do until EndAreas = 1
    if AreaNum = Area|Upper(Area) = 'ALL' then do
        GetFileArea AreaNum 'Name'
        say 'Area: 'AreaNum' - 'RESULT

        Call FileList
        end
    PeruseFileAreas AreaNum 'Next'
    AreaNum = RESULT
    if RC ~= 0 then EndAreas = 1
    end


Call Close('fl')

exit

FileList:
    InitFileKey 'AREA' AreaNum 'ALPHA' 'FWD'
    Key = RESULT
    if RC = 0 then do

        Call WriteLn( 'fl', '' )
        Call WriteLn( 'fl', Hdr )
        GetFileArea AreaNum 'Name'
        AreaName = RESULT
        GetFileArea AreaNum 'MinAge'
        MinDlBaud = RESULT

        AreaName = Left( AreaName, 26 )
        Line = '   Filelisting of area 'AreaNum' - 'AreaName' Min D/L Baud: 'MinDlBaud
        Call WriteLn( 'fl', Line )

        Call WriteLn( 'fl', Hdr )
        Call WriteLn( 'fl', '' )

        EndFiles    = 0
        TotalFiles      = 0
        TotalSize   = 0

        do until EndFiles = 1
            GetFileFromKey Key 'Name'
            Name = RESULT
            if RC ~= 0 then EndFiles = 1
            else do
                Name   = Left( Name, 30+36-DescSize )

                GetFileFromKey Key 'Uploaded'
                if RESULT > SearchDate then do
                    GetFileFromKey Key 'Size'
                    Size   = RESULT
                    Size   = Right( Size, 8 )

                    GetFileFromKey Key 'DLS'
                    DLS    = RESULT
                    DLS    = Right( DLS, 3 )

                    GetFileFromKey Key 'Wrap' DescSize 'Desc'
                    if RESULT = 'RESULT' then RESULT = ''
                    Descr = Result
                    if Right( Descr, 1 )='0a'x then
                       Descr = Left( Descr, Length(Descr)-1 ) /* Kill last line feed */

                    start=1
                    do until indx=0
                       indx=index( Descr, '0A'x, start )
                       if indx ~= 0 then do
                          Descr = insert( Copies( ' ', 81-DescSize), Descr, indx )
                          start=indx+1
                       end
                    end


                    Line = ' 'Name' 'Size' 'DLS' 'Descr
                    if WriteLn( 'fl', Line ) = 0 then say 'Error writing to file!'

                    Totalfiles = Totalfiles + 1
                    TotalSize  = TotalSize  + Size
                    end
                end

            GetFileFromKey Key 'Next'
            if RC = 5 then EndFiles = 1
            end

        CloseFileKey Key

        Call WriteLn( 'fl', '' )
        TotalFiles = Left( TotalFiles, 8 )
        Line = 'Files: 'TotalFiles'  Bytes: 'TotalSize
        Call WriteLn( 'fl', Line )

        end
    else say 'No files to be scanned in area 'AreaNum'!'
    return

Usage:
    say 'Usage: FileLister.rexx <Area Number> <Output> <Description Size> <Number of Days>'
    say ''
    say '  <Area Number>    Area number to list, or ALL for all areas.'
    say '  <Output>         File to output to - use * to send to shell window.'
    say '  <Description     Sets the width of the description, at the expense of'
    say '    Size>           shortening the filename.  Descriptions are'
    say '                    normally 44 chars, but this defaults to 36.'
    say '  <Number of days> Oldest files to list, in days old.'
    say ''
    exit

