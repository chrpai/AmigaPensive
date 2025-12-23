/**********************************************
 ** Rot13 a message  *  Sample arexx script  **
 **********************************************
 **                                          **
 ** Set up the user command like this:       **
 ** +--------------------------------------+ **
 ** |         Command: RX AmyBW:Rot13.rexx | **
 ** |        Filename: T:AmyBW.temp        | ** 
 ** |  Write msg text: Y                   | **
 ** |   Incude header: N                   | **
 ** | Reload msg text: Y                   | **
 ** +--------------------------------------+ **
 **********************************************/

	/***
	 *** Open input and output file
	 ***/

call open(in,"T:AmyBW.temp",read)
call open(out,"T:AmyBW.rot",write)

	/***
	 *** Translate the lines
	 ***/

do while ~EOF(in)
 inline = readln(in)
 outline = TRANSLATE(inline,'nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM','abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')
 call writeln(out, outline)
end

	/***
	 *** Close the files
	 ***/

call close(in)
call close(out)

	/***
	 *** Delete original, rename new file
	 ***/

address command 'Delete >NIL: T:AmyBW.temp'
address command 'Rename >NIL: T:AmyBW.rot T:AmyBW.temp'

