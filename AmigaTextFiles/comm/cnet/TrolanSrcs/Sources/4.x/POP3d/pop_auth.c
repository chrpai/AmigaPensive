#include "pop3d.h"

int cmd_auth(int argc, char **argv)
{
   pop_msg(POP_ERR, "This command is not supported");

   return POP_FAILURE;
} /* cmd_auth */

