/**/
/* $VER: userbase_lib.fd 37.0 (21.02.95)*/
/**/
#pragma libcall UserBase ubInitialise 1e 801
#pragma libcall UserBase ubQuit 24 0
#pragma libcall UserBase ubInitUserKey 2a 001
#pragma libcall UserBase ubNextUserKey 30 801
#pragma libcall UserBase ubEndUserKey 36 801
#pragma libcall UserBase ubDeleteUser 3c 801
#pragma libcall UserBase ubFindUserIdx 42 9802
#pragma libcall UserBase ubLoadUser 48 9802
#pragma libcall UserBase ubLoadUserFromKey 4e 9802
#pragma libcall UserBase ubSaveUser 54 801
/* ubDebug()()*/
