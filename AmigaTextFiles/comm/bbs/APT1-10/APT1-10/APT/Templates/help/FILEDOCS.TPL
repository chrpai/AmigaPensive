%CLS.

%BC=7.%TC=4. User Information/Help >> %BC=0.%TC=7.

Here follows various information that should help you overcome any
problems that you may have within the file-area bases. As each new
topic begins a 'heading' will be shown.

%BC=4.%TC=7. Pattern Matching Help >> %BC=0.%TC=7.

Here is a list of supplied pattern matching that may be used in marking
a file if using the filename. The patterns are fairly extensive, and
approximate some of the ability of Unix/grep "regular expression"
patterns. Here are teh available tokens.


?          Matches a single character
#          Matches the following expression 0 or more times
(ab|cd)    Matches any one of the items seperated by '|'
~          Negates the following expression. It matches all strings
           that do not match the expression (aka ~(foo) matches all
           strings that are not exactly "foo").
[abc]      Character class: matches any of the characters in the class.
[~bc]      Character class: matches any of the characters not in the
           class.
a-z        Character range (only within character classes).
%          Matches 0 Characters always (useful in "(foo|bar|%)").
*          Synonym for '#?", not available by default, however some
           systems may have this feature turned on.

"Expression" in the above table means either a single character
(ex: "#?"), or an alternation (ex: "#(ab|cd|ef)"), or a character
ckass (ex: "#[a-zA-Z]").

Example, to mark all files that have the name 'global' within them
you would enter for a filename.

global#?.lha

would match: global_1.lha, global_2.lha, global_3.lha

#?global#?

would match: Hamglobal.lha, SpamGlobal.LZH, Globaliser.arc


