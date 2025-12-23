/** xproto.h
*
*   Include file for External Protocol Handling
*
**/
/*
*   The structure
*/
struct XPR_IO {
                  char  *xpr_filename;      /* File name(s)             */
                  long  *xpr_data;          /* Initialized by Setup.    */
              };
/*
*   Number of defined extensions
*/
#define XPR_EXTENSION 4L

/*
*   Flags returned by XProtocolSetup()
*/
#define XPRS_FAILURE    0x00000000L
#define XPRS_SUCCESS    0x00000001L
#define XPRS_NORECREQ   0x00000002L
#define XPRS_NOSNDREQ   0x00000004L
#define XPRS_HOSTMON    0x00000008L
#define XPRS_USERMON    0x00000010L
#define XPRS_HOSTNOWAIT 0x00000020L
/*
*   The update structure
*/
struct XPR_UPDATE {     long  xpru_updatemask;
                        char *xpru_protocol;
                        char *xpru_filename;
                        long  xpru_filesize;
                        char *xpru_msg;
                        char *xpru_errormsg;
                        long  xpru_blocks;
                        long  xpru_blocksize;
                        long  xpru_bytes;
                        long  xpru_errors;
                        long  xpru_timeouts;
                        long  xpru_packettype;
                        long  xpru_packetdelay;
                        long  xpru_chardelay;
                        char *xpru_blockcheck;
                        char *xpru_expecttime;
                        char *xpru_elapsedtime;
                        long  xpru_datarate;
                        long  xpru_reserved1;
                        long  xpru_reserved2;
                        long  xpru_reserved3;
                        long  xpru_reserved4;
                        long  xpru_reserved5;
                   };
/*
*   The possible bit values for the xpru_updatemask are:
*/
#define XPRU_PROTOCOL           0x00000001L
#define XPRU_FILENAME           0x00000002L
#define XPRU_FILESIZE           0x00000004L
#define XPRU_MSG                0x00000008L
#define XPRU_ERRORMSG           0x00000010L
#define XPRU_BLOCKS             0x00000020L
#define XPRU_BLOCKSIZE          0x00000040L
#define XPRU_BYTES              0x00000080L
#define XPRU_ERRORS             0x00000100L
#define XPRU_TIMEOUTS           0x00000200L
#define XPRU_PACKETTYPE         0x00000400L
#define XPRU_PACKETDELAY        0x00000800L
#define XPRU_CHARDELAY          0x00001000L
#define XPRU_BLOCKCHECK         0x00002000L
#define XPRU_EXPECTTIME         0x00004000L
#define XPRU_ELAPSEDTIME        0x00008000L
#define XPRU_DATARATE           0x00010000L
/*
*   The xpro_option structure
*/
struct xpr_option {
   char *xpro_description;      /* description of the option                  */
   long  xpro_type;             /* type of option                             */
   char *xpro_value;            /* pointer to a buffer with the current value */
   long  xpro_length;           /* buffer size                                */
};
/*
*   Valid values for xpro_type are:
*/
#define XPRO_BOOLEAN 1L         /* xpro_value is "yes", "no", "on" or "off"   */
#define XPRO_LONG    2L         /* xpro_value is string representing a number */
#define XPRO_STRING  3L         /* xpro_value is a string                     */
#define XPRO_HEADER  4L         /* xpro_value is ignored                      */
#define XPRO_COMMAND 5L         /* xpro_value is ignored                      */
#define XPRO_COMMPAR 6L         /* xpro_value contains command parameters     */
