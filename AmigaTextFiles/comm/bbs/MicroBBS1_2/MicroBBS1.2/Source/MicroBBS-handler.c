/* Handler fuer MicroBBS */

#include <exec/types.h>
#include <exec/nodes.h>
#include <exec/ports.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <dos/dosextens.h>
#include <dos/filehandler.h>
#include <intuition/intuition.h>

#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include    "Spawn.h"

#define BTOC( BPTR)  (( void *) (( long) ( BPTR) << 2))
#define CTOB( cptr)  (( BPTR) ((( long) cptr) >> 2))

void ReturnPacket( struct DosPacket *);
void Initialize( void);
void UnInitialize( void);

struct DeviceNode   *DevNode;
struct MsgPort      *PktPort;

const char  *Version        = "$VER: MicroBBS-handler 1.10 (14.02.96)";

long    LineUsage   = 0;
long    ConMode     = SM_CON;

long    Node;
char    SpawnPortName[ STRING_LENGTH ];

BOOL    Die = FALSE;

void _main()
{
    struct DosPacket    *Packet;
    struct Process      *ThisProcess = FindTask( NULL);

    PktPort = &ThisProcess->pr_MsgPort;

    Initialize();

    while( Die != TRUE)
    {
        struct Message *PaketMsg;

        while(( PaketMsg = GetMsg( PktPort)) == NULL)            /* auf Msg warten */
            Wait( 1 << PktPort->mp_SigBit);

        if( Packet = ( struct DosPacket *) PaketMsg->mn_Node.ln_Name)
        {
            Packet->dp_Res1 = DOSTRUE;
            Packet->dp_Res2 = 0;

            switch( Packet->dp_Type)
            {
//+               ACTION_DIE
                case ACTION_DIE :
                {
                    if( !( LineUsage))
                    {
                        Die = TRUE;
                    }
                }
                break;
//-
//+               ACTION_FIND#?
                case ACTION_FINDUPDATE :
                case ACTION_FINDINPUT :
                case ACTION_FINDOUTPUT :
                {
                    struct MsgPort      *SpawnPort;
                    struct SpawnMessage *SpawnMessage;

                    struct FileHandle  *FileHandle = BTOC( Packet->dp_Arg1);
                    char    *SpawnFileName         = BTOC( Packet->dp_Arg3);

                    FileHandle->fh_Arg1 = 0;
                    FileHandle->fh_Port = ( struct MsgPort *) DOSTRUE;

                    if( SpawnFileName[ 1 ] != '*')
                    {
                        memcpy( SpawnPortName, SpawnFileName + 1, ( char) *SpawnFileName);
                        SpawnPortName[ ( char) *SpawnFileName ] = 0x00;

                        Node = atol( FilePart( SpawnPortName));

                        sprintf( SpawnPortName, "MicroBBS SpawnPort Unit %lu", Node);
                    }

                    if( SpawnMessage = AllocVec( sizeof( struct SpawnMessage), MEMF_ANY|MEMF_CLEAR|MEMF_PUBLIC))
                    {
                        SpawnMessage->sm_Message.mn_Node.ln_Type    = NT_MESSAGE;
                        SpawnMessage->sm_Message.mn_Length          = sizeof( struct SpawnMessage);
                        SpawnMessage->sm_Type                       = SM_SPAWN_OPEN;

                        Forbid();

                        if( SpawnPort = FindPort( SpawnPortName))
                        {
                            PutMsg( SpawnPort, ( struct Message *) SpawnMessage);

                            LineUsage++;
                        }
                        else
                        {
                            FreeVec( SpawnMessage);

                            Packet->dp_Res2 = ERROR_OBJECT_NOT_FOUND;
                        }

                        Permit();
                    }
                    else Packet->dp_Res2 = ERROR_NO_FREE_STORE;
                }
                break;
//-
//+               ACTION_READ
                case ACTION_READ :
                {
                    struct MsgPort      *SpawnPort;
                    struct SpawnMessage *SpawnMessage;

                    Packet->dp_Res1 = 0;

                    if( SpawnMessage = AllocVec( sizeof( struct SpawnMessage), MEMF_ANY|MEMF_CLEAR|MEMF_PUBLIC))
                    {
                        if( SpawnMessage->sm_Message.mn_ReplyPort = CreateMsgPort())
                        {
                            SpawnMessage->sm_Message.mn_Node.ln_Type    = NT_MESSAGE;
                            SpawnMessage->sm_Message.mn_Length          = sizeof( struct SpawnMessage);
                            SpawnMessage->sm_Type                       = SM_SPAWN_DATA_IN;
                            SpawnMessage->sm_Length                     = Packet->dp_Arg3;
                            SpawnMessage->sm_Task                       = Packet->dp_Port->mp_SigTask;
                            SpawnMessage->sm_Data                       = ( char *) Packet->dp_Arg2;
                            SpawnMessage->sm_Mode                       = ConMode;

                            Forbid();

                            if( SpawnPort = FindPort( SpawnPortName))
                            {
                                PutMsg( SpawnPort, ( struct Message *) SpawnMessage);

                                Permit();

                                WaitPort( SpawnMessage->sm_Message.mn_ReplyPort);

                                if( GetMsg( SpawnMessage->sm_Message.mn_ReplyPort))
                                {
                                    Packet->dp_Res1 = SpawnMessage->sm_Length;

                                    if( ConMode == SM_RAW) if( SpawnMessage->sm_Length) if( SpawnMessage->sm_Data[ SpawnMessage->sm_Length - 1 ] == 0x0a) SpawnMessage->sm_Data[ SpawnMessage->sm_Length - 1 ] = 0x0d;
                                }
                                else Packet->dp_Res2 = ABORT_BUSY;
                            }
                            else
                            {
                                Permit();
                                Packet->dp_Res2 = ABORT_BUSY;
                            }

                            DeleteMsgPort( SpawnMessage->sm_Message.mn_ReplyPort);
                        }
                        else Packet->dp_Res2 = ERROR_NO_FREE_STORE;

                        FreeVec( SpawnMessage);
                    }
                    else Packet->dp_Res2 = ERROR_NO_FREE_STORE;
                }
                break;
//-
//+               ACTION_WRITE
                case ACTION_WRITE :
                {
                    struct MsgPort      *SpawnPort;
                    struct SpawnMessage *SpawnMessage;

                    long    Bytes = Packet->dp_Arg3;

                    if( SpawnMessage = AllocVec( sizeof( struct SpawnMessage), MEMF_ANY|MEMF_CLEAR|MEMF_PUBLIC))
                    {
                        if( SpawnMessage->sm_Data = AllocVec( Bytes, MEMF_ANY|MEMF_PUBLIC))
                        {
                            Packet->dp_Res1 = Bytes;

                            movmem(( char *) Packet->dp_Arg2, SpawnMessage->sm_Data, Bytes);

                            SpawnMessage->sm_Message.mn_Node.ln_Type    = NT_MESSAGE;
                            SpawnMessage->sm_Message.mn_Length          = sizeof( struct SpawnMessage);
                            SpawnMessage->sm_Type   = SM_SPAWN_DATA_OUT;
                            SpawnMessage->sm_Length = Bytes;
                            SpawnMessage->sm_Mode   = ConMode;

                            Forbid();

                            if( SpawnPort = FindPort( SpawnPortName)) PutMsg( SpawnPort, ( struct Message *) SpawnMessage);
                            else Packet->dp_Res2 = ABORT_BUSY;

                            Permit();
                        }
                        else Packet->dp_Res2 = ERROR_NO_FREE_STORE;

                        if( Packet->dp_Res2)
                        {
                            if( SpawnMessage->sm_Data) FreeVec( SpawnMessage->sm_Data);
                            FreeVec( SpawnMessage);
                        }
                    }
                    else Packet->dp_Res2 = ERROR_NO_FREE_STORE;
                }
                break;
//-
//+               ACTION_END
                case ACTION_END :
                {
                    struct MsgPort      *SpawnPort;
                    struct SpawnMessage *SpawnMessage;

                    if( SpawnMessage = AllocVec( sizeof( struct SpawnMessage), MEMF_ANY|MEMF_CLEAR|MEMF_PUBLIC))
                    {
                        SpawnMessage->sm_Message.mn_Node.ln_Type    = NT_MESSAGE;
                        SpawnMessage->sm_Message.mn_Length          = sizeof( struct SpawnMessage);
                        SpawnMessage->sm_Type                       = SM_SPAWN_CLOSE;

                        Forbid();

                        if( SpawnPort = FindPort( SpawnPortName)) PutMsg( SpawnPort, ( struct Message *) SpawnMessage);
                        else FreeVec( SpawnMessage);

                        Permit();

                        if( !( --LineUsage)) Die = TRUE;
                    }
                    else Packet->dp_Res2 = ERROR_NO_FREE_STORE;
                }
                break;
//-

//+               ACTION_SCREEN_MODE
                case ACTION_SCREEN_MODE :
                {
                    if( Packet->dp_Arg1 == DOSTRUE) ConMode = SM_RAW;
                    else                            ConMode = SM_CON;
                }
                break;
//-
//+               ACTION_IS_FILESYSTEM
                case ACTION_IS_FILESYSTEM :
                {
                    Packet->dp_Res1 = DOSFALSE;
                }
                break;
//-
//+               ACTION_WAIT_CHAR
                case ACTION_WAIT_CHAR :
                {
                    struct MsgPort      *SpawnPort;
                    struct SpawnMessage *SpawnMessage;

                    if( SpawnMessage = AllocVec( sizeof( struct SpawnMessage), MEMF_ANY|MEMF_CLEAR|MEMF_PUBLIC))
                    {
                        SpawnMessage->sm_Message.mn_Node.ln_Type    = NT_MESSAGE;
                        SpawnMessage->sm_Message.mn_Length          = sizeof( struct SpawnMessage);
                        SpawnMessage->sm_Type                       = SM_SPAWN_WAIT_CHAR;
                        SpawnMessage->sm_Length                     = Packet->dp_Arg1;

                        Forbid();

                        if( SpawnPort = FindPort( SpawnPortName)) PutMsg( SpawnPort, ( struct Message *) SpawnMessage);
                        else FreeVec( SpawnMessage);

                        Permit();
                    }
                    else Packet->dp_Res2 = ERROR_NO_FREE_STORE;
                }
                break;
//-

                default :
                {
                    Packet->dp_Res2 = ERROR_ACTION_NOT_KNOWN;
                }
                break;
            }

            if( Packet->dp_Res2) Packet->dp_Res1 = DOSFALSE;

            ReturnPacket( Packet);
        }
    }

    UnInitialize();

    _exit( 0);
}

//+ void ReturnPacket( struct DosPacket *Packet)
void ReturnPacket( struct DosPacket *Packet)
{
    struct Message *Msg;
    struct MsgPort *ReplyPort;

    ReplyPort                = Packet->dp_Port;
    Msg                      = Packet->dp_Link;
    Packet->dp_Port          = PktPort;
    Msg->mn_Node.ln_Name     = ( char *) Packet;
    PutMsg( ReplyPort, Msg);
} //-

//+ void Initialize( void)
void Initialize( void)
{
    struct DosPacket   *Packet;
    struct Message *InitialMsg;

    WaitPort( PktPort);
    InitialMsg = GetMsg( PktPort);
    Packet = ( struct DosPacket *) InitialMsg->mn_Node.ln_Name;

    Packet->dp_Res1 = DOSTRUE;
    Packet->dp_Res2 = 0;

    DevNode = BTOC( Packet->dp_Arg3);
    DevNode->dn_Task = NULL;

    if( Packet->dp_Res2) Packet->dp_Res1 = DOSFALSE;

    ReturnPacket( Packet);
} //-
//+ void UnInitialize( void)
void UnInitialize( void)
{
    DevNode->dn_Task = NULL;
    /* dn->dn_SegList = NULL; */
} //-
