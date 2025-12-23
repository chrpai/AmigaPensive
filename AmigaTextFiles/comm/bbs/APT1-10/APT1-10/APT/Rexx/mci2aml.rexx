/* MCI2AML.rexx
**
** $VER: MCI2AML 0.0.1 (26.4.93)
**
** ARexx program for controlling ApT-BBS by ApT-Design.
**
** Copyright © 1993 ApT-Design All Rights Reserved,TS
**
** This script will take in a text file with MCI codes and
** copy the file to a new one, altering the control codes on
** the way to their AML equivelents.
**
*/

/*

- stuff to convert -

%TC=<colour>.			TEXT COLOUR
%BC=<colour>.			BACKGROUND COLOUR
%CLS.				Clear Screen
%ANSIRESET.			Reset ansi emulation via ESQ[0m
%CURSOR=<0/1>.			Turn cursor off/on
%CURSOR_LEFT=<positions>.	Postion cursor X positions to left.
%CURSOR_RIGHT=<position>.	Position cursor X positions to right.
%CURSOR_UP=<position>.		Position cursor X positions up.
%CURSOR_DOWN=<position>.	Position cursor X positions down.
%UNDERLINE=<0/1>.		Turn Underline Mode off/on.
%RVIDEO=<0/1>.			Turn Reverse Video off/on.
%BOLD=<0/1>.			Turn Bold off/on.
%ITALICS=<0/1>			Turn Italics off/on.
%WAITKEY.			Wait for a keypress.


Allow room for other things such as:-

%CREDITS+1.
%CREDITS/20.
%CREDITS=213021.

Ahm, and here is the actual table then:-

\c{0-7}			=>		%TC={0-7}.
\z{0-7}			=>		%BC={0-7}.
\@			=>		%CLS.
\z0\c7 or \c7\z0	=>		%ANSIRESET./*Not sure so not implmtd*/
\o0			=>		%CURSOR=0.
\o1			=>		%CURSOR=1.
\<{0-9}			=>		%CURSOR_LEFT={0-9}.
\>{0-9}			=>		%CURSOR_RIGHT={0-9}.
\^{0-9}			=>		%CURSOR_UP={0-9}.
\!{0-9}			=>		%CURSOR_DOWN={0-9}.
\u0			=>		%UNDERLINE=0.
\u1			=>		%UNDERLINE=1.
\b0			=>		%BOLD=0.
\b1			=>		%BOLD=1.
\i0			=>		%ITALICS=0.
\i1			=>		%ITALICS=1.
\g{anything}		=>		%WAITKEY.

*/

MCI.1 = 'C'		;	AML.1 = 'TC'
MCI.2 = 'Z'		;	AML.2 = 'BC'
MCI.3 = 'O'		;	AML.3 = 'CURSOR'
MCI.4 = '<'		;	AML.4 = 'CURSOR_LEFT'
MCI.5 = '>'		;	AML.5 = 'CURSOR_RIGHT'
MCI.6 = '^'		;	AML.6 = 'CURSOR_UP'
MCI.7 = '!'		;	AML.7 = 'CURSOR_DOWN'
MCI.8 = 'U'		;	AML.8 = 'UNDERLINE'
MCI.9 = 'B'		;	AML.9 = 'BOLD'
MCI.10= 'I'		;	AML.10= 'ITALICS'
MCI.11= 'G'		;	AML.11= 'WAITKEY'
MCI.12= '@'		;	AML.12= 'CLS'

parse upper arg infile outfile .
if ~open('in',infile,'R') then DO
	say 'Cannot open MCI source file!'
	EXIT
END
if ~open('out',outfile,'W') then DO
	say 'Cannot open AML output file!'
	EXIT
END

DO while ~eof('in')
	curline = readln('in')
	newline = ''
	DO charnum=1 to length(curline)
                curchar=substr(curline,charnum,1)
		if curchar = '\' then DO
			charnum=charnum+1
	                curchar=substr(curline,charnum,1)
			DO tst=1 to 12
				if upper(curchar) = upper(mci.tst) then DO
					supplement=''
					if tst ~>10 then supplement=supplement||substr(curline,charnum+1,1)
					call swapcode(tst,supplement)
					newcode=result
					if tst=11 then charnum=charnum+1
					charnum=charnum+length(supplement)
					newline=newline||newcode
				END
			END
		END
		ELSE newline=newline||curchar
	END
	call writeln('out',newline)
END

say 'Reached end!'
EXIT


/*****************************************************
 * swapcode: Just gets the number of the MCI stem    *
 *           and turns it into a similar AML thingy. *
 *****************************************************/

swapcode:
parse upper arg mcicode,magntd

if mcicode ~> 10 then return '%'||AML.mcicode||'='||magntd||'.'
return '%'||AML.mcicode||'.'

/*Uhm............. well.. */
RETURN /* indeedy! */
