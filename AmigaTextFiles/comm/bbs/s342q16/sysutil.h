void getUtilDate ( int *year , int *month , int *day , int *hours , int *minutes );
void getRawDate ( int *year , int *month , int *day , int *hours , int *minutes , int *seconds , int *milli );
int strCmpU ( char s[], char  t[]);
int UtilGetch ( void );
int NEUtilGetch ( void );
FILE *safeopen ( char *name , char *mode );
char *LastOn ( long lastdate , char shortstyle );
void civTime(int *hours, char **which);
