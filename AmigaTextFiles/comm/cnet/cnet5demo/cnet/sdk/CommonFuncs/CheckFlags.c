BOOL CheckFlags(LONG MandFlags, LONG Flags)
{
	short xxx=0;

	/* MandFlags - user must have ALL these flags! */
	/* Flags - Flags user has */

	if( (MandFlags & Flags) == Flags)
		return TRUE;
	return FALSE;
}
