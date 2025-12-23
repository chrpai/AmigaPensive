#define AUXDEBUGPORT "Aux_Debug_Port"
struct debugmsg
  {
  struct Message ipc_Msg;
  char *msg;                 /* null terminated text string */
  int a,b,c;
  int flag;
  };

