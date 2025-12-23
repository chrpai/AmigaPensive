/***************************************************************************

 © copyright 1993 P. Eastham

*****************************************************************************/

/* USER:<uname>/M-Tag.CDL
   This holds the list of files which have been deemed to be CD files. 
   Sorted on the first field. */

struct M_Tag_CDL
{
  char    Filename[36]
  char    Fdpath[53] /* The path which SHOULD be the comment. eg:
                        FILE:125/5.fd */
};


/* T:<port>.MArea
   This file holds the current users area list. If producing, it MUST have a
   filenote of the users name as returned from T:<port>.User. It MUST also be
   in assending order of file area. */

struct M_Tag_Area
{
  SHORT   Area_Number   /* The area of the number the user has access to. */
  char    Area_Name[34] /* The name of the area the user has access to. */
  char    Area_Path[24] /* The path to where the files are stored */
};
