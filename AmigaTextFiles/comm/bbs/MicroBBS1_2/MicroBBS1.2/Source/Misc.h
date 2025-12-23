// Misc.h

#ifndef MBBS_MISC_H
#define MBBS_MISC_H

#define     TYPEBUFFER_SIZE     1024
#define     STRING_LENGTH       80
#define     COMMAND_LENGTH      120
#define     MAX_LINE_LENGTH     80
#define     TIMESTRING_SIZE     18

#define     FAIL            20

#define     CONSOLENAME     "console.device"

struct  StatsStruct
{
    long    *st_Calls;
    long    *st_Files;
};

struct  UserStruct
{
    long    *us_Calls;
    long    *us_Files;
    long    *us_MaxLines;
    long    *us_Level;
    char    **us_Password;
};

#endif
