/*
** CNet/4 Email Delivery Utility
** by Kelly Cochran
** Last updated: Sun Sep 20 10:56:40 1998
*/

const char *_version = "\0$VER: CNSendEmail 1.1 "__AMIGADATE__;

struct Library *CNetBase  = NULL;
struct Library *CNet4Base = NULL;

void send_message(LONG *args);

#define TEMPLATE    "FROM/A,FILE/A,REALNAME/K,SUBJECT/K,TO/A/M"
#define MTT_PATTERN CNETOUTBOX"/%08X.mtt"
#define MTL_PATTERN CNETOUTBOX"/%08X.mtl"
#define CMH_PATTERN CNETOUTBOX"/%08X.cmh"

enum
{
   ARG_FROM = 0,
   ARG_FILE,
   ARG_REALNAME,
   ARG_SUBJECT,
   ARG_TO,
   NUM_ARGS
};

int main(int argc, char **argv)
{
   struct RDArgs *rda;

   LONG args [ NUM_ARGS ];

   memset(args, 0, sizeof(LONG) * NUM_ARGS);

   if(CNetBase = OpenLibrary("cnet.library", 4L))
   {
      if(CNet4Base = OpenLibrary("cnet4.library", 4L))
      {
         if(rda = ReadArgs(TEMPLATE, args, NULL))
         {
            if(FileExists((char *)args[ARG_FILE]))
            {
               send_message(args);
            }
            else
            {
               Printf("No such file %s!\n", args[ARG_FILE]);
            }

            FreeArgs(rda);
         }
         CloseLibrary(CNet4Base);
      }
      CloseLibrary(CNetBase);
   }
}

void send_message(LONG *args)
{
   BPTR mh4fp,
        mtfp;

   ULONG Time = CNetTime();

   struct MailHeader4 mh4;
   struct MailToList  mtl;

   char buf1 [  256 ] = "",
        buf2 [ 1024 ] = "",
        buf3 [  256 ] = "";

   sprintf(buf1, MTT_PATTERN, Time);
   sprintf(buf2, CMH_PATTERN, Time);
   sprintf(buf3, MTL_PATTERN, Time);

   memset(&mh4, 0, sizeof(struct MailHeader4));
   memset(&mtl, 0, sizeof(struct MailToList));

   if(mh4fp = Open(buf2, MODE_NEWFILE))
   {
      if(mtfp = Open(buf1, MODE_NEWFILE))
      {
         BPTR fp;
         char **to = (char **)args[ARG_TO];

         FPuts(mtfp, buf2);
         FPuts(mtfp, "\n");

         while(to && *to)
         {
            UBYTE type = CNetAddressType(*to);
            strcpy(mtl.Name, *to);

            switch(type)
            {
               case ADDRESSTYPE_FIDO:
                  mtl.MailType = MAILTO_FIDO;
                  break;
               case ADDRESSTYPE_INTERNET:
                  mtl.MailType = MAILTO_UUCP;
                  break;
               case ADDRESSTYPE_LOCAL:
                  mtl.MailType = MAILTO_HANDLE;
                  break;
               default:
                  Printf("WARNING: Unknown type for address: \"%s\", using LOCAL delivery\n", *to);
                  mtl.MailType = MAILTO_HANDLE;
                  break;
            }

            FWrite(mtfp, (APTR)&mtl, 1, sizeof(struct MailToList));
            to++;
         }

         sprintf(mh4.Subject, args[ARG_SUBJECT]  ? (char *)args[ARG_SUBJECT] :"<No Subject>");
         strncpy(mh4.From   , args[ARG_REALNAME] ? (char *)args[ARG_REALNAME]:"", 26);
         strcpy(mh4.NetAddress, (char *)args[ARG_FROM]);

         mh4.Date      = Time;
         mh4.Number    = Time;
         mh4.Length    = FileSize((char *)args[ARG_FILE]);

         Write(mh4fp, (APTR)&mh4, sizeof(struct MailHeader4));

         if(fp = Open((char *)args[ARG_FILE], MODE_OLDFILE))
         {
            while(Read(fp, buf2, sizeof(buf2) - 1))
            {
               Write(mh4fp, buf2, strlen(buf2));
               memset(buf2, 0, sizeof(buf2));
            }
            Close(fp);
         }

         Close(mtfp);
      }
      Close(mh4fp);
   }
   Rename(buf1, buf3);
}
