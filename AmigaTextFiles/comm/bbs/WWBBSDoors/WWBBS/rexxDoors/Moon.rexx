/* Moon.rexx        by Greg Cunningham 1990       converted from Unix TODAY.C
 *
 * GMT Settings: (America)
 *
 * Eastern  5
 * Central  6
 * Mountain 7
 * Pacific  8
 *
 */

CR='0D'x
TimeZone=8
DaylightSavings=0 /* set to 1 if daylight */

if ~Show(L,'rexxmathlib.library') then
   if ~AddLib('rexxmathlib.library',0,-30,0) then
      Say 'No RexxMathLib, running anyway.'CR

say lunar(date('S'),time(),TimeZone-DaylightSavings)||CR

exit

/* Phase of the Moon. Calculates the current phase of the moon.
 * Based on routines from `Practical Astronomy with Your Calculator',
 * by Duffett-Smith.
 *
 * Synopsis:
 *
 * string=lunar(year,time,gmt)
 * date  -- Current sorted date
 * time    -- Current time of the day
 * gmt     -- Hours from GMT time zone (subtract one if daylight savings)
 *
 * say lunar(date('S'),time(),0) current phase of the moon
 * say lunar(date('S')+1,time(),0)  same time tommorow
 *
 * say lunar('19900225','12:00:00',0)  Feb 25,1990 at noon
 *
 */

lunar:   procedure
   arg cdate,ctime,GMToffset

   /* days between Jan 1,1985 and Now */
   days=date('B',cdate,'S')-date('B','19850101','S')+1

   /* add fraction of day */
   parse var ctime hour':'min':'sec
   hour=hour+GMToffset
   if(hour>23) then do
      days=days+1
      hour=hour-24
   end
   /* add on percentage of day */
   days=days+((hour+(min/60)+(sec/3600))/24)

   phase=potm(days)

   cp='The Moon is'

   if(phase=0) then     cp=cp 'New'
   else if(phase=100) then    cp=cp 'Full'
   else if(phase=50) then do
      phase2=potm(days+1)
      if(phase2>phase) then   cp=cp 'at the First Quarter'
      else        cp=cp 'at the Last Quarter'
   end
   else if(phase>50) then do
      phase2=potm(days+1)
      if(phase2>phase) then   cp=cp 'Waxing'
      else        cp=cp 'Waning'
      cp=cp 'Gibbous ('phase'% Full)'
   end
   else do
      phase2=potm(days+1)
      if(phase2>phase) then   cp=cp 'Waxing'
      else        cp=cp 'Waning'
      cp=cp 'Crescent ('phase'% Full)'
   end
   return cp


/* calc phase-of-the-moon; returns percentage of full */
potm: procedure
   arg days

   EPSILONg=279.611371  /* solar ecliptic long at EPOCH */
   RHOg  =282.680403 /* solar ecliptic long of perigee at EPOCH */
   e  =0.01671542 /* solar orbit eccentricity */
   lzero =18.251907  /* lunar mean long at EPOCH */
   Pzero =192.917585 /* lunar mean long of perigee at EPOCH */
   Nzero =55.204723  /* lunar mean long of node at EPOCH */
   PI =3.141592654

   N= 360 * days / 365.2422   /* days*360/real_days_of_a_year */
   N=pos360(N)       /* round up to positive 360 degrees */

   Msol=N+EPSILONg-RHOg
   Msol=pos360(Msol)

   Ec=360 / PI * e * sin( dtor(Msol) )

   LambdaSol = N + Ec + EPSILONg
   LambdaSol = pos360(LambdaSol)

   l = 13.1763966 * days + lzero
   l = pos360(l)

   Mm = l - (0.1114041 * days) - Pzero
   Mm = pos360(Mm)

   Nm = Nzero - (0.0529539 * days)
   Nm = pos360(Nm)

   Ev = 1.2739 * sin( dtor(2*(l - LambdaSol) - Mm) )

   Ac = 0.1858 * sin( dtor(Msol) )
   A3 = 0.37 * sin( dtor(Msol) )

   Mmprime = Mm + Ev - Ac - A3

   Ec = 6.2886 * sin( dtor(Mmprime) )

   A4 = 0.214 * sin( dtor(2 * Mmprime) )

   lprime = l + Ev + Ec - Ac + A4

   V = 0.6583 * sin(dtor(2 * (lprime - LambdaSol)))

   ldprime = lprime + V

   D = ldprime - LambdaSol

   perc= 50 * (1 - cos( dtor(D) ))

   return trunc(perc+.5)   /* return percent rounded to nearest integer */



/* get a positive 360 degree mod */
pos360: arg deg

   deg=deg//360
   if(deg<0) then deg=deg+360
   return deg


/* convert degrees to radians */
dtor: procedure
   arg deg

   PI=3.141592654
   return (deg * PI) / 180

