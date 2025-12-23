/* WATCHER v1.00 by Case of Energy Telecommunications (412)339-0590 */

path='PFILES:BBS/' /* Make sure the path ends with / or : */

options results;getuser 51;if result=0 then exit;getuser 49;name=result
address command "run rx "path"watchermain.rexx "name;exit
