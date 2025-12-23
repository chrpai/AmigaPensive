/*
** UserDetails.rexx -- Called by Voyeur with user name and public screen name
**                      to fire up your preferred User Details watcher.
*/

Options Results
Parse Arg Line PubScreen

address command 'Zeus:Utils/UserDetails LINE 'Line' PUBSCREEN 'PubScreen
