/***************************************************************************/
/*                                                                         */
/*                                                                         */
/*              R A I N B O W   H Y D R A   v0.2   by R R R                */
/*                                                                         */
/*                                                                         */
/*            Based on FLUX POINT fphydra v0.2 - Thanks boys!              */
/*                                                                         */
/*                        Released 26-apr-95                               */
/*                                                                         */
/*    Always remember that Rainbow Hydra is named after the most famous    */
/*  Rainbow Dragon, which is a cunning enemy and a marvelous dwarf eater.  */
/*                                                                         */
/*   Adventurers! Resistance is useless! Rainbow Hydra is here and it's    */
/*                  going to devour your very livers!                      */
/*                                                                         */
/*                      RHydra Development team                            */
/*                                                                         */
/*                      Programming: Tailgunner                            */
/*                   On-site testing: Sorsselsson                          */
/*                      PC Test client: Verlorc                            */
/*                                                                         */
/*                  Contact - rrr@klinja.fipnet.fi                         */
/*                                                                         */
/***************************************************************************/
/*             CONFIG - Modify these to suit your needs                    */


usecur=0                            /* Use CURRENT AREA for uploads?   */
                                    /* 1=Use CURRENT, 0=Use GLOBAL     */

gdir="Udbase0:Hydra/"               /* Global Hydra UL dir             */

reward=2.00                         /* Upload time reward : 0.00=0%,   */
                                    /* 1.00=100%, 1.50=150% etc        */

cdaily=1                            /* When user uploads, remove the   */
                                    /* UL:ed byte&file amount from his */
                                    /* downloads of the day. So he     */
                                    /* can DL much more than his daily */
                                    /* limit allows. 1=YES, 0=NO       */

SS=1                                /* "Scan selected" at startup      */
                                    /* 1=Yes, 0=No                     */

/***************************************************************************/
/*   Nothing worth modifying after this line :)                            */
/***************************************************************************/

ver="$VER: Rainbow Hydra 0.2 (26.04.95) by RRR"

options results;signal on SYNTAX ; signal on ERROR ; signal on IOERR
if ~show('l','rexxsupport.library') then if ~addlib('rexxsupport.library',0,-30,0) then signal BYE

changewhere "RHydra xfer"

getuser 1410474;if ~bittst(d2c(result,4),5) then do      /* Check for shared mode */
    transmit "Sorry, this node is not in the Shared Mode. Nag to SysOp."
    logentry "RHydra - Port is not in SHARED MODE!"
    exit
    end

getuser 1310486;device=result;if device='' then do       /* Local Port */
    transmit "Sorry, there is no device defined for that node. Nag to SysOp."
    logentry "RHydra - What the fuck? Port has no device!"
    exit
    end

getuser 1410470;serport=result
getuser 1210478;maxbaud=result*10
getuser 1;handle=result                         /* Users handle (for ?logs) */
getuser 12;aika=result                          /* Current time (for ?logs) */
getuser 23;port=result                          /* Port Number */
getuser 24;baud=result*10                       /* Baud Rate of User Online */
getuser 25;gfr=result                      /* Users personal file-ratio #1 */
getuser 26;gbr=result                      /* Users personal byte-ratio #1 */
getuser 1311992;pfdir=result               /* PFile Root Dir */
if usecur=1 then do                        /* Use current dir for uploads! */
         getuser 1306724;curdir=result
         getuser 54;if ~result then call PERSE /* Is UL allowed on this area? */         */
end
if usecur=0 then curdir=gdir               /* Use global hydra UL dir */
getuser 1307954;uudir=result               /* Users own Dir (UUCP Dir) */
getuser 34;avfile=result                   /* Total Available File Credits */
getuser 35;avbyte=result                   /* Total Available Byte Credits */
getuser 1200686;maxdf=result               /* Max downloads (files) per day */
getuser 1400664;maxdb=result               /* Max downloads (bytes) per day */
getuser 1400532;dfoc=result                /* Files downloaded today */
getuser 1400528;dboc=result                /* Bytes downloaded today */

/* Infinite file/byte/daily credz? */

getuser 1100663;krah=d2c(result+(result<0)*256)
ffdl=BitTST(krah,6);fbdl=BitTST(krah,7)

if maxdf=0 then maxdf=99999       /* Should be enough for everyone? */
if maxdb=0 then maxdb=999999999

maxdf=maxdf-dfoc
maxdb=maxdb-dboc

if SS=1 then do
        transmit "n1Files selected for download:"
        BBSCOMMAND "SS"
end

/* Create a Hydra download list - Don't add files if not enough credz */

getuser 1401978;sbuf=x2c(d2x(result,8))
fil=uudir'HYDRA.DL';paska="///"
call open(fp,fil,'W');tot=1;getuser 1209644;do a=1 to result
        mullus=0
        tm=import(sbuf,156);sbuf=offset(sbuf,156);parse var tm 5 fi'00'x 36 pt'00'x 136 q 137 fs 138 q 139 br 143 fr +2 .
        file.tot=pt||fi;brate.tot=c2d(br);frate.tot=c2d(fr);mullus=c2d(fs,1)
        if mullus=1 then do
            transmit (fi" is FREE download...")
            freedl.tot=pt||fi
            tot=tot+1
            paska=pt||fi
            call writeln fp, paska
            iterate
            end
        if avfile-frate.tot<0&ffdl=0 then do
            transmit "Not enough file credits for "fi", skipping."
            iterate
            end
        if maxdf-frate.tot<0 then do
            transmit "Not enough daily files for "fi", skipping."
            iterate
            end
        if avbyte-brate.tot<0&fbdl=0 then do
            transmit "Not enough byte credits for "fi", skipping."
            iterate
            end
        if maxdb-brate.tot<0 then do
            transmit "Not enough daily bytes for "fi", skipping."
            iterate
            end
        avfile=avfile-frate.tot;avbyte=avbyte-brate.tot;
        maxdf=maxdf-frate.tot;maxdb=maxdb-brate.tot
        freedl.tot="///"                             /* Not a free dl */
        tot=tot+1;paska=pt||fi
        call writeln fp, paska
end;call close(fp);count=tot

if SS=0&paska="///" then transmit "n1No files selected for download, starting anyway..."

if exists('pfiles:hydra/banner.txt') then sendfile ('pfiles:hydra/banner.txt')

transmit "n1RAiNBOW HYDRA v0.2 ready to transfer, ^X to abort..."

setobject 0;putuser 1209644     /* Clear CNet's Select buffer.  */
setobject 1;putuser 1110341     /* FreezeIdle, stop idle timer. */

/* Starting HydraCOM (These setups are for Olaf's revision 5) */

signal off ERROR
address command 'PFiles:Hydra/HYDRACOM DEV 'device' POR 'serport' SPE 'maxbaud' LIN 'baud' handshake HARD nostamp nobell RES 'pfdir'HydraLog'port' REC 'curdir' SEN @'fil
signal on ERROR

setobject 0;putuser 1110341     /* Enable idle timer again.     */

call delete(fil)

transmit "Transfer complete."
transmit ""

bufferflush       /* This removes annoying letters Hydra sends at termination */

/* Check for existing logs and facelift Hydra log :) */

address command ('sort 'pfdir'HydraLog'port' 'pfdir'HydraLog'port)

if exists("sysdata:log/uloads") then do
        call open ulog, "sysdata:log/uloads", 'A'
        uloga=1
        end
if exists("sysdata:log/dloads") then do
        call open dlog, "sysdata:log/dloads", 'A'
        dloga=1
        end

/* Adjust credits & other values */

tukb=0;tdkb=0;tdf=0;tuf=0;tgive=0
call open(fp,pfdir'HydraLog'port,'R')
do until EOF(fp)
        z=readln(fp);parse var z ttype size . . cps . . . . . name .

        if ttype='H' then do                                          /* A download! */
                call FREEDL                                           /* Check for FREEDL */
                if vapaad=1 then do
                        logentry "FREEHDL: "name", "size"b, "cps" cps."
                        if dloga=1 then call writeln dlog, ("c2"aika" cc"left(handle,19)" c6H"baud/100" "cps" "left(right(name,12),15)" "size/1000)
                        tdkb=tdkb+size
                        tdf=tdf+1
                end
                if vapaad=1 then iterate
        end

        if ttype='H' then do
                getuser 34;setobject result-1;putuser 34              /* Distract credz */
                getuser 35;setobject result-size;putuser 35
                getuser 1400532;setobject result+1;putuser 1400532    /* Modify the DL-today variables */
                getuser 1400528;setobject result+size;putuser 1400528

                logentry "HydraDL: "name", "size"b, "cps" cps."
                if dloga=1 then call writeln dlog, ("c2"aika" cc"left(handle,19)" c6H"baud/100" "cps" "left(right(name,12),15)" "size/1000)
                tdkb=tdkb+size
                tdf=tdf+1
        end

        if ttype='R' then do                                          /* An upload! */
                getuser 34;setobject result+gfr;putuser 34            /* Add credits */
                getuser 35;setobject result+size*gbr;putuser 35
                getuser 1200532;setobject result+1;putuser 1200532    /* Modify the UL-today variables */
                getuser 1200526;setobject result+size;putuser 1200526

                if cdaily=1 then call MAUKKA

                logentry "HydraUL: "name", "size"b, "cps" cps."
                if uloga=1 then call writeln ulog, ("c2"aika" cc"left(handle,19)" c6H"baud/100" "cps" "left(right(name,12),15)" "size/1000)

                tgive=tgive+((size/cps)/60)*reward

                transmit "Please enter one line description for "name /* Asks for description, adds as a filenote */
                prompt 78 normal ": ";desc=result
                address command ('filenote 'curdir||name' "'desc'"')
                transmit ""
                tukb=tukb+size
                tuf=tuf+1
        end

end

call close(fp);call delete(pfdir'HydraLog'port)

if uloga=1 then call close ulog
if dloga=1 then call close dlog
tdkb=tdkb/1000;tukb=tukb/1000

addtime tgive

getuser 30;setobject result+tukb;putuser 30          /* User's total upload KB add */
getuser 31;setobject result+tuf;putuser 31           /* User's total upload fi add */
getuser 32;setobject result+tdkb;putuser 32          /* User's total download KB add */
getuser 33;setobject result+tdf;putuser 33           /* User's total download fi add */
transmit "Last transfer - UL: "tuf" files, "tukb" kilobytes. (UL time bonus: "tgive" min.)"
transmit "                DL: "tdf" files, "tdkb" kilobytes."
transmit ""

exit

/* Subroutines */

CHECK:;if ARG() & ARG(1)~='###PANIC' then return ARG(1)
getcarrier;if result='TRUE' then if ARG() then return ARG(1);else return
logentry 'Lost Carrier!!';bufferflush;exit

ERROR:;IOERR:;SYNTAX:;em='Error in line: 'sigl' Code: 'errortext(rc);logentry em;transmit em;bufferflush;exit

PERSE:
    transmit ""
    transmit "Sorry, you are not allowed to upload on this area."
    transmit "Please go to a suitable area and type HYDRA again."
    transmit ""
    exit

return

MAUKKA:

    /* Removes the uploads from user's downloads-of-the-day */

    getuser 1400532;setobject result-1;putuser 1400532
    getuser 1400528;setobject result-size;putuser 1400528

return

FREEDL:

tot=0;vapaad=0
do until tot=count
    if name=freedl.tot then vapaad=1
    tot=tot+1
end

return


/***************************************************************************/
