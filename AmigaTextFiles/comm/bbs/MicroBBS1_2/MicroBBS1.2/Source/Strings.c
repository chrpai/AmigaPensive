// Strings.c

#include "Strings.h"

//+ BOOL StrCmpNoCase( char *SrcString, char *DstString)
BOOL StrCmpNoCase( char *SrcString, char *DstString)
{
    while( *SrcString && *SrcString != 0x0a && tolower( *DstString) == tolower( *SrcString))
    {
        SrcString++;
        DstString++;
    }

    if(( *SrcString == 0x00 || *SrcString == 0x0a) && ( *DstString == 0x00 || *DstString == 0x0a))
        return( FALSE);
    else
        return( TRUE);
}
//-
//+ BOOL StrCmpNoCaseOpen( char *SrcString, char *DstString)
BOOL StrCmpNoCaseOpen( char *SrcString, char *DstString)
{
    while( *SrcString && tolower( *DstString) == tolower( *SrcString))
    {
        SrcString++;
        DstString++;
    }

    if( *SrcString == 0x00 || *SrcString == 0x0a)
        return( FALSE);
    else
        return( TRUE);
}
//-
