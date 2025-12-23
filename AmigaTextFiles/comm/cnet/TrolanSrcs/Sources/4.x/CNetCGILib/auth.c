/*
** CNet/4 CGI User Authentication Routines
** by Kelly Cochran
** Last updated: Sat Feb 07 14:00:28 1998
*/

char *from64(char *indata, char *outdata);
BOOL auth_user(struct UserData *udata);
BOOL authenticate(struct UserData *udata);

extern struct MainPort *myp;
extern struct Library  *CNetBase;

BOOL auth_user(struct UserData *udata)
{
   if(authenticate(udata))
   {
      return TRUE;
   }
   else
   {
      printf("Status: 401 Forbidden\n");
      printf("WWW-authenticate: Basic realm=\"Login to %s\"\n", myp->gc.MySystemName);
      printf("Content-type: text/html\n");
      printf("\n");
      printf("<HTML>\n");
      printf("<HEAD>\n");
      printf("<TITLE>Authorization Required</TITLE>\n");
      printf("</HEAD>\n");
      printf("<BODY>\n");
      printf("<H1>Authorization Required</H1>\n");
      printf("Invalid password or login supplied\n");
      printf("</BODY>\n");
      printf("</HTML>\n");
   }

   return FALSE;
}

BOOL authenticate(struct UserData *udata)
{
   char *auth = getenv("HTTP_AUTHORIZATION");

   if(!auth)
   {
      return FALSE;
   }
   else
   {
      char *value = strchr(auth, ' ');

      if(value && *value)
      {
         char *pass;

         *value = '\0';
         value++;

         if(stricmp(auth, "Basic"))
         {
            return FALSE;
         }

         from64(value, value);

         if(pass = strchr(value, ':'))
         {
            int acc = atoi(value);

            *pass = '\0';
            pass++;

            if(acc > 0)
            {
               acc--;

               if((acc > myp->Nums[0]) | (*myp->Key[acc].Handle == '!') | (!*myp->Key[acc].Handle))
               {
                  return FALSE;
               }
               else
               {
                  value = myp->Key[acc].Handle;
               }
            }
            else
            {
               char *handle;

               for(acc = 0; acc < myp->Nums[0]; acc++)
               {
                  handle = myp->Key[acc].Handle;
                  if (!handle || !*handle)
                     return FALSE;
                  if (*handle == '!')
                     continue;
                  if (!stricmp(handle, value) || !stricmp(myp->Key[acc].UUCP, value))
                  {
                     value = handle;
                     break;
                  }
                  if (!stricmp(myp->Key[acc].RealName, value))
                  {
                     value = handle;
                     break;
                  }
               }
            }

            if(value)
            {
               struct UserData *udata_p;

               acc++;

               if((udata_p = LockAccount(acc)) && !stricmp(udata_p->PassWord, pass))
               {
                  CopyMem(udata_p, (APTR)udata, sizeof(struct UserData));
                  UnLockAccount(acc, FALSE);

                  return TRUE;
               }
               else
               {
                  if(udata_p)
                  {
                     udata_p->LogonAttempts++;
                     UnLockAccount(acc, TRUE);
                  }
               }
            }
         }
      }
   }

   return FALSE;
}

#define XX 127

/*
 * Table for decoding base64
 */
static char index_64[256] = {
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,62, XX,XX,XX,63,
    52,53,54,55, 56,57,58,59, 60,61,XX,XX, XX,XX,XX,XX,
    XX, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
    15,16,17,18, 19,20,21,22, 23,24,25,XX, XX,XX,XX,XX,
    XX,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
    41,42,43,44, 45,46,47,48, 49,50,51,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
    XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX, XX,XX,XX,XX,
};
#define CHAR64(c)  (index_64[(unsigned char)(c)])

char *from64(char *indata, char *outdata)
{
   int c1, c2, c3, c4;
   int DataDone = 0;
   char buf[3];

   char *in  = indata;
   char *out = outdata;

   while ((c1 = *in) != '\0')
   {
      in++;
      if (c1 != '=' && CHAR64(c1) == XX)
      {
         continue;
      }
      if (DataDone)
         continue;
      do
      {
         c2 = *in;
         in++;
      } while (c2 != EOF && c2 != '=' && CHAR64(c2) == XX);
      do
      {
         c3 = *in;
         in++;
      } while (c3 != EOF && c3 != '=' && CHAR64(c3) == XX);
      do
      {
         c4 = *in;
         in++;
      } while (c4 != EOF && c4 != '=' && CHAR64(c4) == XX);
      if (c2 == EOF || c3 == EOF || c4 == EOF)
      {
         break;
      }
      if (c1 == '=' || c2 == '=')
      {
         DataDone=1;
         continue;
      }
      c1 = CHAR64(c1);
      c2 = CHAR64(c2);
   	buf[0] = ((c1<<2) | ((c2&0x30)>>4));

      if (buf[0] != '\r')
      {
         *out = buf[0];
         out++;
      }
      if (c3 == '=')
      {
         DataDone = 1;
      }
      else
      {
         c3 = CHAR64(c3);
	      buf[1] = (((c2&0x0F) << 4) | ((c3&0x3C) >> 2));
         if (buf[1] != '\r')
         {
            *out = buf[1];
            out++;
         }
         if (c4 == '=')
         {
            DataDone = 1;
         }
         else
         {
            c4 = CHAR64(c4);
      		buf[2] = (((c3&0x03) << 6) | c4);
            if (buf[2] != '\r')
            {
               *out = buf[2];
               out++;
            }
         }
      }
   }

   *out = '\0';

   return (outdata);
}
