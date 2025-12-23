/*
** Play source for mail item parsing
*/

struct CNdomoCmd
{
   char *name;          /* Name of command   */
   int  (*func)(void);  /* Function to call  */
   int  min_args;
   int  max_args;
   int  domo_also;      /* Command available from main CNdomo addr? */
#define CND_DA_YES   1
#define CND_DA_NO    2
#define CND_DA_REQ   3
};

struct CNdomoCmd CND_Cmds[] =
{
   "subscribe"  , cmd_subscribe  , 0, 2, CND_DA_YES,
   "confirm"    , cmd_confirm    , 2, 3, CND_DA_YES,
   "unsubscribe", cmd_unsubscribe, 0, 2, CND_DA_YES,
   "config"     , cmd_config     , 2, 2, CND_DA_YES,
   "newconfig"  , cmd_newconfig  , 2, 2, CND_DA_YES,
   "lists"      , cmd_lists      , 0, 0, CND_DA_REQ,
   "who"        , cmd_who        , 0, 1, CND_DA_YES,
   "info"       , cmd_info       , 0, 1, CND_DA_YES,
   "help"       , cmd_help       , 0, 0, CND_DA_YES,
};

struct MailHeaderClass
{
   char *name;
   int   type;
};

enum
{
   HDR_FROM = 1    ,
   HDR_TO          ,
   HDR_DATE        ,
   HDR_SUBJECT     ,
   HDR_REPLYTO     ,
   HDR_ORGANIZATION,
   HDR_APPROVED    ,
   HDR_XLISTSERVER ,
};

static struct MailHeaderClass Headers[] =
{
  "From"        , HDR_FROM        ,
  "To"          , HDR_TO          ,
  "Date"        , HDR_DATE        ,
  "Subject"     , HDR_SUBJECT     ,
  "Reply-to"    , HDR_REPLYTO     ,
  "Organization", HDR_ORGANIZATION,
  "Approved"    , HDR_APPROVED    ,
  "X-ListServer", HDR_XLISTSERVER ,
  NULL          , 0               ,
};

struct MailItemHeader
{
   char FullFrom [ 256 ];
   char From     [  64 ];
   char Date     [  64 ];
   char Subject  [  64 ];
   char Org      [  64 ];
   LONG Flags;
#define MIH_LISTINSUBJECT (1<<0)
#define MIH_LOOP          (1<<1)
#define MIH_APPROVED      (1<<2)
};

void process_line(char *line);

void main(int argc, char **argv)
{
   if(argc == 2)
   {
      BPTR fp;

      if(fp = Open(argv[1], MODE_OLDFILE))
      {
         char buff [ 1024 * 10 ] = "";

         while(FGets(fp, buff, sizeof(buff)))
         {
            process_line(buff);
         }
         Close(fp);
      }
   }
}

void process_line(char *line)
{
   char *temp = line,
        *head = line,
        *val;

   static BOOL headers = FALSE;

   if(!headers)
   {
      if(!strncmp(line, "From ", 5))
      {
         Printf("Found possible \"From \"\n");
         headers = TRUE;
         return;
      }
   }
   else
   {
      if((*temp == '\n') || (*temp == '\r'))
         headers = FALSE;
      else
      {
         if(isspace(*line))
         {
            temp += strlen(temp);

            while(*temp && !isalnum(*temp))
            {
               --temp;
            }

            *(--temp) = '\0';

            head = "";
         }
         else
         {
            if(!(temp = strchr(line, ':')))
            {
               if(isspace(*temp))
               {
                  Printf("Invalid header!\n");
                  return;
               }
               temp++;
            }
         }
         if(*temp == ':')
         {
            *temp = '\0';
            temp++;
            while(*temp && isspace(*temp))
            {
               temp++;
            }
         }

         val = temp;

         temp += strlen(temp);

         while(*temp && !isalnum(*temp))
         {
            --temp;
         }

         *(--temp) = '\0';

         {
            struct MailHeaderClass *mhc = Headers;

            while(mhc->name)
            {
               if(!strcmp(mhc->name, head))
               {
                  Printf("Header [val = %2ld]: \"%s\"\n", mhc->type, mhc->name);
                  Printf("Value: \"%s\"\n", val);

                  break;
               }
               mhc++;
            }

            if(mhc->name)
            {
               switch(mhc->type)
               {
                  case HDR_FROM:
                     {
                        char *tmp,
                             *tmp2 = NULL;

                        while(tmp = strchr(val, '<'))
                           val = tmp2 = (++tmp);                 

                        if(tmp2)
                        {
                           val = tmp2;
                           if(tmp2 = strchr(val, '>'))
                              *tmp2 = '\0';
                        }

                        Printf("From-addr: \"%s\"\n", val);
                     }
                     break;

                  default:
                     break;
               }
            }
         }
      }
   }
}

void cmd_subscribe(void)
{
}

void cmd_confirm(void)
{
}

void cmd_unsubscribe(void)
{
}

void cmd_config(void)
{
}

void cmd_newconfig(void)
{
}

void cmd_lists(void)
{
}

void cmd_who(void)
{
}

void cmd_info(void)
{
}

void cmd_help(void)
{
}

