// Output.c

#include    "Output.h"

extern long    SerialDevice;
extern long    ConsoleDevice;

extern struct MsgPort  *SerialWriteMP;
extern struct IOExtSer *SerialWriteIO;

extern struct MsgPort      *ConsoleWriteMP;
extern struct IOStdReq     *ConsoleWriteIO;

//+ void PutChar( unsigned char BufferChar)
void PutChar( unsigned char BufferChar)
{
    if( !( SerialDevice))   SerialWriteIO->IOSer.io_Command = CMD_WRITE;
    if( !( ConsoleDevice))  ConsoleWriteIO->io_Command      = CMD_WRITE;

    switch ( BufferChar)
    {
        case 0x0a :
        {
            if( !( SerialDevice))
            {
                SerialWriteIO->IOSer.io_Length  = 2;
                SerialWriteIO->IOSer.io_Data    = "\r\n";
            }

            if( !( ConsoleDevice))
            {
                ConsoleWriteIO->io_Length       = 2;
                ConsoleWriteIO->io_Data         = "\r\n";
            }
        }
        break;

        case 0x9b :
        {
            if( !( SerialDevice))
            {
                SerialWriteIO->IOSer.io_Length  = 2;
                SerialWriteIO->IOSer.io_Data    = "[";
            }

            if( !( ConsoleDevice))
            {
                ConsoleWriteIO->io_Length       = 2;
                ConsoleWriteIO->io_Data         = "[";
            }
        }
        break;

        default :
        {
            if( !( SerialDevice))
            {
                SerialWriteIO->IOSer.io_Length  = 1;
                SerialWriteIO->IOSer.io_Data    = &BufferChar;
            }

            if( !( ConsoleDevice))
            {
                ConsoleWriteIO->io_Length       = 1;
                ConsoleWriteIO->io_Data         = &BufferChar;
            }
        }
    }

    if( !( SerialDevice))   DoIO( SerialWriteIO);
    if( !( ConsoleDevice))  DoIO( ConsoleWriteIO);
} //-
//+ void PutString( char *String)
void PutString( char *String)
{
    if( !( SerialDevice))
    {
        SerialWriteIO->IOSer.io_Command = CMD_WRITE;
        SerialWriteIO->IOSer.io_Length  = strlen( String);
        SerialWriteIO->IOSer.io_Data    = String;

        DoIO( SerialWriteIO);
    }

    if( !( ConsoleDevice))
    {
        ConsoleWriteIO->io_Command      = CMD_WRITE;
        ConsoleWriteIO->io_Length       = strlen( String);
        ConsoleWriteIO->io_Data         = String;

        DoIO( ConsoleWriteIO);
    }
} //-
//+ void PutStringF( char *FString, ...)
void PutStringF( char *FString, ...)
{
    va_list Arguments;

    char    String[ 120 ];

    va_start( Arguments, FString);

    vsprintf( String, FString, Arguments);
    PutString( String);

    va_end( Arguments);
} //-
//+ void PutData( char *Data, long Length)
void PutData( char *Data, long Length)
{
    if( !( SerialDevice))
    {
        SerialWriteIO->IOSer.io_Command = CMD_WRITE;
        SerialWriteIO->IOSer.io_Length  = Length;
        SerialWriteIO->IOSer.io_Data    = Data;

        DoIO( SerialWriteIO);
    }

    if( !( ConsoleDevice))
    {
        ConsoleWriteIO->io_Command  = CMD_WRITE;
        ConsoleWriteIO->io_Length   = Length;
        ConsoleWriteIO->io_Data     = Data;

        DoIO( ConsoleWriteIO);
    }
} //-
