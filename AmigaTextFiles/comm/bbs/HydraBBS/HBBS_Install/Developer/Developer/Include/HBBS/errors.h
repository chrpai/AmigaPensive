// define our error messages

#define EMSG_NOREQTOOLS                 1
#define EMSG_NOMEM                      2
#define EMSG_NOLIBS                     3
#define EMSG_NOFONT                     4
#define EMSG_NOSCREEN                   5
#define EMSG_NOWINDOW                   6
#define EMSG_BBSGLOBALERR               7
#define EMSG_NOTENOUGHNL                8
#define EMSG_NODESETTIGSERROR           9
#define EMSG_ALREADYSTARTED             10
#define EMSG_CANTCHANGESCREEN           11
#define EMSG_NOAUTOSTART                12
#define EMSG_NOHBBS                     13
#define EMSG_CONFPATHSERROR             14
#define EMSG_PATHMISSING                15
#define EMSG_LEVELSFILEERROR            16
#define EMSG_PROTOCOLSFILEERROR         17
#define EMSG_CANTLOCKPUBSCREEN          18
#define EMSG_NOSCREENQUIT               19
#define EMSG_CANTSAVEFILE               20
#define EMSG_NODEDIRMISSING             21
#define EMSG_NOTHBBS                    22
#define EMSG_CONFLISTMISMATCH           23
#define EMSG_NOAMIGAGUIDE               24
#define EMSG_NODEVICEFILE               25
#define EMSG_NOUSERDATA                 26
#define EMSG_WRONGHBBSCOMMON            27
#define EMSG_WRONGHBBSNODE              28

#define EMSG_DEVICECFG                  70
//                                      vv
// all the way to                       99

// messages from "Node"

#define EMSG_NOCTRL                     100
#define EMSG_NODEVICE                   101
#define EMSG_NONODEWINDOW               102
#define EMSG_NOWATCHSCRWIN              103
#define EMSG_NODEALREADYUP              104
#define EMSG_NODEINVALID                105
#define EMSG_NOCOMMON                   106
#define EMSG_COMMONERR                  107
#define EMSG_NONODE                     108
#define EMSG_NODEERR                    109
#define EMSG_MODEMPROBLEM               110

#define EMSG_NODEINIT                   150
//                                      vvv
// all the way to                       159

#define EMSG_NODESETTINGS               160
//                                      vvv
// all the way to                       XXX

#define EMSG_CONFCFG                    180
//                                      vv
// all the way to                       199

#define EMSG_SECPROB                    200

#ifdef MAIN

  UBYTE *TYPE_STR[8]={"None!","Fatal","Critical","Important","Warning","Information","Memory","Info"};
  UBYTE *ESTR_NOSERIAL="Error Opening Serial Port!";

#else

  extern UBYTE *TYPE_STR[7];
  extern UBYTE *ESTR_NOSERIAL;

#endif
