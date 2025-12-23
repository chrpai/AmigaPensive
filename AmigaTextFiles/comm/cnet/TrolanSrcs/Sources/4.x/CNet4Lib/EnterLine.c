#include "cnet4lib.h"

/**************************************************************************
 *    ENTERLINE FLAGS:                                                    *
 *    Construct by ORing the following bit defines:                       *
 *                                                                        *
 * ie. EnterLine(3, ELINE_ALLCAPS|ELINE_INPOUTBOX, "My Prompt>");         *
 *                                                                        *
 *     FLAGS:                                                             *
 *     ------                                                             *
 *     ELINE_ALLCAPS       = All capitalized                              *
 *     ELINE_FILENAME      = FILENAME.  Don't allow =":; or asterisk      *
 *     ELINE_USEINBUFF     = Begin with existing z.InBuffer               *
 *     ELINE_NOLEADSPACE   = Chop leading spaces                          *
 *     ELINE_CAPWORDSTART  = Force 1st letter of word caps                *
 *     ELINE_RESTWORDLOWER = Force all others lower case                  *
 *     ELINE_NUMBERSONLY   = Numeric input only                           *
 *     ELINE_INPUTBOX      = Print input box (terminated with . )         *
 *     ELINE_ALLOWMCI      = DO allow MCI                                 *
 *     ELINE_HANDLESPECIAL = HANDLES/SPECIAL.  Don't allow ^_`{|}~@       *
 *     ELINE_XSLASHSTART   = Exit for . or / at beginning of line         *
 *     ELINE_XBKSPACESTART = Exit for backspace at beginning of line      *
 *     ELINE_NOOLMS        = Do not allow OLM's to appear while editing   *
 *     ELINE_ALLOWCHAT     = Allow Chat break in at this prompt. COMMAND  *
 *                           PROMPT.                                      *
 *     ELINE_NOSPACES      = Don't allow SPACES                           *
 *     ELINE_NOCURSORMOVE  = DON'T ALLOW MOVEMENT                         *
 *     ELINE_NOSLASHES     = Don't allow forward slash                    *
 *                                                                        *
 *  NOTE: The old "numeric" flag values still work 100% ..                *
 **************************************************************************/
int EnterLine( UBYTE len, ULONG flags, char *prompt )
{
   cmess.arg1 = (ULONG)len;     /* how many chars max to input	*/
   cmess.arg2 = (ULONG)flags;   /* flags describing required input handling */
   cmess.arg3 = (ULONG)prompt;  /* text to print before input	*/
   CallHost( 2 );               /* result is in z->InBuffer	*/

   return( (int)strlen( z->InBuffer ));
}
