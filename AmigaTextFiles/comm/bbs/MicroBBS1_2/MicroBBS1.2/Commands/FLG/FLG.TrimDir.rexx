/* Blahhh */

parse arg InfileName OutfileName

if open( Infile, strip( InfileName), Read) then
do
    if open( Outfile, strip( OutfileName), Write) then
    do
        do while ~eof( Infile)
            String = Readln( Infile)
            call writeln( Outfile, left( String, 80))
            String = strip( substr( String, 81, 35))
            if length( String) ~= 0 then call writeln( Outfile, "                                            "String)
        end

        close( Outfile)
    end
    close( Infile)
end
