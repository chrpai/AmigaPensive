/* Example usage of SETITEMFLAGS and ADDITEM
 *
 * Please note that SETITEMFLAGS and ADDITEM cannot be used from
 * AmigaDOS ARexx scripts.  Any scripts that use these ARexx commands
 * must be executed ONLINE.  eg. By an online user or CNet BotUser. */

options results
signal on error
signal on syntax

address CNETREXX0

SETITEMFLAGS "CLEAR"         /* clear the temp space used for adding new posts/resps */

SETITEMFLAGS "FROMACCT 1"    /* new post/response from account 1 */

SETITEMFLAGS "TITLE Test post from ARexx" /* title of post/response */

SETITEMFLAGS "LONG Just a test post.. Please ignore me."||d2c(10) /* Message body text */

SETITEMFLAGS "SUBBOARD getfreetest" /* go argument of destination subboard */

SETITEMFLAGS "ORG My Organization"  /* organization of message "author" */

ADDITEM /* Now add the item that we set attributes for above */
