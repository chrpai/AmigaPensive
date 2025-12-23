// Spawn.h

#include    "Misc.h"

struct  SpawnMessage
{
    struct Message  sm_Message;

    long            sm_Type;
    long            sm_Length;
    long            sm_Mode;

    char            sm_SpawnPortName[ STRING_LENGTH ];

    struct Process  *sm_Task;

    char            *sm_Data;
};

#define     SM_SPAWN_END        0
#define     SM_SPAWN_DATA_OUT   1
#define     SM_SPAWN_DATA_IN    2
#define     SM_SPAWN_WAIT_CHAR  3
#define     SM_SPAWN_OPEN       4
#define     SM_SPAWN_CLOSE      5

#define     SM_RAW  0
#define     SM_CON  1


