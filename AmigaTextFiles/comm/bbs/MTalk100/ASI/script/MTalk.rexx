/* Simulate the MTalk client with AREXX! */
/* Written by Sam Yee.  Aug 31, 1993 */
/* Freely Distributable.  Do whatever you want with it! */

outfile = 'pipe:sam'    /* file where server sends data */
me      = 'Sam'         /* user id */
cat     = 'mycat <'     /* which command to use to read pipe? */

address 'MTalkD_REXX'   /* default Mtalk server port */
say "Please press return whenever you want to update output!"
say ""
command = ''

login id me msgfile outfile

/* output anything in the pipe */
address command 'echo >' outfile    /* make sure the pipe won't block the cat. */
address command cat outfile         /* normally, if pipe is "completely" empty cat
                                       will wait until pipe has something in there */
do forever
    parse pull command              /* get a command line from user */

    if upper(command) = '/QUIT' then    /* quit? */
       leave

    exe id me cmd command   /* tell server to do something */

    if upper(command) = '/DIE' then /* kill server?, if so let's get out! */
       exit

    /* output anything in the pipe */
    address command 'echo >' outfile
    address command cat outfile
end

logout id me    /* effectively executing /quit */
