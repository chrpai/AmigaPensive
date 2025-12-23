Grotte 2.0, version demo

This is a action-game which can run on any ANSI terminal,
including on a BBS or directly on Amiga's screen.

This is currently a demo version, with 3 levels instead of 100.

Main features:
 - System friendly
 - Work on any Amiga
 - High animation
 - Written 100% in Modula-2
 - Documentation en français


INSTALLATION:

 Just copy the 'Grotte' drawer where you want the game to be installed.
(all necessary files are kept together in it).


USAGE:

 There is two main programs, 'GrotteDemo' and 'GrotteBDemo'.
They do exactly the same, exept that GrotteBDemo quits directly
and without any requester in the case of an error, unlike
GrotteDemo which display descriptive requesters.
 You should use 'GrotteBDemo' to install the game on a BBS (to
avoid any requester to lock the BBS).
 Use 'GrotteDemo' to play directly on the Amiga.

 To specify what kind of terminal the program should use, modify
the 'TERMINAL_LIB' tolltype with the 'Informations' menu from the
workbench. Two kind are available (they are implemented by libraries):
- Libs/topazterm.library:
 This one makes the program open an intuition screen and play
on it, simulating a terminal.
- Libs/stdio.library:
 This one makes the program use the standart I/O (which can be modified
in a cli-command with '<>') as an ANSI terminal. You can specify any
interactive file or device, including '<> RAW:' which opens and use an
AmigaDos window. Warning: the 'CON:' device doesn't work with this game
as it only send chars that are followed by <RETURN>.
 With this library, the program cannot be launched form Workbench as
no standart I/O are specified.
 A deconnection (EOF) makes the program quit immediatly.

 The 'SOUND_LIB' tooltype must be set to 'Libs/grottenosound' and must
be present.

 If you use Kickstart 1.3 or earlier, specify the full pathname for
the two libraries.

 When specified, the 'TASKPRIORITY' tooltype can be use to change the
task's priority.

 If you are trying to install GrotteBDemo on your BBS and it doesn't
work, you can find out the cause by temporaly using GrotteDemo; but
don't forget to correctly set the 'TERMINAL_LIB' tooltype to
Libs/stdio.library !!

NOTE:

 Works only with Kickstart 1.1 or more  ;-)))

 You can read some other informations in the 'Grotte/Docs' drawer.
Warning: these files are readable by any text displayer but can look
a bit strange as they are usually displayed from the program Grotte.

 This demo version has all the features of the full version,
exept that only 3 levels are available.

 The file 'Decors' and every files of the 'Grotte/Docs' drawer can be
packed with any xpk method: they will still be readable by the
program.

 Files are all copyrighted, exect the doc ('Grotte/Docs/#?' and #?.txt');
fell free to modify or translate these files in another language.


FULL VERSION:

 Not finished yet.
