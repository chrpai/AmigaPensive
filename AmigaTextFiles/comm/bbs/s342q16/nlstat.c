#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define UP "System brought up"
#define DOWN "System brought down"
#define NET "System in network mode"
#define NOLOGIN "<No Login>"
#define BAUD300 "300)"
#define BAUD1200 "1200)"
#define BAUD2400 "2400)"
#define BAUD4800 "4800)"
#define BAUD9600 "9600)"
#define BAUD14400 "14400)"
#define BAUD19200 "19200)"
#define CONSOLE "sysConsole)"
#define DEFAULT_READ "calllog.sys"
#define DEFAULT_WRITE "dumbstats"
int main (int , char **);
int main (int argc, char *argv[])
  {
  long calls, net, timeout, error, badword, chat;
  long nologin, restart, calltime[96], nettime[96], tottime[96];
  long duration[1440], baud[8], i, j, k, l, longest, lasthour, atime;
  long lastmin, h1, h2, m1, m2, newuser = 0, stay = 0, busytime[96];
  char firstdate[10], firsttime[10], buffer[80], *name, *date, *starthour,
  *startmin, *stophour, *stopmin, *other, *readname, *writename;
  char lasttime[10],lastdate[10];
  FILE *readfile, *writefile;

  calls = net = timeout = error = badword = chat = nologin = 0;
  restart = longest = atime = newuser = stay = 0;

  printf ("Citadel log file cruncher Version 1.01\n");
  printf ("  by Lobotosoft\n");
  for (i = 0; i < 96; ++i)
    {
    calltime[i] = 0;
    nettime[i] = 0;
    tottime[i] = 0;
    busytime[i] = 0;

    }
  for (i = 0; i < 8; ++i)     baud[i] = 0;
  for (i = 0; i < 1440; ++i)  duration[i] = 0;
  writename = DEFAULT_WRITE;
  readname  = DEFAULT_READ;
  if (argc > 2)  readname = argv[2];
  if (argc > 1)  writename = argv[1];
  readfile = fopen (readname, "r");
  if (readfile == NULL)
    {
    printf ("Unable to open log file [%s] for input!\n", readname);
    printf ("Usage: logcrunch [output file] [input file]\n");
    exit (0);

    }
  writefile = fopen (writename, "w");
  if (writefile == NULL)
    {
    printf ("Unable to open stats file [%s] for output!\n", writename);
    printf ("Usage: logcrunch [output file] [input file]\n");
    fclose (readfile);
    exit (0);

    }
  name      = buffer;
  date      = &buffer[24];
  starthour = &buffer[32];
  startmin  = &buffer[35];
  stophour  = &buffer[40];
  stopmin   = &buffer[43];
  other     = &buffer[47];
  fgets (buffer, 80, readfile);
  while (!feof (readfile))
    {
    if (strncmp (buffer, UP, strlen (UP)) == 0) ++restart;
    if (strlen (buffer) > 23  &&  buffer[22] == ':')
      {
      buffer[22] = 0;
      buffer[31] = 0;
      buffer[37] = 0;
      buffer[42] = 0;
      buffer[45] = 0;
      strcpy (lasttime, starthour);
      strcpy (lastdate, date);
      if (calls == 0  &&  net == 0)
        {
        strcpy (firsttime, starthour);
        strcpy (firstdate, date);
        buffer[34] = 0;
        lasthour = atoi (starthour);
        lastmin = atoi (startmin);

        }
      buffer[34] = 0;
      h1 = atoi (starthour);
      h2 = atoi (stophour);
      m1 = atoi (startmin);
      m2 = atoi (stopmin);
      i = h1 * 4 + m1 / 15;
      j = h2 * 4 + m2 / 15;
      k = lasthour * 4 + lastmin / 15;
      tottime[k] += (k + 1) * 15 - lasthour * 60 - lastmin;
      if (j < k)
        {
        for (l = k + 1; l < 96; ++l) tottime[l] += 15;
        for (l = 0; l < j + 1; ++l)  tottime[l] += 15;
        tottime[j] -= (j + 1) * 15 - h2 * 60 - m2;

        }
      else
        {
        for (l = k + 1; l < j + 1; ++l) tottime[l] += 15;
        tottime[j] -= (j + 1) * 15 - h2 * 60 - m2;

        }
      lasthour = h2;
      lastmin = m2;
      if (strncmp (name, NET, strlen (NET)) == 0)
        {
        ++net;
        ++nettime[i];
        busytime[i] += (i + 1) * 15 - h1 * 60 - m1;
        if (j < i)
          {
          for (l = i + 1; l < 96; ++l) busytime[l] += 15;
          for (l = 0; l < j + 1; ++l)  busytime[l] += 15;
          busytime[j] -= (j + 1) * 15 - h2 * 60 - m2;

          }
        else
          {
          for (l = i + 1; l < j + 1; ++l) busytime[l] += 15;
          busytime[j] -= (j + 1) * 15 - h2 * 60 - m2;

          }

        }
      else
        {
        ++calls;
        ++calltime[i];
        busytime[i] += (i + 1) * 15 - h1 * 60 - m1;
        if (j < i)
          {
          for (l = i + 1; l < 96; ++l) busytime[l] += 15;
          for (l = 0; l < j + 1; ++l)  busytime[l] += 15;
          busytime[j] -= (j + 1) * 15 - h2 * 60 - m2;

          }
        else
          {
          for (l = i + 1; l < j + 1; ++l) busytime[l] += 15;
          busytime[j] -= (j + 1) * 15 - h2 * 60 - m2;

          }
        l = (h2 - h1) * 60 + m2 - m1;
        if (l < 0)
        l += 1440;
        ++duration[l];
        if (l > longest) longest = l;
        atime += l;
        if (strncmp (NOLOGIN,    name, strlen (NOLOGIN))  == 0) ++nologin;
        if (strncmp (CONSOLE,   other, strlen (CONSOLE))  == 0)++baud[0];
        if (strncmp (BAUD300,   other, strlen (BAUD300))  == 0)++baud[1];
        if (strncmp (BAUD1200,  other, strlen (BAUD1200)) == 0)++baud[2];
        if (strncmp (BAUD2400,  other, strlen (BAUD2400)) == 0)++baud[3];
        if (strncmp (BAUD4800,  other, strlen (BAUD4800)) == 0)++baud[4];
        if (strncmp (BAUD9600,  other, strlen (BAUD9600)) == 0)++baud[5];
        if (strncmp (BAUD14400, other, strlen (BAUD14400))== 0)++baud[6];
        if (strncmp (BAUD19200, other, strlen (BAUD19200))== 0)++baud[7];
        for (l = 0; other[l] != ')'  &&  other[l] != 0; ++l);
        for (;other[l] != 0; ++l)
          {
          if (other[l] == '+')
             ++newuser;
          else if (other[l] == '-')
             ++stay;
          else if (other[l] == 't'  ||  other[l] == 'T')
             ++timeout;
          else if (other[l] == 'e'  ||  other[l] == 'E')
             ++error;
          else if (other[l] == 'b'  ||  other[l] == 'B')
             ++badword;
          else if (other[l] == 'c'  ||  other[l] == 'C')
             ++chat;

          }

        }

      }
    fgets (buffer, 80, readfile);
    printf ("%ld calls, %ld net sessions\r", calls, net);

    }
  printf ("\nCrunching data....\n");
  fprintf (writefile, "        System Usage Data\n \n");
  fprintf (writefile, "  Based on usage from  %s, %s to %s, %s\n\n"
  , firstdate,firsttime,lastdate,lasttime);
  fprintf (writefile, "        SYSTEM CALL DATA: \n\n");
  fprintf (writefile, " There were %ld restarts during this period.\n", restart);
  fprintf (writefile, " There were %ld calls, and %ld network sessions\n", calls, net);
  fprintf (writefile, "                     Number   Percent\n");
  fprintf (writefile, " -------------------------------------\n");
  fprintf (writefile, " New Users            %5ld     %3ld\n", newuser, 100 * newuser / calls);
  fprintf (writefile, " No Login Calls       %5ld     %3ld\n", nologin, 100 * nologin / calls);
  fprintf (writefile, " .Terminate Stay      %5ld     %3ld\n", stay,    100 * stay / calls);
  fprintf (writefile, " Chat Attempts        %5ld     %3ld\n", chat,    100 * chat / calls);
  fprintf (writefile, " Timeouts             %5ld     %3ld\n", timeout, 100 * timeout / calls);
  fprintf (writefile, " Exceeded Error Limit %5ld     %3ld\n", error,   100 * error / calls);
  fprintf (writefile, " Badwords usage       %5ld     %3ld\n", badword, 100 * badword / calls);
  fprintf (writefile, " \n    Caller Baud Rates:\n");
  if (baud[1] > 0) fprintf (writefile, "   300 Baud           %5ld     %3ld\n", baud[1], 100 * baud[1] / calls);
  if (baud[2] > 0) fprintf (writefile, "  1200 Baud           %5ld     %3ld\n", baud[2], 100 * baud[2] / calls);
  if (baud[3] > 0) fprintf (writefile, "  2400 Baud           %5ld     %3ld\n", baud[3], 100 * baud[3] / calls);
  if (baud[4] > 0) fprintf (writefile, "  4800 Baud           %5ld     %3ld\n", baud[4], 100 * baud[4] / calls);
  if (baud[5] > 0) fprintf (writefile, "  9600 Baud           %5ld     %3ld\n", baud[5], 100 * baud[5] / calls);
  if (baud[6] > 0) fprintf (writefile, " 14400 Baud           %5ld     %3ld\n", baud[6], 100 * baud[6] / calls);
  if (baud[7] > 0) fprintf (writefile, " 19200 Baud           %5ld     %3ld\n", baud[6], 100 * baud[6] / calls);
  if (baud[0] > 0) fprintf (writefile, " sysConsole           %5ld     %3ld\n", baud[0], 100 * baud[0] / calls);
  fprintf (writefile, " \n \n   CALL DURATION DATA\n \n");
  fprintf (writefile, "    The longest call was %ld minutes,", longest);
  fprintf (writefile, " The average call was %ld minutes.\n \n", atime /  calls);
  for (k = 0, i = 0; k < calls - calls / 25; ++i)  k += duration[i];
  h1 = calls - k;
  longest = i;
  i = i / 50 + 1;
  for (k = 0, m2 = h1; k < longest; ++k)
    {
    for (l = k * i, m1 = 0; l < (k + 1) * i; ++l) m1 += duration[l];
    duration[k] = m1;
    if (m1 > m2) m2 = m1;

    }
  j = m2 / 60 + 1;
  fprintf (writefile, " Call duration histogram:\n");
  fprintf (writefile, "  Each bar represents the number of calls of that\n");
  fprintf (writefile, "  duration or up to %ld minutes longer.", i);
  fprintf (writefile, "  Each '=' represents %ld calls.\n \n", j);
  for (k = 0; longest >= k * i; ++k)
    {
    fprintf (writefile, "  %4ld", k * i);
    for (l = 0; l < duration[k] / j; ++l) fprintf (writefile, "=");
    fprintf (writefile, " (%ld)\n", duration[k]);

    }
  fprintf (writefile, " >%4ld", --k * i);
  for (l = 0; l < h1 / j; ++l)
  fprintf (writefile, "=");
  fprintf (writefile, " (%ld)\n", h1);
  fprintf (writefile, " \n \n   CALL TIMING DATA\n \n");
  i = 4;
  m1 = 0;
  m2 = 0;
  for (k = 0; k < 24; ++k)
    {
    h1 = 0;
    h2 = 0;
    for (l = k * i; l < (k + 1) * i; ++l)
      {
      h1 += calltime[l];
      h2 += nettime[l];

      }
    calltime[k] = h1;
    nettime[k] = h2;
    if (h1 > m1)    m1 = h1;
    if (h2 > m2)    m2 = h2;

    }
  h1 = m1 / 60 + 1;
  h2 = m2 / 60 + 1;
  fprintf (writefile, " Call timing histogram:\n");
  fprintf (writefile, "  Each bar represents the number of calls\n");
  fprintf (writefile, "  during that hour.");
  fprintf (writefile, "  Each '=' represents %ld calls.\n", h1);
  for (k = 0; k < 24; ++k)
    {
    fprintf (writefile, " %2ld:00", k);
    for (l = 0; l < calltime[k] / h1; ++l) fprintf (writefile, "=");
    fprintf (writefile, " (%ld)\n", calltime[k]);

    }
  fprintf (writefile, "\n Net-Session timing histogram:\n");
  fprintf (writefile, "  Each bar represents the number of net-sessions\n");
  fprintf (writefile, "  during that hour.");
  fprintf (writefile, "  Each '=' represents %ld net-sessions.\n", h2);
  for (k = 0; k < 24; ++k)
    {
    fprintf (writefile, " %2ld:00", k);
    for (l = 0; l < nettime[k] / h2; ++l) fprintf (writefile, "=");
    fprintf (writefile, " (%ld)\n", nettime[k]);

    }
  fprintf (writefile, "\n   BEST TIME TO CALL DATA \n");
  for (i = 0, m1 = 0, j = 0; i < 96; ++i)
    {
    if (tottime[i] == 0)
      {
      fprintf (writefile, "  Best time to call data is unavailable,");
      fprintf (writefile, " since there is less than 24 hours of data.\n");
      fclose (writefile);
      fclose (readfile);
      exit (0);

      }
    if ((tottime[i] - busytime[i]) * 100 / tottime[i] > m1)
      {
      m1 = (tottime[i] - busytime[i]) * 100 / tottime[i];
      j = i;

      }

    }
  fprintf (writefile, " The best time to call is around %ld:%02ld,\n", j / 4, (j % 4) * 15);
  fprintf (writefile, "  when there is a %ld percent chance of connecting.\n",  m1);
  i = 4;
  for (m1 = 0, j = 0; j < 24; ++j)
    {
    for (k = j * i, h1 = 0, h2 = 0; k < (j + 1) * i; ++k)
      {
      h1 += tottime[k];
      h2 += busytime[k];

      }
    busytime[j] = (h1 - h2) * 100 / h1;
    if (busytime[j] > m1) m1 = busytime[j];

    }
  j = m1 / 60 + 1;
  fprintf (writefile, " \n Best Time to Call Graph:\n");
  fprintf (writefile, "  Each bar represents the chance of connecting\n");
  fprintf (writefile, "   during that hour.\n \n");
  for (k = 0; k < 24; ++k)
    {
    fprintf (writefile, " %2ld:00", k);
    for (l = 0; l < busytime[k] / j; ++l)    fprintf (writefile, "=");
    fprintf (writefile, " %ld\n", busytime[k]);

    }
  fclose (readfile);
  fclose (writefile);
  return 0;
  }
