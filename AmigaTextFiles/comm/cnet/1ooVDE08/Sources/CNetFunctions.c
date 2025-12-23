void ShutDown( char *spawn )
{
	/* spawn = full path/file to run */
	if( spawn )
		strcpy( z->CSpawn, spawn );

	CallHost( 0 );
}

void CallHost( UBYTE c )
{
	cmess.command = c;
	PutMsg  ( (struct MsgPort *)cport, (struct Message *)&cmess );
	WaitPort( replyp );
	GetMsg  ( replyp );
}

void PutText( char *text )
{
	cmess.arg1 = (ULONG)text;	/* text to print		*/
	CallHost( 1 );
}

void PutA( void )
{
	PutText( z->ABuffer );
}

/**************************************************************************
 *    ENTERLINE FLAGS:                                                    *
 *      1: All capitalized                                                *
 *      2: FILENAME.  Don't allow =":; or asterisk                        *
 *      4: Begin with existing z.InBuffer                                 *
 *      8: Chop leading spaces                                            *
 *     16: Force 1st letter of word caps                                  *
 *     32: Force all others lower case                                    *
 *     64: Numeric input only                                             *
 *    128: Print input box (.)                                            *
 *    256: DO allow MCI                                                   *
 *    512: HANDLES/SPECIAL.  Don't allow ^_`{|}~@                         *
 *   1024: Exit for . or / at beginning of line                           *
 *   2048: Exit for backspace at beginning of line                        *
 *   4096: Do not allow OLM's to appear while editing                     *
 *   8192: Allow Chat break in at this prompt. COMMAND PROMPT.            *
 *  16384: Don't allow SPACE, either                                      *
 *  32768: DON'T ALLOW MOVEMENT                                           *
 *  65536: Don't allow forward slash                                      *
 **************************************************************************/
int EnterLine( UBYTE len, ULONG flags, char *prompt )
{
	cmess.arg1 = (ULONG)len;	/* how many chars max to input	*/
	cmess.arg2 = (ULONG)flags;	/* 1=UpperCase			*/
	cmess.arg3 = (ULONG)prompt;	/* text to print before input	*/
	CallHost( 2 );			/* result is in z->InBuffer	*/
	return( (int)strlen( z->InBuffer ));
}

short FindAccount( char *a, struct UserData *b )
{
	cmess.arg1 = (ULONG)a;
	cmess.arg2 = (ULONG)b;
	CallHost( 20 );
	return( (short)cmess.result );
}


short VisualDataEditor( char *file, void *data, long size )
{
	cmess.arg1 = (ULONG)file;
	cmess.arg2 = (ULONG)data;
	cmess.arg3 = (ULONG)size;
	CallHost( 40 );
	return( (short)cmess.result );
}
