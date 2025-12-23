/*

	Proto/Pragmas fuer clmacro.library V3.0

*/


#pragma libcall CLMacroBase CLMacro_Encode 1E 09803
#pragma libcall CLMacroBase CLMacro_Decode 24 9802
#pragma libcall CLMacroBase CLMacro_Flush 2a 0

extern struct Library *CLMacroBase;

long CLMacro_Encode( char *from, char *to, long maxlen );
long CLMacro_Decode( char *from, char *to );
void CLMacro_Flush( void );
