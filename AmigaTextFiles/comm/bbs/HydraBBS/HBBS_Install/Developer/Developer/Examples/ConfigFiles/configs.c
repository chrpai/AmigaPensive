// example of reading items from a config file..
// last updated: 23/06/1999

// Add this code to one of the example program or doors to see it work...
// HBBSCommon.library has to be open, and the program/door must be talking
// to HBBS before this can be called..


/* Here's an example config file:

--( CUT HERE )------------------------

FrEd=A string ;skjjshdjskjksjk
num=83634
list_1=fred
list_2=sid
list_3=moocow
path=dh0:system        ; note lack of trailing "/"
pathlist_1=cd0:may     ; note lack of trailing "/"
pathlist_2=cd0:jun/    ; the actual variables will have "/" added to the end of them if you use VTYPE_PATH or VTYPE_PATHLIST
pathlist_3=cd0:jul/

bool=YES
;bool=NO
;bool=ON
;bool=OFF
;bool=TRUE
;bool=FALSE

--( CUT HERE )------------------------

*/


void LoadMyDoorCFG( void )
{
  struct CfgFileData *DeviceCFG;
  V_ERROR error=TYPE_NONE;
  UBYTE *filename="Progdir:Test.CFG";

  V_PATH mypath=NULL;
  V_STRING mystring=NULL;          // ALWAYS INITILISE VARIABLES that you pass to HBBS_GetSetting to
  V_STRINGLIST mylist=NULL;        // NULL or 0 otherwise you can expect a system crash!
  V_PATHLIST mypathlist=NULL;      // when HBBS tries to free them!
  V_BIGNUM mynum=0;
  V_BOOL mybool=FALSE;

  if (DeviceCFG=HBBS_LoadConfig(filename,LCFG_NONE)))
  {

    HBBS_GetSetting(DeviceCFG,(void *)&mystring,VTYPE_STRING    ,"fred",OPT_SINGLE);
    HBBS_GetSetting(DeviceCFG,(void *)&mystring,VTYPE_PATH    ,"path",OPT_SINGLE);
    HBBS_GetSetting(DeviceCFG,(void *)&mynum   ,VTYPE_BIGNUM    ,"num" ,OPT_SINGLE);
    HBBS_GetSetting(DeviceCFG,(void *)&mylist  ,VTYPE_STRINGLIST,"list",OPT_MULTI);
    HBBS_GetSetting(DeviceCFG,(void *)&mylist  ,VTYPE_PATHLIST,"pathlist",OPT_MULTI);
    HBBS_GetSetting(DeviceCFG,(void *)&mybool  ,VTYPE_BOOL      ,"bool",OPT_SINGLE);

    HBBS_FlushConfig(DeviceCFG);
  }

  /*
     you can now do stuff with the variables that have been loaded from
     the config file as you would with any other variable
  */

  // Don't forgetting to free them after use though (the functions below are in the HBBSCommon.library)
  FreeStrList(mylist);
  FreeStr(mystring);
}

