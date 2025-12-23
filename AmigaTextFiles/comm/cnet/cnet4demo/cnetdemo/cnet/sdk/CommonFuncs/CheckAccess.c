BOOL CheckAccess( LONG MandAccess, BYTE Access )
{
	/* MandAcesss is the accesses allowed, Access is the user's access */

	if(MandAccess & 1<<Access)
		return TRUE;
	return FALSE;
}
