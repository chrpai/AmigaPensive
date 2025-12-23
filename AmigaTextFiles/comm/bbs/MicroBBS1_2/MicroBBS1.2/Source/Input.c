// Input.c

#include    "Input.h"

extern struct MsgPort  *SerialReadMP;
extern struct IOExtSer *SerialReadIO;
extern struct MsgPort  *SerialWriteMP;
extern struct IOExtSer *SerialWriteIO;

extern struct MsgPort      *ConsoleReadMP;
extern struct IOStdReq     *ConsoleReadIO;

extern struct MsgPort      *TimerMP;
extern struct timerequest  *TimerIO;

extern long    SerialDevice;
extern long    ConsoleDevice;

extern char    SerialChar;
extern char    SerialReadChar;
extern char    LastSerialChar;
extern char    ConsoleChar;
extern char    ConsoleReadChar;

//+ BOOL WaitChar( long WaitTime)
BOOL WaitChar( long WaitTime)
{
    BOOL BufferedChars = FALSE;

    TimerIO->tr_node.io_Command = TR_ADDREQUEST;
    TimerIO->tr_time.tv_secs    = 0;
    TimerIO->tr_time.tv_micro   = WaitTime;

    SendIO( TimerIO);

    Wait( 1L << ConsoleReadMP->mp_SigBit | 1L << SerialReadMP->mp_SigBit | 1L << TimerMP->mp_SigBit);

    if( !( ConsoleDevice) && CheckIO( ConsoleReadIO))    BufferedChars = TRUE;
    else if( !( SerialDevice) && CheckIO( SerialReadIO)) BufferedChars = TRUE;

    if( !( CheckIO( TimerIO)))
    {
        AbortIO( TimerIO);
        WaitIO( TimerIO);
        Wait( 1L << TimerMP->mp_SigBit);
    }
    else WaitIO( TimerIO);

    return BufferedChars;
}
//-
//+ void DGetChar( unsigned char *BufferChar)
void DGetChar( unsigned char *BufferChar)
{
    long    MinutesCount = 5;

    while( TRUE)
    {
        if( !( SerialDevice))
        {
            SerialWriteIO->IOSer.io_Command  = SDCMD_QUERY;
            DoIO( SerialWriteIO);

            if( SerialWriteIO->io_Status & 0x20) Close_All( FALSE);

            SerialWriteIO->IOSer.io_Command  = CMD_WRITE;
        }

        if( !( ConsoleDevice) && CheckIO( ConsoleReadIO))
        {
            WaitIO( ConsoleReadIO);

            ConsoleChar = ConsoleReadChar;

            SendIO( ConsoleReadIO);

            if( ConsoleChar == 0x0d) *BufferChar = ConsoleChar = 0x0a;
            else                     *BufferChar = ConsoleChar;

            #ifdef DEBUG
            printf( "DGetCharC: %x\n", ConsoleChar);
            #endif

            return;
        }
        else
        if( !( SerialDevice) && CheckIO( SerialReadIO))
        {
            WaitIO( SerialReadIO);

            SerialChar = SerialReadChar;

            SendIO( SerialReadIO);

            switch( SerialChar)
            {
                case 0x0d :
                {
                    LastSerialChar = 0x0d;

                    #ifdef DEBUG
                    printf( "DGetCharS: received 0x0d, transforming...");
                    #endif

                    *BufferChar = SerialChar = 0x0a;
                }
                break;

                case 0x0a :
                {
                    if( LastSerialChar == 0x0d)
                    {
                        LastSerialChar = 0x0a;

                        #ifdef DEBUG
                        printf( "DGetCharS: received 0x0a, skipping...");
                        #endif

                        DGetChar( BufferChar);
                    }
                    else *BufferChar = LastSerialChar = SerialChar;
                }
                break;

                case 0x00 :
                {
                    LastSerialChar = 0x00;

                    #ifdef DEBUG
                    printf( "DGetCharS: received 0x00, reget...");
                    #endif

                    DGetChar( BufferChar);
                }
                break;

                default :
                {
                    *BufferChar = LastSerialChar = SerialChar;
                }
                break;
            }

            #ifdef DEBUG
            printf( "DGetCharS: %x\n", SerialChar);
            #endif

            return;
        }

        TimerIO->tr_node.io_Command = TR_ADDREQUEST;
        TimerIO->tr_time.tv_secs    = 60;
        TimerIO->tr_time.tv_micro   = 0;

        SendIO( TimerIO);

        Wait( 1L << ConsoleReadMP->mp_SigBit | 1L << SerialReadMP->mp_SigBit | 1L << TimerMP->mp_SigBit);

        if( CheckIO( TimerIO))
        {
            WaitIO( TimerIO);

            if( --MinutesCount)
            {
                PutStringF( "\r\nKeine Reaktion vom User! %d Minuten bis zum Logoff.\r\n\a", MinutesCount);
            }
            else
            {
                PutString( "\r\nAuf wiedersehen! Bis zum naechsten Anruf...\r\n");

                FPutsLog( "Kicked out.");

                Close_All( NULL);
            }
        }
        else
        {
            AbortIO( TimerIO);
            WaitIO( TimerIO);
            Wait( 1 << TimerMP->mp_SigBit);
        }
    }
} //-
//+ void GetChar( unsigned char *BufferChar)
void GetChar( unsigned char *BufferChar)
{
    DGetChar( BufferChar);
    PutChar( *BufferChar);
} //-
//+ BOOL GetString( char *String)
BOOL GetString( char *String)
{
    long    StringCount = 0;

    do
    {
        GetChar( &String[ StringCount ]);

        if( String[ StringCount ] == 0x08)
        {
            PutChar( 0x20);

            if( StringCount)
            {
                String[ StringCount-- ] = 0x00;
                PutChar( 0x08);
            }
        }
        else if( String[ StringCount ] == 0x03)
        {
            String[ StringCount ] = 0x00;
            return( FALSE);
        }
        else if( String[ StringCount ] != 0x0a) StringCount++;
    }
    while( StringCount != ( STRING_LENGTH - 1) && String[ StringCount ] != 0x0a);

    String[ StringCount ] = 0x00;

    return( TRUE);
} //-
//+ BOOL GetStringHidden( char *String)
BOOL GetStringHidden( char *String)
{
    long    StringCount = 0;

    do
    {
        DGetChar( &String[ StringCount ]);

        if( String[ StringCount] == 0x08)
        {
            if( StringCount)
            {
                String[ StringCount--] = 0x00;
                PutChar( 0x08);
                PutChar( 0x20);
                PutChar( 0x08);
            }
        }
        else if( String[ StringCount] == 0x03)
        {
            String[ StringCount] = 0x00;
            return( FALSE);
        }
        else if( String[ StringCount] != 0x0a)
        {
            PutChar( '*');
            StringCount++;
        }
        else PutChar( 0x0a);
    }
    while( StringCount != ( STRING_LENGTH - 1) && String[ StringCount ] != 0x0a);

    String[ StringCount ] = 0x00;

    return( TRUE);
} //-
