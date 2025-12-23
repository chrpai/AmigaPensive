/*
** PopUpWatcher.rexx -- Called by Voyeur with line number argument to fire
**                       up your preferred Watcher.
*/

Options Results
Parse Arg Line

address command 'Zeus:Bin/Watchers/ANSI 'Line' JUMPY'
