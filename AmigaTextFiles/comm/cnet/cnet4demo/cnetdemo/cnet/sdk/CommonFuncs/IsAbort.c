BOOL IsAbort( void );         /* prototype */




if(IsAbort()) {          /* this goes in your function */
	/* your code to exit or whatever you want to do on abortion */
	GetOut();
	}




BOOL IsAbort( void )    /* the actual function that checks for space or slash */
{
	CheckFlowControl();
	if (z->PrintFlags >=1)
		{
		if (z->PrintFlags == 1) PutText("[SKIP]");
		if (z->PrintFlags > 1) PutText("^C");
		return TRUE;
		}
return FALSE;
}
